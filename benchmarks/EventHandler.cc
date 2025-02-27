#include <benchmark/benchmark.h>

#include <EventHandler.hh>
#include <functional>
#include <memory>
#include <variant>

constinit size_t numberOfElements = 100000;

auto lambdas(const VariantEvent& event) {
    return std::visit(
        Overloads{
            [](int i) { return std::hash<int>{}(i); },
            [](std::string s) { return std::hash<std::string>{}(s); },
            [](FatType ft) { return std::hash<FatType>{}(ft); },
        },
        event);
}

auto holdsAlternative(const VariantEvent& event) {
    if (std::holds_alternative<int>(event)) {
        return std::hash<int>{}(std::get<int>(event));
    } else if (std::holds_alternative<std::string>(event)) {
        return std::hash<std::string>{}(std::get<std::string>(event));
    } else if (std::holds_alternative<FatType>(event)) {
        return std::hash<FatType>{}(std::get<FatType>(event));
    } else {
        return static_cast<size_t>(0);
    }
}

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

auto inheritance(std::shared_ptr<BaseEvent> event) {
    if (auto ie = dynamic_cast<IntEvent*>(event.get())) {
        return std::hash<int>{}(ie->i);
    } else if (auto se = dynamic_cast<StringEvent*>(event.get())) {
        return std::hash<std::string>{}(se->s);
    } else if (auto fte = dynamic_cast<FatTypeEvent*>(event.get())) {
        return std::hash<FatType>{}(fte->ft);
    } else {
        return static_cast<size_t>(0);
    }
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
BENCHMARK(BM_holdsAlternative);
BENCHMARK(BM_inheritance);

BENCHMARK_MAIN();
