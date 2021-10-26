#pragma once

#include <telegraph/containers/dense_graph.hpp>

DenseGraph::DenseGraph() {}

DenseGraph::DenseGraph(const DenseGraph &other) : AbstractGraph(other), A(other.A), M(other.M) {}

DenseGraph::~DenseGraph() {}

DenseGraph::DenseGraph(std::size_t n) {
    // Initialize VIDs in map.
    for (std::size_t i = 0; i < n; i++) M.left.insert({i, AdjacencyMatrix::Index(i)});
    // Allocate a squared zero matrix.
    A = AdjacencyMatrix::Zero(n, n);
}

template <typename I, require_iter_value_type(I, VID)>
DenseGraph::DenseGraph(const I &begin, const I &end) {
    // Given that the sequence is assumed to be *not* locally stored, it is
    // necessary to store sequence for duplicate removal and vertex sorting.
    VIDs V(begin, end);
    // Initialize graph order.
    std::size_t n = 0;
    // Insert VIDs into map.
    for (auto i = V.begin(); i != V.end(); ++i, n++) M.left.insert({*i, AdjacencyMatrix::Index(n)});
    // Allocate a squared zero matrix.
    A = AdjacencyMatrix::Zero(n, n);
}

template <typename I, require_iter_value_type(I, EID)>
DenseGraph::DenseGraph(const I &begin, const I &end) {
    // Given that the sequence is assumed to be *not* locally stored, it is
    // necessary to store sequence for duplicate removal and vertex sorting.
    VIDs V;
    EIDs E(begin, end);
    for (const auto &[v, u] : E) V.insert(v), V.insert(u);
    // Initialize graph order.
    std::size_t n = 0;
    // Insert VIDs into map.
    for (auto i = V.begin(); i != V.end(); ++i, n++) M.left.insert({*i, AdjacencyMatrix::Index(n)});
    // Allocate a squared zero matrix.
    A = AdjacencyMatrix::Zero(n, n);
    // Set edges.
    for (const auto &[v, u] : E) A(M.left.at(v), M.left.at(u)) = 1;
}

DenseGraph::DenseGraph(const AdjacencyList &other) {
    // Given that the adjacency list *is* locally stored, it is possible to
    // check whether each VID in V is (1) *unique* and (2) *between 0 and n-1* or not.

    // Check if the sequence is non-empty.
    if (!other.empty()) {
        // Initialize graph order.
        std::size_t n = 0;
        // Insert VIDs into map.
        for (auto i = other.begin(); i != other.end(); ++i, n++) M.left.insert({i->first, AdjacencyMatrix::Index(n)});
        // Allocate a squared zero matrix.
        A = AdjacencyMatrix::Zero(n, n);
        // Fill the matrix.
        for (const auto &[v, adj] : other) {
            for (const auto &u : adj) {
                // Check EID if is inconsistent with given VIDs.
                if (other.find(u) == other.end()) throw std::invalid_argument("AdjacencyList ill formed.");
                // Set the edge.
                A(M.left.at(v), M.left.at(u)) = 1;
            }
        }
    }
}

DenseGraph::DenseGraph(const AdjacencyMatrix &other) : A(other) {
    if (other.rows() != other.cols()) throw std::invalid_argument("AdjacencyMatrix must be squared.");
    for (std::size_t i = 0; i < order(); i++) M.left.insert({i, AdjacencyMatrix::Index(i)});  // Insert VIDs in map.
}

DenseGraph::DenseGraph(const SparseAdjacencyMatrix &other) : A(other) {
    if (other.rows() != other.cols()) throw std::invalid_argument("SparseAdjacencyMatrix must be squared.");
    for (std::size_t i = 0; i < order(); i++) M.left.insert({i, AdjacencyMatrix::Index(i)});  // Insert VIDs in map.
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

ITYPE VTYPE::begin() const { return ITYPE(G->M.right.begin(), get_key); }

ITYPE VTYPE::end() const { return ITYPE(G->M.right.end(), get_key); }

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

inline bool DenseGraph::has_vertex(const VID &X) const { return M.left.find(X) != M.left.end(); }

inline VID DenseGraph::add_vertex() {
    // Initialize new VID.
    VID X = 0;
    // Get highest VID if present.
    auto i = M.right.rbegin();
    if (i != M.right.rend()) X = (i->second + 1);
    // Add vertex
    return add_vertex(X);
}

inline VID DenseGraph::add_vertex(const VID &X) {
    if (has_vertex(X)) throw ALREADY_DEFINED(X);
    // Get current matrix size.
    std::size_t n = order();
    // Insert new VID into map.
    M.left.insert({X, AdjacencyMatrix::Index(n)});
    // Resize the matrix.
    A.conservativeResize(n + 1, n + 1);
    // Initialize the allocated memory to zero.
    A.row(n) *= 0;
    A.col(n) *= 0;
    // Return new VID.
    return X;
}

inline VID DenseGraph::del_vertex(const VID &X) {
    if (!has_vertex(X)) throw NOT_DEFINED(X);
    // Copy the remaining edges and resize matrix.
    std::size_t rows = A.rows();
    std::size_t cols = A.cols();
    A.block(X, 0, rows - X - 1, cols) = A.block(X + 1, 0, rows - X - 1, cols);
    A.block(0, X, rows, cols - X - 1) = A.block(0, X + 1, rows, cols - X - 1);
    A.conservativeResize(rows - 1, cols - 1);
    // Update VIDs map accordingly.
    auto i = M.left.find(X);
    auto index = i->second;
    for (auto it = M.left.begin(); it != M.left.end(); it++) {
        // Decrease matrix index by one if needed.
        if (it->second > index) M.left.replace_data(it, (it->second - 1));
    }
    // Delete VID.
    M.left.erase(i);
    // Delete associated label.
    auto j = vlbs.left.find(X);
    if (j != vlbs.left.end()) vlbs.left.erase(j);
    // Delete associated attributes.
    auto k = vattrs.find(X);
    if (k != vattrs.end()) vattrs.erase(k);
    // Return vertex id.
    return X;
}

inline bool DenseGraph::has_edge(const EID &X) const {
    if (!has_vertex(X.first)) throw NOT_DEFINED(X.first);
    if (!has_vertex(X.second)) throw NOT_DEFINED(X.second);
    return A(M.left.at(X.first), M.left.at(X.second));
}

inline EID DenseGraph::add_edge(const EID &X) {
    if (has_edge(X)) throw ALREADY_DEFINED(X.first, X.second);
    A(M.left.at(X.first), M.left.at(X.second)) = 1;
    return X;
}

inline EID DenseGraph::del_edge(const EID &X) {
    if (!has_edge(X)) throw NOT_DEFINED(X.first, X.second);
    A(M.left.at(X.first), M.left.at(X.second)) = 0;
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
    // Hash GLB.
    if (has_label()) boost::hash_combine(seed, get_label());
    // Hash VIDs.
    if (std::size(V())) boost::hash_combine(seed, boost::hash_range(V().begin(), V().end()));
    // Hash EIDs.
    if (std::size(E())) boost::hash_combine(seed, boost::hash_range(E().begin(), E().end()));
    // Hash VLBs.
    if (std::size(Vp())) boost::hash_combine(seed, boost::hash_range(Vl().begin(), Vl().end()));
    // Hash ELBs.
    if (std::size(Ep())) boost::hash_combine(seed, boost::hash_range(El().begin(), El().end()));
    // Return hash
    return seed;
}

void DenseGraph::to_stream(std::ostream &out) const {
    // Print graph class.
    out << "(";

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

    // Print vertex pairs set.
    out << ", Vp = (";
    // Iterate over vertices.
    for (const auto &X : Vp())
        out << "(" << X.first << ", '" << X.second << "')"
            << ", ";
    // Close vertex set.
    out << ")";

    // Print edge pairs set.
    out << ", Ep = (";
    // Iterate over vertices.
    for (const auto &X : Ep()) out << "((" << X.first.first << ", " << X.first.second << "), '" << X.second << "'), ";
    // Close edge set.
    out << ")";

    // Close graph class.
    out << ")";
}
