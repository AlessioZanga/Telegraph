#pragma once

#include <boost/bimap.hpp>

#include "interface_graph.hpp"

//! Graph label.
using GLB = std::string;
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
    //! Graph label, aka its name.
    GLB glb;
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
    inline virtual ~AbstractGraph();

    /**
     * @brief Check if the graph has a label.
     *
     * @return true If the graph has a label,
     * @return false Otherwise.
     */
    inline bool has_label() const;

    /**
     * @brief Check if a vertex has a label.
     *
     * @param X Given vertex id.
     * @return true If the vertex has a label,
     * @return false Otherwise.
     */
    inline bool has_label(const VID &X) const;

    /**
     * @brief Check if an edge has a label.
     *
     * @param X First edge vertex id.
     * @param Y Second edge vertex id.
     * @return true If the edge has a label,
     * @return false Otherwise.
     */
    inline bool has_label(const VID &X, const VID &Y) const;

    /**
     * @brief Get the graph label.
     *
     * @return GLB The graph label.
     */
    inline GLB get_label() const;

    /**
     * @brief Get the vertex label given its id.
     *
     * @param X Given vertex id.
     * @return VLB The vertex label.
     */
    inline VLB get_label(const VID &X) const;

    /**
     * @brief Get the edge label given its vertices ids.
     *
     * @param X First edge vertex id.
     * @param Y Second edge vertex id.
     * @return ELB The edge label.
     */
    inline ELB get_label(const VID &X, const VID &Y) const;

    /**
     * @brief Set the graph label.
     *
     * @param label Given graph label.
     */
    inline void set_label(const GLB &label);

    /**
     * @brief Set the vertex label given its id.
     *
     * @param X Given vertex id.
     * @param label Given vertex label.
     */
    inline void set_label(const VID &X, const VLB &label);

    /**
     * @brief Set the edge label given its vertices ids.
     *
     * @param X First edge vertex id.
     * @param Y Second edge vertex id.
     * @param label Given edge label.
     */
    inline void set_label(const VID &X, const VID &Y, const ELB &label);

    /**
     * @brief Delete the graph label;
     */
    inline void del_label();

    /**
     * @brief Delete the vertex label given its id.
     *
     * @param X Given vertex id.
     */
    inline void del_label(const VID &X);

    /**
     * @brief Delete the edge label given its vertices ids.
     *
     * @param X First edge vertex id.
     * @param Y Second edge vertex id.
     */
    inline void del_label(const VID &X, const VID &Y);

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
    inline virtual VLBs adjacent(const VLB &X) const;

    /**
     * @brief The number of incident edges on a vertex.
     * @details The degree of a vertex X is computed by summing the number
     * of proper edges and *twice* the number of self-edges incident on X.
     *
     * @param X Given vertex label.
     * @return size_t Degree of the given vertex label.
     */
    inline virtual size_t degree(const VLB &X) const = 0;
};
