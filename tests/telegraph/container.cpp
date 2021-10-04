#include <gtest/gtest.h>

#include <telegraph/telegraph>

// Define fixture class template
template <typename T>
class ContainerTest : public ::testing::Test {};

// Define list of types
using GraphTypes = ::testing::Types<DenseGraph>;

// Create typed test suite
TYPED_TEST_SUITE(ContainerTest, GraphTypes);

TYPED_TEST(ContainerTest, DefaultContructor) { TypeParam g; }

TYPED_TEST(ContainerTest, CopyConstructor) {
    TypeParam g;
    TypeParam h(g);
}

TYPED_TEST(ContainerTest, AssignmentOperator) {
    TypeParam g;
    TypeParam h = g;
}

TYPED_TEST(ContainerTest, Destructor) {
    TypeParam *g = new TypeParam();
    delete g;
}
