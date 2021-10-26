#pragma once

#include <telegraph/containers/directed_dense_graph.hpp>

DirectedDenseGraph::DirectedDenseGraph() : DenseGraph() {}

DirectedDenseGraph::DirectedDenseGraph(const DirectedDenseGraph &other) : DenseGraph(other) {}

DirectedDenseGraph &DirectedDenseGraph::operator=(const DirectedDenseGraph &other) {
    DirectedDenseGraph tmp(other);
    if (this != &other) {
        std::swap(glb, tmp.glb);
        std::swap(vlbs, tmp.vlbs);
        std::swap(elbs, tmp.elbs);
        std::swap(gattrs, tmp.gattrs);
        std::swap(vattrs, tmp.vattrs);
        std::swap(eattrs, tmp.eattrs);
        std::swap(A, tmp.A);
        std::swap(M, tmp.M);
    }
    return *this;
}

DirectedDenseGraph::~DirectedDenseGraph() {}

inline bool DirectedDenseGraph::is_directed() const { return true; }

inline bool DirectedDenseGraph::is_partially_directed() const { return true; }
