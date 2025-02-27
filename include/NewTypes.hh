#include <compare>

struct NewType {
    int id;
    explicit NewType(int id) : id(id) {}
    constexpr std::strong_ordering
    operator<=>(const NewType& rhs) const noexcept = default;
};
