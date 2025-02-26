#include <benchmark/benchmark.h>

#include <functional>
#include <vector>

template <typename T>
class TemplateEmitter {
public:
    using Callback = std::function<void(const T&)>;
    void subscribe(Callback callback) { callbacks.emplace_back(callback); }

protected:
    void notify_listeners(const T& event) {
        for (auto&& callback : callbacks) {
            callback(event);
        }
    }

private:
    std::vector<Callback> callbacks;
};

template <typename Emitter, typename... Callbacks>
void subscribe_to(Emitter emitter, Callbacks... callbacks) {
    [&emitter, &callbacks...] { emitter->subscribe(callbacks...); }();
}

class TemplateImpl1 : public TemplateEmitter<int> {};

template <typename T>
class VirtualEmitter;

template <typename T>
class VirtualListener {
protected:
    friend VirtualEmitter<T>;
    virtual void notify(const T& event) = 0;
};

template <typename T>
class VirtualEmitter {
public:
    void subscribe(VirtualListener<T>* listener) { listeners.emplace_back(listener); }

protected:
    void notify_listeners(const T& event) {
        for (auto&& listener : listeners) {
            listener->notify(event);
        }
    }

private:
    std::vector<VirtualListener<T>*> listeners;
};

class ListenerImpl : public VirtualListener<int> {
protected:
    void notify(const int& event) override {}
};

class EmitterImpl : public VirtualEmitter<int> {
public:
    void do_thing() { notify_listeners(std::rand()); }
};

static void BM_Template(benchmark::State& state) {
    ListenerImpl listener;
    EmitterImpl  emitter;

    emitter.subscribe(&listener);

    for (auto _ : state) {
        emitter.do_thing();
    }
}

static void BM_Virtual(benchmark::State& state) {
    ListenerImpl listener;
    EmitterImpl  emitter;

    emitter.subscribe(&listener);

    for (auto _ : state) {
        emitter.do_thing();
    }
}

BENCHMARK(BM_Template);
BENCHMARK(BM_Virtual);

BENCHMARK_MAIN();
