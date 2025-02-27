#include <benchmark/benchmark.h>

#include <NewTypes.hh>
#include <map>

constinit size_t numberOfElements = 100000;

template <typename T>
std::string_view find(const std::map<T, std::string>& map) {
    if (map.find(T(std::rand())) != map.end()) {
        return "exists";
    } else {
        return "missing";
    }
}

template <typename T>
void insert(std::map<T, std::string>& map) {
    for (size_t i = 0; i < numberOfElements; ++i) {
        map.emplace(T(std::rand()), "foobar");
    }
}

template <typename T>
static void BM_insert(benchmark::State& state) {
    std::srand(time(0));
    for (auto _ : state) {
        std::map<T, std::string> map;
        insert(map);
    }
}

template <typename T>
static void BM_find(benchmark::State& state) {
    std::srand(time(0));
    std::map<T, std::string> map;
    insert(map);
    for (auto _ : state) {
        benchmark::DoNotOptimize(find(map));
    }
}

BENCHMARK(BM_insert<NewType>);
BENCHMARK(BM_insert<int>);

BENCHMARK(BM_find<NewType>);
BENCHMARK(BM_find<int>);

BENCHMARK_MAIN();
