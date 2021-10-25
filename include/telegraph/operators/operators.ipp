#pragma once

#include <telegraph/operators/operators.hpp>

template <typename A, typename B, require_base_of(AbstractGraph, A), require_base_of(AbstractGraph, B)>
bool operator==(const A &G, const B &H) {
    if (G.order() != H.order() || G.size() != H.size()) return false;
    if ((G.has_label() != H.has_label()) || (G.has_label() && G.get_label() != H.get_label())) return false;
    if (!std::equal(V(G).begin(), V(G).end(), V(H).begin())) return false;
    if (!std::equal(E(G).begin(), E(G).end(), E(H).begin())) return false;
    if (!std::equal(Vp(G).begin(), Vp(G).end(), Vp(H).begin())) return false;
    if (!std::equal(Ep(G).begin(), Ep(G).end(), Ep(H).begin())) return false;
    return true;
}

template <typename A, typename B, require_base_of(AbstractGraph, A), require_base_of(AbstractGraph, B)>
bool operator!=(const A &G, const B &H) {
    return !(G == H);
}

template <typename A, typename B, require_base_of(AbstractGraph, A), require_base_of(AbstractGraph, B)>
bool operator<(const A &G, const B &H) {
    return H > G;
}

template <typename A, typename B, require_base_of(AbstractGraph, A), require_base_of(AbstractGraph, B)>
bool operator>(const A &G, const B &H) {
    if (G.order() < H.order() || G.size() < H.size()) return false;
    if (H.has_label() && (!G.has_label() || G.get_label() != H.get_label())) return false;
    if (!std::includes(V(G).begin(), V(G).end(), V(H).begin(), V(H).end())) return false;
    if (!std::includes(E(G).begin(), E(G).end(), E(H).begin(), E(H).end())) return false;
    if (!std::includes(Vp(G).begin(), Vp(G).end(), Vp(H).begin(), Vp(H).end())) return false;
    if (!std::includes(Ep(G).begin(), Ep(G).end(), Ep(H).begin(), Ep(H).end())) return false;
    if (G == H) return false;
    return true;
}

template <typename A, typename B, require_base_of(AbstractGraph, A), require_base_of(AbstractGraph, B)>
bool operator<=(const A &G, const B &H) {
    return H >= G;
}

template <typename A, typename B, require_base_of(AbstractGraph, A), require_base_of(AbstractGraph, B)>
bool operator>=(const A &G, const B &H) {
    return G > H || G == H;
}

template <typename A, typename B, require_base_of(AbstractGraph, A), require_base_of(AbstractGraph, B)>
B operator~(const A &G) {
    // Copy graph.
    B out(G);
    // Invert edges.
    const std::size_t n = out.order();
    for (VID i = 0; i < n; i++) {
        for (VID j = 0; j < n; j++) {
            if (out.has_edge(i, j)) {
                out.del_edge(i, j);
            } else {
                out.add_edge(i, j);
            }
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
