#include <benchmark/benchmark.h>

#include <compare>
#include <map>

struct NewType {
    int id;
    explicit NewType(int id) : id(id) {}
    constexpr std::strong_ordering operator<=>(const NewType& rhs) const noexcept = default;
};

template <typename T>
std::string_view find(const std::map<T, std::string>& map) {
    if (map.find(T(std::rand())) != map.end()) {
        return "exist";
    } else {
        return "gones";
    }
}

template <typename T>
void initialize(std::map<T, std::string>& map) {
    for (auto i = 0; i < 10000000; ++i) {
        map.emplace(T(i), "hello");
    }
}

static void BM_NewType(benchmark::State& state) {
    std::map<NewType, std::string> map;
    initialize(map);

    for (auto _ : state) {
        benchmark::DoNotOptimize(find(map));
    }
}

static void BM_Primitive(benchmark::State& state) {
    std::map<int, std::string> map;
    initialize(map);

    for (auto _ : state) {
        benchmark::DoNotOptimize(find(map));
    }
}

BENCHMARK(BM_NewType);
BENCHMARK(BM_Primitive);

BENCHMARK_MAIN();
