#pragma once

#include <any>
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

    //! Graph attributes.
    std::map<std::string, std::any> gattrs;
    //! Vertices attributes.
    std::map<VID, std::map<std::string, std::any>> vattrs;
    //! Edges attributes.
    std::map<EID, std::map<std::string, std::any>> eattrs;

   public:
    //! Default constructor for a new Abstract Graph object
    AbstractGraph();

    //! Copy constructor for a new Abstract Graph object
    AbstractGraph(const AbstractGraph &other);

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
     * @brief Get the graph label.
     *
     * @return GLB The graph label.
     */
    inline GLB get_label() const;

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
     * @brief The vertices of a graph.
     *
     * @return VLBs Vertex set of the graph.
     */
    inline VLBs labels() const;

    /**
     * @brief Check if a vertex has a label.
     *
     * @param X Given vertex id.
     * @return true If the vertex has a label,
     * @return false Otherwise.
     */
    inline bool has_label(const VID &X) const;

    /**
     * @brief Get the vertex label given its id.
     *
     * @param X Given vertex id.
     * @return VLB The vertex label.
     */
    inline VLB get_label(const VID &X) const;

    /**
     * @brief Set the vertex label given its id.
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
     * @brief Delete the vertex label given its id.
     *
     * @param X Given vertex id.
     */
    inline void del_label(const VID &X);

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

    /**
     * @brief Whether a vertex label exists or not.
     *
     * @param X Given vertex label.
     * @return true If the vertex label exists,
     * @return false Otherwise.
     */
    inline bool has_vertex(const VLB &X) const;

    /**
     * @brief Add a vertex label to the graph.
     *
     * @param X Given vertex label.
     */
    inline void add_vertex(const VLB &X);

    /**
     * @brief Delete a vertex label from the graph.
     *
     * @param X Given vertex label.
     */
    inline void del_vertex(const VLB &X);

    /**
     * @brief The set of vertices adjacent to a given vertex label.
     *
     * @param X Given vertex label.
     * @return VLBs Vertices adjacent to a given vertex label.
     */
    inline VLBs adjacent(const VLB &X) const;

    /**
     * @brief The number of incident edges on a vertex.
     * @details The degree of a vertex X is computed by summing the number
     * of proper edges and *twice* the number of self-edges incident on X.
     *
     * @param X Given vertex label.
     * @return size_t Degree of the given vertex label.
     */
    inline size_t degree(const VLB &X) const;

    /** @}*/

    /** \addtogroup edges
     *  @{
     */

    /**
     * @brief The edges of a graph.
     *
     * @return ELBs Edge set of the graph.
     */
    inline ELBs labels() const;

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
     * @return ELB The edge label.
     */
    inline ELB get_label(const EID &X) const;

    /**
     * @brief Get the edge label.
     *
     * @param X First edge vertex id.
     * @param Y Second edge vertex id.
     * @return ELB The edge label.
     */
    inline ELB get_label(const VID &X, const VID &Y) const;

    /**
     * @brief Get the edge label.
     *
     * @param X First edge vertex label.
     * @param Y Second edge vertex label.
     * @return ELB The edge label.
     */
    inline ELB get_label(const VLB &X, const VLB &Y) const;

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

    /**
     * @brief Whether an edge exists or not.
     *
     * @param X Given edge label.
     * @return true If the edge exists,
     * @return false Otherwise.
     */
    inline bool has_edge(const ELB &X) const;

    /**
     * @brief Whether an edge exists or not.
     *
     * @param X First edge vertex label.
     * @param Y Second edge vertex label.
     * @return true If the edge exists,
     * @return false Otherwise.
     */
    inline bool has_edge(const VLB &X, const VLB &Y) const;

    /**
     * @brief Add an edge to the graph.
     *
     * @param X First edge vertex label.
     * @param Y Second edge vertex label.
     */
    inline void add_edge(const VLB &X, const VLB &Y);

    /**
     * @brief Delete an edge from the graph.
     *
     * @param X First edge vertex label.
     * @param Y Second edge vertex label.
     */
    inline void del_edge(const VLB &X, const VLB &Y);

    /** @}*/
};
