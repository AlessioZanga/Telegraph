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
    std::size_t n = std::distance(begin, end);
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
        std::size_t n = A.rows();
        // Get the highest VID for the current EID, plus one to align the size.
        VID v = (i->first > i->second ? i->first : i->second) + 1;
        // Check if the VID exceeds the matrix size.
        if (n < v) {
            // Resize the matrix.
            A.conservativeResize(v, v);
            // Initialize the allocated memory to zero.
            A(Eigen::all, Eigen::seq(n, Eigen::last)) = 0;
            A(Eigen::seq(n, Eigen::last), Eigen::all) = 0;
        }
        // Set the edge
        A(i->first, i->second) = 1;
    }
}

DenseGraph::DenseGraph(const AdjacencyList &other) {}

DenseGraph::DenseGraph(const AdjacencyMatrix &other) : A(other) {}

DenseGraph::DenseGraph(const SparseAdjacencyMatrix &other) {}

inline AdjacencyList DenseGraph::adjacency_list() const {}

inline AdjacencyMatrix DenseGraph::adjacency_matrix() const {}

inline SparseAdjacencyMatrix DenseGraph::sparse_adjacency_matrix() const {}

inline std::size_t DenseGraph::order() const {}

inline std::size_t DenseGraph::size() const {}

inline VIDs DenseGraph::vertices() const {}

inline bool DenseGraph::has_vertex(const VID &X) const {}

inline VID DenseGraph::add_vertex() {}

inline void DenseGraph::add_vertex(const VID &X) {}

inline void DenseGraph::del_vertex(const VID &X) {}

inline EIDs DenseGraph::edges() const {}

inline bool DenseGraph::has_edge(const EID &X) const {}

inline void DenseGraph::add_edge(const EID &X) {}

inline void DenseGraph::del_edge(const EID &X) {}

inline std::size_t DenseGraph::hash() const {}

void DenseGraph::print(std::ostream &out) const {}
