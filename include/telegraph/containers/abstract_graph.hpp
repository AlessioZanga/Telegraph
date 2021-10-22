#pragma once

#include <Eigen/Eigen>
#include <any>
#include <algorithm>
#include <boost/bimap.hpp>
#include <boost/bimap/support/lambda.hpp>
#include <boost/container_hash/hash.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <telegraph/exceptions.hpp>
#include <telegraph/requires.hpp>
#include <utility>

//! Vertex identifier.
using VID = uintmax_t;
//! Edge identifier.
using EID = std::pair<VID, VID>;
//! Vertex set.
using VIDs = std::set<VID>;
//! Edge set.
using EIDs = std::set<EID>;

//! Graph label.
using GLB = std::string;
//! Vertex label.
using VLB = std::string;
//! Edge label.
class ELB : public std::string {
   public:
    explicit ELB(const std::string &&other) : std::string(other) {}
};
//! Vertex labels set.
using VLBs = std::set<VLB>;
//! Edge labels set.
using ELBs = std::set<ELB>;

//! Adjacency list.
using AdjacencyList = std::map<VID, VIDs>;
//! Dense adjacency matrix.
using AdjacencyMatrix = Eigen::Matrix<int8_t, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;
//! Sparse adjacency matrix.
using SparseAdjacencyMatrix = Eigen::SparseMatrix<int8_t, Eigen::RowMajor>;

//! Infix vertex id set iterator.
template <typename T>
static inline auto V(const T &G) {
    return G.V();
}

//! Infix edge id set iterator.
template <typename T>
static inline auto E(const T &G) {
    return G.E();
}

//! Infix vertex label set iterator.
template <typename T>
static inline auto Vl(const T &G) {
    return G.Vl();
}

//! Infix edge label set iterator.
template <typename T>
static inline auto El(const T &G) {
    return G.El();
}

class AbstractGraph {
   protected:
    //! Graph label, aka its name.
    GLB glb;
    //! Bidirectional mapping for vertex ids and labels.
    boost::bimap<VID, VLB> vlbs;
    //! Bidirectional mapping for edge ids and labels.
    boost::bimap<EID, ELB> elbs;

    //! Graph attributes.
    std::map<std::string, std::any> gattrs;
    //! Vertices attributes.
    std::map<VID, std::map<std::string, std::any>> vattrs;
    //! Edges attributes.
    std::map<EID, std::map<std::string, std::any>> eattrs;

   public:
    //! Default constructor for a new Abstract Graph object.
    AbstractGraph();

    //! Copy constructor for a new Abstract Graph object.
    AbstractGraph(const AbstractGraph &other);

    //! Destroy the Abstract Graph object.
    inline virtual ~AbstractGraph();

    /**
     * @brief The adjacency list representation of the graph.
     *
     * @return AdjacencyList Adjacency list of the graph.
     */
    inline virtual operator AdjacencyList() const = 0;

    /**
     * @brief The adjacency matrix representation of the graph in a dense format.
     *
     * @return AdjacencyMatrix Dense adjacency matrix of the graph.
     */
    inline virtual operator AdjacencyMatrix() const = 0;

    /**
     * @brief The adjacency matrix representation of the graph in a sparse format.
     *
     * @return SparseAdjacencyMatrix Sparse adjacency matrix of the graph.
     */
    inline virtual operator SparseAdjacencyMatrix() const = 0;

    /**
     * @brief The number of vertices of a graph is called its order.
     *
     * @return std::size_t Number of vertices of the graph.
     */
    inline virtual std::size_t order() const = 0;

    /**
     * @brief The number of edges of a graph is called its size.
     *
     * @return std::size_t Number of edges of the graph.
     */
    inline virtual std::size_t size() const = 0;

    /**
     * @brief A graph is null if it has no vertices.
     *
     * @return true If the graph is null,
     * @return false Otherwise.
     */
    inline virtual bool is_null() const;

    /**
     * @brief A graph is trivial if it has one vertex and no edges.
     *
     * @return true If the graph is trivial,
     * @return false Otherwise.
     */
    inline virtual bool is_trivial() const;

    /**
     * @brief A graph is complete if every vertex is connected to all the others.
     *
     * @return true If the graph is complete,
     * @return false Otherwise.
     */
    inline virtual bool is_complete() const;

    /**
     * @brief Check if the graph has a label.
     *
     * @return true If the graph has a label,
     * @return false Otherwise.
     */
    inline bool has_label() const;

    /**
     * @brief Get the graph label.
     *
     * @return const GLB& The graph label.
     */
    inline const GLB &get_label() const;

    /**
     * @brief Set the graph label.
     *
     * @param label Given graph label.
     */
    inline void set_label(const GLB &label);

    /**
     * @brief Delete the graph label;
     */
    inline void del_label();

    /**
     * @brief Check if the graph has an attribute given its key.
     *
     * @param key Given attribute key.
     * @return true If the graph has the attribute,
     * @return false Otherwise.
     */
    inline bool has_attr(const std::string &key) const;

    /**
     * @brief Get the graph attribute value given its key.
     *
     * @tparam T Return attribute value type.
     * @param key Given attribute key.
     * @return T The attribute value.
     */
    template <typename T>
    inline T get_attr(const std::string &key) const;

    /**
     * @brief Set the graph attribute given its key and value.
     *
     * @tparam T Attribute value type.
     * @param key Given attribute key.
     * @param value Given attribute value.
     */
    template <typename T>
    inline void set_attr(const std::string &key, const T &value);

    /**
     * @brief Delete the graph attribute given its key.
     *
     * @param key Given attribute key.
     */
    inline void del_attr(const std::string &key);

    /** \addtogroup vertices
     *  @{
     */

    /**
     * @brief Whether a vertex id exists or not.
     *
     * @param X Given vertex id.
     * @return true If the vertex id exists,
     * @return false Otherwise.
     */
    inline virtual bool has_vertex(const VID &X) const = 0;

    /**
     * @brief Whether a vertex exists or not.
     *
     * @param X Given vertex label.
     * @return true If the vertex label exists,
     * @return false Otherwise.
     */
    inline virtual bool has_vertex(const VLB &X) const;

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
     * @return VID The vertex id.
     */
    inline virtual VID add_vertex(const VID &X) = 0;

    /**
     * @brief Add a vertex to the graph.
     *
     * @param X Given vertex label.
     * @return VID Added vertex id.
     */
    inline virtual VID add_vertex(const VLB &label);

    /**
     * @brief Delete a vertex id from the graph.
     *
     * @param X Given vertex id.
     * @return VID The vertex id.
     */
    inline virtual VID del_vertex(const VID &X) = 0;

    /**
     * @brief Delete a vertex from the graph.
     *
     * @param X Given vertex label.
     * @return VID Added vertex id.
     */
    inline virtual VID del_vertex(const VLB &X);

    /**
     * @brief Get the vertex id.
     *
     * @param X Given vertex label.
     * @return VID The vertex id.
     */
    inline VID get_id(const VLB &X) const;

    /**
     * @brief Check if a vertex has a label.
     *
     * @param X Given vertex id.
     * @return true If the vertex has a label,
     * @return false Otherwise.
     */
    inline bool has_label(const VID &X) const;

    /**
     * @brief Check if a vertex has a label.
     *
     * @param X Given vertex label.
     * @return true If the vertex has a label,
     * @return false Otherwise.
     */
    inline bool has_label(const VLB &X) const;

    /**
     * @brief Get the vertex label.
     *
     * @param X Given vertex id.
     * @return const VLB& The vertex label.
     */
    inline const VLB &get_label(const VID &X) const;

    /**
     * @brief Set the vertex label.
     *
     * @param X Given vertex id.
     * @param label Given vertex label.
     */
    inline void set_label(const VID &X, const VLB &label);

    /**
     * @brief Set the vertex label given its previous label.
     *
     * @param X Previous vertex label.
     * @param label Given vertex label.
     */
    inline void set_label(const VLB &X, const VLB &label);

    /**
     * @brief Delete the vertex label.
     *
     * @param X Given vertex id.
     */
    inline void del_label(const VID &X);

    /**
     * @brief Delete the vertex label.
     *
     * @param X Given vertex label.
     */
    inline void del_label(const VLB &X);

    /**
     * @brief Check if a vertex has an attribute given its key.
     *
     * @param X Given vertex id.
     * @param key Given attribute key.
     * @return true If the vertex has the attribute,
     * @return false Otherwise.
     */
    inline bool has_attr(const VID &X, const std::string &key) const;

    /**
     * @brief Check if a vertex has an attribute given its key.
     *
     * @param X Given vertex label.
     * @param key Given attribute key.
     * @return true If the vertex has the attribute,
     * @return false Otherwise.
     */
    inline bool has_attr(const VLB &X, const std::string &key) const;

    /**
     * @brief Get the vertex attribute value given its key.
     *
     * @tparam T Return attribute value type.
     * @param X Given vertex id.
     * @param key Given attribute key.
     * @return T The attribute value.
     */
    template <typename T>
    inline T get_attr(const VID &X, const std::string &key) const;

    /**
     * @brief Get the vertex attribute value given its key.
     *
     * @tparam T Return attribute value type.
     * @param X Given vertex label.
     * @param key Given attribute key.
     * @return T The attribute value.
     */
    template <typename T>
    inline T get_attr(const VLB &X, const std::string &key) const;

    /**
     * @brief Set the vertex attribute given its key and value.
     *
     * @tparam T Attribute value type.
     * @param X Given vertex id.
     * @param key Given attribute key.
     * @param value Given attribute value.
     */
    template <typename T>
    inline void set_attr(const VID &X, const std::string &key, const T &value);

    /**
     * @brief Set the vertex attribute given its key and value.
     *
     * @tparam T Attribute value type.
     * @param X Given vertex label.
     * @param key Given attribute key.
     * @param value Given attribute value.
     */
    template <typename T>
    inline void set_attr(const VLB &X, const std::string &key, const T &value);

    /**
     * @brief Delete the vertex attribute given its key.
     *
     * @param X Given vertex id.
     * @param key Given attribute key.
     */
    inline void del_attr(const VID &X, const std::string &key);

    /**
     * @brief Delete the vertex attribute given its key.
     *
     * @param X Given vertex label.
     * @param key Given attribute key.
     */
    inline void del_attr(const VLB &X, const std::string &key);

    /** @}*/

    /** \addtogroup edges
     *  @{
     */

    /**
     * @brief Whether an edge exists or not.
     *
     * @param X Given edge id.
     * @return true If the edge exists,
     * @return false Otherwise.
     */
    inline virtual bool has_edge(const EID &X) const = 0;

    /**
     * @brief Whether an edge exists or not.
     *
     * @param X Given edge label.
     * @return true If the edge exists,
     * @return false Otherwise.
     */
    inline virtual bool has_edge(const ELB &X) const;

    /**
     * @brief Whether an edge exists or not.
     *
     * @param X First edge vertex id.
     * @param Y Second edge vertex id.
     * @return true If the edge exists,
     * @return false Otherwise.
     */
    inline virtual bool has_edge(const VID &X, const VID &Y) const;

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
     * @param X Given edge id.
     * @return EID The edge id.
     */
    inline virtual EID add_edge(const EID &X) = 0;

    /**
     * @brief Add an edge to the graph.
     *
     * @param X First edge vertex id.
     * @param Y Second edge vertex id.
     * @return EID The edge id.
     */
    inline virtual EID add_edge(const VID &X, const VID &Y);

    /**
     * @brief Add an edge to the graph.
     *
     * @param X First edge vertex label.
     * @param Y Second edge vertex label.
     * @return EID The edge id.
     */
    inline virtual EID add_edge(const VLB &X, const VLB &Y);

    /**
     * @brief Delete an edge from the graph.
     *
     * @param X Given edge id.
     * @return EID The edge id.
     */
    inline virtual EID del_edge(const EID &X) = 0;

    /**
     * @brief Delete an edge from the graph.
     *
     * @param X Given edge label.
     * @return EID The edge id.
     */
    inline virtual EID del_edge(const ELB &X);

    /**
     * @brief Delete an edge from the graph.
     *
     * @param X First edge vertex id.
     * @param Y Second edge vertex id.
     * @return EID The edge id.
     */
    inline virtual EID del_edge(const VID &X, const VID &Y);

    /**
     * @brief Delete an edge from the graph.
     *
     * @param X First edge vertex label.
     * @param Y Second edge vertex label.
     * @return EID The edge id.
     */
    inline virtual EID del_edge(const VLB &X, const VLB &Y);

    /**
     * @brief Get the edge id.
     *
     * @param X Given edge label.
     * @return EID The edge id.
     */
    inline EID get_id(const ELB &X) const;

    /**
     * @brief Check if an edge has a label.
     *
     * @param X Given edge id.
     * @return true If the edge has a label,
     * @return false Otherwise.
     */
    inline bool has_label(const EID &X) const;

    /**
     * @brief Check if an edge has a label.
     *
     * @param X Given edge label.
     * @return true If the edge has a label,
     * @return false Otherwise.
     */
    inline bool has_label(const ELB &X) const;

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
     * @brief Check if an edge has a label.
     *
     * @param X First edge vertex label.
     * @param Y Second edge vertex label.
     * @return true If the edge has a label,
     * @return false Otherwise.
     */
    inline bool has_label(const VLB &X, const VLB &Y) const;

    /**
     * @brief Get the edge label.
     *
     * @param X Given edge id.
     * @return const ELB& The edge label.
     */
    inline const ELB &get_label(const EID &X) const;

    /**
     * @brief Get the edge label.
     *
     * @param X First edge vertex id.
     * @param Y Second edge vertex id.
     * @return const ELB& The edge label.
     */
    inline const ELB &get_label(const VID &X, const VID &Y) const;

    /**
     * @brief Get the edge label.
     *
     * @param X First edge vertex label.
     * @param Y Second edge vertex label.
     * @return const ELB& The edge label.
     */
    inline const ELB &get_label(const VLB &X, const VLB &Y) const;

    /**
     * @brief Set the edge label.
     *
     * @param X Given edge id.
     * @param label Given edge label.
     */
    inline void set_label(const EID &X, const ELB &label);

    /**
     * @brief Set the edge label.
     *
     * @param X Previous edge label.
     * @param label Given edge label.
     */
    inline void set_label(const ELB &X, const ELB &label);

    /**
     * @brief Set the edge label.
     *
     * @param X First edge vertex id.
     * @param Y Second edge vertex id.
     * @param label Given edge label.
     */
    inline void set_label(const VID &X, const VID &Y, const ELB &label);

    /**
     * @brief Set the edge label.
     *
     * @param X First edge vertex label.
     * @param Y Second edge vertex label.
     * @param label Given edge label.
     */
    inline void set_label(const VLB &X, const VLB &Y, const ELB &label);

    /**
     * @brief Delete the edge label.
     *
     * @param X Given edge id.
     */
    inline void del_label(const EID &X);

    /**
     * @brief Delete the edge label.
     *
     * @param X Given edge label.
     */
    inline void del_label(const ELB &X);

    /**
     * @brief Delete the edge label.
     *
     * @param X First edge vertex id.
     * @param Y Second edge vertex id.
     */
    inline void del_label(const VID &X, const VID &Y);

    /**
     * @brief Delete the edge label.
     *
     * @param X First edge vertex label.
     * @param Y Second edge vertex label.
     */
    inline void del_label(const VLB &X, const VLB &Y);

    /**
     * @brief Check if an edge has an attribute given its key.
     *
     * @param X Given edge id.
     * @param key Given attribute key.
     * @return true If the edge has the attribute,
     * @return false Otherwise.
     */
    inline bool has_attr(const EID &X, const std::string &key) const;

    /**
     * @brief Check if an edge has an attribute given its key.
     *
     * @param X Given edge label.
     * @param key Given attribute key.
     * @return true If the edge has the attribute,
     * @return false Otherwise.
     */
    inline bool has_attr(const ELB &X, const std::string &key) const;

    /**
     * @brief Check if an edge has an attribute given its key.
     *
     * @param X First edge vertex id.
     * @param Y Second edge vertex id.
     * @param key Given attribute key.
     * @return true If the edge has the attribute,
     * @return false Otherwise.
     */
    inline bool has_attr(const VID &X, const VID &Y, const std::string &key) const;

    /**
     * @brief Check if an edge has an attribute given its key.
     *
     * @param X First edge vertex label.
     * @param Y Second edge vertex label.
     * @param key Given attribute key.
     * @return true If the edge has the attribute,
     * @return false Otherwise.
     */
    inline bool has_attr(const VLB &X, const VLB &Y, const std::string &key) const;

    /**
     * @brief Get the edge attribute value given its key.
     *
     * @tparam T Return attribute value type.
     * @param X Given edge id.
     * @param key Given attribute key.
     * @return T The attribute value.
     */
    template <typename T>
    inline T get_attr(const EID &X, const std::string &key) const;

    /**
     * @brief Get the edge attribute value given its key.
     *
     * @tparam T Return attribute value type.
     * @param X Given edge label.
     * @param key Given attribute key.
     * @return T The attribute value.
     */
    template <typename T>
    inline T get_attr(const ELB &X, const std::string &key) const;

    /**
     * @brief Get the edge attribute value given its key.
     *
     * @tparam T Return attribute value type.
     * @param X First edge vertex id.
     * @param Y Second edge vertex id.
     * @param key Given attribute key.
     * @return T The attribute value.
     */
    template <typename T>
    inline T get_attr(const VID &X, const VID &Y, const std::string &key) const;

    /**
     * @brief Get the edge attribute value given its key.
     *
     * @tparam T Attribute value type.
     * @param X First edge vertex label.
     * @param Y Second edge vertex label.
     * @param key Given attribute key.
     * @return T The attribute value.
     */
    template <typename T>
    inline T get_attr(const VLB &X, const VLB &Y, const std::string &key) const;

    /**
     * @brief Set the edge attribute given its key and value.
     *
     * @tparam T Attribute value type.
     * @param X Given edge id.
     * @param key Given attribute key.
     * @param value Given attribute value.
     */
    template <typename T>
    inline void set_attr(const EID &X, const std::string &key, const T &value);

    /**
     * @brief Set the edge attribute given its key and value.
     *
     * @tparam T Attribute value type.
     * @param X Given edge label.
     * @param key Given attribute key.
     * @param value Given attribute value.
     */
    template <typename T>
    inline void set_attr(const ELB &X, const std::string &key, const T &value);

    /**
     * @brief Set the edge attribute given its key and value.
     *
     * @tparam T Attribute value type.
     * @param X First edge vertex id.
     * @param Y Second edge vertex id.
     * @param key Given attribute key.
     * @param value Given attribute value.
     */
    template <typename T>
    inline void set_attr(const VID &X, const VID &Y, const std::string &key, const T &value);

    /**
     * @brief Set the edge attribute given its key and value.
     *
     * @tparam T Attribute value type.
     * @param X First edge vertex label.
     * @param Y Second edge vertex label.
     * @param key Given attribute key.
     * @param value Given attribute value.
     */
    template <typename T>
    inline void set_attr(const VLB &X, const VLB &Y, const std::string &key, const T &value);

    /**
     * @brief Delete the edge attribute given its key.
     *
     * @param X Given edge id.
     * @param key Given attribute key.
     */
    inline void del_attr(const EID &X, const std::string &key);

    /**
     * @brief Delete the edge attribute given its key.
     *
     * @param X Given edge label.
     * @param key Given attribute key.
     */
    inline void del_attr(const ELB &X, const std::string &key);

    /**
     * @brief Delete the edge attribute given its key.
     *
     * @param X First edge vertex id.
     * @param Y Second edge vertex id.
     * @param key Given attribute key.
     */
    inline void del_attr(const VID &X, const VID &Y, const std::string &key);

    /**
     * @brief Delete the edge attribute given its key.
     *
     * @param X First edge vertex label.
     * @param Y Second edge vertex label.
     * @param key Given attribute key.
     */
    inline void del_attr(const VLB &X, const VLB &Y, const std::string &key);

    /** @}*/

    /**
     * @brief Output stream operator.
     *
     * @param out Output stream reference.
     * @param G Given graph reference.
     * @return std::ostream& Output stream reference.
     */
    friend std::ostream &operator<<(std::ostream &out, const AbstractGraph &G);

    //! Standard hash function.
    friend class std::hash<AbstractGraph>;

   protected:
    /**
     * @brief Hash function of a graph.
     *
     * @return std::size_t Hash of a graph.
     */
    inline virtual std::size_t hash() const = 0;

    /**
     * @brief Incrementally write text representation of the graph to output stream.
     *
     * @param out Output stream reference.
     */
    virtual void to_stream(std::ostream &out) const = 0;
};
