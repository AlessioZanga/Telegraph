#pragma once

#include <telegraph/containers/abstract_graph.hpp>
#include <telegraph/requires.hpp>

/**
 * @brief Equality of graphs.
 *
 * @tparam A First graph type.
 * @tparam B Second graph type.
 * @param X First graph.
 * @param Y Second graph.
 * @return true If the graphs are equal,
 * @return false Otherwise.
 */
template <typename A, typename B, require_base_of(AbstractGraph, A) = 0, require_base_of(AbstractGraph, B) = 0>
bool operator==(const A &X, const B &Y);

/**
 * @brief Inequality of graphs.
 *
 * @tparam A First graph type.
 * @tparam B Second graph type.
 * @param X First graph.
 * @param Y Second graph.
 * @return true If the graphs are *not* equal,
 * @return false Otherwise.
 */
template <typename A, typename B, require_base_of(AbstractGraph, A) = 0, require_base_of(AbstractGraph, B) = 0>
bool operator!=(const A &X, const B &Y);

/**
 * @brief Complement of graph.
 *
 * @tparam A Graph type.
 * @tparam C Return graph type (defaults to given graph type).
 * @param X Given graph.
 * @return C Return graph.
 */
template <typename A, typename B = A, require_base_of(AbstractGraph, A) = 0, require_base_of(AbstractGraph, B) = 0>
B operator~(const A &X);

/**
 * @brief Intersection of graphs.
 *
 * @tparam A First graph type.
 * @tparam B Second graph type.
 * @tparam C Return graph type (defaults to first graph type).
 * @param X First graph.
 * @param Y Second graph.
 * @return C Return graph.
 */
template <typename A, typename B, typename C = A, require_base_of(AbstractGraph, A) = 0,
          require_base_of(AbstractGraph, B) = 0, require_base_of(AbstractGraph, C) = 0>
C operator&(const A &X, const B &Y);

/**
 * @brief Union of graphs.
 *
 * @tparam A First graph type.
 * @tparam B Second graph type.
 * @tparam C Return graph type (defaults to first graph type).
 * @param X First graph.
 * @param Y Second graph.
 * @return C Return graph.
 */
template <typename A, typename B, typename C = A, require_base_of(AbstractGraph, A) = 0,
          require_base_of(AbstractGraph, B) = 0, require_base_of(AbstractGraph, C) = 0>
C operator|(const A &X, const B &Y);

/**
 * @brief Difference of graphs.
 *
 * @tparam A First graph type.
 * @tparam B Second graph type.
 * @tparam C Return graph type (defaults to first graph type).
 * @param X First graph.
 * @param Y Second graph.
 * @return C Return graph.
 */
template <typename A, typename B, typename C = A, require_base_of(AbstractGraph, A) = 0,
          require_base_of(AbstractGraph, B) = 0, require_base_of(AbstractGraph, C) = 0>
C operator/(const A &X, const B &Y);

/**
 * @brief Join of graphs.
 *
 * @tparam A First graph type.
 * @tparam B Second graph type.
 * @tparam C Return graph type (defaults to first graph type).
 * @param X First graph.
 * @param Y Second graph.
 * @return C Return graph.
 */
template <typename A, typename B, typename C = A, require_base_of(AbstractGraph, A) = 0,
          require_base_of(AbstractGraph, B) = 0, require_base_of(AbstractGraph, C) = 0>
C operator*(const A &X, const B &Y);

/**
 * @brief Cartesian product of graphs.
 *
 * @tparam A First graph type.
 * @tparam B Second graph type.
 * @tparam C Return graph type (defaults to first graph type).
 * @param X First graph.
 * @param Y Second graph.
 * @return C Return graph.
 */
template <typename A, typename B, typename C = A, require_base_of(AbstractGraph, A) = 0,
          require_base_of(AbstractGraph, B) = 0, require_base_of(AbstractGraph, C) = 0>
C operator%(const A &X, const B &Y);
