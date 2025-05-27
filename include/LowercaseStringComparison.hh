#ifndef LOWERCASE_STRING_ELEMENT_COMPARISON_HH
#define LOWERCASE_STRING_ELEMENT_COMPARISON_HH

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <functional>
#include <random>
#include <ranges>
#include <utility>
#include <vector>

using IntegerDistribution = std::uniform_int_distribution<int8_t>;
using CharacterDistribution = std::uniform_int_distribution<char>;
using Strings = std::vector<std::pair<std::string, std::string>>;

/// Generate a random set of string pairs.
constexpr auto generate(auto&& device, size_t numberOfElements)
    -> Strings {
    // Set up random sources and distributions
    auto generator = std::default_random_engine{device};
    auto chars     = CharacterDistribution{'A', 'Z'};
    auto ints      = IntegerDistribution{5, 35};

    // Generate two random strings, put them in a pair in the
    // vector.
    auto strings = Strings{};
    strings.reserve(numberOfElements);
    for (size_t i = 0; i < numberOfElements; ++i) {
        auto s = std::string(ints(generator), '0');
        for (size_t j = 0; j < s.size(); ++j) {
            s[j] = chars(generator);
        }
        auto t = std::string(ints(generator), '0');
        for (size_t j = 0; j < t.size(); ++j) {
            t[j] = chars(generator);
        }
        strings.emplace_back(std::pair{s, t});
    }

    return strings;
}

/// Use ranges and lazily-evaluated views to compare the sequences
/// of lowercase characters.
constexpr auto ranges(Strings& strings) -> bool {
    for (auto& pair : strings) {
        auto& [first, second] = pair;

        auto transform = std::views::transform(::tolower);

        auto firstRange  = first | transform;
        auto secondRange = second | transform;

        if (std::ranges::equal(firstRange, secondRange)) {
            return true;
        }
    }

    return false;
}

/// Use `strcmp` from C to compare the strings after materializing
/// them into strings with all lowercase characters.
constexpr auto strcmp(Strings& strings) -> bool {
    for (auto& pair : strings) {
        auto& [first, second] = pair;

        std::transform(first.begin(), first.end(), first.begin(),
                       ::tolower);

        std::transform(second.begin(), second.end(),
                       second.begin(), ::tolower);

        if (!strcmp(first.c_str(), second.c_str())) {
            return true;
        }
    }

    return false;
}

/// Use regular string comparison after materializing strings into
/// all lowercase character strings.
constexpr auto materialize(Strings& strings) -> bool {
    for (auto& pair : strings) {
        auto& [first, second] = pair;

        std::transform(first.begin(), first.end(), first.begin(),
                       ::tolower);

        std::transform(second.begin(), second.end(),
                       second.begin(), ::tolower);

        if (first == second) {
            return true;
        }
    }

    return false;
}

/// Use a naive for-loop to iterate all characters in both strings
/// while converting the characters on demand.
constexpr auto forLoop(Strings& strings) -> bool {
    for (auto& pair : strings) {
        auto& [first, second] = pair;

        if (first.size() != second.size()) {
            continue;
        }

        auto inequal = false;
        for (size_t i = 0; i < first.size(); ++i) {
            if (tolower(first[i]) != tolower(second[i])) {
                inequal = true;
                break;
            }
        }

        if (inequal) {
            continue;
        }

        return true;
    }

    return false;
}

/// Use `std::ranges::equal` to compare the sequences of lowercase
/// characters.
constexpr auto equal(Strings& strings) -> bool {
    for (auto& pair : strings) {
        if (std::ranges::equal(pair.first, pair.second,
                               std::ranges::equal_to{}, ::tolower,
                               ::tolower)) {
            return true;
        }
    }

    return false;
}

#endif
