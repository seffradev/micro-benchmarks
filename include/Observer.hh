#ifndef _OBSERVER_HH_
#define _OBSERVER_HH_

#include <functional>
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
    void subscribe(VirtualListener<T>* listener) {
        listeners.emplace_back(listener);
    }

protected:
    void notify_listeners(const T& event) {
        for (auto&& listener : listeners) {
            listener->notify(event);
        }
    }

private:
    std::vector<VirtualListener<T>*> listeners;
};

#endif
