#include <benchmark/benchmark.h>

#include <LowercaseStringComparison.hh>

/// These benchmarks are worst-case benchmarks for finding if two
/// strings are equal when lowercase.

// TODO: Add best-case benchmarks (and some intermediary
// benchmarks) to see if "assumed regular use-case" is more
// performant in other variants than the winner in the worst-case
// scenario.

constinit size_t numberOfElements = 100000;

static void BM_ranges(benchmark::State& state) {
    auto device  = std::random_device{};
    auto strings = generate(device(), numberOfElements);
    for (auto _ : state) {
        benchmark::DoNotOptimize(ranges(strings));
    }
}

static void BM_strcmp(benchmark::State& state) {
    auto device  = std::random_device{};
    auto strings = generate(device(), numberOfElements);
    for (auto _ : state) {
        benchmark::DoNotOptimize(strcmp(strings));
    }
}

static void BM_materialize(benchmark::State& state) {
    auto device  = std::random_device{};
    auto strings = generate(device(), numberOfElements);
    for (auto _ : state) {
        benchmark::DoNotOptimize(materialize(strings));
    }
}

static void BM_forLoop(benchmark::State& state) {
    auto device  = std::random_device{};
    auto strings = generate(device(), numberOfElements);
    for (auto _ : state) {
        benchmark::DoNotOptimize(forLoop(strings));
    }
}

static void BM_equal(benchmark::State& state) {
    auto device  = std::random_device{};
    auto strings = generate(device(), numberOfElements);
    for (auto _ : state) {
        benchmark::DoNotOptimize(equal(strings));
    }
}

BENCHMARK(BM_ranges);
BENCHMARK(BM_strcmp);
BENCHMARK(BM_materialize);
BENCHMARK(BM_forLoop);
BENCHMARK(BM_equal);

BENCHMARK_MAIN();
