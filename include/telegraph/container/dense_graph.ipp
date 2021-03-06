#pragma once

#include "dense_graph.hpp"

DenseGraph::DenseGraph() {}

DenseGraph::DenseGraph(const DenseGraph &other) : AbstractGraph(other), A(other.A) {}

DenseGraph &DenseGraph::operator=(const DenseGraph &other) {
    DenseGraph tmp(other);
    if (this != &other) {
        std::swap(glb, tmp.glb);
        std::swap(vlbs, tmp.vlbs);
        std::swap(elbs, tmp.elbs);
        std::swap(gattrs, tmp.gattrs);
        std::swap(vattrs, tmp.vattrs);
        std::swap(eattrs, tmp.eattrs);
        std::swap(A, tmp.A);
    }
    return *this;
}

DenseGraph::~DenseGraph() {}

DenseGraph::DenseGraph(std::size_t n) { A = AdjacencyMatrix::Zero(n, n); }

template <typename I, require_iter_value_type(I, VID)>
DenseGraph::DenseGraph(const I &begin, const I &end) {
    // Since each VID in V is assumed to be (1) *unique* and (2) *between 0 and n-1*,
    // it is sufficient to count the number of VIDs between [begin, end).
    VID n = std::distance(begin, end);
    // Allocate a squared zero matrix.
    A = AdjacencyMatrix::Zero(n, n);
}

template <typename I, require_iter_value_type(I, EID)>
DenseGraph::DenseGraph(const I &begin, const I &end) {
    // Each VID in V is assumed to be (1) *unique* and (2) *between 0 and n-1*,
    // given that the sequence is assumed to be *not* locally stored, it is
    // necessary to *incrementally* resize the matrix when needed.
    for (auto i = begin; i != end; i++) {
        // Get the current matrix size.
        VID n = order();
        // Get the highest VID for the current EID, plus one to align the size.
        VID v = (i->first > i->second ? i->first : i->second) + 1;
        // Check if the VID exceeds the matrix size.
        if (v > n) {
            // Resize the matrix.
            A.conservativeResize(v, v);
            // Initialize the allocated memory to zero.
            A(Eigen::all, Eigen::seq(n, Eigen::last)) *= 0;
            A(Eigen::seq(n, Eigen::last), Eigen::all) *= 0;
        }
        // Set the edge.
        A(i->first, i->second) = 1;
    }
}

DenseGraph::DenseGraph(const AdjacencyList &other) {
    // Given that the adjacency list *is* locally stored, it is possible to
    // check whether each VID in V is (1) *unique* and (2) *between 0 and n-1* or not.

    // Check if the sequence is non-empty.
    if (!other.empty()) {
        // Get maximum VID in the sorted adjacency list and allign to size.
        VID n = other.rbegin()->first + 1;
        // Since the adjacency list is sorted, each VID in V is (1) *unique* by default,
        // we still need to check if each VID is (2) *between 0 and n-1* or not.
        if (n != other.size()) throw std::invalid_argument("AdjacencyList must contain VIDs *between 0 and size-1*.");
        // Allocate a squared zero matrix.
        A = AdjacencyMatrix::Zero(n, n);
        // Fill the matrix.
        for (const auto &[v, adj] : other) {
            for (const auto &u : adj) {
                // Check EID if is inconsistent with given VIDs.
                if (other.find(u) == other.end()) throw std::out_of_range("AdjacencyList ill formed.");
                // Set the edge.
                A(v, u) = 1;
            }
        }
    }
}

DenseGraph::DenseGraph(const AdjacencyMatrix &other) : A(other) {
    if (other.rows() != other.cols()) throw std::invalid_argument("AdjacencyMatrix must be squared.");
}

DenseGraph::DenseGraph(const SparseAdjacencyMatrix &other) : A(other) {
    if (other.rows() != other.cols()) throw std::invalid_argument("AdjacencyMatrix must be squared.");
}

inline DenseGraph::operator AdjacencyList() const {
    // Set out parameter.
    AdjacencyList out;
    // Get current matrix size.
    std::size_t n = order();
    // Iterate over rows and columns as AdjacencyMatrix is RowMajor.
    for (std::size_t i = 0; i < n; i++) {
        for (std::size_t j = 0; j < n; j++) {
            if (A(i, j) != 0) out[i].insert(j);
        }
    }
    return out;
}

inline DenseGraph::operator AdjacencyMatrix() const { return A; }

inline DenseGraph::operator SparseAdjacencyMatrix() const { return A.sparseView(); }

inline std::size_t DenseGraph::order() const { return A.rows(); }

inline std::size_t DenseGraph::size() const {
    // Cast-view to bool to avoid overflow.
    return A.cast<bool>().count();
}

inline bool DenseGraph::has_vertex(const VID &X) const { return X < order(); }

inline VID DenseGraph::add_vertex() {
    // Get current matrix size.
    VID n = order();
    // Resize the matrix.
    A.conservativeResize(n + 1, n + 1);
    // Initialize the allocated memory to zero.
    A.row(n) *= 0;
    A.col(n) *= 0;
    // Return new VID.
    return n;
}

inline void DenseGraph::add_vertex(const VID &X) {
    // Get current matrix size.
    VID n = order();
    // Check if it is possible to add the requested VID.
    if (X < n) throw ALREADY_DEFINED(X);
    if (X > n) throw std::out_of_range("VID " + std::to_string(X) + " out of range.");
    // Add requested vertex.
    add_vertex();
}

inline void DenseGraph::del_vertex(const VID &X) {
    if (!has_vertex(X)) throw NOT_DEFINED(X);
    // Resize matrix.

    // Get current matrix size.
    std::size_t n = order();
    // Delete associated label.
    auto i = vlbs.left.find(X);
    if (i != vlbs.left.end()) vlbs.left.erase(i);
    // Update labels keys after X.
    for (VID k = X; k < n; k++) {
        // Check if vertex has label.
        auto p = vlbs.left.find(k);
        if (p != vlbs.left.end()) {
            vlbs.left.modify_key(p, boost::bimaps::_key = (k - 1));
        }
    }
    // Delete associated attributes.
    auto j = vattrs.find(X);
    if (j != vattrs.end()) vattrs.erase(j);
    // Update attributes keys after X.
    for (VID k = X; k < n; k++) {
        // Check if vertex has attributes.
        auto p = vattrs.find(k);
        if (p != vattrs.end()) {
            auto pair = vattrs.extract(p);   // Extract pair from map.
            pair.key() = k - 1;              // Decrease key.
            vattrs.insert(std::move(pair));  // Move pair back into map.
        }
    }
}

inline bool DenseGraph::has_edge(const EID &X) const {
    if (!has_vertex(X.first)) throw NOT_DEFINED(X.first);
    if (!has_vertex(X.second)) throw NOT_DEFINED(X.second);
    return A(X.first, X.second) != 0;
}

inline void DenseGraph::add_edge(const EID &X) {
    if (has_edge(X)) throw ALREADY_DEFINED(X.first, X.second);
    A(X.first, X.second) = 1;
}

inline void DenseGraph::del_edge(const EID &X) {
    if (!has_edge(X)) throw NOT_DEFINED(X.first, X.second);
    A(X.first, X.second) = 0;
    // Delete associated label.
    auto i = elbs.left.find(X);
    if (i != elbs.left.end()) elbs.left.erase(i);
    // Delete associated attributes.
    auto j = eattrs.find(X);
    if (j != eattrs.end()) eattrs.erase(j);
}

inline std::size_t DenseGraph::hash() const {
    // Initialize seed hash.
    std::size_t seed = 0;
    // Get current matrix size.
    std::size_t n = order();
    // Hash VIDs.
    for (VID i = 0; i < n; i++) boost::hash_combine(seed, i);
    // Hash EIDs.
    for (VID i = 0; i < n; i++) {
        for (VID j = 0; j < n; j++) {
            if (A(i, j) != 0) boost::hash_combine(seed, EID(i, j));
        }
    }
    // Hash GLB.
    if (has_label()) boost::hash_combine(seed, glb);
    // Hash VLBs.
    boost::hash_combine(seed, boost::hash_range(vlbs.begin(), vlbs.end()));
    // Hash ELBs.
    boost::hash_combine(seed, boost::hash_range(elbs.begin(), elbs.end()));
    // Return hash
    return seed;
}

void DenseGraph::print(std::ostream &out) const {
    // Get current matrix size.
    std::size_t n = order();

    // Print graph class.
    out << "DenseGraph( ";

    // Print vertex set.
    out << "V = ( ";
    // Iterate over vertices.
    for (VID i = 0; i < n; i++) out << i << ", ";
    // Close vertex set.
    out << " )";

    // Print edge set.
    out << ", E = ( ";
    // Iterate over vertices.
    for (VID i = 0; i < n; i++) {
        for (VID j = 0; j < n; j++) {
            if (A(i, j) != 0) out << "(" << i << ", " << j << "), ";
        }
    }
    for (VID i = 0; i < n - 1; i++) out << i << ", ";
    // Close edge set.
    out << " )";

    // Close graph class.
    out << " )" << std::endl;
}
