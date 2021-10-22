#pragma once

#include <telegraph/operators/operators.hpp>

template <typename A, typename B, require_base_of(AbstractGraph, A), require_base_of(AbstractGraph, B)>
bool operator==(const A &G, const B &H) {
    if (G.order() != H.order()) return false;
    if (G.size() != H.size()) return false;
    if (G.has_label() != H.has_label() && (G.has_label() && G.get_label() != H.get_label())) return false;
    if (!std::equal(V(G).begin(), V(G).end(), V(H).begin())) return false;
    if (!std::equal(E(G).begin(), E(G).end(), E(H).begin())) return false;
    if (!std::equal(Vl(G).begin(), Vl(G).end(), Vl(H).begin())) return false;
    if (!std::equal(El(G).begin(), El(G).end(), El(H).begin())) return false;
    return true;
}

template <typename A, typename B, require_base_of(AbstractGraph, A), require_base_of(AbstractGraph, B)>
bool operator!=(const A &G, const B &H) { return !(G == H); }

template <typename A, typename B, require_base_of(AbstractGraph, A), require_base_of(AbstractGraph, B)>
B operator~(const A &G) {
    const std::size_t n = G.order();
    B out(n);
    for (VID i = 0; i < n; i++) {
        for (VID j = 0; j < n; j++) {
            if (!G.has_edge(i, j)) out.add_edge(i, j);
        }
    }
    return out;
}

template <
    typename A,
    typename B,
    typename C,
    require_base_of(AbstractGraph, A),
    require_base_of(AbstractGraph, B),
    require_base_of(AbstractGraph, C)>
C operator&(const A &G, const B &H) {}

template <
    typename A,
    typename B,
    typename C,
    require_base_of(AbstractGraph, A),
    require_base_of(AbstractGraph, B),
    require_base_of(AbstractGraph, C)>
C operator|(const A &G, const B &H) {}

template <
    typename A,
    typename B,
    typename C,
    require_base_of(AbstractGraph, A),
    require_base_of(AbstractGraph, B),
    require_base_of(AbstractGraph, C)>
C operator/(const A &G, const B &H) {}

template <
    typename A,
    typename B,
    typename C,
    require_base_of(AbstractGraph, A),
    require_base_of(AbstractGraph, B),
    require_base_of(AbstractGraph, C)>
C operator*(const A &G, const B &H) {}

template <
    typename A,
    typename B,
    typename C,
    require_base_of(AbstractGraph, A),
    require_base_of(AbstractGraph, B),
    require_base_of(AbstractGraph, C)>
C operator%(const A &G, const B &H) {}
