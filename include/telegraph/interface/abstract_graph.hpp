#pragma once

#include <any>
#include <boost/bimap.hpp>
#include <boost/bimap/support/lambda.hpp>
#include <boost/container_hash/hash.hpp>

#include "interface_graph.ipp"

//! Graph label.
using GLB = std::string;
//! Vertex label.
using VLB = std::string;
//! Edge label.
class ELB : public std::string {
    explicit ELB(const std::string &&other) : std::string(other) {}
};
//! Vertex labels set.
using VLBs = std::set<VLB>;
//! Edge labels set.
using ELBs = std::set<ELB>;

class AbstractGraph : public IGraph {
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
     * @brief Get the vertex id.
     *
     * @param X Given vertex label.
     * @return VID The vertex id.
     */
    inline VID get_vid(const VLB &X) const;

    /**
     * @brief Check if a vertex has a label.
     *
     * @param X Given vertex id.
     * @return true If the vertex has a label,
     * @return false Otherwise.
     */
    inline bool has_label(const VID &X) const;

    /**
     * @brief Get the vertex label.
     *
     * @param X Given vertex id.
     * @return VLB The vertex label.
     */
    inline VLB get_label(const VID &X) const;

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
     * @param X Given vertex label.
     * @return VID Added vertex id.
     */
    inline virtual VID add_vertex(const VLB &label);

    /**
     * @brief Delete a vertex from the graph.
     *
     * @param X Given vertex label.
     */
    inline virtual void del_vertex(const VLB &X);

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
     * @brief Add an edge to the graph.
     *
     * @param X Given edge id.
     */
    inline virtual void add_edge(const EID &X) = 0;

    /**
     * @brief Delete an edge from the graph.
     *
     * @param X Given edge id.
     */
    inline virtual void del_edge(const EID &X) = 0;

    /**
     * @brief Get the edge id.
     *
     * @param X Given edge label.
     * @return EID The edge id.
     */
    inline EID get_eid(const ELB &X) const;

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
     * @param X First edge vertex id.
     * @param Y Second edge vertex id.
     */
    inline virtual void add_edge(const VID &X, const VID &Y);

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
     * @param X Given edge label.
     */
    inline virtual void del_edge(const ELB &X);

    /**
     * @brief Delete an edge from the graph.
     *
     * @param X First edge vertex id.
     * @param Y Second edge vertex id.
     */
    inline virtual void del_edge(const VID &X, const VID &Y);

    /**
     * @brief Delete an edge from the graph.
     *
     * @param X First edge vertex label.
     * @param Y Second edge vertex label.
     */
    inline virtual void del_edge(const VLB &X, const VLB &Y);

    /** @}*/
};
