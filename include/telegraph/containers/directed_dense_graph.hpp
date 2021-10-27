#pragma once

#include <telegraph/containers/dense_graph.ipp>
#include <telegraph/containers/directed_graph.ipp>

class DirectedDenseGraph : public DenseGraph, public DirectedGraph {
   public:
    using DenseGraph::DenseGraph;

    //! Default constructor for a new Directed Dense Graph object.
    DirectedDenseGraph();

    //! Copy constructor for a new Directed Dense Graph object.
    DirectedDenseGraph(const DirectedDenseGraph &other);

    //! Assignment constructor for a new Directed Dense Graph object.
    DirectedDenseGraph &operator=(const DirectedDenseGraph &other);

    //! Destroy the Directed Dense Graph object.
    virtual ~DirectedDenseGraph();
};
