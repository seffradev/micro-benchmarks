#ifndef _EVENT_HANDLER_HH_
#define _EVENT_HANDLER_HH_

#include <functional>
#include <string>
#include <variant>

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

template <typename... Ts>
struct Overloads : Ts... {
    using Ts::operator()...;
};

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

#endif
