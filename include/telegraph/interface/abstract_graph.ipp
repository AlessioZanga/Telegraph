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
    gattrs[key] = std::make_any<T>(value);
}

inline void AbstractGraph::del_attr(const std::string &key) {
    auto i = gattrs.find(key);
    if (i == gattrs.end()) throw NO_KEY(key);
    gattrs.erase(i);
}

inline VID AbstractGraph::get_vid(const VLB &X) const {
    auto i = vlbs.right.find(X);
    if (i == vlbs.right.end()) throw NO_LABEL(X);
    return i->second;
}

inline bool AbstractGraph::has_label(const VID &X) const {
    if (!has_vertex(X)) throw NOT_DEFINED(X);     // Check if X is a valid vertex.
    return vlbs.left.find(X) != vlbs.left.end();  // Check if X has a defined label.
}

inline bool AbstractGraph::has_label(const VLB &X) const { return has_vertex(X); }

inline VLB AbstractGraph::get_label(const VID &X) const {
    if (!has_vertex(X)) throw NOT_DEFINED(X);  // Check if X is a valid vertex.
    auto i = vlbs.left.find(X);
    if (i == vlbs.left.end()) throw NO_LABEL(X);  // Check if X has a defined label.
    return i->second;
}

inline void AbstractGraph::set_label(const VID &X, const VLB &label) {
    if (label.empty()) throw INVALID_LABEL;    // Check if label is valid.
    if (!has_vertex(X)) throw NOT_DEFINED(X);  // Check if X is a valid vertex.
    // Check if the label as already been defined.
    auto i = vlbs.right.find(label);
    if (i != vlbs.right.end()) {
        // It has been defined for a different vertex.
        if (i->second != X) throw DUPLICATED_LABEL(label);
    } else {
        auto j = vlbs.left.find(X);
        if (j == vlbs.left.end()) {
            vlbs.left.insert({X, label});  // Insert label if none found.
        } else {
            vlbs.left.replace(j, {X, label});  // Replace label if one found.
        }
    }
}

inline void AbstractGraph::set_label(const VLB &X, const VLB &label) { set_label(get_vid(X), label); }

inline void AbstractGraph::del_label(const VID &X) {
    if (!has_vertex(X)) throw NOT_DEFINED(X);  // Check if X is a valid vertex.
    auto i = vlbs.left.find(X);
    if (i == vlbs.left.end()) throw NO_LABEL(X);  // Check if X has a valid label.
    vlbs.left.erase(i);
}

inline void AbstractGraph::del_label(const VLB &X) { del_label(get_vid(X)); }

inline bool AbstractGraph::has_attr(const VID &X, const std::string &key) const {
    if (!has_vertex(X)) throw NOT_DEFINED(X);  // Check if X is a valid vertex.
    auto i = vattrs.find(X);
    return i != vattrs.end() && i->second.find(key) != i->second.end();
}

inline bool AbstractGraph::has_attr(const VLB &X, const std::string &key) const { return has_attr(get_vid(X), key); }

template <typename T>
inline T AbstractGraph::get_attr(const VID &X, const std::string &key) const {
    if (!has_attr(X, key)) throw NO_KEY(key);  // Check if key is valid.
    return std::any_cast<T>(vattrs.find(X)->second.find(key)->second);
}

template <typename T>
inline T AbstractGraph::get_attr(const VLB &X, const std::string &key) const {
    return get_attr<T>(get_vid(X), key);
}

template <typename T>
inline void AbstractGraph::set_attr(const VID &X, const std::string &key, const T &value) {
    if (!has_vertex(X)) throw NOT_DEFINED(X);  // Check if X is a valid vertex.
    vattrs[X][key] = std::make_any<T>(value);
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

inline bool AbstractGraph::has_vertex(const VLB &X) const { return vlbs.right.find(X) != vlbs.right.end(); }

inline VID AbstractGraph::add_vertex(const VLB &label) {
    if (label.empty()) throw INVALID_LABEL;                                         // Check if label is valid.
    if (vlbs.right.find(label) != vlbs.right.end()) throw DUPLICATED_LABEL(label);  // Check if label is valid.
    VID X = add_vertex();
    set_label(X, label);
    return X;
}

inline VID AbstractGraph::del_vertex(const VLB &X) { return del_vertex(get_vid(X)); }

inline EID AbstractGraph::get_eid(const ELB &X) const {
    auto i = elbs.right.find(X);
    if (i == elbs.right.end()) throw NO_LABEL(X, "\b\b");  // Use backspaces to patch NO_LABEL argument.
    return i->second;
}

inline bool AbstractGraph::has_label(const EID &X) const {
    if (!has_edge(X)) throw NOT_DEFINED(X.first, X.second);  // Check if X is a valid edge.
    return elbs.left.find(X) != elbs.left.end();
}

inline bool AbstractGraph::has_label(const ELB &X) const { return has_edge(X); }

inline bool AbstractGraph::has_label(const VID &X, const VID &Y) const { return has_label(EID(X, Y)); }

inline bool AbstractGraph::has_label(const VLB &X, const VLB &Y) const {
    return has_label(std::make_pair(get_vid(X), get_vid(Y)));
}

inline ELB AbstractGraph::get_label(const EID &X) const {
    auto i = elbs.left.find(X);
    if (i == elbs.left.end()) throw NOT_DEFINED(X.first, X.second);  // Check if X is a valid edge.
    return i->second;
}

inline ELB AbstractGraph::get_label(const VID &X, const VID &Y) const { return get_label(EID(X, Y)); }

inline ELB AbstractGraph::get_label(const VLB &X, const VLB &Y) const { return get_label(get_vid(X), get_vid(Y)); }

inline void AbstractGraph::set_label(const EID &X, const ELB &label) {
    if (label.empty()) throw INVALID_LABEL;                  // Check if label is valid.
    if (!has_edge(X)) throw NOT_DEFINED(X.first, X.second);  // Check if X is a valid edge.
    // Check if the label as already been defined.
    auto i = elbs.right.find(label);
    if (i != elbs.right.end()) {
        // It has been defined for a different edge.
        if (i->second != X) throw DUPLICATED_LABEL(label);
    } else {
        auto j = elbs.left.find(X);
        if (j == elbs.left.end()) {
            elbs.left.insert({X, label});  // Insert label if none found.
        } else {
            elbs.left.replace(j, {X, label});  // Replace label if one found.
        }
    }
}

inline void AbstractGraph::set_label(const ELB &X, const ELB &label) { set_label(get_eid(X), label); }

inline void AbstractGraph::set_label(const VID &X, const VID &Y, const ELB &label) { set_label(EID(X, Y), label); }

inline void AbstractGraph::set_label(const VLB &X, const VLB &Y, const ELB &label) {
    set_label(get_vid(X), get_vid(Y), label);
}

inline void AbstractGraph::del_label(const EID &X) {
    if (!has_edge(X)) throw NOT_DEFINED(X.first, X.second);  // Check if X is a valid edge.
    auto i = elbs.left.find(X);
    if (i == elbs.left.end()) throw NO_LABEL(X.first, X.second);  // Check if X is a valid edge.
    elbs.left.erase(i);
}

inline void AbstractGraph::del_label(const ELB &X) { del_label(get_eid(X)); }

inline void AbstractGraph::del_label(const VID &X, const VID &Y) { del_label(EID(X, Y)); }

inline void AbstractGraph::del_label(const VLB &X, const VLB &Y) { del_label(get_vid(X), get_vid(Y)); }

inline bool AbstractGraph::has_attr(const EID &X, const std::string &key) const {
    if (!has_edge(X)) throw NOT_DEFINED(X.first, X.second);  // Check if X is a valid edge.
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
    return std::any_cast<T>(eattrs.find(X)->second.find(key)->second);
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
    if (!has_edge(X)) throw NOT_DEFINED(X.first, X.second);  // Check if X is a valid vertex.
    eattrs[X][key] = std::make_any<T>(value);
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

inline bool AbstractGraph::has_edge(const ELB &X) const { return elbs.right.find(X) != elbs.right.end(); }

inline bool AbstractGraph::has_edge(const VID &X, const VID &Y) const { return has_edge(EID(X, Y)); }

inline bool AbstractGraph::has_edge(const VLB &X, const VLB &Y) const { return has_edge(get_vid(X), get_vid(Y)); }

inline EID AbstractGraph::add_edge(const VID &X, const VID &Y) { return add_edge(EID(X, Y)); }

inline EID AbstractGraph::add_edge(const VLB &X, const VLB &Y) { return add_edge(get_vid(X), get_vid(Y)); }

inline EID AbstractGraph::del_edge(const ELB &X) { return del_edge(get_eid(X)); }

inline EID AbstractGraph::del_edge(const VID &X, const VID &Y) { return del_edge(EID(X, Y)); }

inline EID AbstractGraph::del_edge(const VLB &X, const VLB &Y) { return del_edge(get_vid(X), get_vid(Y)); }
