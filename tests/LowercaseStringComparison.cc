#include <gtest/gtest.h>

#include <LowercaseStringComparison.hh>
#include <random>

/// Test "statistically" that the functions do the same thing
TEST(LowercaseStringComparison, Equality) {
    using namespace std::literals;

    auto deviceResult = std::random_device{}();

    // Ensure that they give the same result in 1000
    // iterations
    for (size_t i = 0; i < 1000; ++i) {
        auto needle   = generateOne(deviceResult);
        auto haystack = generate(deviceResult, i);
        ASSERT_EQ(ranges(needle, haystack),
                  strcmp(needle, haystack));
        ASSERT_EQ(ranges(needle, haystack),
                  materialize(needle, haystack));
        ASSERT_EQ(ranges(needle, haystack),
                  forLoop(needle, haystack));
        ASSERT_EQ(ranges(needle, haystack),
                  equal(needle, haystack));
    }

    // Ensure that the first string is equal
    auto needle   = "abcdefg"s;
    auto haystack = generate(deviceResult, 1000);
    haystack[0]   = "abcdefg"s;
    ASSERT_TRUE(ranges(needle, haystack));
    ASSERT_EQ(ranges(needle, haystack), strcmp(needle, haystack));
    ASSERT_EQ(ranges(needle, haystack),
              materialize(needle, haystack));
    ASSERT_EQ(ranges(needle, haystack), forLoop(needle, haystack));
    ASSERT_EQ(ranges(needle, haystack), equal(needle, haystack));

    // Ensure that the last string is equal
    needle        = "gfedcba"s;
    haystack      = generate(deviceResult, 1000);
    haystack[999] = "gfedcba"s;
    ASSERT_TRUE(ranges(needle, haystack));
    ASSERT_EQ(ranges(needle, haystack), strcmp(needle, haystack));
    ASSERT_EQ(ranges(needle, haystack),
              materialize(needle, haystack));
    ASSERT_EQ(ranges(needle, haystack), forLoop(needle, haystack));
    ASSERT_EQ(ranges(needle, haystack), equal(needle, haystack));

    // Assume that no string will be equal
    needle   = "etetetetetetetetetetetetetetetetet"s;
    haystack = generate(deviceResult, 1000);
    ASSERT_FALSE(ranges(needle, haystack));
    ASSERT_EQ(ranges(needle, haystack), strcmp(needle, haystack));
    ASSERT_EQ(ranges(needle, haystack),
              materialize(needle, haystack));
    ASSERT_EQ(ranges(needle, haystack), forLoop(needle, haystack));
    ASSERT_EQ(ranges(needle, haystack), equal(needle, haystack));
}
