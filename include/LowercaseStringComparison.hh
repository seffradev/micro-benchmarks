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
#include <vector>

using IntegerDistribution = std::uniform_int_distribution<int8_t>;
using CharacterDistribution = std::uniform_int_distribution<char>;
using Strings               = std::vector<std::string>;

/// Generate a random vector of strings.
constexpr auto generate(auto&& device, size_t numberOfElements) {
    // Set up random sources and distributions
    auto generator = std::default_random_engine{device};
    auto chars     = CharacterDistribution{'A', 'Z'};
    auto ints      = IntegerDistribution{5, 35};

    // Generate a random string and put it in the vector.
    auto strings = Strings{};
    strings.reserve(numberOfElements);
    for (size_t i = 0; i < numberOfElements; ++i) {
        auto s = std::string(ints(generator), '0');
        for (size_t j = 0; j < s.size(); ++j) {
            s[j] = chars(generator);
        }
        strings.emplace_back(s);
    }

    return strings;
}

/// Generate a random string.
constexpr auto generateOne(auto&& device) {
    // Set up random sources and distributions
    auto generator = std::default_random_engine{device};
    auto chars     = CharacterDistribution{'A', 'Z'};
    auto ints      = IntegerDistribution{5, 35};

    // Generate a random string.
    auto s = std::string(ints(generator), '0');
    for (size_t j = 0; j < s.size(); ++j) {
        s[j] = chars(generator);
    }

    return s;
}

/// Use ranges and lazily-evaluated views to compare the sequences
/// of lowercase characters.
constexpr auto ranges(const std::string& needle,
                      const Strings&     haystack) -> bool {
    auto transform         = std::views::transform(::tolower);
    auto transformedNeedle = needle | transform;

    for (auto& hay : haystack) {
        if (std::ranges::equal(transformedNeedle,
                               hay | transform)) {
            return true;
        }
    }

    return false;
}

/// Use `strcmp` from C to compare the strings after materializing
/// them into strings with all lowercase characters.
constexpr auto strcmp(std::string needle, Strings& haystack)
    -> bool {
    std::transform(needle.begin(), needle.end(), needle.begin(),
                   ::tolower);
    for (auto hay : haystack) {
        std::transform(hay.begin(), hay.end(), hay.begin(),
                       ::tolower);

        if (!strcmp(needle.c_str(), hay.c_str())) {
            return true;
        }
    }

    return false;
}

/// Use regular string comparison after materializing strings into
/// all lowercase character strings.
constexpr auto materialize(std::string needle, Strings& haystack)
    -> bool {
    std::transform(needle.begin(), needle.end(), needle.begin(),
                   ::tolower);

    for (auto hay : haystack) {
        std::transform(hay.begin(), hay.end(), hay.begin(),
                       ::tolower);

        if (needle == hay) {
            return true;
        }
    }

    return false;
}

/// Use a naive for-loop to iterate all characters in both strings
/// while converting the characters on demand.
constexpr auto
forLoop(const std::string& needle, Strings& haystack) -> bool {
    for (auto& hay : haystack) {
        if (needle.size() != hay.size()) {
            continue;
        }

        auto inequal = false;
        for (size_t i = 0; i < needle.size(); ++i) {
            if (tolower(needle[i]) != tolower(hay[i])) {
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
constexpr auto equal(const std::string& needle, Strings& haystack)
    -> bool {
    for (auto& hay : haystack) {
        if (std::ranges::equal(needle, hay,
                               std::ranges::equal_to{}, ::tolower,
                               ::tolower)) {
            return true;
        }
    }

    return false;
}

#endif
