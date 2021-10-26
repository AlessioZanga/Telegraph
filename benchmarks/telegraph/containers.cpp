#include <benchmark/benchmark.h>

#include <telegraph/telegraph>

template <typename T>
void SizeConstructor(benchmark::State& state) {
    for (auto _ : state) {
        T G(state.range(0));
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK(SizeConstructor<DirectedDenseGraph>)->RangeMultiplier(2)->Range(8, 8 << 10)->Complexity(benchmark::oNSquared);

BENCHMARK_MAIN();
