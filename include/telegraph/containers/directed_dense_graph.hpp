#pragma once

#include <telegraph/containers/dense_graph.ipp>

class DirectedDenseGraph : public DenseGraph {
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

    inline bool is_directed() const;

    inline bool is_partially_directed() const;
};
