#include <benchmark/benchmark.h>

#include <cstdlib>
#include <ctime>
#include <numeric>
#include <random>
#include <telegraph/telegraph>
#include <unordered_set>

template <typename TypeParam>
void DefaultConstructor(benchmark::State& state) {
    for (auto _ : state) TypeParam G;
    state.SetComplexityN(state.range(0));
}

template <typename TypeParam>
void CopyConstructor(benchmark::State& state) {
    TypeParam G(state.range(0));
    for (auto _ : state) TypeParam H(G);
    state.SetComplexityN(state.range(0));
}

template <typename TypeParam>
void AssignmentOperator(benchmark::State& state) {
    TypeParam G(state.range(0)), H;
    for (auto _ : state) H = G;
    state.SetComplexityN(state.range(0));
}

template <typename TypeParam>
void Destructor(benchmark::State& state) {
    TypeParam* G = new TypeParam(state.range(0));
    for (auto _ : state) delete G;
    state.SetComplexityN(state.range(0));
}

template <typename TypeParam>
void OrderConstructor(benchmark::State& state) {
    for (auto _ : state) TypeParam G(state.range(0));
    state.SetComplexityN(state.range(0));
}

template <typename TypeParam>
void VertexIteratorConstructor(benchmark::State& state) {
    VIDs V;
    // Use current time as seed.
    std::mt19937 rand(std::time(0));
    while (int64_t(V.size()) < state.range(0)) {
        // Insert random edge into edge set.
        V.insert(rand() % (state.range(0) * 2));
    }
    for (auto _ : state) TypeParam G(V.begin(), V.end());
    state.SetComplexityN(state.range(0));
}

template <typename TypeParam>
void EdgeIteratorConstructor(benchmark::State& state) {
    std::unordered_set<EID> E;
    std::mt19937 rand(std::time(0));
    while (int64_t(E.size()) < state.range(0)) {
        E.insert({rand() % state.range(0), rand() % state.range(0)});
    }
    for (auto _ : state) TypeParam G(E.begin(), E.end());
    state.SetComplexityN(state.range(0));
}

template <typename TypeParam>
void AdjacencyListConstructor(benchmark::State& state) {
    std::unordered_set<EID> E;
    std::mt19937 rand(std::time(0));
    while (int64_t(E.size()) < state.range(0)) {
        E.insert({rand() % state.range(0), rand() % state.range(0)});
    }
    // Convert to adjacency list.
    AdjacencyList A;
    for (const auto& [v, u] : E) {
        A[v].insert(u);
        A[u];
    }
    for (auto _ : state) TypeParam G(A);
    state.SetComplexityN(state.range(0));
}

template <typename TypeParam>
void AdjacencyMatrixConstructor(benchmark::State& state) {
    std::unordered_set<EID> E;
    std::mt19937 rand(std::time(0));
    while (int64_t(E.size()) < state.range(0)) {
        E.insert({rand() % state.range(0), rand() % state.range(0)});
    }
    // Convert to adjacency list.
    AdjacencyMatrix A(state.range(0), state.range(0));
    for (const auto& [v, u] : E) A(v, u) = 1;
    for (auto _ : state) TypeParam G(A);
    state.SetComplexityN(state.range(0));
}

template <typename TypeParam>
void SparseAdjacencyMatrixConstructor(benchmark::State& state) {
    std::unordered_set<EID> E;
    std::mt19937 rand(std::time(0));
    while (int64_t(E.size()) < state.range(0)) {
        E.insert({rand() % state.range(0), rand() % state.range(0)});
    }
    // Convert to adjacency list.
    SparseAdjacencyMatrix A(state.range(0), state.range(0));
    for (const auto& [v, u] : E) A.coeffRef(v, u) = 1;
    for (auto _ : state) TypeParam G(A);
    state.SetComplexityN(state.range(0));
}

BENCHMARK(DefaultConstructor<DirectedDenseGraph>)->RangeMultiplier(2)->Range(8, 8 << 10)->Complexity(benchmark::o1);

BENCHMARK(CopyConstructor<DirectedDenseGraph>)->RangeMultiplier(2)->Range(8, 8 << 10)->Complexity(benchmark::oNSquared);

BENCHMARK(AssignmentOperator<DirectedDenseGraph>)
    ->RangeMultiplier(2)
    ->Range(8, 8 << 10)
    ->Complexity(benchmark::oNSquared);

// BENCHMARK(Destructor<DirectedDenseGraph>)->RangeMultiplier(2)->Range(8, 8 << 10)->Complexity(benchmark::oNSquared);

BENCHMARK(OrderConstructor<DirectedDenseGraph>)
    ->RangeMultiplier(2)
    ->Range(8, 8 << 10)
    ->Complexity(benchmark::oNSquared);

BENCHMARK(VertexIteratorConstructor<DirectedDenseGraph>)
    ->RangeMultiplier(2)
    ->Range(8, 8 << 10)
    ->Complexity(benchmark::oNSquared);

BENCHMARK(EdgeIteratorConstructor<DirectedDenseGraph>)
    ->RangeMultiplier(2)
    ->Range(8, 8 << 10)
    ->Complexity(benchmark::oN);

BENCHMARK(AdjacencyListConstructor<DirectedDenseGraph>)
    ->RangeMultiplier(2)
    ->Range(8, 8 << 10)
    ->Complexity(benchmark::oN);

BENCHMARK(AdjacencyMatrixConstructor<DirectedDenseGraph>)
    ->RangeMultiplier(2)
    ->Range(8, 8 << 10)
    ->Complexity(benchmark::oN);

BENCHMARK(SparseAdjacencyMatrixConstructor<DirectedDenseGraph>)
    ->RangeMultiplier(2)
    ->Range(8, 8 << 10)
    ->Complexity(benchmark::oN);

BENCHMARK_MAIN();
