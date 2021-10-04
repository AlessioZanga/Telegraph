#pragma once

#include "abstract_graph.hpp"
#include "exceptions.hpp"

AbstractGraph::AbstractGraph() {}

AbstractGraph::AbstractGraph(const AbstractGraph &other)
    : glb(other.glb),
      vlbs(other.vlbs),
      elbs(other.elbs),
      gattrs(other.gattrs),
      vattrs(other.vattrs),
      eattrs(other.eattrs) {}

inline AbstractGraph::~AbstractGraph() {}

inline bool AbstractGraph::has_label() const { return !glb.empty(); }

inline GLB AbstractGraph::get_label() const {
    if (!has_label()) throw NO_LABEL();
    return glb;
}

inline void AbstractGraph::set_label(const GLB &label) {
    if (label.empty()) throw INVALID_LABEL;  // Check if label is valid.
    glb = label;
}

inline void AbstractGraph::del_label() {
    glb.clear();          // Remove the label characters.
    glb.shrink_to_fit();  // Deallocate the unused memory.
}

inline bool AbstractGraph::has_attr(const std::string &key) const { return gattrs.find(key) != gattrs.end(); }

template <typename T>
inline T AbstractGraph::get_attr(const std::string &key) const {
    auto i = gattrs.find(key);
    if (i == gattrs.end()) throw NO_KEY(key);
    return std::any_cast<T>(i->second);
}

template <typename T>
inline void AbstractGraph::set_attr(const std::string &key, const T &value) {
    gattrs[key] = std::make_any(value);
}

inline void AbstractGraph::del_attr(const std::string &key) {
    auto i = gattrs.find(key);
    if (i == gattrs.end()) throw NO_KEY(key);
    gattrs.erase(i);
}

inline VID AbstractGraph::get_vid(const VLB &X) const {
    auto i = vlbs.find(X);
    if (i == vlbs.end()) throw NO_LABEL(X);
    return i->left;
}

inline VLBs AbstractGraph::labels() const { return VLBs(vlbs.begin(), vlbs.end()); }

inline bool AbstractGraph::has_label(const VID &X) const {
    if (!IGraph::has_vertex(X)) throw NOT_DEFINED(X);  // Check if X is a valid vertex.
    return vlbs.find(X) != vlbs.end();                 // Check if X has a defined label.
}

inline VLB AbstractGraph::get_label(const VID &X) const {
    if (!IGraph::has_vertex(X)) throw NOT_DEFINED(X);  // Check if X is a valid vertex.
    auto i = vlbs.find(X);
    if (i == vlbs.end()) throw NO_LABEL(X);  // Check if X has a defined label.
    return i->right;
}

inline void AbstractGraph::set_label(const VID &X, const VLB &label) {
    if (label.empty()) throw INVALID_LABEL;            // Check if label is valid.
    if (!IGraph::has_vertex(X)) throw NOT_DEFINED(X);  // Check if X is a valid vertex.
    // Check if the label as already been defined.
    auto i = vlbs.find(label);
    if (i != vlbs.end()) {
        // It has been defined for a different vertex.
        if (i->left != X) throw DUPLICATED_LABEL(label);
    } else {
        auto j = vlbs.find(X);
        if (j == vlbs.end()) {
            vlbs.insert({X, label});  // Insert label if none found.
        } else {
            vlbs.replace(j, {X, label});  // Replace label if one found.
        }
    }
}

inline void AbstractGraph::set_label(const VLB &X, const VLB &label) { set_label(get_vid(X), label); }

inline void AbstractGraph::del_label(const VID &X) {
    if (!IGraph::has_vertex(X)) throw NOT_DEFINED(X);  // Check if X is a valid vertex.
    auto i = vlbs.find(X);
    if (i == vlbs.end()) throw NO_LABEL(X);  // Check if X has a valid label.
    vlbs.erase(i);
}

inline void AbstractGraph::del_label(const VLB &X) { del_label(get_vid(X)); }

inline bool AbstractGraph::has_attr(const VID &X, const std::string &key) const {
    if (!IGraph::has_vertex(X)) throw NOT_DEFINED(X);  // Check if X is a valid vertex.
    auto i = vattrs.find(X);
    return i != vattrs.end() && i->second.find(key) != i->second.end();
}

inline bool AbstractGraph::has_attr(const VLB &X, const std::string &key) const { return has_attr(get_vid(X), key); }

template <typename T>
inline T AbstractGraph::get_attr(const VID &X, const std::string &key) const {
    if (!has_attr(X, key)) throw NOT_KEY(key);  // Check if key is valid.
    return std::any_cast<T>(vattrs[X][key]->second);
}

template <typename T>
inline T AbstractGraph::get_attr(const VLB &X, const std::string &key) const {
    return get_attr<T>(get_vid(X), key);
}

template <typename T>
inline void AbstractGraph::set_attr(const VID &X, const std::string &key, const T &value) {
    if (!IGraph::has_vertex(X)) throw NOT_DEFINED(X);  // Check if X is a valid vertex.
    vattrs[X][key] = std::make_any(value);
}

template <typename T>
inline void AbstractGraph::set_attr(const VLB &X, const std::string &key, const T &value) {
    set_attr<T>(get_vid(X), key, value);
}

inline void AbstractGraph::del_attr(const VID &X, const std::string &key) {
    if (!has_attr(X, key)) throw NO_KEY(key);  // Check if key is valid.
    vattrs[X].erase(key);
}

inline void AbstractGraph::del_attr(const VLB &X, const std::string &key) { del_attr(get_vid(X), key); }

inline bool AbstractGraph::has_vertex(const VLB &X) const { return vlbs.find(X) != vlbs.end(); }

inline VID AbstractGraph::add_vertex(const VLB &label) {
    if (label.empty()) throw INVALID_LABEL;                             // Check if label is valid.
    if (vlbs.find(label) != vlbs.end()) throw DUPLICATED_LABEL(label);  // Check if label is valid.
    VID X = IGraph::add_vertex();
    set_label(X, label);
    return X;
}

inline void AbstractGraph::del_vertex(const VID &X) {
    // Call parent method
    IGraph::del_vertex(X);
    // Delete vertex label
    auto i = vlbs.find(X);
    if (i != vlbs.end()) vlbs.erase(i);
    // Delete vertex attributes
    auto j = vattrs.find(X);
    if (j != vattrs.end()) vattrs.erase(j);
}

inline void AbstractGraph::del_vertex(const VLB &X) { del_vertex(get_vid(X)); }

inline EID AbstractGraph::get_eid(const ELB &X) const {
    auto i = elbs.find(X);
    if (i == elbs.end()) throw NO_LABEL(X, "\b\b");  // Use backspaces to patch NO_LABEL argument.
    return i->left;
}

inline ELBs AbstractGraph::labels() const { return ELBs(elbs.begin(), elbs.end()); }

inline bool AbstractGraph::has_label(const EID &X) const {
    if (!IGraph::has_edge(X)) throw NOT_DEFINED(X.first, X.second);  // Check if X is a valid edge.
    return elbs.find(X) != elbs.end();
}

inline bool AbstractGraph::has_label(const VID &X, const VID &Y) const { return has_label(EID(X, Y)); }

inline bool AbstractGraph::has_label(const VLB &X, const VLB &Y) const {
    return has_label(std::make_pair(get_vid(X), get_vid(Y)));
}

inline ELB AbstractGraph::get_label(const EID &X) const {
    auto i = elbs.find(X);
    if (i == elbs.end()) throw NOT_DEFINED(X.first, X.second);  // Check if X is a valid edge.
    return i->right;
}

inline ELB AbstractGraph::get_label(const VID &X, const VID &Y) const { return get_label(EID(X, Y)); }

inline ELB AbstractGraph::get_label(const VLB &X, const VLB &Y) const { return get_label(get_vid(X), get_vid(Y)); }

inline void AbstractGraph::set_label(const EID &X, const ELB &label) {
    if (label.empty()) throw INVALID_LABEL;                          // Check if label is valid.
    if (!IGraph::has_edge(X)) throw NOT_DEFINED(X.first, X.second);  // Check if X is a valid edge.
    // Check if the label as already been defined.
    auto i = elbs.find(label);
    if (i != elbs.end()) {
        // It has been defined for a different edge.
        if (i->left != X) throw DUPLICATED_LABEL(label);
    } else {
        auto j = elbs.find(X);
        if (j == elbs.end()) {
            elbs.insert({X, label});  // Insert label if none found.
        } else {
            elbs.replace(j, {X, label});  // Replace label if one found.
        }
    }
}

inline void AbstractGraph::set_label(const ELB &X, const ELB &label) { set_label(get_eid(X), label); }

inline void AbstractGraph::set_label(const VID &X, const VID &Y, const ELB &label) { set_label(EID(X, Y), label); }

inline void AbstractGraph::set_label(const VLB &X, const VLB &Y, const ELB &label) {
    set_label(get_vid(X), get_vid(Y), label);
}

inline void AbstractGraph::del_label(const EID &X) {
    if (!IGraph::has_edge(X)) throw NOT_DEFINED(X.first, X.second);  // Check if X is a valid edge.
    auto i = elbs.find(X);
    if (i == elbs.end()) throw NO_LABEL(X.first, X.second);  // Check if X is a valid edge.
    elbs.erase(i);
}

inline void AbstractGraph::del_label(const ELB &X) { del_label(get_eid(X)); }

inline void AbstractGraph::del_label(const VID &X, const VID &Y) { del_label(EID(X, Y)); }

inline void AbstractGraph::del_label(const VLB &X, const VLB &Y) { del_label(get_vid(X), get_vid(Y)); }

inline bool AbstractGraph::has_attr(const EID &X, const std::string &key) const {
    if (!IGraph::has_edge(X)) throw NOT_DEFINED(X.first, X.second);  // Check if X is a valid edge.
    auto i = eattrs.find(X);
    return i != eattrs.end() && i->second.find(key) != i->second.end();
}

inline bool AbstractGraph::has_attr(const ELB &X, const std::string &key) const { return has_attr(get_eid(X), key); }

inline bool AbstractGraph::has_attr(const VID &X, const VID &Y, const std::string &key) const {
    return has_attr(EID(X, Y), key);
}

inline bool AbstractGraph::has_attr(const VLB &X, const VLB &Y, const std::string &key) const {
    return has_attr(get_vid(X), get_vid(Y), key);
}

template <typename T>
inline T AbstractGraph::get_attr(const EID &X, const std::string &key) const {
    if (!has_attr(X, key)) throw NO_KEY(key);  // Check if key is valid.
    return std::any_cast<T>(eattrs[X][key]->second);
}

template <typename T>
inline T AbstractGraph::get_attr(const ELB &X, const std::string &key) const {
    return get_attr<T>(get_eid(X), key);
}

template <typename T>
inline T AbstractGraph::get_attr(const VID &X, const VID &Y, const std::string &key) const {
    return get_attr<T>(EID(X, Y), key);
}

template <typename T>
inline T AbstractGraph::get_attr(const VLB &X, const VLB &Y, const std::string &key) const {
    return get_attr<T>(get_vid(X), get_vid(Y), key);
}

template <typename T>
inline void AbstractGraph::set_attr(const EID &X, const std::string &key, const T &value) {
    if (!IGraph::has_edge(X)) throw NOT_DEFINED(X.first, X.second);  // Check if X is a valid vertex.
    eattrs[X][key] = std::make_any(value);
}

template <typename T>
inline void AbstractGraph::set_attr(const ELB &X, const std::string &key, const T &value) {
    set_attr<T>(get_eid(X), key, value);
}

template <typename T>
inline void AbstractGraph::set_attr(const VID &X, const VID &Y, const std::string &key, const T &value) {
    set_attr<T>(EID(X, Y), key, value);
}

template <typename T>
inline void AbstractGraph::set_attr(const VLB &X, const VLB &Y, const std::string &key, const T &value) {
    set_attr<T>(get_vid(X), get_vid(Y), key, value);
}

inline void AbstractGraph::del_attr(const EID &X, const std::string &key) {
    if (!has_attr(X, key)) throw NO_KEY(key);  // Check if key is valid.
    eattrs[X].erase(key);
}

inline void AbstractGraph::del_attr(const ELB &X, const std::string &key) { del_attr(get_eid(X), key); }

inline void AbstractGraph::del_attr(const VID &X, const VID &Y, const std::string &key) { del_attr(EID(X, Y), key); }

inline void AbstractGraph::del_attr(const VLB &X, const VLB &Y, const std::string &key) {
    del_attr(get_vid(X), get_vid(Y), key);
}

inline bool AbstractGraph::has_edge(const ELB &X) const { return IGraph::has_edge(get_eid(X)); }

inline bool AbstractGraph::has_edge(const VID &X, const VID &Y) const { return IGraph::has_edge(EID(X, Y)); }

inline bool AbstractGraph::has_edge(const VLB &X, const VLB &Y) const { return has_edge(get_vid(X), get_vid(Y)); }

inline void AbstractGraph::add_edge(const VID &X, const VID &Y) { IGraph::add_edge(EID(X, Y)); }

inline void AbstractGraph::add_edge(const VLB &X, const VLB &Y) { add_edge(get_vid(X), get_vid(Y)); }

inline void AbstractGraph::del_edge(const EID &X) {
    // Call parent method
    IGraph::del_edge(X);
    // Delete edge label
    auto i = elbs.find(X);
    if (i != elbs.end()) elbs.erase(i);
    // Delete edge attributes
    auto j = eattrs.find(X);
    if (j != eattrs.end()) eattrs.erase(j);
}

inline void AbstractGraph::del_edge(const VID &X, const VID &Y) { del_edge(EID(X, Y)); }

inline void AbstractGraph::del_edge(const VLB &X, const VLB &Y) { del_edge(get_vid(X), get_vid(Y)); }
