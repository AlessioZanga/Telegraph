#include <gtest/gtest.h>

#include <cstdlib>
#include <ctime>
#include <numeric>
#include <telegraph/telegraph>

#define MAX 256

/** METAPROGRAMMING FOR BINARY OPERATORS */

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
using GraphTypes = typename TestTypes<Product<Types<DenseGraph>>>::type;

// Create typed test suite
TYPED_TEST_SUITE(OperatorsTest, GraphTypes);

TYPED_TEST(OperatorsTest, DISABLED_EqualityOperator) {}

TYPED_TEST(OperatorsTest, DISABLED_InequalityOperator) {}

TYPED_TEST(OperatorsTest, DISABLED_ComplementOperator) {}

TYPED_TEST(OperatorsTest, DISABLED_IntersectionOperator) {}

TYPED_TEST(OperatorsTest, DISABLED_UnionOperator) {}

TYPED_TEST(OperatorsTest, DISABLED_DifferenctOperator) {}

TYPED_TEST(OperatorsTest, DISABLED_JointOperator) {}

TYPED_TEST(OperatorsTest, DISABLED_CartesianProductOperator) {}
