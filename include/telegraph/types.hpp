#pragma once

#include <Eigen/Eigen>
#include <any>
#include <algorithm>
#include <boost/bimap.hpp>
#include <boost/bimap/support/lambda.hpp>
#include <boost/container_hash/hash.hpp>
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
