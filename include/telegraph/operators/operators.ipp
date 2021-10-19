#pragma once

#include <telegraph/operators/operators.hpp>

template <typename A, typename B, require_base_of(AbstractGraph, A), require_base_of(AbstractGraph, B)>
bool operator==(const A &X, const B &Y) {}

template <typename A, typename B, require_base_of(AbstractGraph, A), require_base_of(AbstractGraph, B)>
bool operator!=(const A &X, const B &Y) {}

template <typename A, typename B, require_base_of(AbstractGraph, A), require_base_of(AbstractGraph, B)>
B operator~(const A &X) {}

template <
    typename A,
    typename B,
    typename C,
    require_base_of(AbstractGraph, A),
    require_base_of(AbstractGraph, B),
    require_base_of(AbstractGraph, C)>
C operator&(const A &X, const B &Y) {}

template <
    typename A,
    typename B,
    typename C,
    require_base_of(AbstractGraph, A),
    require_base_of(AbstractGraph, B),
    require_base_of(AbstractGraph, C)>
C operator|(const A &X, const B &Y) {}

template <
    typename A,
    typename B,
    typename C,
    require_base_of(AbstractGraph, A),
    require_base_of(AbstractGraph, B),
    require_base_of(AbstractGraph, C)>
C operator/(const A &X, const B &Y) {}

template <
    typename A,
    typename B,
    typename C,
    require_base_of(AbstractGraph, A),
    require_base_of(AbstractGraph, B),
    require_base_of(AbstractGraph, C)>
C operator*(const A &X, const B &Y) {}

template <
    typename A,
    typename B,
    typename C,
    require_base_of(AbstractGraph, A),
    require_base_of(AbstractGraph, B),
    require_base_of(AbstractGraph, C)>
C operator%(const A &X, const B &Y) {}
