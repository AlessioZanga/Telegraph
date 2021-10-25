#include <benchmark/benchmark.h>
#include <telegraph/telegraph>

static void BM_SizeConstructor(benchmark::State& state) {
  for (auto _ : state) {
      DenseGraph G(state.range(0));
  }
}
BENCHMARK(BM_SizeConstructor)->RangeMultiplier(2)->Range(8, 8<<10)->Complexity(benchmark::oNSquared);

BENCHMARK_MAIN();
