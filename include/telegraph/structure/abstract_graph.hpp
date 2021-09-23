#pragma once

#include <boost/bimap.hpp>

#include "interface_graph.hpp"

//! Vertex label.
using VLB = std::string;
//! Edge label.
using ELB = std::string;
//! Vertex labels set.
using VLBs = std::set<VLB>;
//! Edge labels set.
using ELBs = std::set<ELB>;

class AbstractGraph : public IGraph {
   private:
    //! Bidirectional mapping for vertex ids and labels.
    boost::bimap<VID, VLB> vlbs;
    //! Bidirectional mapping for edge ids and labels.
    boost::bimap<EID, ELB> elbs;

   public:
    //! Default constructor for a new Abstract Graph object
    AbstractGraph();

    //! Copy constructor for a new Abstract Graph object
    AbstractGraph(const AbstractGraph &other);

    /**
     * @brief Assign constructor for a new Abstract Graph object
     *
     * @param other Given Abstract Graph object
     * @return AbstractGraph& New Abstract Graph object
     */
    AbstractGraph &operator=(const AbstractGraph &other);

    //! Destroy the Abstract Graph object
    virtual ~AbstractGraph();

    /**
     * @brief Whether a vertex label exists or not.
     *
     * @param X Given vertex label.
     * @return true If the vertex label exists,
     * @return false Otherwise.
     */
    inline virtual bool has_vertex(const VLB &X) const;

    /**
     * @brief Add a vertex label to the graph.
     *
     * @param X Given vertex label.
     */
    inline virtual void add_vertex(const VLB &X);

    /**
     * @brief Delete a vertex label from the graph.
     *
     * @param X Given vertex label.
     */
    inline virtual void del_vertex(const VLB &X);

    /**
     * @brief The vertices of a graph.
     *
     * @return VLBs Vertex set of the graph.
     */
    inline virtual VLBs vertices_labels() const;

    /**
     * @brief Whether an edge exists or not.
     *
     * @param X First edge vertex label.
     * @param Y Second edge vertex label.
     * @return true If the edge exists,
     * @return false Otherwise.
     */
    inline virtual bool has_edge(const VLB &X, const VLB &Y) const;

    /**
     * @brief Add an edge to the graph.
     *
     * @param X First edge vertex label.
     * @param Y Second edge vertex label.
     */
    inline virtual void add_edge(const VLB &X, const VLB &Y);

    /**
     * @brief Delete an edge from the graph.
     *
     * @param X First edge vertex label.
     * @param Y Second edge vertex label.
     */
    inline virtual void del_edge(const VLB &X, const VLB &Y);

    /**
     * @brief The edges of a graph.
     *
     * @return ELBs Edge set of the graph.
     */
    inline virtual ELBs edges_labels() const;

    /**
     * @brief The set of vertices adjacent to a given vertex label.
     *
     * @param X Given vertex label.
     * @return VLBs Vertices adjacent to a given vertex label.
     */
    virtual VLBs adjacent(const VLB &X) const;

    /**
     * @brief The number of incident edges on a vertex.
     * @details The degree of a vertex X is computed by summing the number
     * of proper edges and *twice* the number of self-edges incident on X.
     *
     * @param X Given vertex label.
     * @return size_t Degree of the given vertex label.
     */
    virtual size_t degree(const VLB &X) const = 0;
};
