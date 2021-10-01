#pragma once

#include "interface_graph.hpp"

inline IGraph::~IGraph() {}

inline bool IGraph::is_null() const { return order() == 0; }

inline bool IGraph::is_trivial() const { return order() == 1 && size() == 0; }

//! Output stream operator adapter.
std::ostream &operator<<(std::ostream &out, const IGraph &G) {
    G.print(out);
    return out;
}

namespace std {

template <>
struct hash<IGraph> {
    //! Hash function adapter.
    size_t operator()(const IGraph &G) const { return G.hash(); }
};

};  // namespace std
