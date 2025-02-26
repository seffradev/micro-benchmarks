#include <benchmark/benchmark.h>

#include <iostream>

void SomeFunction() { std::cout << "Hello, world!" << std::endl; }

static void BM_SomeFunction(benchmark::State& state) {
    for (auto _ : state) {
        // SomeFunction();
    }
}

BENCHMARK(BM_SomeFunction);

BENCHMARK_MAIN();
