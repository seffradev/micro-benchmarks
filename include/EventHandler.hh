#ifndef _EVENT_HANDLER_HH_
#define _EVENT_HANDLER_HH_

#include <functional>
#include <memory>
#include <string>
#include <variant>

template <typename... Ts>
struct Overloads : Ts... {
    using Ts::operator()...;
};

struct FatType {
    int         i;
    std::string s;
};

template <>
struct std::hash<FatType> {
    size_t operator()(const FatType& ft) const noexcept {
        return std::hash<int>{}(ft.i) ^ (std::hash<std::string>{}(ft.s) << 1);
    }
};

using VariantEvent = std::variant<int, std::string, FatType>;

constexpr auto lambdas(const VariantEvent& event) {
    return std::visit(
        Overloads{
            [](int i) { return std::hash<int>{}(i); },
            [](std::string s) { return std::hash<std::string>{}(s); },
            [](FatType ft) { return std::hash<FatType>{}(ft); },
        },
        event);
}

constexpr auto noexceptLambdas(const VariantEvent& event) {
    return std::visit(
        Overloads{
            [](int i) noexcept { return std::hash<int>{}(i); },
            [](std::string s) noexcept { return std::hash<std::string>{}(s); },
            [](FatType ft) noexcept { return std::hash<FatType>{}(ft); },
        },
        event);
}

constexpr auto getIf(const VariantEvent& event) {
    if (auto ie = std::get_if<int>(&event)) {
        return std::hash<int>{}(*ie);
    } else if (auto se = std::get_if<std::string>(&event)) {
        return std::hash<std::string>{}(*se);
    } else if (auto fte = std::get_if<FatType>(&event)) {
        return std::hash<FatType>{}(*fte);
    } else {
        return static_cast<size_t>(0);
    }
}

constexpr auto holdsAlternative(const VariantEvent& event) {
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

constexpr auto inheritance(std::shared_ptr<BaseEvent> event) {
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

#endif
