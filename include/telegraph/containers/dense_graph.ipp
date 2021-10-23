#pragma once

#include <telegraph/containers/dense_graph.hpp>

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
                if (other.find(u) == other.end()) throw std::invalid_argument("AdjacencyList ill formed.");
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
    // Insert VIDs
    for (const VID &X : V()) out[X];
    // Iterate over rows and columns as AdjacencyMatrix is RowMajor.
    for (const EID &X : E()) out[X.first].insert(X.second);
    // Return result.
    return out;
}

inline DenseGraph::operator AdjacencyMatrix() const { return A; }

inline DenseGraph::operator SparseAdjacencyMatrix() const { return A.sparseView(); }

#define VTYPE DenseGraph::VIDsIterator
#define ITYPE VTYPE::const_iterator

VTYPE::VIDsIterator() : G(NULL) {}

VTYPE::VIDsIterator(const DenseGraph *G) : G(G) {}

VTYPE::VIDsIterator(const VIDsIterator &other) : G(other.G) {}

VTYPE &VTYPE::operator=(const VIDsIterator &other) {
    VTYPE tmp(other);
    if (this != &other) {
        std::swap(G, tmp.G);
    }
    return *this;
}

VTYPE::~VIDsIterator() {}

ITYPE::const_iterator() : G(NULL), curr(0) {}

ITYPE::const_iterator(const DenseGraph *G, const VID &curr) : G(G), curr(curr) {}

ITYPE::const_iterator(const const_iterator &other) : G(other.G), curr(other.curr) {}

ITYPE &ITYPE::operator=(const const_iterator &other) {
    ITYPE tmp(other);
    if (this != &other) {
        std::swap(G, tmp.G);
        std::swap(curr, tmp.curr);
    }
    return *this;
}

ITYPE::~const_iterator() {}

inline bool ITYPE::operator==(const const_iterator &other) const { return G == other.G && curr == other.curr; }

inline bool ITYPE::operator!=(const const_iterator &other) const { return G != other.G || curr != other.curr; }

inline ITYPE::reference ITYPE::operator*() const { return curr; }

inline ITYPE::reference ITYPE::operator->() const { return curr; }

inline ITYPE &ITYPE::operator++() {
    assert(G);  // Assert G is a valid pointer.
    if (curr != G->order()) curr++;
    return *this;
}

inline ITYPE ITYPE::operator++(int) {
    ITYPE prev = *this;
    ++*this;
    return prev;
}

inline ITYPE &ITYPE::operator--() {
    assert(G);  // Assert G is a valid pointer.
    if (curr != 0) curr--;
    return *this;
}

inline ITYPE ITYPE::operator--(int) {
    ITYPE prev = *this;
    --*this;
    return prev;
}

ITYPE VTYPE::begin() const { return ITYPE(G, 0); }

ITYPE VTYPE::end() const { return ITYPE(G, G->order()); }

std::size_t VTYPE::size() const { return G->order(); }

VTYPE DenseGraph::V() const { return VTYPE(this); }

#undef ITYPE
#undef VTYPE

#define VTYPE DenseGraph::EIDsIterator
#define ITYPE VTYPE::const_iterator

VTYPE::EIDsIterator() : G(NULL) {}

VTYPE::EIDsIterator(const DenseGraph *G) : G(G) {}

VTYPE::EIDsIterator(const EIDsIterator &other) : G(other.G) {}

VTYPE &VTYPE::operator=(const EIDsIterator &other) {
    VTYPE tmp(other);
    if (this != &other) {
        std::swap(G, tmp.G);
    }
    return *this;
}

VTYPE::~EIDsIterator() {}

ITYPE::const_iterator() : G(NULL), curr(0, 0) {}

ITYPE::const_iterator(const DenseGraph *G, const EID &curr) : G(G), curr(curr) {}

ITYPE::const_iterator(const const_iterator &other) : G(other.G), curr(other.curr) {}

ITYPE &ITYPE::operator=(const const_iterator &other) {
    ITYPE tmp(other);
    if (this != &other) {
        std::swap(G, tmp.G);
        std::swap(curr, tmp.curr);
    }
    return *this;
}

ITYPE::~const_iterator() {}

inline bool ITYPE::operator==(const const_iterator &other) const { return G == other.G && curr == other.curr; }

inline bool ITYPE::operator!=(const const_iterator &other) const { return G != other.G || curr != other.curr; }

inline ITYPE::reference ITYPE::operator*() const { return curr; }

inline ITYPE::reference ITYPE::operator->() const { return curr; }

inline ITYPE &ITYPE::operator++() {
    assert(G);  // Assert G is a valid pointer.
    // Get current matrix size.
    const std::size_t n = G->order();
    // Compute next 1D index.
    std::size_t i = (curr.first * n + curr.second) + 1;
    // Interate over matrix.
    for (; i < (n * n); i++) {
        // Check if current index is valid.
        if (G->A(i)) {
            // Compute current 2D index.
            curr = EID(i / n, i % n);
            return *this;
        }
    }
    // If end of matrix is reached, set end sentinel value.
    curr = EID(n - 1, n);
    return *this;
}

inline ITYPE ITYPE::operator++(int) {
    ITYPE prev = *this;
    ++*this;
    return prev;
}

inline ITYPE &ITYPE::operator--() {
    assert(G);  // Assert G is a valid pointer.
    // Get current matrix size.
    const std::size_t n = G->order();
    // Compute prev 1D index.
    std::size_t i = (curr.first * n + curr.second) - 1;
    // Interate over matrix.
    for (; i != std::size_t(-1); i--) {
        // Check if current index is valid.
        if (G->A(i)) {
            // Update current 2D index.
            curr = EID(i / n, i % n);
            return *this;
        }
    }
    // If end of matrix is reached, set end sentinel value.
    curr = EID(n - 1, n);
    return *this;
}

inline ITYPE ITYPE::operator--(int) {
    ITYPE prev = *this;
    --*this;
    return prev;
}

ITYPE VTYPE::begin() const {
    // Get current matrix size.
    const std::size_t n = G->order();
    // Interate over matrix.
    for (std::size_t i = 0; i < n * n; i++) {
        // Check if current index is valid and return current 2D index.
        if (G->A(i)) return ITYPE(G, EID(i / n, i % n));
    }
    return ITYPE(G, EID(n - 1, n));
}

ITYPE VTYPE::end() const { return ITYPE(G, EID(G->order() - 1, G->order())); }

std::size_t VTYPE::size() const { return G->size(); }

VTYPE DenseGraph::E() const { return VTYPE(this); }

#undef ITYPE
#undef VTYPE

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

inline VID DenseGraph::add_vertex(const VID &X) {
    // Get current matrix size.
    VID n = order();
    // Check if it is possible to add the requested VID.
    if (X < n) throw ALREADY_DEFINED(X);
    if (X > n) throw std::out_of_range("VID " + std::to_string(X) + " out of range.");
    // Add requested vertex.
    return add_vertex();
}

inline VID DenseGraph::del_vertex(const VID &X) {
    if (!has_vertex(X)) throw NOT_DEFINED(X);
    // Copy the remaining edges and resize matrix.
    std::size_t rows = A.rows();
    std::size_t cols = A.cols();
    A.block(X, 0, rows - X - 1, cols) = A.block(X + 1, 0, rows - X - 1, cols);
    A.block(0, X, rows, cols - X - 1) = A.block(0, X + 1, rows, cols - X - 1);
    A.conservativeResize(rows - 1, cols - 1);
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
    // Return vertex id.
    return X;
}

inline bool DenseGraph::has_edge(const EID &X) const {
    if (!has_vertex(X.first)) throw NOT_DEFINED(X.first);
    if (!has_vertex(X.second)) throw NOT_DEFINED(X.second);
    return A(X.first, X.second) != 0;
}

inline EID DenseGraph::add_edge(const EID &X) {
    if (has_edge(X)) throw ALREADY_DEFINED(X.first, X.second);
    A(X.first, X.second) = 1;
    return X;
}

inline EID DenseGraph::del_edge(const EID &X) {
    if (!has_edge(X)) throw NOT_DEFINED(X.first, X.second);
    A(X.first, X.second) = 0;
    // Delete associated label.
    auto i = elbs.left.find(X);
    if (i != elbs.left.end()) elbs.left.erase(i);
    // Delete associated attributes.
    auto j = eattrs.find(X);
    if (j != eattrs.end()) eattrs.erase(j);
    // Return edge id.
    return X;
}

inline std::size_t DenseGraph::hash() const {
    // Initialize seed hash.
    std::size_t seed = 0;
    // Hash VIDs.
    if (std::size(V())) boost::hash_combine(seed, boost::hash_range(V().begin(), V().end()));
    // Hash EIDs.
    if (std::size(E())) boost::hash_combine(seed, boost::hash_range(E().begin(), E().end()));
    // Hash GLB.
    if (has_label()) boost::hash_combine(seed, glb);
    // Hash VLBs.
    if (std::size(Vl())) boost::hash_combine(seed, boost::hash_range(Vl().begin(), Vl().end()));
    // Hash ELBs.
    if (std::size(El())) boost::hash_combine(seed, boost::hash_range(El().begin(), El().end()));
    // Return hash
    return seed;
}

void DenseGraph::to_stream(std::ostream &out) const {
    // Print graph class.
    out << "DenseGraph(";

    // Print graph label.
    out << "label = '" << (has_label() ? get_label() : "") << "', ";

    // Print vertex set.
    out << "V = (";
    // Iterate over vertices.
    for (const VID &X : V()) out << X << ", ";
    // Close vertex set.
    out << ")";

    // Print edge set.
    out << ", E = (";
    // Iterate over vertices.
    for (const EID &X : E()) out << "(" << X.first << ", " << X.second << "), ";
    // Close edge set.
    out << ")";

    // Print vertex set.
    out << ", Vl = (";
    // Iterate over vertices.
    for (const VLB &X : Vl()) out << X << ", ";
    // Close vertex set.
    out << ")";

    // Print edge set.
    out << ", El = (";
    // Iterate over vertices.
    for (const ELB &X : El()) out << "(" << X << "), ";
    // Close edge set.
    out << ")";

    // Close graph class.
    out << ")";
}
