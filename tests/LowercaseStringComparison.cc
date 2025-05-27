#include <gtest/gtest.h>

#include <LowercaseStringComparison.hh>
#include <random>

/// Test "statistically" that the functions do the same thing
TEST(LowercaseStringComparison, Equality) {
    auto deviceResult = std::random_device{}();

    // Ensure that they give the same result in 1000
    // iterations
    for (size_t i = 0; i < 1000; ++i) {
        auto strings = generate(deviceResult, i);
        ASSERT_EQ(ranges(strings), strcmp(strings));
        ASSERT_EQ(ranges(strings), materialize(strings));
        ASSERT_EQ(ranges(strings), forLoop(strings));
        ASSERT_EQ(ranges(strings), equal(strings));
    }

    // Ensure that the first string is equal
    auto strings = generate(deviceResult, 1000);
    strings[0]   = std::pair{"abcdefg", "abcdefg"};
    ASSERT_TRUE(ranges(strings));
    ASSERT_EQ(ranges(strings), strcmp(strings));
    ASSERT_EQ(ranges(strings), materialize(strings));
    ASSERT_EQ(ranges(strings), forLoop(strings));
    ASSERT_EQ(ranges(strings), equal(strings));

    // Ensure that the last string is equal
    strings      = generate(deviceResult, 1000);
    strings[999] = std::pair{"gfedcba", "gfedcba"};
    ASSERT_TRUE(ranges(strings));
    ASSERT_EQ(ranges(strings), strcmp(strings));
    ASSERT_EQ(ranges(strings), materialize(strings));
    ASSERT_EQ(ranges(strings), forLoop(strings));
    ASSERT_EQ(ranges(strings), equal(strings));

    // Assume that no string will be equal
    strings = generate(deviceResult, 1000);
    ASSERT_FALSE(ranges(strings));
    ASSERT_EQ(ranges(strings), strcmp(strings));
    ASSERT_EQ(ranges(strings), materialize(strings));
    ASSERT_EQ(ranges(strings), forLoop(strings));
    ASSERT_EQ(ranges(strings), equal(strings));
}
