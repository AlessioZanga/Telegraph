#pragma once

#include <Eigen/Eigen>
#include <cstdint>
#include <iostream>
#include <map>
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
//! Adjacency list.
using AdjacencyList = std::map<VID, VIDs>;
//! Dense adjacency matrix.
using AdjacencyMatrix = Eigen::Matrix<int8_t, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;
//! Sparse adjacency matrix.
using SparseAdjacencyMatrix = Eigen::SparseMatrix<int8_t, Eigen::RowMajor>;

class IGraph {
   public:
    //! Destroy the IGraph object.
    inline virtual ~IGraph(){};

    /**
     * @brief Output stream operator.
     *
     * @param out Output stream reference.
     * @param G Given graph reference.
     * @return std::ostream& Modified output stream reference.
     */
    friend std::ostream &operator<<(std::ostream &out, const IGraph &G);

    //! Standard hash function.
    friend class std::hash<IGraph>;

    /**
     * @brief The adjacency list representation of the graph.
     *
     * @return AdjacencyList Adjacency list of the graph.
     */
    inline virtual AdjacencyList adjacency_list() const = 0;

    /**
     * @brief The adjacency matrix representation of the graph in a dense format.
     *
     * @return AdjacencyMatrix Dense adjacency matrix of the graph.
     */
    inline virtual AdjacencyMatrix adjacency_matrix() const = 0;

    /**
     * @brief The adjacency matrix representation of the graph in a sparse format.
     *
     * @return SparseAdjacencyMatrix Sparse adjacency matrix of the graph.
     */
    inline virtual SparseAdjacencyMatrix sparse_adjacency_matrix() const = 0;

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
    inline virtual bool is_null() const = 0;

    /**
     * @brief A graph is trivial if it has one vertex and no edges.
     *
     * @return true If the graph is trivial,
     * @return false Otherwise.
     */
    inline virtual bool is_trivial() const = 0;

    /**
     * @brief A graph is complete if every vertex is connected to all the others.
     *
     * @return true If the graph is complete,
     * @return false Otherwise.
     */
    inline virtual bool is_complete() const = 0;

    /**
     * @brief A graph is regular if every vertex has the same degree.
     *
     * @return true If the graph is regular,
     * @return false Otherwise.
     */
    inline virtual bool is_regular() const = 0;

   private:
    /**
     * @brief Hash function of a graph.
     *
     * @return size_t Hash of a graph.
     */
    inline virtual size_t hash() const = 0;

    /**
     * @brief Print utility function.
     *
     * @param out Output stream reference.
     */
    virtual void print(std::ostream &out) const = 0;

   public:
    class VertexView {
       public:
        /**
         * @brief The vertices of a graph.
         *
         * @return VIDs Vertex set of the graph.
         */
        inline virtual VIDs operator()() const = 0;

        /**
         * @brief Whether a vertex id exists or not.
         *
         * @param X Given vertex id.
         * @return true If the vertex id exists,
         * @return false Otherwise.
         */
        inline virtual bool has_vertex(const VID &X) const = 0;

        /**
         * @brief Add a vertex to the graph.
         *
         * @return VID The vertex id.
         */
        inline virtual VID add_vertex() = 0;

        /**
         * @brief Add a vertex id to the graph.
         *
         * @param X Given vertex id.
         */
        inline virtual void add_vertex(const VID &X) = 0;

        /**
         * @brief Delete a vertex id from the graph.
         *
         * @param X Given vertex id.
         */
        inline virtual void del_vertex(const VID &X) = 0;

        /**
         * @brief The set of vertices adjacent to a given vertex id.
         *
         * @param X Given vertex id.
         * @return VIDs Vertices adjacent to a given vertex id.
         */
        inline virtual VIDs adjacent(const VID &X) const = 0;

        /**
         * @brief The number of incident edges on a vertex.
         * @details The degree of a vertex X is computed by summing the number
         * of proper edges and *twice* the number of self-edges incident on X.
         *
         * @param X Given vertex id.
         * @return size_t Degree of the given vertex id.
         */
        inline virtual size_t degree(const VID &X) const = 0;
    };

    class EdgeView {
       public:
        /**
         * @brief The edges of a graph.
         *
         * @return EIDs Edge set of the graph.
         */
        inline virtual EIDs operator()() const = 0;

        /**
         * @brief Whether an edge exists or not.
         *
         * @param X First edge vertex id.
         * @param Y Second edge vertex id.
         * @return true If the edge exists,
         * @return false Otherwise.
         */
        inline virtual bool has_edge(const VID &X, const VID &Y) const = 0;

        /**
         * @brief Add an edge to the graph.
         *
         * @param X First edge vertex id.
         * @param Y Second edge vertex id.
         */
        inline virtual void add_edge(const VID &X, const VID &Y) = 0;

        /**
         * @brief Delete an edge from the graph.
         *
         * @param X First edge vertex id.
         * @param Y Second edge vertex id.
         */
        inline virtual void del_edge(const VID &X, const VID &Y) = 0;
    };
};

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
