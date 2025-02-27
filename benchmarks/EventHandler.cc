#include <benchmark/benchmark.h>

#include <EventHandler.hh>
#include <memory>

constinit size_t numberOfElements = 100000;

auto setupVariant() {
    auto events = std::vector<VariantEvent>{};
    events.reserve(numberOfElements);
    for (size_t i = 0; i < events.capacity(); ++i) {
        if (i % 3 == 0) {
            events.emplace_back(std::rand());
        } else if (i % 3 == 1) {
            events.emplace_back("foobarbaz");
        } else {
            events.emplace_back(FatType{std::rand(), "foobarbaz"});
        }
    }
    return events;
}

static void BM_lambda(benchmark::State& state) {
    auto events = setupVariant();
    for (auto _ : state) {
        for (const auto& event : events) {
            benchmark::DoNotOptimize(lambdas(event));
        }
    }
}

static void BM_getIf(benchmark::State& state) {
    auto events = setupVariant();
    for (auto _ : state) {
        for (const auto& event : events) {
            benchmark::DoNotOptimize(getIf(event));
        }
    }
}

static void BM_holdsAlternative(benchmark::State& state) {
    auto events = setupVariant();
    for (auto _ : state) {
        for (const auto& event : events) {
            benchmark::DoNotOptimize(holdsAlternative(event));
        }
    }
}

auto setupInheritance() {
    auto events = std::vector<std::shared_ptr<BaseEvent>>{};
    events.reserve(numberOfElements);

    for (size_t i = 0; i < events.capacity(); ++i) {
        if (i % 3 == 0) {
            events.emplace_back(std::make_unique<IntEvent>(std::rand()));
        } else if (i % 3 == 1) {
            events.emplace_back(std::make_unique<StringEvent>("foobarbaz"));
        } else {
            events.emplace_back(
                std::make_unique<FatTypeEvent>(std::rand(), "foobarbaz"));
        }
    }

    return events;
}

static void BM_inheritance(benchmark::State& state) {
    auto events = setupInheritance();
    for (auto _ : state) {
        for (const auto& event : events) {
            benchmark::DoNotOptimize(inheritance(event));
        }
    }
}

BENCHMARK(BM_lambda);
BENCHMARK(BM_getIf);
BENCHMARK(BM_holdsAlternative);
BENCHMARK(BM_inheritance);

BENCHMARK_MAIN();
