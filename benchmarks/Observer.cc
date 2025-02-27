#include <benchmark/benchmark.h>

#include <Observer.hh>

class TemplateImpl1 : public TemplateEmitter<int> {};

class ListenerImpl : public VirtualListener<int> {
protected:
    void notify(const int&) override {}
};

class EmitterImpl : public VirtualEmitter<int> {
public:
    void dispatch() { notifyListeners(std::rand()); }
};

// static void BM_template(benchmark::State& state) {
//     ListenerImpl listener;
//     EmitterImpl  emitter;
//
//     emitter.subscribe(&listener);
//
//     for (auto _ : state) {
//         emitter.dispatch();
//     }
// }

static void BM_virtual(benchmark::State& state) {
    ListenerImpl listener;
    EmitterImpl  emitter;

    emitter.subscribe(&listener);

    for (auto _ : state) {
        emitter.dispatch();
    }
}

// BENCHMARK(BM_template);
BENCHMARK(BM_virtual);

BENCHMARK_MAIN();
