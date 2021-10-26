#include <gtest/gtest.h>

#include <cstdlib>
#include <ctime>
#include <numeric>
#include <telegraph/telegraph>

#define MAX 256

/** TEMPLATE PROGRAMMING FOR BINARY OPERATORS */

template <class... Args>
struct Types {};

template <class TypeList, template <class> class Mapper>
struct Map {
    using type = void;
};

template <template <class> class Mapper, class... Args>
struct Map<Types<Args...>, Mapper> {
    using type = Types<Mapper<Args>...>;
};

template <class Arg, class TypeList>
struct Pairs {
    template <class T>
    using make_pair = std::pair<T, Arg>;
    using type = typename Map<TypeList, make_pair>::type;
};

template <class T, class... Ts>
struct Concat {
    using type = void;
};

template <class... Ts>
struct Concat<Types<Ts...>> {
    using type = Types<Ts...>;
};

template <class... Ts, class... Us>
struct Concat<Types<Ts...>, Types<Us...>> {
    using type = typename Concat<Types<Ts..., Us...>>::type;
};

template <class... Ts, class... Us>
struct Concat<Types<Ts...>, Us...> {
    using type = typename Concat<Types<Ts...>, typename Concat<Us...>::type>::type;
};

template <class T, template <class...> class Reducer>
struct Reduce {
    using type = void;
};

template <class... Args, template <class...> class Reducer>
struct Reduce<Types<Args...>, Reducer> {
    using type = typename Reducer<Args...>::type;
};

template <class T, class U>
struct CartesianProduct {
    using type = void;
};

template <class T, class... Args>
struct CartesianProduct<T, Types<Args...>> {
    using type = typename Reduce<Types<typename Pairs<Args, T>::type...>, Concat>::type;
};

template <class T, class U = T>
using Product = typename CartesianProduct<T, U>::type;

template <class T>
struct TestTypes {
    using type = void;
};

template <class... Ts>
struct TestTypes<Types<Ts...>> {
    using type = ::testing::Types<Ts...>;
};

// Define fixture class template
template <typename T>
class OperatorsTest : public ::testing::Test {};

// Define list of types
using GraphTypes = typename TestTypes<Product<Types<DirectedDenseGraph>>>::type;

// Create typed test suite
TYPED_TEST_SUITE(OperatorsTest, GraphTypes);

TYPED_TEST(OperatorsTest, Equality_InequalityOperator) {
    typename TypeParam::first_type G;
    typename TypeParam::second_type H;
    ASSERT_EQ(G, H);

    G.add_vertex();
    ASSERT_NE(G, H);
    H.add_vertex();
    ASSERT_EQ(G, H);

    G.add_vertex();
    H.add_vertex();

    G.add_edge(0, 1);
    ASSERT_NE(G, H);
    H.add_edge(0, 1);
    ASSERT_EQ(G, H);

    G.set_label("G");
    ASSERT_NE(G, H);
    H.set_label("G");
    ASSERT_EQ(G, H);

    G.set_label(0, "0");
    ASSERT_NE(G, H);
    H.set_label(0, "0");
    ASSERT_EQ(G, H);

    G.set_label(0, 1, ELB("0 --- 1"));
    ASSERT_NE(G, H);
    H.set_label(0, 1, ELB("0 --- 1"));
    ASSERT_EQ(G, H);

    G.set_label(1, "1");
    G.add_vertex("2");
    H.set_label(1, "2");
    H.add_vertex("1");
    ASSERT_NE(G, H);

    G.del_label(1);
    G.del_label(2);
    H.del_label(1);
    H.del_label(2);

    G.add_edge(1, 1);
    G.set_label(1, 1, ELB("1 --- 1"));
    H.add_edge(2, 2);
    H.set_label(2, 2, ELB("1 --- 1"));
    ASSERT_NE(G, H);
}

TYPED_TEST(OperatorsTest, LT_GT_LE_GE_Operator) {
    typename TypeParam::first_type G;
    typename TypeParam::second_type H;
    ASSERT_FALSE(H < G);
    ASSERT_FALSE(G > H);
    ASSERT_LE(H, G);
    ASSERT_GE(G, H);
    ASSERT_EQ(G, H);

    G.add_vertex();
    ASSERT_LT(H, G);
    ASSERT_GT(G, H);
    ASSERT_LE(H, G);
    ASSERT_GE(G, H);

    G.add_vertex();
    G.add_edge(0, 1);
    ASSERT_LT(H, G);
    ASSERT_GT(G, H);
    ASSERT_LE(H, G);
    ASSERT_GE(G, H);

    H.add_vertex();
    ASSERT_LT(H, G);
    ASSERT_GT(G, H);
    ASSERT_LE(H, G);
    ASSERT_GE(G, H);

    H.add_vertex();
    ASSERT_LT(H, G);
    ASSERT_GT(G, H);
    ASSERT_LE(H, G);
    ASSERT_GE(G, H);

    H.add_edge(0, 1);
    ASSERT_FALSE(H < G);
    ASSERT_FALSE(G > H);
    ASSERT_LE(H, G);
    ASSERT_GE(G, H);
    ASSERT_EQ(G, H);

    G.add_edge(1, 1);
    ASSERT_LT(H, G);
    ASSERT_GT(G, H);
    ASSERT_LE(H, G);
    ASSERT_GE(G, H);

    H.add_edge(1, 1);
    ASSERT_FALSE(H < G);
    ASSERT_FALSE(G > H);
    ASSERT_LE(H, G);
    ASSERT_GE(G, H);
    ASSERT_EQ(G, H);

    G.set_label("G");  // Check for labels copy.
    ASSERT_LT(H, G);
    ASSERT_GT(G, H);
    ASSERT_LE(H, G);
    ASSERT_GE(G, H);

    H.set_label("G");
    ASSERT_FALSE(H < G);
    ASSERT_FALSE(G > H);
    ASSERT_LE(H, G);
    ASSERT_GE(G, H);
    ASSERT_EQ(G, H);

    G.set_label(0, "0");
    ASSERT_LT(H, G);
    ASSERT_GT(G, H);
    ASSERT_LE(H, G);
    ASSERT_GE(G, H);

    H.set_label(0, "0");
    ASSERT_FALSE(H < G);
    ASSERT_FALSE(G > H);
    ASSERT_LE(H, G);
    ASSERT_GE(G, H);
    ASSERT_EQ(G, H);

    G.set_label(0, 1, ELB("0 --- 1"));
    ASSERT_LT(H, G);
    ASSERT_GT(G, H);
    ASSERT_LE(H, G);
    ASSERT_GE(G, H);

    H.set_label(0, 1, ELB("0 --- 1"));
    ASSERT_FALSE(H < G);
    ASSERT_FALSE(G > H);
    ASSERT_LE(H, G);
    ASSERT_GE(G, H);
    ASSERT_EQ(G, H);

    // Assert partial ordering.
    H.set_label(0, 1, ELB("0 -*- 1"));
    ASSERT_FALSE(G < H);
    ASSERT_FALSE(G > H);
    ASSERT_FALSE(G <= H);
    ASSERT_FALSE(G >= H);
    ASSERT_FALSE(G == H);
    ASSERT_NE(G, H);
}

TYPED_TEST(OperatorsTest, ComplementOperator) {
    typename TypeParam::first_type G;
    typename TypeParam::second_type H;

    G.set_label("G");  // Check for labels copy.

    H = ~G;
    ASSERT_EQ(G, H);

    G.add_vertex();
    G.set_label(0, "0");  // Check for labels copy.
    H = ~G;
    ASSERT_NE(G, H);
    ASSERT_EQ(G.order(), H.order());
    ASSERT_NE(G.size(), H.size());
    ASSERT_TRUE(std::equal(V(G).begin(), V(G).end(), V(H).begin()));
    ASSERT_TRUE(std::equal(Vp(G).begin(), Vp(G).end(), Vp(H).begin()));

    EIDs X, Y, Z;
    X = {{0, 1}, {1, 0}};
    G = typename TypeParam::first_type(X.begin(), X.end());
    H = ~G;

    Y = EIDs(E(H).begin(), E(H).end());
    ASSERT_EQ(Y, EIDs({{0, 0}, {1, 1}}));

    std::set_intersection(X.begin(), X.end(), Y.begin(), Y.end(), std::inserter(Z, Z.begin()));
    ASSERT_EQ(Z.size(), 0);
    std::set_union(X.begin(), X.end(), Y.begin(), Y.end(), std::inserter(Z, Z.begin()));
    ASSERT_EQ(Z.size(), (G.order() * G.order()));
}

TYPED_TEST(OperatorsTest, DISABLED_IntersectionOperator) {}

TYPED_TEST(OperatorsTest, DISABLED_UnionOperator) {}

TYPED_TEST(OperatorsTest, DISABLED_DifferenctOperator) {}

TYPED_TEST(OperatorsTest, DISABLED_JointOperator) {}

TYPED_TEST(OperatorsTest, DISABLED_CartesianProductOperator) {}
