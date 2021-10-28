#pragma once

#include <Eigen/Eigen>
#include <algorithm>
#include <any>
#include <boost/bimap.hpp>
#include <boost/bimap/support/lambda.hpp>
#include <boost/bimap/unordered_set_of.hpp>
#include <boost/container_hash/hash.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <cstdint>
#include <iostream>
#include <iterator>
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

//! Bidirectional VID-VLB map.
using VIDmVLB = boost::bimap<VID, VLB>;
//! Bidirectional EID-ELB map.
using EIDmELB = boost::bimap<EID, ELB>;
//! Bidirectional VID-Index map.
using VIDmIndex = boost::bimap<boost::bimaps::unordered_set_of<VID>, AdjacencyMatrix::Index>;

//! Graph attributes.
using GAttrs = std::map<const std::string, std::any>;
//! Vertex attributes.
using VAttrs = std::map<VID, std::map<const std::string, std::any>>;
//! Edge attributes.
using EAttrs = std::map<EID, std::map<const std::string, std::any>>;

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

//! Infix vertex pairs set iterator.
template <typename T>
static inline auto Vp(const T &G) {
    return G.Vp();
}

//! Infix edge pairs set iterator.
template <typename T>
static inline auto Ep(const T &G) {
    return G.Ep();
}

namespace std {

template <>
struct hash<EID> {
    std::size_t operator()(const EID &value) const {
        // Initialize seed hash.
        std::size_t seed = 0;
        // Hash EID.
        boost::hash_combine(seed, value.first);
        boost::hash_combine(seed, value.second);
        // Return hash.
        return seed;
    }
};

}  // namespace std

namespace boost {

template <>
struct hash<VIDmVLB::left_value_type> {
    std::size_t operator()(const VIDmVLB::left_value_type &value) const {
        // Initialize seed hash.
        std::size_t seed = 0;
        // Hash VID.
        boost::hash_combine(seed, value.first);
        // Hash VLB.
        boost::hash_combine(seed, value.second);
        // Return hash.
        return seed;
    }
};

template <>
struct hash<EIDmELB::left_value_type> {
    std::size_t operator()(const EIDmELB::left_value_type &value) const {
        // Initialize seed hash.
        std::size_t seed = 0;
        // Hash EID.
        boost::hash_combine(seed, value.first);
        // Hash ELB.
        boost::hash_combine(seed, value.second);
        // Return hash.
        return seed;
    }
};

}  // namespace boost
