#include <gtest/gtest.h>

#include <EventHandler.hh>
#include <memory>

TEST(EventHandler, InheritanceAndVariantEqual) {
    EXPECT_EQ(lambdas(5), inheritance(std::make_shared<IntEvent>(5)));
    EXPECT_EQ(getIf(5), inheritance(std::make_shared<IntEvent>(5)));
    EXPECT_EQ(holdsAlternative(5), inheritance(std::make_shared<IntEvent>(5)));

    EXPECT_EQ(lambdas("foobar"),
              inheritance(std::make_shared<StringEvent>("foobar")));
    EXPECT_EQ(getIf("foobar"),
              inheritance(std::make_shared<StringEvent>("foobar")));
    EXPECT_EQ(holdsAlternative("foobar"),
              inheritance(std::make_shared<StringEvent>("foobar")));

    EXPECT_EQ(lambdas(FatType{5, "foobar"}),
              inheritance(std::make_shared<FatTypeEvent>(5, "foobar")));
    EXPECT_EQ(getIf(FatType{5, "foobar"}),
              inheritance(std::make_shared<FatTypeEvent>(5, "foobar")));
    EXPECT_EQ(holdsAlternative(FatType{5, "foobar"}),
              inheritance(std::make_shared<FatTypeEvent>(5, "foobar")));
}
