#pragma once

#include <telegraph/containers/directed_graph.hpp>

DirectedGraph::DirectedGraph() {}

constexpr inline bool DirectedGraph::is_directed() { return true; }

constexpr inline bool DirectedGraph::is_partially_directed() { return true; }
