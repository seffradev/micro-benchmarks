#include <benchmark/benchmark.h>

#include <functional>
#include <memory>
#include <variant>

constinit size_t number_of_iterations = 100000;

struct FatType {
    int         i;
    std::string s;
};

template <>
struct std::hash<FatType> {
    size_t operator()(const FatType& ft) const noexcept { return std::hash<int>{}(ft.i) ^ (std::hash<std::string>{}(ft.s) << 1); }
};

using VariantEvent = std::variant<int, std::string, FatType>;

template <typename... Ts>
struct Overloads : Ts... {
    using Ts::operator()...;
};

auto variant(const VariantEvent& event) {
    return std::visit(Overloads{
                          [](int i) { return std::hash<int>{}(i); },
                          [](std::string s) { return std::hash<std::string>{}(s); },
                          [](FatType ft) { return std::hash<FatType>{}(ft); },
                      },
                      event);
}

static void BM_Variant(benchmark::State& state) {
    auto events = std::vector<VariantEvent>{};
    events.reserve(number_of_iterations);

    for (size_t i = 0; i < events.capacity(); ++i) {
        if (i % 3 == 0) {
            events.emplace_back(std::rand());
        } else if (i % 3 == 1) {
            events.emplace_back("foobarbaz");
        } else {
            events.emplace_back(FatType{std::rand(), "foobarbaz"});
        }
    }

    for (auto _ : state) {
        for (const auto& event : events) {
            benchmark::DoNotOptimize(variant(event));
        }
    }
}

class BaseEvent {
public:
    virtual ~BaseEvent() {}
};

class IntEvent final : public BaseEvent {
public:
    explicit IntEvent(int i) : i(i) {}
    int i;
    ~IntEvent() {}
};

class StringEvent final : public BaseEvent {
public:
    explicit StringEvent(std::string s) : s(std::move(s)) {}
    std::string s;
    ~StringEvent() {}
};

class FatTypeEvent final : public BaseEvent {
public:
    explicit FatTypeEvent(int i, std::string s) : ft(i, std::move(s)) {}
    FatType ft;
    ~FatTypeEvent() {}
};

auto inheritance(std::shared_ptr<BaseEvent> event) {
    if (auto int_event = dynamic_cast<IntEvent*>(event.get())) {
        return std::hash<int>{}(int_event->i);
    } else if (auto string_event = dynamic_cast<StringEvent*>(event.get())) {
        return std::hash<std::string>{}(string_event->s);
    } else if (auto fat_type_event = dynamic_cast<FatTypeEvent*>(event.get())) {
        return std::hash<FatType>{}(fat_type_event->ft);
    } else {
        return static_cast<size_t>(0);
    }
}

static void BM_Inheritance(benchmark::State& state) {
    auto events = std::vector<std::shared_ptr<BaseEvent>>{};
    events.reserve(number_of_iterations);

    for (size_t i = 0; i < events.capacity(); ++i) {
        if (i % 3 == 0) {
            events.emplace_back(std::make_unique<IntEvent>(std::rand()));
        } else if (i % 3 == 1) {
            events.emplace_back(std::make_unique<StringEvent>("foobarbaz"));
        } else {
            events.emplace_back(std::make_unique<FatTypeEvent>(std::rand(), "foobarbaz"));
        }
    }

    for (auto _ : state) {
        for (const auto& event : events) {
            benchmark::DoNotOptimize(inheritance(event));
        }
    }
}

BENCHMARK(BM_Variant);
BENCHMARK(BM_Inheritance);

BENCHMARK_MAIN();
