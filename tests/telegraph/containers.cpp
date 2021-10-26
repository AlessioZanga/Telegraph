#include <gtest/gtest.h>

#include <cstdlib>
#include <ctime>
#include <numeric>
#include <telegraph/telegraph>

#define MAX 256

// Define fixture class template
template <typename T>
class ContainersTest : public ::testing::Test {};

// Define list of types
using GraphTypes = ::testing::Types<DirectedDenseGraph>;

// Create typed test suite
TYPED_TEST_SUITE(ContainersTest, GraphTypes);

TYPED_TEST(ContainersTest, DefaultContructor) { TypeParam G; }

TYPED_TEST(ContainersTest, CopyConstructor) {
    TypeParam G;
    TypeParam H(G);
}

TYPED_TEST(ContainersTest, AssignmentOperator) {
    TypeParam G, H;
    H = G;
}

TYPED_TEST(ContainersTest, Destructor) {
    TypeParam *G = new TypeParam();
    delete G;
}

TYPED_TEST(ContainersTest, SizedConstructor) {
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

TYPED_TEST(ContainersTest, VertexIteratorConstructor) {
    std::vector<VID> U = {};  // Empty sequence.
    TypeParam G(U.begin(), U.end());
    ASSERT_EQ(G.order(), U.size());
    ASSERT_TRUE(std::is_sorted(V(G).begin(), V(G).end()));

    std::vector<VID> W = {0};  // Single sequence.
    TypeParam H(W.begin(), W.end());
    ASSERT_EQ(H.order(), W.size());
    ASSERT_TRUE(std::is_sorted(V(H).begin(), V(H).end()));

    std::vector<VID> X = {0, 1, 2, 3, 4};  // Ordered sequence.
    TypeParam J(X.begin(), X.end());
    ASSERT_EQ(J.order(), X.size());
    ASSERT_TRUE(std::is_sorted(V(J).begin(), V(J).end()));

    std::vector<VID> Y = {2, 1, 3, 4, 0};  // Unordered sequence.
    TypeParam K(Y.begin(), Y.end());
    ASSERT_EQ(K.order(), Y.size());
    ASSERT_TRUE(std::is_sorted(V(K).begin(), V(K).end()));

    std::vector<VID> Z(MAX);  // Very long sequence.
    std::iota(Z.begin(), Z.end(), 0);
    TypeParam L(Z.begin(), Z.end());
    ASSERT_EQ(L.order(), Z.size());
    ASSERT_TRUE(std::is_sorted(V(L).begin(), V(L).end()));

    std::list<VID> N(MAX);  // Very long sequence with non-contiguous memory.
    std::iota(N.begin(), N.end(), 0);
    TypeParam M(N.begin(), N.end());
    ASSERT_EQ(M.order(), N.size());
    ASSERT_TRUE(std::is_sorted(V(M).begin(), V(M).end()));
}

TYPED_TEST(ContainersTest, EdgeIteratorConstructor) {
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

TYPED_TEST(ContainersTest, AdjacencyListConstructor) {
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

    AdjacencyList D = {{0, {3}}, {1, {0}}};  // Invalid argument: vertex not defined.
    ASSERT_THROW({ TypeParam K(D); }, std::invalid_argument);
}

TYPED_TEST(ContainersTest, AdjacencyMatrixConstructor) {
    AdjacencyMatrix A;  // Empty sequence.
    TypeParam G(A);
    ASSERT_EQ(G.order(), 0);
    ASSERT_EQ(G.size(), 0);

    AdjacencyMatrix B(1, 1);
    B << 0;  // Single sequence.
    TypeParam H(B);
    ASSERT_EQ(H.order(), 1);
    ASSERT_EQ(H.size(), 0);
    ASSERT_TRUE(H.has_vertex(0));

    AdjacencyMatrix C(3, 3);
    C << 0, 1, 0, 0, 0, 1, 0, 0, 0;  // Multiple sequence.
    TypeParam J(C);
    ASSERT_EQ(J.order(), 3);
    ASSERT_EQ(J.size(), 2);
    ASSERT_TRUE(J.has_vertex(2));

    AdjacencyMatrix D(3, 3);
    D << 0, 1, 0, 0, 0, 3, 0, 0, 0;  // Non-binary sequence.
    TypeParam K(D);
    ASSERT_EQ(K.order(), 3);
    ASSERT_EQ(K.size(), 2);
    ASSERT_TRUE(K.has_vertex(2));

    AdjacencyMatrix E(2, 3);  // Invalid argument: non-squared matrix.
    ASSERT_THROW({ TypeParam L(E); }, std::invalid_argument);
}

TYPED_TEST(ContainersTest, SparseAdjacencyMatrixConstructor) {
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

TYPED_TEST(ContainersTest, AdjacencyListOperator) {
    AdjacencyList A;
    TypeParam G;
    ASSERT_EQ(((AdjacencyList)G), A);

    AdjacencyList B = {{0, {0, 1, 3}}, {1, {1, 2}}, {2, {4}}, {3, {}}, {4, {0, 2, 3, 4}}};
    TypeParam H(B);
    ASSERT_EQ(((AdjacencyList)H), B);
}

TYPED_TEST(ContainersTest, AdjacencyMatrixOperator) {
    AdjacencyMatrix A;
    TypeParam G;
    ASSERT_EQ(((AdjacencyMatrix)G), A);

    AdjacencyMatrix B(3, 3);
    B << 0, 1, 0, 0, 0, 1, 0, 0, 0;  // Multiple sequence.
    TypeParam H(B);
    ASSERT_EQ(((AdjacencyMatrix)H), B);
}

TYPED_TEST(ContainersTest, SparseAdjacencyMatrixOperator) {
    SparseAdjacencyMatrix A(1, 1);
    TypeParam G(A);
    ASSERT_TRUE(((SparseAdjacencyMatrix)G).isApprox(A, 0));

    SparseAdjacencyMatrix B(3, 3);
    B.coeffRef(0, 1) = 1;
    B.coeffRef(1, 2) = 1;  // Multiple sequence.
    TypeParam H(B);
    ASSERT_TRUE(((SparseAdjacencyMatrix)H).isApprox(B, 0));
}

TYPED_TEST(ContainersTest, VerticesIterator) {
    TypeParam G;

    typename TypeParam::VIDsIterator v, w(v);
    ASSERT_NO_THROW({ w = v; });

    typename TypeParam::VIDsIterator::const_iterator it, ite;
    ASSERT_DEATH({ it++; }, ".*");

    it = V(G).begin();
    ASSERT_EQ(it, V(G).begin());
    ASSERT_EQ(V(G).begin(), V(G).end());
    ASSERT_EQ(std::distance(V(G).begin(), V(G).end()), 0);
    for (const VID &X : V(G)) ASSERT_EQ(X, -1);

    TypeParam H(1);
    ASSERT_NE(V(H).begin(), V(H).end());
    ASSERT_EQ(std::distance(V(H).begin(), V(H).end()), 1);
    for (const VID &X : V(H)) ASSERT_EQ(X, 0);

    VID i = 0;
    TypeParam J(MAX);
    for (const VID &X : V(J)) {
        ASSERT_TRUE(J.has_vertex(X));
        ASSERT_EQ(X, i);
        i++;
    }

    // Assert size is correct.
    ASSERT_EQ(std::distance(V(J).begin(), V(J).end()), J.order());

    i = J.order() - 1;
    it = --V(J).end();
    ite = V(J).begin();
    for (; it != ite; it--) {
        ASSERT_TRUE(J.has_vertex(*it));
        ASSERT_EQ(*it, i);
        i--;
    }

    // Assert sequence is always sorted.
    ASSERT_TRUE(std::is_sorted(V(J).begin(), V(J).end()));
}

TYPED_TEST(ContainersTest, EdgesIterator) {
    TypeParam G;

    typename TypeParam::EIDsIterator v, w(v);
    ASSERT_NO_THROW({ w = v; });

    typename TypeParam::EIDsIterator::const_iterator it, ite;
    ASSERT_DEATH({ it++; }, ".*");

    it = E(G).begin();
    ASSERT_EQ(it, E(G).begin());
    ASSERT_EQ(E(G).begin(), E(G).end());
    ASSERT_EQ(std::distance(E(G).begin(), E(G).end()), 0);
    for (const EID &X : E(G)) ASSERT_EQ(X, EID(-1, -1));

    TypeParam H(1);
    ASSERT_EQ(E(H).begin(), E(H).end());
    ASSERT_EQ(std::distance(E(H).begin(), E(H).end()), 0);
    for (const EID &X : E(H)) ASSERT_EQ(X, EID(0, 0));

    H.add_vertex();
    H.add_edge(0, 1);
    ASSERT_NE(E(H).begin(), E(H).end());
    ASSERT_EQ(std::distance(E(H).begin(), E(H).end()), 1);
    for (const EID &X : E(H)) ASSERT_EQ(X, EID(0, 1));

    VID i = 0, j = 0;
    TypeParam J(AdjacencyMatrix::Ones(MAX, MAX));

    // Assert size is correct.
    ASSERT_EQ(std::distance(E(J).begin(), E(J).end()), J.size());

    i = 0;
    j = 0;
    std::size_t n = 0;
    for (const EID &X : E(J)) {
        ASSERT_TRUE(J.has_edge(X));
        ASSERT_EQ(X, EID(i, j));
        n++;
        i = n / MAX;
        j = n % MAX;
    }

    n = J.order();
    i = n - 1;
    j = n - 1;
    n = (n * n) - 1;
    it = --E(J).end();
    ite = E(J).begin();
    for (; it != ite; it--) {
        ASSERT_TRUE(J.has_edge(*it));
        ASSERT_EQ(*it, EID(i, j));
        n--;
        i = n / MAX;
        j = n % MAX;
    }

    // Assert sequence is always sorted.
    ASSERT_TRUE(std::is_sorted(E(J).begin(), E(J).end()));

    EIDs F;
    TypeParam K(MAX);
    std::srand(std::time(0));
    for (std::size_t l = 0; l < MAX; l++) {
        EID e = EID(std::rand() % MAX, std::rand() % MAX);
        if (!K.has_edge(e)) K.add_edge(e);
        F.insert(e);
    }

    ASSERT_TRUE(std::is_sorted(E(K).begin(), E(K).end()));
    ASSERT_TRUE(std::equal(E(K).begin(), E(K).end(), F.begin()));
}

TYPED_TEST(ContainersTest, VerticesLabelsIterator) {
    TypeParam G;

    typename TypeParam::VLBsIterator v, w(v);
    ASSERT_NO_THROW({ w = v; });

    typename TypeParam::VLBsIterator::const_iterator it, ite;
    ASSERT_DEATH({ it++; }, ".*");

    it = Vl(G).begin();
    ASSERT_EQ(it, Vl(G).begin());
    ASSERT_EQ(Vl(G).begin(), Vl(G).end());
    ASSERT_EQ(std::distance(Vl(G).begin(), Vl(G).end()), 0);
    for (const VLB &X : Vl(G)) ASSERT_EQ(X, "");

    TypeParam H(1);
    H.set_label(0, "0");
    ASSERT_NE(Vl(H).begin(), Vl(H).end());
    ASSERT_EQ(std::distance(Vl(H).begin(), Vl(H).end()), 1);
    for (const VLB &X : Vl(H)) ASSERT_EQ(X, "0");

    TypeParam J(MAX);
    for (const VID &X : V(J)) J.set_label(X, std::to_string(X));

    // Assert size is correct.
    ASSERT_EQ(std::distance(Vl(J).begin(), Vl(J).end()), J.order());

    for (const VLB &X : Vl(J)) ASSERT_TRUE(J.has_vertex(X));

    it = --Vl(J).end();
    ite = Vl(J).begin();
    for (; it != ite; it--) ASSERT_TRUE(J.has_vertex(*it));

    // Assert sequence is always sorted.
    ASSERT_TRUE(std::is_sorted(Vl(J).begin(), Vl(J).end()));
}

TYPED_TEST(ContainersTest, EdgesLabelsIterator) {
    TypeParam G;

    typename TypeParam::ELBsIterator v, w(v);
    ASSERT_NO_THROW({ w = v; });

    typename TypeParam::ELBsIterator::const_iterator it, ite;
    ASSERT_DEATH({ it++; }, ".*");

    it = El(G).begin();
    ASSERT_EQ(it, El(G).begin());
    ASSERT_EQ(El(G).begin(), El(G).end());
    ASSERT_EQ(std::distance(El(G).begin(), El(G).end()), 0);
    for (const ELB &X : El(G)) ASSERT_EQ(X, ELB(""));

    TypeParam H(1);
    ASSERT_EQ(E(H).begin(), E(H).end());
    ASSERT_EQ(std::distance(E(H).begin(), E(H).end()), 0);
    for (const ELB &X : El(H)) ASSERT_EQ(X, ELB("0 --- 0"));

    H.add_vertex();
    H.add_edge(0, 1);
    H.set_label(0, 1, ELB("0 --- 1"));
    ASSERT_NE(El(H).begin(), El(H).end());
    ASSERT_EQ(std::distance(El(H).begin(), El(H).end()), 1);
    for (const ELB &X : El(H)) ASSERT_EQ(X, ELB("0 --- 1"));

    TypeParam J(AdjacencyMatrix::Ones(MAX, MAX));
    for (const EID &X : E(J)) J.set_label(X, ELB(std::to_string(X.first) + " --- " + std::to_string(X.second)));

    // Assert size is correct.
    ASSERT_EQ(std::distance(El(J).begin(), El(J).end()), J.size());

    for (const ELB &X : El(J)) ASSERT_TRUE(J.has_edge(X));

    it = --El(J).end();
    ite = El(J).begin();
    for (; it != ite; it--) ASSERT_TRUE(J.has_edge(*it));

    // Assert sequence is always sorted.
    ASSERT_TRUE(std::is_sorted(El(J).begin(), El(J).end()));
}

TYPED_TEST(ContainersTest, VerticesPairsIterator) {
    TypeParam G;

    typename TypeParam::VIDsVLBsIterator v, w(v);
    ASSERT_NO_THROW({ w = v; });

    typename TypeParam::VIDsVLBsIterator::const_iterator it, ite;

    it = Vp(G).begin();
    ASSERT_EQ(it, Vp(G).begin());
    ASSERT_EQ(Vp(G).begin(), Vp(G).end());
    ASSERT_EQ(std::distance(Vp(G).begin(), Vp(G).end()), 0);
    for (const auto &[id, lb] : Vp(G)) ASSERT_EQ(lb, "");

    TypeParam H(1);
    H.set_label(0, "0");
    ASSERT_NE(Vp(H).begin(), Vp(H).end());
    ASSERT_EQ(std::distance(Vp(H).begin(), Vp(H).end()), 1);
    for (const auto &[id, lb] : Vp(H)) ASSERT_EQ(lb, "0");

    TypeParam J(MAX);
    for (const VID &X : V(J)) J.set_label(X, std::to_string(X));

    // Assert size is correct.
    ASSERT_EQ(std::distance(Vp(J).begin(), Vp(J).end()), J.order());

    for (const auto &[id, lb] : Vp(J)) ASSERT_TRUE(J.has_vertex(lb));

    it = --Vp(J).end();
    ite = Vp(J).begin();
    for (; it != ite; it--) ASSERT_TRUE(J.has_vertex((*it).second));

    // Assert sequence is always sorted.
    ASSERT_TRUE(std::is_sorted(Vp(J).begin(), Vp(J).end()));
}

TYPED_TEST(ContainersTest, EdgesPairsIterator) {
    TypeParam G;

    typename TypeParam::EIDsELBsIterator v, w(v);
    ASSERT_NO_THROW({ w = v; });

    typename TypeParam::EIDsELBsIterator::const_iterator it, ite;

    it = Ep(G).begin();
    ASSERT_EQ(it, Ep(G).begin());
    ASSERT_EQ(Ep(G).begin(), Ep(G).end());
    ASSERT_EQ(std::distance(Ep(G).begin(), Ep(G).end()), 0);
    for (const auto &[id, lb] : Ep(G)) ASSERT_EQ(lb, ELB(""));

    TypeParam H(1);
    ASSERT_EQ(E(H).begin(), E(H).end());
    ASSERT_EQ(std::distance(E(H).begin(), E(H).end()), 0);
    for (const auto &[id, lb] : Ep(H)) ASSERT_EQ(lb, ELB("0 --- 0"));

    H.add_vertex();
    H.add_edge(0, 1);
    H.set_label(0, 1, ELB("0 --- 1"));
    ASSERT_NE(Ep(H).begin(), Ep(H).end());
    ASSERT_EQ(std::distance(Ep(H).begin(), Ep(H).end()), 1);
    for (const auto &[id, lb] : Ep(H)) ASSERT_EQ(lb, ELB("0 --- 1"));

    TypeParam J(AdjacencyMatrix::Ones(MAX, MAX));
    for (const EID &X : E(J)) J.set_label(X, ELB(std::to_string(X.first) + " --- " + std::to_string(X.second)));

    // Assert size is correct.
    ASSERT_EQ(std::distance(Ep(J).begin(), Ep(J).end()), J.size());

    for (const auto &[id, lb] : Ep(J)) ASSERT_TRUE(J.has_edge(lb));

    it = --Ep(J).end();
    ite = Ep(J).begin();
    for (; it != ite; it--) ASSERT_TRUE(J.has_edge((*it).second));

    // Assert sequence is always sorted.
    ASSERT_TRUE(std::is_sorted(Ep(J).begin(), Ep(J).end()));
}

TYPED_TEST(ContainersTest, Order) {
    TypeParam G;
    ASSERT_EQ(G.order(), 0);

    TypeParam H(0);
    ASSERT_EQ(H.order(), 0);

    for (std::size_t i = 1; i < MAX; i++) {
        TypeParam J(i);
        ASSERT_EQ(J.order(), i);
    }
}

TYPED_TEST(ContainersTest, Size) {
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

TYPED_TEST(ContainersTest, GetVID) {
    TypeParam G(1);
    G.set_label(0, "0");
    ASSERT_EQ(G.get_id("0"), 0);                     // Valid argument.
    ASSERT_THROW(G.get_id("1"), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, HasVertexVID) {
    TypeParam G(1);
    ASSERT_TRUE(G.has_vertex(0));   // Valid argument.
    ASSERT_FALSE(G.has_vertex(1));  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, HasVertexVLB) {
    TypeParam G(1);
    G.set_label(0, "0");
    ASSERT_TRUE(G.has_vertex("0"));   // Valid argument.
    ASSERT_FALSE(G.has_vertex("1"));  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, AddVertexVOID) {
    TypeParam G;

    ASSERT_EQ(G.add_vertex(), 0);                          // Valid argument.
    ASSERT_THROW(G.add_vertex(0), std::invalid_argument);  // Invalid argument: vertex already defined.
}

TYPED_TEST(ContainersTest, AddVertexVID) {
    TypeParam G;

    ASSERT_EQ(G.add_vertex(0), 0);                         // Valid argument.
    ASSERT_THROW(G.add_vertex(0), std::invalid_argument);  // Invalid argument: vertex already defined.
}

TYPED_TEST(ContainersTest, AddVertexVLB) {
    TypeParam G;

    VID i = 0;
    VLB j = std::to_string(i);
    ASSERT_EQ(G.add_vertex(j), i);  // Valid argument.
    ASSERT_EQ(G.get_label(i), j);
    ASSERT_THROW(G.add_vertex(j), std::invalid_argument);  // Invalid argument: vertex already defined.
}

TYPED_TEST(ContainersTest, DelVertexVID) {
    TypeParam G;

    VID i = G.add_vertex();
    VLB l = std::to_string(i);
    G.set_label(i, l);
    G.set_attr(i, "key", true);
    ASSERT_EQ(G.del_vertex(i), i);                          // Valid argument.
    ASSERT_THROW(G.del_vertex(i), std::out_of_range);       // Invalid argument: vertex not defined.
    ASSERT_THROW(G.has_label(i), std::out_of_range);        // Invalid argument: vertex not defined.
    ASSERT_THROW(G.has_attr(i, "key"), std::out_of_range);  // Invalid argument: vertex not defined.

    TypeParam H(MAX);
    i = MAX / 2;
    l = std::to_string(i);
    H.set_label(i, l);
    H.set_attr(i, "key", true);
    ASSERT_EQ(H.del_vertex(i), i);                          // Valid argument.
    ASSERT_THROW(H.del_vertex(i), std::out_of_range);       // Invalid argument: vertex not defined.
    ASSERT_THROW(H.has_label(i), std::out_of_range);        // Invalid argument: vertex not defined.
    ASSERT_THROW(H.has_attr(i, "key"), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, DelVertexVLB) {
    TypeParam G;

    VLB l = "0";
    VID i = G.add_vertex(l);
    G.set_attr(i, "key", true);
    ASSERT_EQ(G.del_vertex(l), i);                          // Valid argument.
    ASSERT_THROW(G.del_vertex(l), std::out_of_range);       // Invalid argument: vertex not defined.
    ASSERT_THROW(G.has_label(i), std::out_of_range);        // Invalid argument: vertex not defined.
    ASSERT_THROW(G.has_attr(i, "key"), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, GetEID) {
    TypeParam G(2);
    EID e = G.add_edge(0, 1);
    G.set_label(0, 1, ELB("0 --- 1"));
    ASSERT_EQ(G.get_id(ELB("0 --- 1")), e);                     // Valid argument.
    ASSERT_THROW(G.get_id(ELB("0 --- 2")), std::out_of_range);  // Invalid argument: edge not defined.
}

TYPED_TEST(ContainersTest, HasEdgeEID) {
    TypeParam G(2);
    EID e = G.add_edge(0, 1);
    ASSERT_TRUE(G.has_edge(e));                           // Valid argument.
    ASSERT_FALSE(G.has_edge({1, 1}));                     // Valid argument.
    ASSERT_THROW(G.has_edge({1, 2}), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, HasEdgeELB) {
    TypeParam G(2);
    EID e = G.add_edge(0, 1);
    G.set_label(e, ELB("0 --- 1"));
    ASSERT_TRUE(G.has_edge(ELB("0 --- 1")));   // Valid argument.
    ASSERT_FALSE(G.has_edge(ELB("1 --- 1")));  // Valid argument.
}

TYPED_TEST(ContainersTest, HasEdgeVIDVID) {
    TypeParam G(2);
    EID e = G.add_edge(0, 1);
    ASSERT_TRUE(G.has_edge(e.first, e.second));         // Valid argument.
    ASSERT_FALSE(G.has_edge(1, 1));                     // Valid argument.
    ASSERT_THROW(G.has_edge(1, 2), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, HasEdgeVLBVLB) {
    TypeParam G(2);
    G.set_label(0, "0");
    G.set_label(1, "1");
    G.add_edge("0", "1");
    ASSERT_TRUE(G.has_edge("0", "1"));                      // Valid argument.
    ASSERT_FALSE(G.has_edge("1", "1"));                     // Valid argument.
    ASSERT_THROW(G.has_edge("1", "2"), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, AddEdgeEID) {
    TypeParam G(2);
    ASSERT_EQ(G.add_edge(EID(0, 1)), EID(0, 1));                 // Valid argument.
    ASSERT_THROW(G.add_edge(EID(0, 1)), std::invalid_argument);  // Invalid argument: edge already defined.
    ASSERT_THROW(G.add_edge(EID(0, 2)), std::out_of_range);      // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, AddEdgeVIDVID) {
    TypeParam G(2);
    ASSERT_EQ(G.add_edge(0, 1), EID(0, 1));                 // Valid argument.
    ASSERT_THROW(G.add_edge(0, 1), std::invalid_argument);  // Invalid argument: edge already defined.
    ASSERT_THROW(G.add_edge(0, 2), std::out_of_range);      // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, AddEdgeVLBVLB) {
    TypeParam G(2);
    G.set_label(0, "0");
    G.set_label(1, "1");
    ASSERT_EQ(G.add_edge("0", "1"), EID(0, 1));                 // Valid argument.
    ASSERT_THROW(G.add_edge("0", "1"), std::invalid_argument);  // Invalid argument: edge already defined.
    ASSERT_THROW(G.add_edge("0", "2"), std::out_of_range);      // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, DelEdgeEID) {
    TypeParam G(2);
    EID e = G.add_edge(EID(0, 1));
    G.set_label(e, ELB("0 --- 1"));
    G.set_attr(e, "key", true);
    ASSERT_EQ(G.del_edge(e), e);                             // Valid argument.
    ASSERT_THROW(G.del_edge(EID(0, 1)), std::out_of_range);  // Invalid argument: edge not defined.
    ASSERT_THROW(G.del_edge(EID(0, 2)), std::out_of_range);  // Invalid argument: vertex not defined.
    ASSERT_THROW(G.has_label(e), std::out_of_range);         // Invalid argument: edge not defined.
    ASSERT_THROW(G.has_attr(e, "key"), std::out_of_range);   // Invalid argument: edge not defined.
}

TYPED_TEST(ContainersTest, DelEdgeELB) {
    TypeParam G(2);
    EID e = G.add_edge(0, 1);
    G.set_label(0, 1, ELB("0 --- 1"));
    G.set_attr(e, "key", true);
    ASSERT_EQ(G.del_edge(ELB("0 --- 1")), EID(0, 1));             // Valid argument.
    ASSERT_THROW(G.del_edge(ELB("0 --- 1")), std::out_of_range);  // Invalid argument: edge not defined.
    ASSERT_THROW(G.del_edge(ELB("0 --- 2")), std::out_of_range);  // Invalid argument: vertex not defined.
    ASSERT_THROW(G.has_label(e), std::out_of_range);              // Invalid argument: edge not defined.
    ASSERT_THROW(G.has_attr(e, "key"), std::out_of_range);        // Invalid argument: edge not defined.
}

TYPED_TEST(ContainersTest, DelEdgeVIDVID) {
    TypeParam G(2);
    G.add_edge(0, 1);
    ASSERT_EQ(G.del_edge(0, 1), EID(0, 1));             // Valid argument.
    ASSERT_THROW(G.del_edge(0, 1), std::out_of_range);  // Invalid argument: edge not defined.
    ASSERT_THROW(G.del_edge(0, 2), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, DelEdgeVLBVLB) {
    TypeParam G(2);
    G.add_edge(0, 1);
    G.set_label(0, "0");
    G.set_label(1, "1");
    ASSERT_EQ(G.del_edge("0", "1"), EID(0, 1));             // Valid argument.
    ASSERT_THROW(G.del_edge("0", "1"), std::out_of_range);  // Invalid argument: edge not defined.
    ASSERT_THROW(G.del_edge("0", "2"), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, HasLabelGLB) {
    TypeParam G;
    ASSERT_FALSE(G.has_label());
    G.set_label("G");
    ASSERT_TRUE(G.has_label());
}

TYPED_TEST(ContainersTest, HasLabelVID) {
    TypeParam G(2);
    G.set_label(0, "0");
    ASSERT_TRUE(G.has_label(0));
    ASSERT_FALSE(G.has_label(1));
    ASSERT_THROW(G.has_label(2), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, HasLabelVLB) {
    TypeParam G(2);
    G.set_label(0, "0");
    ASSERT_TRUE(G.has_label("0"));
    ASSERT_FALSE(G.has_label("1"));
}

TYPED_TEST(ContainersTest, HasLabelEID) {
    TypeParam G(3);
    G.add_edge(0, 1);
    G.set_label(0, 1, ELB("0 --- 1"));
    ASSERT_TRUE(G.has_label(EID(0, 1)));
    ASSERT_THROW(G.has_label(EID(1, 2)), std::out_of_range);  // Invalid argument: edge not defined.
    ASSERT_THROW(G.has_label(EID(1, 3)), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, HasLabelELB) {
    TypeParam G(3);
    G.add_edge(0, 1);
    G.set_label(0, 1, ELB("0 --- 1"));
    ASSERT_TRUE(G.has_label(ELB("0 --- 1")));
    ASSERT_FALSE(G.has_label(ELB("1 --- 1")));
}

TYPED_TEST(ContainersTest, HasLabelVIDVID) {
    TypeParam G(3);
    G.add_edge(0, 1);
    G.set_label(0, 1, ELB("0 --- 1"));
    ASSERT_TRUE(G.has_label(0, 1));
    ASSERT_THROW(G.has_label(1, 2), std::out_of_range);  // Invalid argument: edge not defined.
    ASSERT_THROW(G.has_label(1, 3), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, HasLabelVLBVLB) {
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

TYPED_TEST(ContainersTest, GetLabelGLB) {
    TypeParam G;
    ASSERT_THROW(G.get_label(), std::out_of_range);  // Invalid argument: graph not defined.

    G.set_label("G");
    ASSERT_EQ(G.get_label(), "G");  // Valid argument.
}

TYPED_TEST(ContainersTest, GetLabelVID) {
    TypeParam G(1);
    ASSERT_THROW(G.get_label(0), std::out_of_range);  // Invalid argument: vertex label not defined.
    ASSERT_THROW(G.get_label(1), std::out_of_range);  // Invalid argument: vertex not defined.

    G.set_label(0, "0");
    ASSERT_EQ(G.get_label(0), "0");  // Valid argument.
}

TYPED_TEST(ContainersTest, GetLabelEID) {
    TypeParam G(2);
    ASSERT_THROW(G.get_label(EID(0, 1)), std::out_of_range);  // Invalid argument: edge not defined.

    G.add_edge(0, 1);
    ASSERT_THROW(G.get_label(EID(0, 1)), std::out_of_range);  // Invalid argument: edge label not defined.

    G.set_label(0, 1, ELB("0 --- 1"));
    ASSERT_EQ(G.get_label(EID(0, 1)), ELB("0 --- 1"));  // Valid argument.
}

TYPED_TEST(ContainersTest, GetLabelVIDVID) {
    TypeParam G(2);
    ASSERT_THROW(G.get_label(0, 1), std::out_of_range);  // Invalid argument: edge not defined.

    G.add_edge(0, 1);
    ASSERT_THROW(G.get_label(0, 1), std::out_of_range);  // Invalid argument: edge label not defined.

    G.set_label(0, 1, ELB("0 --- 1"));
    ASSERT_EQ(G.get_label(0, 1), ELB("0 --- 1"));  // Valid argument.
}

TYPED_TEST(ContainersTest, GetLabelVLBVLB) {
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

TYPED_TEST(ContainersTest, SetLabelGLB) {
    TypeParam G;
    ASSERT_THROW(G.set_label(""), std::invalid_argument);  // Invalid argument: empty label.

    G.set_label("G");
    ASSERT_EQ(G.get_label(), "G");  // Valid argument.

    G.set_label("G*");
    ASSERT_EQ(G.get_label(), "G*");  // Valid argument.
}

TYPED_TEST(ContainersTest, SetLabelVIDVLB) {
    TypeParam G(2);
    G.set_label(0, "0");  // Valid argument.
    ASSERT_EQ(G.get_label(0), "0");

    G.set_label(0, "1");  // Valid argument: replace label.
    ASSERT_EQ(G.get_label(0), "1");

    ASSERT_THROW(G.set_label(0, ""), std::invalid_argument);   // Invalid argument: empty label.
    ASSERT_THROW(G.set_label(1, "1"), std::invalid_argument);  // Invalid argument: duplicated label.
    ASSERT_THROW(G.set_label(2, "2"), std::out_of_range);      // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, SetLabelVLBVLB) {
    TypeParam G(2);
    G.set_label(0, "0");  // Valid argument.
    ASSERT_EQ(G.get_label(0), "0");

    G.set_label("0", "1");  // Valid argument: replace label.
    ASSERT_EQ(G.get_label(0), "1");

    ASSERT_THROW(G.set_label("1", ""), std::invalid_argument);  // Invalid argument: empty label.
    ASSERT_THROW(G.set_label(1, "1"), std::invalid_argument);   // Invalid argument: duplicated label.
    ASSERT_THROW(G.set_label(2, "2"), std::out_of_range);       // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, SetLabelEIDELB) {
    TypeParam G(3);
    EID e = G.add_edge(EID(0, 1));
    G.set_label(e, ELB("0 --- 1"));  // Valid argument.
    ASSERT_EQ(G.get_label(e), ELB("0 --- 1"));

    G.set_label(e, ELB("0 -*- 1"));  // Valid argument: replace label.
    ASSERT_EQ(G.get_label(e), ELB("0 -*- 1"));

    EID f = G.add_edge(EID(0, 2));
    ASSERT_THROW(G.set_label(e, ELB("")), std::invalid_argument);  // Invalid argument: empty label.
    ASSERT_THROW(G.set_label(f, ELB("0 -*- 1")),
                 std::invalid_argument);  // Invalid argument: duplicated label.
    ASSERT_THROW(G.set_label(EID(1, 2), ELB("1 --- 2")),
                 std::out_of_range);  // Invalid argument: edge not defined.
    ASSERT_THROW(G.set_label(EID(0, 3), ELB("0 --- 3")),
                 std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, SetLabelELBELB) {
    TypeParam G(3);
    EID e = G.add_edge(EID(0, 1));
    G.set_label(e, ELB("0 --- 1"));  // Valid argument.
    ASSERT_EQ(G.get_label(e), ELB("0 --- 1"));

    G.set_label(ELB("0 --- 1"), ELB("0 -*- 1"));  // Valid argument: replace label.
    ASSERT_EQ(G.get_label(e), ELB("0 -*- 1"));

    EID f = G.add_edge(EID(0, 2));
    ASSERT_THROW(G.set_label(e, ELB("")), std::invalid_argument);  // Invalid argument: empty label.
    ASSERT_THROW(G.set_label(f, ELB("0 -*- 1")),
                 std::invalid_argument);  // Invalid argument: duplicated label.
    ASSERT_THROW(G.set_label(EID(1, 2), ELB("1 --- 2")),
                 std::out_of_range);  // Invalid argument: edge not defined.
    ASSERT_THROW(G.set_label(EID(0, 3), ELB("0 --- 3")),
                 std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, SetLabelVIDVIDELB) {
    TypeParam G(3);
    G.add_edge(0, 1);
    G.set_label(0, 1, ELB("0 --- 1"));  // Valid argument.
    ASSERT_EQ(G.get_label(0, 1), ELB("0 --- 1"));

    G.set_label(0, 1, ELB("0 -*- 1"));  // Valid argument: replace label.
    ASSERT_EQ(G.get_label(0, 1), ELB("0 -*- 1"));

    G.add_edge(0, 2);
    ASSERT_THROW(G.set_label(0, 1, ELB("")), std::invalid_argument);  // Invalid argument: empty label.
    ASSERT_THROW(G.set_label(0, 2, ELB("0 -*- 1")),
                 std::invalid_argument);  // Invalid argument: duplicated label.
    ASSERT_THROW(G.set_label(1, 2, ELB("1 --- 2")),
                 std::out_of_range);  // Invalid argument: edge not defined.
    ASSERT_THROW(G.set_label(0, 3, ELB("0 --- 3")),
                 std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, SetLabelVLBVLBELB) {
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
    ASSERT_THROW(G.set_label("0", "1", ELB("")), std::invalid_argument);  // Invalid argument: empty label.
    ASSERT_THROW(G.set_label("0", "2", ELB("0 -*- 1")),
                 std::invalid_argument);  // Invalid argument: duplicated label.
    ASSERT_THROW(G.set_label("1", "2", ELB("1 --- 2")),
                 std::out_of_range);  // Invalid argument: edge not defined.
    ASSERT_THROW(G.set_label("0", "3", ELB("0 --- 3")),
                 std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, DelLabelVOID) {
    TypeParam G;
    G.set_label("G");
    G.del_label();
    ASSERT_FALSE(G.has_label());
    ASSERT_THROW(G.del_label(), std::out_of_range);  // Invalid argument: graph label not defined.
}

TYPED_TEST(ContainersTest, DelLabelVID) {
    TypeParam G(1);
    G.set_label(0, "0");
    G.del_label(0);
    ASSERT_FALSE(G.has_label(0));
    ASSERT_THROW(G.del_label(0), std::out_of_range);  // Invalid argument: vertex label not defined.
    ASSERT_THROW(G.del_label(1), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, DelLabelVLB) {
    TypeParam G(1);
    G.set_label(0, "0");
    G.del_label("0");
    ASSERT_FALSE(G.has_label("0"));
    ASSERT_THROW(G.del_label("0"), std::out_of_range);  // Invalid argument: vertex label not defined.
    ASSERT_THROW(G.del_label("1"), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, DelLabelEID) {
    TypeParam G(2);
    EID i = G.add_edge(0, 1);
    G.set_label(i, ELB("0 --- 1"));
    G.del_label(i);
    ASSERT_FALSE(G.has_label(i));
    ASSERT_THROW(G.del_label(i), std::out_of_range);          // Invalid argument: vertex label not defined.
    ASSERT_THROW(G.del_label(EID(1, 1)), std::out_of_range);  // Invalid argument: edge not defined.
    ASSERT_THROW(G.del_label(EID(1, 2)), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, DelLabelELB) {
    TypeParam G(2);
    G.add_edge(0, 1);
    G.set_label(0, 1, ELB("0 --- 1"));
    G.del_label(ELB("0 --- 1"));
    ASSERT_FALSE(G.has_label(ELB("0 --- 1")));
    ASSERT_THROW(G.del_label(ELB("0 --- 1")),
                 std::out_of_range);  // Invalid argument: edge label not defined.
}

TYPED_TEST(ContainersTest, DelLabelVIDVID) {
    TypeParam G(2);
    G.add_edge(0, 1);
    G.set_label(0, 1, ELB("0 --- 1"));
    G.del_label(0, 1);
    ASSERT_FALSE(G.has_label(0, 1));
    ASSERT_THROW(G.del_label(0, 1), std::out_of_range);  // Invalid argument: vertex label not defined.
    ASSERT_THROW(G.del_label(1, 1), std::out_of_range);  // Invalid argument: edge not defined.
    ASSERT_THROW(G.del_label(1, 2), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, DelLabelVLBVLB) {
    TypeParam G(2);
    G.set_label(0, "0");
    G.set_label(1, "1");
    G.add_edge("0", "1");
    G.set_label("0", "1", ELB("0 --- 1"));
    G.del_label("0", "1");
    ASSERT_FALSE(G.has_label("0", "1"));
    ASSERT_THROW(G.del_label("0", "1"), std::out_of_range);  // Invalid argument: vertex label not defined.
    ASSERT_THROW(G.del_label("1", "1"), std::out_of_range);  // Invalid argument: edge not defined.
    ASSERT_THROW(G.del_label("1", "2"), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, HasAttrVOID) {
    TypeParam G;
    G.set_attr("key", true);
    ASSERT_TRUE(G.has_attr("key"));
    ASSERT_FALSE(G.has_attr("ney"));
}

TYPED_TEST(ContainersTest, HasAttrVID) {
    TypeParam G(1);
    G.set_attr(0, "key", true);
    ASSERT_TRUE(G.has_attr(0, "key"));
    ASSERT_FALSE(G.has_attr(0, "ney"));
    ASSERT_THROW(G.has_attr(1, "key"), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, HasAttrVLB) {
    TypeParam G(1);
    G.set_label(0, "0");
    G.set_attr("0", "key", true);
    ASSERT_TRUE(G.has_attr("0", "key"));
    ASSERT_FALSE(G.has_attr("0", "ney"));
    ASSERT_THROW(G.has_attr("1", "key"), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, HasAttrEID) {
    TypeParam G(2);
    EID e = G.add_edge(EID(0, 1));
    G.set_attr(e, "key", true);
    ASSERT_TRUE(G.has_attr(e, "key"));
    ASSERT_FALSE(G.has_attr(e, "ney"));
    ASSERT_THROW(G.has_attr(EID(1, 1), "key"), std::out_of_range);  // Invalid argument: edge not defined.
    ASSERT_THROW(G.has_attr(EID(1, 2), "key"), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, HasAttrELB) {
    TypeParam G(2);
    G.add_edge(0, 1);
    G.set_label(0, 1, ELB("0 --- 1"));
    G.set_attr(0, 1, "key", true);
    ASSERT_TRUE(G.has_attr(ELB("0 --- 1"), "key"));
    ASSERT_FALSE(G.has_attr(ELB("0 --- 1"), "ney"));
    ASSERT_THROW(G.has_attr(ELB("1 --- 1"), "key"),
                 std::out_of_range);  // Invalid argument: edge label not defined.
}

TYPED_TEST(ContainersTest, HasAttrVIDVID) {
    TypeParam G(2);
    G.add_edge(0, 1);
    G.set_attr(0, 1, "key", true);
    ASSERT_TRUE(G.has_attr(0, 1, "key"));
    ASSERT_FALSE(G.has_attr(0, 1, "ney"));
    ASSERT_THROW(G.has_attr(1, 1, "key"), std::out_of_range);  // Invalid argument: edge not defined.
    ASSERT_THROW(G.has_attr(1, 2, "key"), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, HasAttrVLBVLB) {
    TypeParam G(2);
    G.set_label(0, "0");
    G.set_label(1, "1");
    G.add_edge("0", "1");
    G.set_attr("0", "1", "key", true);
    ASSERT_TRUE(G.has_attr("0", "1", "key"));
    ASSERT_FALSE(G.has_attr("0", "1", "ney"));
    ASSERT_THROW(G.has_attr("1", "1", "key"), std::out_of_range);  // Invalid argument: edge not defined.
    ASSERT_THROW(G.has_attr("1", "2", "key"), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, GetAttrVOID) {
    TypeParam G;
    G.set_attr("key", true);
    auto f0 = [&G]() { return G.template get_attr<bool>("key"); };
    ASSERT_TRUE(f0());

    auto f1 = [&G]() { return G.template get_attr<std::string>("key"); };
    ASSERT_THROW(f1(), std::bad_any_cast);  // Invalid argument: wrong template parameter.

    auto f2 = [&G]() { return G.template get_attr<bool>("ney"); };
    ASSERT_THROW(f2(), std::out_of_range);  // Invalid argument: key not defined.
}

TYPED_TEST(ContainersTest, GetAttrVID) {
    TypeParam G(1);
    G.set_attr(0, "key", true);
    auto f0 = [&G]() { return G.template get_attr<bool>(0, "key"); };
    ASSERT_TRUE(f0());

    auto f1 = [&G]() { return G.template get_attr<std::string>(0, "key"); };
    ASSERT_THROW(f1(), std::bad_any_cast);  // Invalid argument: wrong template parameter.

    auto f2 = [&G]() { return G.template get_attr<bool>(0, "ney"); };
    ASSERT_THROW(f2(), std::out_of_range);  // Invalid argument: key not defined.

    auto f3 = [&G]() { return G.template get_attr<bool>(1, "key"); };
    ASSERT_THROW(f3(), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, GetAttrVLB) {
    TypeParam G(1);
    G.set_label(0, "0");
    G.set_attr("0", "key", true);
    auto f0 = [&G]() { return G.template get_attr<bool>("0", "key"); };
    ASSERT_TRUE(f0());

    auto f1 = [&G]() { return G.template get_attr<std::string>("0", "key"); };
    ASSERT_THROW(f1(), std::bad_any_cast);  // Invalid argument: wrong template parameter.

    auto f2 = [&G]() { return G.template get_attr<bool>("0", "ney"); };
    ASSERT_THROW(f2(), std::out_of_range);  // Invalid argument: key not defined.

    auto f3 = [&G]() { return G.template get_attr<bool>("1", "key"); };
    ASSERT_THROW(f3(), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, GetAttrEID) {
    TypeParam G(2);
    G.add_edge(EID(0, 1));
    G.set_attr(EID(0, 1), "key", true);
    auto f0 = [&G]() { return G.template get_attr<bool>(EID(0, 1), "key"); };
    ASSERT_TRUE(f0());

    auto f1 = [&G]() { return G.template get_attr<std::string>(EID(0, 1), "key"); };
    ASSERT_THROW(f1(), std::bad_any_cast);  // Invalid argument: wrong template parameter.

    auto f2 = [&G]() { return G.template get_attr<bool>(EID(0, 1), "ney"); };
    ASSERT_THROW(f2(), std::out_of_range);  // Invalid argument: key not defined.

    auto f3 = [&G]() { return G.template get_attr<bool>(EID(1, 1), "key"); };
    ASSERT_THROW(f3(), std::out_of_range);  // Invalid argument: edge not defined.

    auto f4 = [&G]() { return G.template get_attr<bool>(EID(1, 2), "key"); };
    ASSERT_THROW(f4(), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, GetAttrELB) {
    TypeParam G(2);
    G.add_edge(EID(0, 1));
    G.set_label(EID(0, 1), ELB("0 --- 1"));
    G.set_attr(ELB("0 --- 1"), "key", true);
    auto f0 = [&G]() { return G.template get_attr<bool>(ELB("0 --- 1"), "key"); };
    ASSERT_TRUE(f0());

    auto f1 = [&G]() { return G.template get_attr<std::string>(ELB("0 --- 1"), "key"); };
    ASSERT_THROW(f1(), std::bad_any_cast);  // Invalid argument: wrong template parameter.

    auto f2 = [&G]() { return G.template get_attr<bool>(ELB("0 --- 1"), "ney"); };
    ASSERT_THROW(f2(), std::out_of_range);  // Invalid argument: key not defined.

    auto f3 = [&G]() { return G.template get_attr<bool>(ELB("1 --- 1"), "key"); };
    ASSERT_THROW(f3(), std::out_of_range);  // Invalid argument: edge not defined.
}

TYPED_TEST(ContainersTest, GetAttrVIDVID) {
    TypeParam G(2);
    G.add_edge(0, 1);
    G.set_attr(0, 1, "key", true);
    auto f0 = [&G]() { return G.template get_attr<bool>(0, 1, "key"); };
    ASSERT_TRUE(f0());

    auto f1 = [&G]() { return G.template get_attr<std::string>(0, 1, "key"); };
    ASSERT_THROW(f1(), std::bad_any_cast);  // Invalid argument: wrong template parameter.

    auto f2 = [&G]() { return G.template get_attr<bool>(0, 1, "ney"); };
    ASSERT_THROW(f2(), std::out_of_range);  // Invalid argument: key not defined.

    auto f3 = [&G]() { return G.template get_attr<bool>(1, 1, "key"); };
    ASSERT_THROW(f3(), std::out_of_range);  // Invalid argument: edge not defined.

    auto f4 = [&G]() { return G.template get_attr<bool>(1, 2, "key"); };
    ASSERT_THROW(f4(), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, GetAttrVLBVLB) {
    TypeParam G(2);
    G.set_label(0, "0");
    G.set_label(1, "1");
    G.add_edge("0", "1");
    G.set_attr("0", "1", "key", true);
    auto f0 = [&G]() { return G.template get_attr<bool>("0", "1", "key"); };
    ASSERT_TRUE(f0());

    auto f1 = [&G]() { return G.template get_attr<std::string>("0", "1", "key"); };
    ASSERT_THROW(f1(), std::bad_any_cast);  // Invalid argument: wrong template parameter.

    auto f2 = [&G]() { return G.template get_attr<bool>("0", "1", "ney"); };
    ASSERT_THROW(f2(), std::out_of_range);  // Invalid argument: key not defined.

    auto f3 = [&G]() { return G.template get_attr<bool>("1", "1", "key"); };
    ASSERT_THROW(f3(), std::out_of_range);  // Invalid argument: edge not defined.

    auto f4 = [&G]() { return G.template get_attr<bool>("1", "2", "key"); };
    ASSERT_THROW(f4(), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, SetAttrVOID) {
    TypeParam G;

    auto f0 = [&G]() { G.set_attr("key", true); };
    ASSERT_NO_THROW(f0());

    auto f1 = [&G]() { G.set_attr("key", 1.23); };
    ASSERT_NO_THROW(f1());

    auto f2 = [&G]() { G.template set_attr<std::string>("key", "value"); };
    ASSERT_NO_THROW(f2());
}

TYPED_TEST(ContainersTest, SetAttrVID) {
    TypeParam G(1);

    auto f0 = [&G]() { G.set_attr(0, "key", true); };
    ASSERT_NO_THROW(f0());

    auto f1 = [&G]() { G.set_attr(0, "key", 1.23); };
    ASSERT_NO_THROW(f1());

    auto f2 = [&G]() { G.template set_attr<std::string>(0, "key", "value"); };
    ASSERT_NO_THROW(f2());

    auto f3 = [&G]() { G.template set_attr<std::string>(1, "key", "value"); };
    ASSERT_THROW(f3(), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, SetAttrVLB) {
    TypeParam G(1);
    G.set_label(0, "0");

    auto f0 = [&G]() { G.set_attr("0", "key", true); };
    ASSERT_NO_THROW(f0());

    auto f1 = [&G]() { G.set_attr("0", "key", 1.23); };
    ASSERT_NO_THROW(f1());

    auto f2 = [&G]() { G.template set_attr<std::string>("0", "key", "value"); };
    ASSERT_NO_THROW(f2());

    auto f3 = [&G]() { G.template set_attr<std::string>("1", "key", "value"); };
    ASSERT_THROW(f3(), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, SetAttrEID) {
    TypeParam G(2);
    G.add_edge(EID(0, 1));

    auto f0 = [&G]() { G.set_attr(EID(0, 1), "key", true); };
    ASSERT_NO_THROW(f0());

    auto f1 = [&G]() { G.set_attr(EID(0, 1), "key", 1.23); };
    ASSERT_NO_THROW(f1());

    auto f2 = [&G]() { G.template set_attr<std::string>(EID(0, 1), "key", "value"); };
    ASSERT_NO_THROW(f2());

    auto f3 = [&G]() { G.template set_attr<std::string>(EID(1, 1), "key", "value"); };
    ASSERT_THROW(f3(), std::out_of_range);  // Invalid argument: edge not defined.

    auto f4 = [&G]() { G.template set_attr<std::string>(EID(1, 2), "key", "value"); };
    ASSERT_THROW(f4(), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, SetAttrELB) {
    TypeParam G(2);
    G.add_edge(0, 1);
    G.set_label(0, 1, ELB("0 --- 1"));

    auto f0 = [&G]() { G.set_attr(ELB("0 --- 1"), "key", true); };
    ASSERT_NO_THROW(f0());

    auto f1 = [&G]() { G.set_attr(ELB("0 --- 1"), "key", 1.23); };
    ASSERT_NO_THROW(f1());

    auto f2 = [&G]() { G.template set_attr<std::string>(ELB("0 --- 1"), "key", "value"); };
    ASSERT_NO_THROW(f2());

    auto f3 = [&G]() { G.template set_attr<std::string>(ELB("1 --- 1"), "key", "value"); };
    ASSERT_THROW(f3(), std::out_of_range);  // Invalid argument: edge not defined.
}

TYPED_TEST(ContainersTest, SetAttrVIDVID) {
    TypeParam G(2);
    G.add_edge(0, 1);

    auto f0 = [&G]() { G.set_attr(0, 1, "key", true); };
    ASSERT_NO_THROW(f0());

    auto f1 = [&G]() { G.set_attr(0, 1, "key", 1.23); };
    ASSERT_NO_THROW(f1());

    auto f2 = [&G]() { G.template set_attr<std::string>(0, 1, "key", "value"); };
    ASSERT_NO_THROW(f2());

    auto f3 = [&G]() { G.template set_attr<std::string>(1, 1, "key", "value"); };
    ASSERT_THROW(f3(), std::out_of_range);  // Invalid argument: edge not defined.

    auto f4 = [&G]() { G.template set_attr<std::string>(1, 2, "key", "value"); };
    ASSERT_THROW(f4(), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, SetAttrVLBVLB) {
    TypeParam G(2);
    G.set_label(0, "0");
    G.set_label(1, "1");
    G.add_edge("0", "1");

    auto f0 = [&G]() { G.set_attr("0", "1", "key", true); };
    ASSERT_NO_THROW(f0());

    auto f1 = [&G]() { G.set_attr("0", "1", "key", 1.23); };
    ASSERT_NO_THROW(f1());

    auto f2 = [&G]() { G.template set_attr<std::string>("0", "1", "key", "value"); };
    ASSERT_NO_THROW(f2());

    auto f3 = [&G]() { G.template set_attr<std::string>("1", "1", "key", "value"); };
    ASSERT_THROW(f3(), std::out_of_range);  // Invalid argument: edge not defined.

    auto f4 = [&G]() { G.template set_attr<std::string>("1", "2", "key", "value"); };
    ASSERT_THROW(f4(), std::out_of_range);  // Invalid argument: vertex not defined.
}

TYPED_TEST(ContainersTest, DelAttrVOID) {
    TypeParam G;

    auto f0 = [&G]() {
        G.set_attr("key", true);
        G.del_attr("key");
    };
    ASSERT_NO_THROW(f0());

    auto f1 = [&G]() {
        G.set_attr("key", 1.23);
        G.del_attr("key");
    };
    ASSERT_NO_THROW(f1());

    auto f2 = [&G]() {
        G.template set_attr<std::string>("key", "value");
        G.del_attr("key");
    };
    ASSERT_NO_THROW(f2());

    auto f3 = [&G]() { G.del_attr("key"); };
    ASSERT_THROW(f3(), std::out_of_range);  // Invalid argument: key not defined.
}

TYPED_TEST(ContainersTest, DelAttrVID) {
    TypeParam G(1);

    auto f0 = [&G]() {
        G.set_attr(0, "key", true);
        G.del_attr(0, "key");
    };
    ASSERT_NO_THROW(f0());

    auto f1 = [&G]() {
        G.set_attr(0, "key", 1.23);
        G.del_attr(0, "key");
    };
    ASSERT_NO_THROW(f1());

    auto f2 = [&G]() {
        G.template set_attr<std::string>(0, "key", "value");
        G.del_attr(0, "key");
    };
    ASSERT_NO_THROW(f2());

    auto f3 = [&G]() { G.del_attr(1, "key"); };
    ASSERT_THROW(f3(), std::out_of_range);  // Invalid argument: vertex not defined.

    auto f4 = [&G]() { G.del_attr(0, "key"); };
    ASSERT_THROW(f4(), std::out_of_range);  // Invalid argument: key not defined.
}

TYPED_TEST(ContainersTest, DelAttrVLB) {
    TypeParam G(1);
    G.set_label(0, "0");

    auto f0 = [&G]() {
        G.set_attr("0", "key", true);
        G.del_attr("0", "key");
    };
    ASSERT_NO_THROW(f0());

    auto f1 = [&G]() {
        G.set_attr("0", "key", 1.23);
        G.del_attr("0", "key");
    };
    ASSERT_NO_THROW(f1());

    auto f2 = [&G]() {
        G.template set_attr<std::string>("0", "key", "value");
        G.del_attr("0", "key");
    };
    ASSERT_NO_THROW(f2());

    auto f3 = [&G]() { G.del_attr("1", "key"); };
    ASSERT_THROW(f3(), std::out_of_range);  // Invalid argument: vertex not defined.

    auto f4 = [&G]() { G.del_attr("0", "key"); };
    ASSERT_THROW(f4(), std::out_of_range);  // Invalid argument: key not defined.
}

TYPED_TEST(ContainersTest, DelAttrEID) {
    TypeParam G(2);
    G.add_edge(EID(0, 1));

    auto f0 = [&G]() {
        G.set_attr(EID(0, 1), "key", true);
        G.del_attr(EID(0, 1), "key");
    };
    ASSERT_NO_THROW(f0());

    auto f1 = [&G]() {
        G.set_attr(EID(0, 1), "key", 1.23);
        G.del_attr(EID(0, 1), "key");
    };
    ASSERT_NO_THROW(f1());

    auto f2 = [&G]() {
        G.template set_attr<std::string>(EID(0, 1), "key", "value");
        G.del_attr(EID(0, 1), "key");
    };
    ASSERT_NO_THROW(f2());

    auto f3 = [&G]() { G.del_attr(EID(1, 1), "key"); };
    ASSERT_THROW(f3(), std::out_of_range);  // Invalid argument: edge not defined.

    auto f4 = [&G]() { G.del_attr(EID(1, 2), "key"); };
    ASSERT_THROW(f4(), std::out_of_range);  // Invalid argument: vertex not defined.

    auto f5 = [&G]() { G.del_attr(EID(0, 1), "key"); };
    ASSERT_THROW(f5(), std::out_of_range);  // Invalid argument: key not defined.
}

TYPED_TEST(ContainersTest, DelAttrELB) {
    TypeParam G(2);
    G.add_edge(0, 1);
    G.set_label(0, 1, ELB("0 --- 1"));

    auto f0 = [&G]() {
        G.set_attr(ELB("0 --- 1"), "key", true);
        G.del_attr(ELB("0 --- 1"), "key");
    };
    ASSERT_NO_THROW(f0());

    auto f1 = [&G]() {
        G.set_attr(ELB("0 --- 1"), "key", 1.23);
        G.del_attr(ELB("0 --- 1"), "key");
    };
    ASSERT_NO_THROW(f1());

    auto f2 = [&G]() {
        G.template set_attr<std::string>(ELB("0 --- 1"), "key", "value");
        G.del_attr(ELB("0 --- 1"), "key");
    };
    ASSERT_NO_THROW(f2());

    auto f3 = [&G]() { G.del_attr(ELB("1 --- 1"), "key"); };
    ASSERT_THROW(f3(), std::out_of_range);  // Invalid argument: edge not defined.

    auto f4 = [&G]() { G.del_attr(ELB("0 --- 1"), "key"); };
    ASSERT_THROW(f4(), std::out_of_range);  // Invalid argument: key not defined.
}

TYPED_TEST(ContainersTest, DelAttrVIDVID) {
    TypeParam G(2);
    G.add_edge(0, 1);

    auto f0 = [&G]() {
        G.set_attr(0, 1, "key", true);
        G.del_attr(0, 1, "key");
    };
    ASSERT_NO_THROW(f0());

    auto f1 = [&G]() {
        G.set_attr(0, 1, "key", 1.23);
        G.del_attr(0, 1, "key");
    };
    ASSERT_NO_THROW(f1());

    auto f2 = [&G]() {
        G.template set_attr<std::string>(0, 1, "key", "value");
        G.del_attr(0, 1, "key");
    };
    ASSERT_NO_THROW(f2());

    auto f3 = [&G]() { G.del_attr(1, 1, "key"); };
    ASSERT_THROW(f3(), std::out_of_range);  // Invalid argument: edge not defined.

    auto f4 = [&G]() { G.del_attr(1, 2, "key"); };
    ASSERT_THROW(f4(), std::out_of_range);  // Invalid argument: vertex not defined.

    auto f5 = [&G]() { G.del_attr(0, 1, "key"); };
    ASSERT_THROW(f5(), std::out_of_range);  // Invalid argument: key not defined.
}

TYPED_TEST(ContainersTest, DelAttrVLBVLB) {
    TypeParam G(2);
    G.set_label(0, "0");
    G.set_label(1, "1");
    G.add_edge("0", "1");

    auto f0 = [&G]() {
        G.set_attr("0", "1", "key", true);
        G.del_attr("0", "1", "key");
    };
    ASSERT_NO_THROW(f0());

    auto f1 = [&G]() {
        G.set_attr("0", "1", "key", 1.23);
        G.del_attr("0", "1", "key");
    };
    ASSERT_NO_THROW(f1());

    auto f2 = [&G]() {
        G.template set_attr<std::string>("0", "1", "key", "value");
        G.del_attr("0", "1", "key");
    };
    ASSERT_NO_THROW(f2());

    auto f3 = [&G]() { G.del_attr("1", "1", "key"); };
    ASSERT_THROW(f3(), std::out_of_range);  // Invalid argument: edge not defined.

    auto f4 = [&G]() { G.del_attr("1", "2", "key"); };
    ASSERT_THROW(f4(), std::out_of_range);  // Invalid argument: vertex not defined.

    auto f5 = [&G]() { G.del_attr("0", "1", "key"); };
    ASSERT_THROW(f5(), std::out_of_range);  // Invalid argument: key not defined.
}

TYPED_TEST(ContainersTest, IsNull) {
    TypeParam G;
    ASSERT_TRUE(G.is_null());
    G.add_vertex();
    ASSERT_FALSE(G.is_null());
}

TYPED_TEST(ContainersTest, IsTrivial) {
    TypeParam G;
    ASSERT_FALSE(G.is_trivial());
    G.add_vertex();
    ASSERT_TRUE(G.is_trivial());
}

TYPED_TEST(ContainersTest, IsComplete) {
    TypeParam G;
    ASSERT_TRUE(G.is_complete());
    G.add_vertex();
    ASSERT_TRUE(G.is_complete());
    G.add_vertex();
    ASSERT_FALSE(G.is_complete());
    G.add_edge(0, 1);
    ASSERT_TRUE(G.is_complete());
}

TYPED_TEST(ContainersTest, Hash) {
    TypeParam G, H;
    ASSERT_EQ(std::hash<AbstractGraph>{}(G), 0);
    ASSERT_EQ(std::hash<AbstractGraph>{}(G), std::hash<AbstractGraph>{}(H));

    G.add_vertex();
    ASSERT_NE(std::hash<AbstractGraph>{}(G), std::hash<AbstractGraph>{}(H));
    H.add_vertex();
    ASSERT_EQ(std::hash<AbstractGraph>{}(G), std::hash<AbstractGraph>{}(H));

    G.add_vertex();
    H.add_vertex();

    G.add_edge(0, 1);
    ASSERT_NE(std::hash<AbstractGraph>{}(G), std::hash<AbstractGraph>{}(H));
    H.add_edge(0, 1);
    ASSERT_EQ(std::hash<AbstractGraph>{}(G), std::hash<AbstractGraph>{}(H));

    G.set_label("HASH");
    ASSERT_NE(std::hash<AbstractGraph>{}(G), std::hash<AbstractGraph>{}(H));
    H.set_label("HASH");
    ASSERT_EQ(std::hash<AbstractGraph>{}(G), std::hash<AbstractGraph>{}(H));

    G.set_label(0, "0");
    ASSERT_NE(std::hash<AbstractGraph>{}(G), std::hash<AbstractGraph>{}(H));
    H.set_label(0, "0");
    ASSERT_EQ(std::hash<AbstractGraph>{}(G), std::hash<AbstractGraph>{}(H));

    G.set_label(0, 1, ELB("0 --- 1"));
    ASSERT_NE(std::hash<AbstractGraph>{}(G), std::hash<AbstractGraph>{}(H));
    H.set_label(0, 1, ELB("0 --- 1"));
    ASSERT_EQ(std::hash<AbstractGraph>{}(G), std::hash<AbstractGraph>{}(H));
}

TYPED_TEST(ContainersTest, ToStream) {
    TypeParam G;
    std::ostringstream out;
    
    out << G;
    ASSERT_EQ(out.str(), ("(label = '', V = (), E = (), Vp = (), Ep = ())"));

    G.set_label("G");
    G.add_vertex("0");
    G.add_vertex("1");
    G.add_edge("0", "1");
    G.set_label("0", "1", ELB("0 --- 1"));
    out.str("");
    out.clear();
    out << G;
    ASSERT_EQ(out.str(), ("(label = 'G', V = (0, 1, ), E = ((0, 1), ), Vp = ((0, '0'), (1, '1'), ), Ep = (((0, 1), '0 --- 1'), ))"));
}
