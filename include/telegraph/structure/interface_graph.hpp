#pragma once

#include <cstdint>
#include <set>
#include <utility>

//! Vertex identifier.
using VID = uintmax_t;
//! Edge identifier.
using EID = std::pair<VID, VID>;
//! Vertex set.
using VIDs = std::set<VID>;
//! Edge set.
using EIDs = std::set<EID>;

class IGraph {
   public:
    //! Construct a new IGraph object.
    IGraph(){};

    //! Destroy the IGraph object.
    ~IGraph(){};

    /**
     * @brief Whether a vertex exists or not.
     *
     * @param X Selected vertex.
     * @return true If the vertex exists,
     * @return false Otherwise.
     */
    inline virtual bool has_vertex(const VID &X) const = 0;

    /**
     * @brief Add a vertex to the graph.
     *
     * @return VID The vertex ID.
     */
    inline virtual VID add_vertex() = 0;

    /**
     * @brief Add a vertex to the graph with a predefined ID.
     *
     * @param X Selected vertex.
     */
    inline virtual void add_vertex(const VID &X) = 0;

    /**
     * @brief Delete a vertex from the graph.
     *
     * @param X Selected vertex.
     */
    inline virtual void del_vertex(const VID &X) = 0;

    /**
     * @brief The vertices of a graph.
     *
     * @return VIDs Vertex set of the graph.
     */
    inline virtual VIDs vertices() const = 0;

    /**
     * @brief Whether an edge exists or not.
     *
     * @param X First edge endpoint.
     * @param Y Second edge endpoint.
     * @return true If the edge exists,
     * @return false Otherwise.
     */
    inline virtual bool has_edge(const VID &X, const VID &Y) const = 0;

    /**
     * @brief Add an edge to the graph.
     *
     * @param X First edge endpoint.
     * @param Y Second edge endpoint.
     */
    inline virtual void add_edge(const VID &X, const VID &Y) = 0;

    /**
     * @brief Delete an edge from the graph.
     *
     * @param X First edge endpoint.
     * @param Y Second edge endpoint.
     */
    inline virtual void del_edge(const VID &X, const VID &Y) = 0;

    /**
     * @brief The edges of a graph.
     *
     * @return EIDs Edge set of the graph.
     */
    inline virtual EIDs edges() const = 0;

    /**
     * @brief The number of vertices of a graph is called its order.
     *
     * @return size_t Number of vertices of the graph.
     */
    inline virtual size_t order() const = 0;

    /**
     * @brief The number of edges of a graph is called its size.
     *
     * @return size_t Number of edges of the graph.
     */
    inline virtual size_t size() const = 0;

    /**
     * @brief A graph is null if it has no vertices.
     *
     * @return true If the graph is null,
     * @return false Otherwise.
     */
    virtual bool is_null() const = 0;

    /**
     * @brief A graph is trivial if it has one vertex and no edges.
     *
     * @return true If the graph is trivial,
     * @return false Otherwise.
     */
    virtual bool is_trivial() const = 0;

    /**
     * @brief A graph is complete if every vertex is connected to all the others.
     *
     * @return true If the graph is complete,
     * @return false Otherwise.
     */
    virtual bool is_complete() const = 0;

    /**
     * @brief The number of incident edges on a vertex.
     * @details The degree of a vertex X is computed by summing the number
     * of proper edges and *twice* the number of self-edges incident on X.
     *
     * @param X Selected vertex.
     * @return size_t Degree of the selected vertex.
     */
    virtual size_t degree(const VID &X) const = 0;

    /**
     * @brief A graph is regular if every vertex has the same degree.
     *
     * @return true If the graph is regular,
     * @return false Otherwise.
     */
    virtual bool is_regular() const = 0;
};
