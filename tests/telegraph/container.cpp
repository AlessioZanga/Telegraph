#include <gtest/gtest.h>

#include <cstdlib>
#include <ctime>
#include <numeric>
#include <telegraph/telegraph>

#define MAX 256

// Define fixture class template
template <typename T>
class ContainerTest : public ::testing::Test {};

// Define list of types
using GraphTypes = ::testing::Types<DenseGraph>;

// Create typed test suite
TYPED_TEST_SUITE(ContainerTest, GraphTypes);

TYPED_TEST(ContainerTest, DefaultContructor) { TypeParam G; }

TYPED_TEST(ContainerTest, CopyConstructor) {
    TypeParam G;
    TypeParam H(G);
}

TYPED_TEST(ContainerTest, AssignmentOperator) {
    TypeParam G;
    TypeParam H = G;
}

TYPED_TEST(ContainerTest, Destructor) {
    TypeParam *G = new TypeParam();
    delete G;
}

TYPED_TEST(ContainerTest, SizedConstructor) {
    TypeParam G(0);  // Zero lower bound.
    ASSERT_EQ(G.order(), 0);

    TypeParam H(1);  // Lower value.
    ASSERT_EQ(H.order(), 1);

    std::srand(std::time(0));           // Use current time as seed.
    std::size_t n = std::rand() % MAX;  // Generate random number.
    TypeParam J(n);
    ASSERT_EQ(J.order(), n);

    TypeParam K(MAX);  // Very high bound.
    ASSERT_EQ(K.order(), MAX);
}

TYPED_TEST(ContainerTest, VertexIteratorConstructor) {
    std::vector<VID> V = {};  // Empty sequence.
    TypeParam G(V.begin(), V.end());
    ASSERT_EQ(G.order(), V.size());

    std::vector<VID> W = {0};  // Single sequence.
    TypeParam H(W.begin(), W.end());
    ASSERT_EQ(H.order(), W.size());

    std::vector<VID> X = {0, 1, 2, 3, 4};  // Ordered sequence.
    TypeParam J(X.begin(), X.end());
    ASSERT_EQ(J.order(), X.size());

    std::vector<VID> Y = {2, 1, 3, 4, 0};  // Unordered sequence.
    TypeParam K(Y.begin(), Y.end());
    ASSERT_EQ(K.order(), Y.size());

    std::vector<VID> Z(MAX);  // Very long sequence.
    std::iota(Z.begin(), Z.end(), 0);
    TypeParam L(Z.begin(), Z.end());
    ASSERT_EQ(L.order(), Z.size());

    std::list<VID> N(MAX);  // Very long sequence with non-contiguous memory.
    std::iota(N.begin(), N.end(), 0);
    TypeParam M(N.begin(), N.end());
    ASSERT_EQ(M.order(), N.size());
}

TYPED_TEST(ContainerTest, EdgeIteratorConstructor) {
    std::vector<EID> E = {};  // Empty sequence.
    TypeParam G(E.begin(), E.end());
    ASSERT_EQ(G.size(), E.size());

    std::vector<EID> W = {{0, 1}};  // Single sequence.
    TypeParam H(W.begin(), W.end());
    ASSERT_EQ(H.size(), W.size());

    std::vector<EID> X = {{0, 1}, {1, 2}, {2, 3}};  // Ordered sequence.
    TypeParam J(X.begin(), X.end());
    ASSERT_EQ(J.size(), X.size());

    std::vector<EID> Y = {{3, 0}, {2, 1}, {2, 0}};  // Unordered sequence.
    TypeParam K(Y.begin(), Y.end());
    ASSERT_EQ(K.size(), Y.size());

    std::vector<EID> Z;  // Very long sequence.
    for (VID i = 0; i < MAX; i++) {
        for (VID j = 0; j < i; j++) {
            Z.push_back({i, j});
        }
    }
    TypeParam L(Z.begin(), Z.end());
    ASSERT_EQ(L.size(), Z.size());

    std::list<EID> N;  // Very long sequence with non-contiguous memory.
    for (VID i = 0; i < MAX; i++) {
        for (VID j = 0; j < i; j++) {
            N.push_back({i, j});
        }
    }
    TypeParam M(N.begin(), N.end());
    ASSERT_EQ(M.size(), M.size());
}

TYPED_TEST(ContainerTest, AdjacencyListConstructor) {
    AdjacencyList A;  // Empty sequence.
    TypeParam G(A);
    ASSERT_EQ(G.order(), 0);
    ASSERT_EQ(G.size(), 0);

    AdjacencyList B = {{0, {}}};  // Single sequence.
    TypeParam H(B);
    ASSERT_EQ(H.order(), 1);
    ASSERT_EQ(H.size(), 0);

    AdjacencyList C = {{0, {0, 1, 3}}, {1, {1, 2}}, {2, {4}}, {3, {}}, {4, {0, 2, 3, 4}}};  // Multiple sequence.
    TypeParam J(C);
    ASSERT_EQ(J.order(), 5);
    ASSERT_EQ(J.size(), 10);

    AdjacencyList D = {{0, {0, 2}}, {2, {0}}};  // Invalid input (1)
    ASSERT_ANY_THROW({ TypeParam K(D); });

    AdjacencyList E = {{0, {3}}, {1, {0}}};  // Invalid input (2)
    ASSERT_ANY_THROW({ TypeParam L(E); });
}

TYPED_TEST(ContainerTest, AdjacencyMatrixConstructor) {
    AdjacencyMatrix A;  // Empty sequence.
    TypeParam G(A);
    ASSERT_EQ(G.order(), 0);
    ASSERT_EQ(G.size(), 0);

    AdjacencyMatrix B(1, 1);
    B << 0;  // Single sequence.
    TypeParam H(B);
    ASSERT_EQ(H.order(), 1);
    ASSERT_EQ(H.size(), 0);

    AdjacencyMatrix C(3, 3);
    C << 0, 1, 0, 0, 0, 1, 0, 0, 0;  // Multiple sequence.
    TypeParam J(C);
    ASSERT_EQ(J.order(), 3);
    ASSERT_EQ(J.size(), 2);

    AdjacencyMatrix D(3, 3);
    D << 0, 1, 0, 0, 0, 3, 0, 0, 0;  // Non-binary sequence.
    TypeParam K(D);
    ASSERT_EQ(K.order(), 3);
    ASSERT_EQ(K.size(), 2);

    AdjacencyMatrix E(2, 3);  // Invalid input (1)
    ASSERT_ANY_THROW({ TypeParam L(E); });
}

TYPED_TEST(ContainerTest, SparseAdjacencyMatrixConstructor) {
    SparseAdjacencyMatrix A;  // Empty sequence.
    TypeParam G(A);
    ASSERT_EQ(G.order(), 0);
    ASSERT_EQ(G.size(), 0);

    SparseAdjacencyMatrix B(1, 1);
    B.coeffRef(0, 0) = 0;  // Single sequence.
    TypeParam H(B);
    ASSERT_EQ(H.order(), 1);
    ASSERT_EQ(H.size(), 0);

    SparseAdjacencyMatrix C(3, 3);
    C.coeffRef(0, 1) = 1;
    C.coeffRef(1, 2) = 1;  // Multiple sequence.
    TypeParam J(C);
    ASSERT_EQ(J.order(), 3);
    ASSERT_EQ(J.size(), 2);

    SparseAdjacencyMatrix D(3, 3);
    D.coeffRef(0, 1) = 1;
    D.coeffRef(1, 2) = 3;  // Non-binary sequence.
    TypeParam K(D);
    ASSERT_EQ(K.order(), 3);
    ASSERT_EQ(K.size(), 2);

    SparseAdjacencyMatrix E(2, 3);  // Invalid input (1)
    ASSERT_ANY_THROW({ TypeParam L(E); });
}

TYPED_TEST(ContainerTest, AdjacencyListOperator) {
    AdjacencyList A;
    TypeParam G;
    ASSERT_EQ(((AdjacencyList)G), A);

    AdjacencyList B = {{0, {0, 1, 3}}, {1, {1, 2}}, {2, {4}}, {3, {}}, {4, {0, 2, 3, 4}}};
    TypeParam H(B);
    ASSERT_EQ(((AdjacencyList)H), B);
}

TYPED_TEST(ContainerTest, AdjacencyMatrixOperator) {
    AdjacencyMatrix A;
    TypeParam G;
    ASSERT_EQ(((AdjacencyMatrix)G), A);

    AdjacencyMatrix B(3, 3);
    B << 0, 1, 0, 0, 0, 1, 0, 0, 0;  // Multiple sequence.
    TypeParam H(B);
    ASSERT_EQ(((AdjacencyMatrix)H), B);
}

TYPED_TEST(ContainerTest, SparseAdjacencyMatrixOperator) {
    SparseAdjacencyMatrix A(1, 1);
    TypeParam G(A);
    ASSERT_TRUE(((SparseAdjacencyMatrix)G).isApprox(A, 0));

    SparseAdjacencyMatrix B(3, 3);
    B.coeffRef(0, 1) = 1;
    B.coeffRef(1, 2) = 1;  // Multiple sequence.
    TypeParam H(B);
    ASSERT_TRUE(((SparseAdjacencyMatrix)H).isApprox(B, 0));
}

TYPED_TEST(ContainerTest, Order) {
    TypeParam G;
    ASSERT_EQ(G.order(), 0);

    TypeParam H(0);
    ASSERT_EQ(H.order(), 0);

    for (std::size_t i = 1; i < MAX; i++) {
        TypeParam J(i);
        ASSERT_EQ(J.order(), i);
    }
}

TYPED_TEST(ContainerTest, DISABLED_Size) {}

TYPED_TEST(ContainerTest, DISABLED_GetVertexID) {}

TYPED_TEST(ContainerTest, DISABLED_HasVertex0) {}

TYPED_TEST(ContainerTest, DISABLED_HasVertex1) {}

TYPED_TEST(ContainerTest, DISABLED_AddVertex0) {}

TYPED_TEST(ContainerTest, DISABLED_AddVertex1) {}

TYPED_TEST(ContainerTest, DISABLED_AddVertex2) {}

TYPED_TEST(ContainerTest, DISABLED_DelVertex0) {}

TYPED_TEST(ContainerTest, DISABLED_DelVertex1) {}

TYPED_TEST(ContainerTest, DISABLED_GetEdgeID) {}

TYPED_TEST(ContainerTest, DISABLED_HasEdge0) {}

TYPED_TEST(ContainerTest, DISABLED_HasEdge1) {}

TYPED_TEST(ContainerTest, DISABLED_HasEdge2) {}

TYPED_TEST(ContainerTest, DISABLED_HasEdge3) {}

TYPED_TEST(ContainerTest, DISABLED_AddEdge0) {}

TYPED_TEST(ContainerTest, DISABLED_AddEdge1) {}

TYPED_TEST(ContainerTest, DISABLED_AddEdge2) {}

TYPED_TEST(ContainerTest, DISABLED_DelEdge0) {}

TYPED_TEST(ContainerTest, DISABLED_DelEdge1) {}

TYPED_TEST(ContainerTest, DISABLED_DelEdge2) {}

TYPED_TEST(ContainerTest, DISABLED_DelEdge3) {}

TYPED_TEST(ContainerTest, DISABLED_HasLabel0) {}

TYPED_TEST(ContainerTest, DISABLED_HasLabel1) {}

TYPED_TEST(ContainerTest, DISABLED_HasLabel2) {}

TYPED_TEST(ContainerTest, DISABLED_HasLabel3) {}

TYPED_TEST(ContainerTest, DISABLED_HasLabel4) {}

TYPED_TEST(ContainerTest, DISABLED_GetLabel0) {}

TYPED_TEST(ContainerTest, DISABLED_GetLabel1) {}

TYPED_TEST(ContainerTest, DISABLED_GetLabel2) {}

TYPED_TEST(ContainerTest, DISABLED_GetLabel3) {}

TYPED_TEST(ContainerTest, DISABLED_GetLabel4) {}

TYPED_TEST(ContainerTest, DISABLED_SetLabel0) {}

TYPED_TEST(ContainerTest, DISABLED_SetLabel1) {}

TYPED_TEST(ContainerTest, DISABLED_SetLabel2) {}

TYPED_TEST(ContainerTest, DISABLED_SetLabel3) {}

TYPED_TEST(ContainerTest, DISABLED_SetLabel4) {}

TYPED_TEST(ContainerTest, DISABLED_SetLabel5) {}

TYPED_TEST(ContainerTest, DISABLED_SetLabel6) {}

TYPED_TEST(ContainerTest, DISABLED_DelLabel0) {}

TYPED_TEST(ContainerTest, DISABLED_DelLabel1) {}

TYPED_TEST(ContainerTest, DISABLED_DelLabel2) {}

TYPED_TEST(ContainerTest, DISABLED_DelLabel3) {}

TYPED_TEST(ContainerTest, DISABLED_DelLabel4) {}

TYPED_TEST(ContainerTest, DISABLED_DelLabel5) {}

TYPED_TEST(ContainerTest, DISABLED_DelLabel6) {}

TYPED_TEST(ContainerTest, DISABLED_HasAttr0) {}

TYPED_TEST(ContainerTest, DISABLED_HasAttr1) {}

TYPED_TEST(ContainerTest, DISABLED_HasAttr2) {}

TYPED_TEST(ContainerTest, DISABLED_HasAttr3) {}

TYPED_TEST(ContainerTest, DISABLED_HasAttr4) {}

TYPED_TEST(ContainerTest, DISABLED_HasAttr5) {}

TYPED_TEST(ContainerTest, DISABLED_HasAttr6) {}

TYPED_TEST(ContainerTest, DISABLED_GetAttr0) {}

TYPED_TEST(ContainerTest, DISABLED_GetAttr1) {}

TYPED_TEST(ContainerTest, DISABLED_GetAttr2) {}

TYPED_TEST(ContainerTest, DISABLED_GetAttr3) {}

TYPED_TEST(ContainerTest, DISABLED_GetAttr4) {}

TYPED_TEST(ContainerTest, DISABLED_GetAttr5) {}

TYPED_TEST(ContainerTest, DISABLED_GetAttr6) {}

TYPED_TEST(ContainerTest, DISABLED_SetAttr0) {}

TYPED_TEST(ContainerTest, DISABLED_SetAttr1) {}

TYPED_TEST(ContainerTest, DISABLED_SetAttr2) {}

TYPED_TEST(ContainerTest, DISABLED_SetAttr3) {}

TYPED_TEST(ContainerTest, DISABLED_SetAttr4) {}

TYPED_TEST(ContainerTest, DISABLED_SetAttr5) {}

TYPED_TEST(ContainerTest, DISABLED_SetAttr6) {}

TYPED_TEST(ContainerTest, DISABLED_DelAttr0) {}

TYPED_TEST(ContainerTest, DISABLED_DelAttr1) {}

TYPED_TEST(ContainerTest, DISABLED_DelAttr2) {}

TYPED_TEST(ContainerTest, DISABLED_DelAttr3) {}

TYPED_TEST(ContainerTest, DISABLED_DelAttr4) {}

TYPED_TEST(ContainerTest, DISABLED_DelAttr5) {}

TYPED_TEST(ContainerTest, DISABLED_DelAttr6) {}

TYPED_TEST(ContainerTest, DISABLED_IsNull) {}

TYPED_TEST(ContainerTest, DISABLED_IsTrivial) {}

TYPED_TEST(ContainerTest, DISABLED_IsComplete) {}

TYPED_TEST(ContainerTest, DISABLED_Hash) {}

TYPED_TEST(ContainerTest, DISABLED_Print) {}

TYPED_TEST(ContainerTest, DISABLED_VerticesIterator) {}

TYPED_TEST(ContainerTest, DISABLED_VerticesLabelsIterator) {}

TYPED_TEST(ContainerTest, DISABLED_EdgesIterator) {}

TYPED_TEST(ContainerTest, DISABLED_EdgesLabelsIterator) {}
