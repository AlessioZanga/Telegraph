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
    TypeParam G, H;
    H = G;
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

    AdjacencyList D = {{0, {0, 2}}, {2, {0}}};  // Invalid argument: non-increasing sequence.
    ASSERT_THROW({ TypeParam K(D); }, std::invalid_argument);

    AdjacencyList E = {{0, {3}}, {1, {0}}};  // Invalid argument: vertex not defined.
    ASSERT_THROW({ TypeParam L(E); }, std::invalid_argument);
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

    AdjacencyMatrix E(2, 3);  // Invalid argument: non-squared matrix.
    ASSERT_THROW({ TypeParam L(E); }, std::invalid_argument);
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

    SparseAdjacencyMatrix E(2, 3);  // Invalid argument: non-squared matrix.
    ASSERT_THROW({ TypeParam L(E); }, std::invalid_argument);
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

TYPED_TEST(ContainerTest, Size) {
    TypeParam G;
    ASSERT_EQ(G.size(), 0);

    TypeParam H(0);
    ASSERT_EQ(H.size(), 0);

    VID i, j;
    TypeParam J(MAX);
    std::size_t count = 0;
    std::srand(std::time(0));
    for (std::size_t k = 0; k < MAX; k++) {
        i = std::rand() % MAX;
        j = std::rand() % MAX;
        if (!J.has_edge(i, j)) {
            J.add_edge(i, j);
            count++;
        }
    }
    ASSERT_EQ(J.size(), count);
}

TYPED_TEST(ContainerTest, GetVID) {
    TypeParam G(1);
    G.set_label(0, "0");
    ASSERT_EQ(G.get_id("0"), 0);                     // Valid argument.
    ASSERT_THROW(G.get_id("1"), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainerTest, HasVertexVID) {
    TypeParam G(1);
    ASSERT_TRUE(G.has_vertex(0));   // Valid argument.
    ASSERT_FALSE(G.has_vertex(1));  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainerTest, HasVertexVLB) {
    TypeParam G(1);
    G.set_label(0, "0");
    ASSERT_TRUE(G.has_vertex("0"));   // Valid argument.
    ASSERT_FALSE(G.has_vertex("1"));  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainerTest, AddVertexVOID) {
    TypeParam G;

    ASSERT_EQ(G.add_vertex(), 0);                          // Valid argument.
    ASSERT_THROW(G.add_vertex(0), std::invalid_argument);  // Invalid argument: vertex already defined.
}

TYPED_TEST(ContainerTest, AddVertexVID) {
    TypeParam G;

    ASSERT_EQ(G.add_vertex(0), 0);                         // Valid argument.
    ASSERT_THROW(G.add_vertex(0), std::invalid_argument);  // Invalid argument: vertex already defined.
}

TYPED_TEST(ContainerTest, AddVertexVLB) {
    TypeParam G;

    VID i = 0;
    VLB j = std::to_string(i);
    ASSERT_EQ(G.add_vertex(j), i);  // Valid argument.
    ASSERT_EQ(G.get_label(i), j);
    ASSERT_THROW(G.add_vertex(j), std::invalid_argument);  // Invalid argument: vertex already defined.
}

TYPED_TEST(ContainerTest, DelVertexVID) {
    TypeParam G;

    VID i = G.add_vertex();
    VLB l = std::to_string(i);
    G.set_label(i, l);
    G.set_attr(i, "key", 1);
    ASSERT_EQ(G.del_vertex(i), i);                          // Valid argument.
    ASSERT_THROW(G.del_vertex(i), std::out_of_range);       // Invalid argument: vertex not defined.
    ASSERT_THROW(G.has_label(i), std::out_of_range);        // Invalid argument: vertex not defined.
    ASSERT_THROW(G.has_attr(i, "key"), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainerTest, DelVertexVLB) {
    TypeParam G;

    VLB l = "0";
    VID i = G.add_vertex(l);
    G.set_attr(i, "key", 1);
    ASSERT_EQ(G.del_vertex(l), i);                          // Valid argument.
    ASSERT_THROW(G.del_vertex(l), std::out_of_range);       // Invalid argument: vertex not defined.
    ASSERT_THROW(G.has_label(i), std::out_of_range);        // Invalid argument: vertex not defined.
    ASSERT_THROW(G.has_attr(i, "key"), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainerTest, GetEID) {
    TypeParam G(2);
    EID e = G.add_edge(0, 1);
    G.set_label(0, 1, ELB("0 --- 1"));
    ASSERT_EQ(G.get_id(ELB("0 --- 1")), e);                     // Valid argument.
    ASSERT_THROW(G.get_id(ELB("0 --- 2")), std::out_of_range);  // Invalid argument: edge not defined.
}

TYPED_TEST(ContainerTest, HasEdgeEID) {
    TypeParam G(2);
    EID e = G.add_edge(0, 1);
    ASSERT_TRUE(G.has_edge(e));                           // Valid argument.
    ASSERT_FALSE(G.has_edge({1, 1}));                     // Valid argument.
    ASSERT_THROW(G.has_edge({1, 2}), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainerTest, HasEdgeELB) {
    TypeParam G(2);
    EID e = G.add_edge(0, 1);
    G.set_label(e, ELB("0 --- 1"));
    ASSERT_TRUE(G.has_edge(ELB("0 --- 1")));   // Valid argument.
    ASSERT_FALSE(G.has_edge(ELB("1 --- 1")));  // Valid argument.
}

TYPED_TEST(ContainerTest, HasEdgeVIDVID) {
    TypeParam G(2);
    EID e = G.add_edge(0, 1);
    ASSERT_TRUE(G.has_edge(e.first, e.second));         // Valid argument.
    ASSERT_FALSE(G.has_edge(1, 1));                     // Valid argument.
    ASSERT_THROW(G.has_edge(1, 2), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainerTest, HasEdgeVLBVLB) {
    TypeParam G(2);
    G.set_label(0, "0");
    G.set_label(1, "1");
    G.add_edge("0", "1");
    ASSERT_TRUE(G.has_edge("0", "1"));                      // Valid argument.
    ASSERT_FALSE(G.has_edge("1", "1"));                     // Valid argument.
    ASSERT_THROW(G.has_edge("1", "2"), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainerTest, AddEdgeEID) {
    TypeParam G(2);
    ASSERT_EQ(G.add_edge(EID(0, 1)), EID(0, 1));                 // Valid argument.
    ASSERT_THROW(G.add_edge(EID(0, 1)), std::invalid_argument);  // Invalid argument: edge already defined.
    ASSERT_THROW(G.add_edge(EID(0, 2)), std::out_of_range);      // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainerTest, AddEdgeVIDVID) {
    TypeParam G(2);
    ASSERT_EQ(G.add_edge(0, 1), EID(0, 1));                 // Valid argument.
    ASSERT_THROW(G.add_edge(0, 1), std::invalid_argument);  // Invalid argument: edge already defined.
    ASSERT_THROW(G.add_edge(0, 2), std::out_of_range);      // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainerTest, AddEdgeVLBVLB) {
    TypeParam G(2);
    G.set_label(0, "0");
    G.set_label(1, "1");
    ASSERT_EQ(G.add_edge("0", "1"), EID(0, 1));                 // Valid argument.
    ASSERT_THROW(G.add_edge("0", "1"), std::invalid_argument);  // Invalid argument: edge already defined.
    ASSERT_THROW(G.add_edge("0", "2"), std::out_of_range);      // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainerTest, DelEdgeEID) {
    TypeParam G(2);
    EID e = G.add_edge(EID(0, 1));
    G.set_label(e, ELB("0 --- 1"));
    G.set_attr(e, "key", 1);
    ASSERT_EQ(G.del_edge(e), e);                             // Valid argument.
    ASSERT_THROW(G.del_edge(EID(0, 1)), std::out_of_range);  // Invalid argument: edge not defined.
    ASSERT_THROW(G.del_edge(EID(0, 2)), std::out_of_range);  // Invalid argument: vertex not defined.
    ASSERT_THROW(G.has_label(e), std::out_of_range);         // Invalid argument: edge not defined.
    ASSERT_THROW(G.has_attr(e, "key"), std::out_of_range);   // Invalid argument: edge not defined.
}

TYPED_TEST(ContainerTest, DelEdgeELB) {
    TypeParam G(2);
    EID e = G.add_edge(0, 1);
    G.set_label(0, 1, ELB("0 --- 1"));
    G.set_attr(e, "key", 1);
    ASSERT_EQ(G.del_edge(ELB("0 --- 1")), EID(0, 1));             // Valid argument.
    ASSERT_THROW(G.del_edge(ELB("0 --- 1")), std::out_of_range);  // Invalid argument: edge not defined.
    ASSERT_THROW(G.del_edge(ELB("0 --- 2")), std::out_of_range);  // Invalid argument: vertex not defined.
    ASSERT_THROW(G.has_label(e), std::out_of_range);              // Invalid argument: edge not defined.
    ASSERT_THROW(G.has_attr(e, "key"), std::out_of_range);        // Invalid argument: edge not defined.
}

TYPED_TEST(ContainerTest, DelEdgeVIDVID) {
    TypeParam G(2);
    G.add_edge(0, 1);
    ASSERT_EQ(G.del_edge(0, 1), EID(0, 1));             // Valid argument.
    ASSERT_THROW(G.del_edge(0, 1), std::out_of_range);  // Invalid argument: edge not defined.
    ASSERT_THROW(G.del_edge(0, 2), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainerTest, DelEdgeVLBVLB) {
    TypeParam G(2);
    G.add_edge(0, 1);
    G.set_label(0, "0");
    G.set_label(1, "1");
    ASSERT_EQ(G.del_edge("0", "1"), EID(0, 1));             // Valid argument.
    ASSERT_THROW(G.del_edge("0", "1"), std::out_of_range);  // Invalid argument: edge not defined.
    ASSERT_THROW(G.del_edge("0", "2"), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainerTest, HasLabelGLB) {
    TypeParam G;
    ASSERT_FALSE(G.has_label());
    G.set_label("G");
    ASSERT_TRUE(G.has_label());
}

TYPED_TEST(ContainerTest, HasLabelVID) {
    TypeParam G(2);
    G.set_label(0, "0");
    ASSERT_TRUE(G.has_label(0));
    ASSERT_FALSE(G.has_label(1));
    ASSERT_THROW(G.has_label(2), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainerTest, HasLabelVLB) {
    TypeParam G(2);
    G.set_label(0, "0");
    ASSERT_TRUE(G.has_label("0"));
    ASSERT_FALSE(G.has_label("1"));
}

TYPED_TEST(ContainerTest, HasLabelEID) {
    TypeParam G(3);
    G.add_edge(0, 1);
    G.set_label(0, 1, ELB("0 --- 1"));
    ASSERT_TRUE(G.has_label(EID(0, 1)));
    ASSERT_THROW(G.has_label(EID(1, 2)), std::out_of_range);  // Invalid argument: edge not defined.
    ASSERT_THROW(G.has_label(EID(1, 3)), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainerTest, HasLabelELB) {
    TypeParam G(3);
    G.add_edge(0, 1);
    G.set_label(0, 1, ELB("0 --- 1"));
    ASSERT_TRUE(G.has_label(ELB("0 --- 1")));
    ASSERT_FALSE(G.has_label(ELB("1 --- 1")));
}

TYPED_TEST(ContainerTest, HasLabelVIDVID) {
    TypeParam G(3);
    G.add_edge(0, 1);
    G.set_label(0, 1, ELB("0 --- 1"));
    ASSERT_TRUE(G.has_label(0, 1));
    ASSERT_THROW(G.has_label(1, 2), std::out_of_range);  // Invalid argument: edge not defined.
    ASSERT_THROW(G.has_label(1, 3), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainerTest, HasLabelVLBVLB) {
    TypeParam G(3);
    G.add_edge(0, 1);
    G.set_label(0, "0");
    G.set_label(1, "1");
    G.set_label(2, "2");
    G.set_label(0, 1, ELB("0 --- 1"));
    ASSERT_TRUE(G.has_label("0", "1"));
    ASSERT_THROW(G.has_label("1", "2"), std::out_of_range);  // Invalid argument: edge not defined.
    ASSERT_THROW(G.has_label("1", "3"), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainerTest, GetLabelGLB) {
    TypeParam G;
    ASSERT_THROW(G.get_label(), std::out_of_range);  // Invalid argument: graph not defined.

    G.set_label("G");
    ASSERT_EQ(G.get_label(), "G");  // Valid argument.
}

TYPED_TEST(ContainerTest, GetLabelVID) {
    TypeParam G(1);
    ASSERT_THROW(G.get_label(0), std::out_of_range);  // Invalid argument: vertex label not defined.
    ASSERT_THROW(G.get_label(1), std::out_of_range);  // Invalid argument: vertex not defined.

    G.set_label(0, "0");
    ASSERT_EQ(G.get_label(0), "0");  // Valid argument.
}

TYPED_TEST(ContainerTest, GetLabelEID) {
    TypeParam G(2);
    ASSERT_THROW(G.get_label(EID(0, 1)), std::out_of_range);  // Invalid argument: edge not defined.

    G.add_edge(0, 1);
    ASSERT_THROW(G.get_label(EID(0, 1)), std::out_of_range);  // Invalid argument: edge label not defined.

    G.set_label(0, 1, ELB("0 --- 1"));
    ASSERT_EQ(G.get_label(EID(0, 1)), ELB("0 --- 1"));  // Valid argument.
}

TYPED_TEST(ContainerTest, GetLabelVIDVID) {
    TypeParam G(2);
    ASSERT_THROW(G.get_label(0, 1), std::out_of_range);  // Invalid argument: edge not defined.

    G.add_edge(0, 1);
    ASSERT_THROW(G.get_label(0, 1), std::out_of_range);  // Invalid argument: edge label not defined.

    G.set_label(0, 1, ELB("0 --- 1"));
    ASSERT_EQ(G.get_label(0, 1), ELB("0 --- 1"));  // Valid argument.
}

TYPED_TEST(ContainerTest, GetLabelVLBVLB) {
    TypeParam G(2);
    ASSERT_THROW(G.get_label("0", "1"), std::out_of_range);  // Invalid argument: vertex label not defined.

    G.set_label(0, "0");
    G.set_label(1, "1");
    ASSERT_THROW(G.get_label("0", "1"), std::out_of_range);  // Invalid argument: edge not defined.

    G.add_edge("0", "1");
    ASSERT_THROW(G.get_label("0", "1"), std::out_of_range);  // Invalid argument: edge label not defined.

    G.set_label("0", "1", ELB("0 --- 1"));
    ASSERT_EQ(G.get_label("0", "1"), ELB("0 --- 1"));  // Valid argument.
}

TYPED_TEST(ContainerTest, SetLabelGLB) {
    TypeParam G;
    ASSERT_THROW(G.set_label(""), std::invalid_argument);  // Invalid argument: empty label.

    G.set_label("G");
    ASSERT_EQ(G.get_label(), "G");  // Valid argument.

    G.set_label("G*");
    ASSERT_EQ(G.get_label(), "G*");  // Valid argument.
}

TYPED_TEST(ContainerTest, SetLabelVIDVLB) {
    TypeParam G(2);
    G.set_label(0, "0");  // Valid argument.
    ASSERT_EQ(G.get_label(0), "0");

    G.set_label(0, "1");  // Valid argument: replace label.
    ASSERT_EQ(G.get_label(0), "1");

    ASSERT_THROW(G.set_label(0, ""), std::invalid_argument);   // Invalid argument: empty label.
    ASSERT_THROW(G.set_label(1, "1"), std::invalid_argument);  // Invalid argument: duplicated label.
    ASSERT_THROW(G.set_label(2, "2"), std::out_of_range);      // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainerTest, SetLabelVLBVLB) {
    TypeParam G(2);
    G.set_label(0, "0");  // Valid argument.
    ASSERT_EQ(G.get_label(0), "0");

    G.set_label("0", "1");  // Valid argument: replace label.
    ASSERT_EQ(G.get_label(0), "1");

    ASSERT_THROW(G.set_label("1", ""), std::invalid_argument);  // Invalid argument: empty label.
    ASSERT_THROW(G.set_label(1, "1"), std::invalid_argument);   // Invalid argument: duplicated label.
    ASSERT_THROW(G.set_label(2, "2"), std::out_of_range);       // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainerTest, SetLabelEIDELB) {
    TypeParam G(3);
    EID e = G.add_edge(EID(0, 1));
    G.set_label(e, ELB("0 --- 1"));  // Valid argument.
    ASSERT_EQ(G.get_label(e), ELB("0 --- 1"));

    G.set_label(e, ELB("0 -*- 1"));  // Valid argument: replace label.
    ASSERT_EQ(G.get_label(e), ELB("0 -*- 1"));

    EID f = G.add_edge(EID(0, 2));
    ASSERT_THROW(G.set_label(e, ELB("")), std::invalid_argument);             // Invalid argument: empty label.
    ASSERT_THROW(G.set_label(f, ELB("0 -*- 1")), std::invalid_argument);      // Invalid argument: duplicated label.
    ASSERT_THROW(G.set_label(EID(1, 2), ELB("1 --- 2")), std::out_of_range);  // Invalid argument: edge not defined.
    ASSERT_THROW(G.set_label(EID(0, 3), ELB("0 --- 3")), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainerTest, SetLabelELBELB) {
    TypeParam G(3);
    EID e = G.add_edge(EID(0, 1));
    G.set_label(e, ELB("0 --- 1"));  // Valid argument.
    ASSERT_EQ(G.get_label(e), ELB("0 --- 1"));

    G.set_label(ELB("0 --- 1"), ELB("0 -*- 1"));  // Valid argument: replace label.
    ASSERT_EQ(G.get_label(e), ELB("0 -*- 1"));

    EID f = G.add_edge(EID(0, 2));
    ASSERT_THROW(G.set_label(e, ELB("")), std::invalid_argument);             // Invalid argument: empty label.
    ASSERT_THROW(G.set_label(f, ELB("0 -*- 1")), std::invalid_argument);      // Invalid argument: duplicated label.
    ASSERT_THROW(G.set_label(EID(1, 2), ELB("1 --- 2")), std::out_of_range);  // Invalid argument: edge not defined.
    ASSERT_THROW(G.set_label(EID(0, 3), ELB("0 --- 3")), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainerTest, SetLabelVIDVIDELB) {
    TypeParam G(3);
    G.add_edge(0, 1);
    G.set_label(0, 1, ELB("0 --- 1"));  // Valid argument.
    ASSERT_EQ(G.get_label(0, 1), ELB("0 --- 1"));

    G.set_label(0, 1, ELB("0 -*- 1"));  // Valid argument: replace label.
    ASSERT_EQ(G.get_label(0, 1), ELB("0 -*- 1"));

    G.add_edge(0, 2);
    ASSERT_THROW(G.set_label(0, 1, ELB("")), std::invalid_argument);         // Invalid argument: empty label.
    ASSERT_THROW(G.set_label(0, 2, ELB("0 -*- 1")), std::invalid_argument);  // Invalid argument: duplicated label.
    ASSERT_THROW(G.set_label(1, 2, ELB("1 --- 2")), std::out_of_range);      // Invalid argument: edge not defined.
    ASSERT_THROW(G.set_label(0, 3, ELB("0 --- 3")), std::out_of_range);      // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainerTest, SetLabelVLBVLBELB) {
    TypeParam G(3);
    G.set_label(0, "0");
    G.set_label(1, "1");
    G.add_edge("0", "1");
    G.set_label("0", "1", ELB("0 --- 1"));  // Valid argument.
    ASSERT_EQ(G.get_label("0", "1"), ELB("0 --- 1"));

    G.set_label("0", "1", ELB("0 -*- 1"));  // Valid argument: replace label.
    ASSERT_EQ(G.get_label("0", "1"), ELB("0 -*- 1"));

    G.set_label(2, "2");
    G.add_edge("0", "2");
    ASSERT_THROW(G.set_label("0", "1", ELB("")), std::invalid_argument);         // Invalid argument: empty label.
    ASSERT_THROW(G.set_label("0", "2", ELB("0 -*- 1")), std::invalid_argument);  // Invalid argument: duplicated label.
    ASSERT_THROW(G.set_label("1", "2", ELB("1 --- 2")), std::out_of_range);      // Invalid argument: edge not defined.
    ASSERT_THROW(G.set_label("0", "3", ELB("0 --- 3")), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainerTest, DelLabelVOID) {
    TypeParam G;
    G.set_label("G");
    G.del_label();
    ASSERT_FALSE(G.has_label());
    ASSERT_THROW(G.del_label(), std::out_of_range);  // Invalid argument: graph label not defined.
}

TYPED_TEST(ContainerTest, DelLabelVID) {
    TypeParam G(1);
    G.set_label(0, "0");
    G.del_label(0);
    ASSERT_FALSE(G.has_label(0));
    ASSERT_THROW(G.del_label(0), std::out_of_range);  // Invalid argument: vertex label not defined.
    ASSERT_THROW(G.del_label(1), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainerTest, DelLabelVLB) {
    TypeParam G(1);
    G.set_label(0, "0");
    G.del_label("0");
    ASSERT_FALSE(G.has_label("0"));
    ASSERT_THROW(G.del_label("0"), std::out_of_range);  // Invalid argument: vertex label not defined.
    ASSERT_THROW(G.del_label("1"), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainerTest, DelLabelEID) {
    TypeParam G(2);
    EID i = G.add_edge(0, 1);
    G.set_label(i, ELB("0 -- 1"));
    G.del_label(i);
    ASSERT_FALSE(G.has_label(i));
    ASSERT_THROW(G.del_label(i), std::out_of_range);          // Invalid argument: vertex label not defined.
    ASSERT_THROW(G.del_label(EID(1, 1)), std::out_of_range);  // Invalid argument: edge not defined.
    ASSERT_THROW(G.del_label(EID(1, 2)), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainerTest, DelLabelELB) {
    TypeParam G(2);
    G.add_edge(0, 1);
    G.set_label(0, 1, ELB("0 -- 1"));
    G.del_label(ELB("0 -- 1"));
    ASSERT_FALSE(G.has_label(ELB("0 -- 1")));
    ASSERT_THROW(G.del_label(ELB("0 -- 1")), std::out_of_range);  // Invalid argument: edge label not defined.
}

TYPED_TEST(ContainerTest, DelLabelVIDVID) {
    TypeParam G(2);
    G.add_edge(0, 1);
    G.set_label(0, 1, ELB("0 -- 1"));
    G.del_label(0, 1);
    ASSERT_FALSE(G.has_label(0, 1));
    ASSERT_THROW(G.del_label(0, 1), std::out_of_range);  // Invalid argument: vertex label not defined.
    ASSERT_THROW(G.del_label(1, 1), std::out_of_range);  // Invalid argument: edge not defined.
    ASSERT_THROW(G.del_label(1, 2), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainerTest, DelLabelVLBVLB) {
    TypeParam G(2);
    G.set_label(0, "0");
    G.set_label(1, "1");
    G.add_edge("0", "1");
    G.set_label("0", "1", ELB("0 -- 1"));
    G.del_label("0", "1");
    ASSERT_FALSE(G.has_label("0", "1"));
    ASSERT_THROW(G.del_label("0", "1"), std::out_of_range);  // Invalid argument: vertex label not defined.
    ASSERT_THROW(G.del_label("1", "1"), std::out_of_range);  // Invalid argument: edge not defined.
    ASSERT_THROW(G.del_label("1", "2"), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainerTest, DISABLED_HasAttrVOID) {}

TYPED_TEST(ContainerTest, DISABLED_HasAttrVID) {}

TYPED_TEST(ContainerTest, DISABLED_HasAttrVLB) {}

TYPED_TEST(ContainerTest, DISABLED_HasAttrEID) {}

TYPED_TEST(ContainerTest, DISABLED_HasAttrELB) {}

TYPED_TEST(ContainerTest, DISABLED_HasAttrVIDVID) {}

TYPED_TEST(ContainerTest, DISABLED_HasAttrVLBVLB) {}

TYPED_TEST(ContainerTest, DISABLED_GetAttrVOID) {}

TYPED_TEST(ContainerTest, DISABLED_GetAttrVID) {}

TYPED_TEST(ContainerTest, DISABLED_GetAttrVLB) {}

TYPED_TEST(ContainerTest, DISABLED_GetAttrEID) {}

TYPED_TEST(ContainerTest, DISABLED_GetAttrELB) {}

TYPED_TEST(ContainerTest, DISABLED_GetAttrVIDVID) {}

TYPED_TEST(ContainerTest, DISABLED_GetAttrVLBVLB) {}

TYPED_TEST(ContainerTest, DISABLED_SetAttrVOID) {}

TYPED_TEST(ContainerTest, DISABLED_SetAttrVID) {}

TYPED_TEST(ContainerTest, DISABLED_SetAttrVLB) {}

TYPED_TEST(ContainerTest, DISABLED_SetAttrEID) {}

TYPED_TEST(ContainerTest, DISABLED_SetAttrELB) {}

TYPED_TEST(ContainerTest, DISABLED_SetAttrVIDVID) {}

TYPED_TEST(ContainerTest, DISABLED_SetAttrVLBVLB) {}

TYPED_TEST(ContainerTest, DISABLED_DelAttrVOID) {}

TYPED_TEST(ContainerTest, DISABLED_DelAttrVID) {}

TYPED_TEST(ContainerTest, DISABLED_DelAttrVLB) {}

TYPED_TEST(ContainerTest, DISABLED_DelAttrEID) {}

TYPED_TEST(ContainerTest, DISABLED_DelAttrELB) {}

TYPED_TEST(ContainerTest, DISABLED_DelAttrVIDVID) {}

TYPED_TEST(ContainerTest, DISABLED_DelAttrVLBVLB) {}

TYPED_TEST(ContainerTest, DISABLED_IsNull) {}

TYPED_TEST(ContainerTest, DISABLED_IsTrivial) {}

TYPED_TEST(ContainerTest, DISABLED_IsComplete) {}

TYPED_TEST(ContainerTest, DISABLED_Hash) {}

TYPED_TEST(ContainerTest, DISABLED_ToStream) {}

TYPED_TEST(ContainerTest, DISABLED_VerticesIterator) {}

TYPED_TEST(ContainerTest, DISABLED_VerticesLabelsIterator) {}

TYPED_TEST(ContainerTest, DISABLED_EdgesIterator) {}

TYPED_TEST(ContainerTest, DISABLED_EdgesLabelsIterator) {}
