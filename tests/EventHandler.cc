#include <gtest/gtest.h>

#include <EventHandler.hh>
#include <memory>

TEST(EventHandler, InheritanceAndVariantEqual) {
    auto ie = std::make_shared<IntEvent>(5);
    EXPECT_EQ(lambdas(5), inheritance(ie));
    EXPECT_EQ(noexceptLambdas(5), inheritance(ie));
    EXPECT_EQ(getIf(5), inheritance(ie));
    EXPECT_EQ(holdsAlternative(5), inheritance(ie));

    auto se = std::make_shared<StringEvent>("foobar");
    EXPECT_EQ(lambdas("foobar"), inheritance(se));
    EXPECT_EQ(noexceptLambdas("foobar"), inheritance(se));
    EXPECT_EQ(getIf("foobar"), inheritance(se));
    EXPECT_EQ(holdsAlternative("foobar"), inheritance(se));

    auto fte = std::make_shared<FatTypeEvent>(5, "foobar");
    EXPECT_EQ(lambdas(FatType{5, "foobar"}), inheritance(fte));
    EXPECT_EQ(noexceptLambdas(FatType{5, "foobar"}), inheritance(fte));
    EXPECT_EQ(getIf(FatType{5, "foobar"}), inheritance(fte));
    EXPECT_EQ(holdsAlternative(FatType{5, "foobar"}), inheritance(fte));
}
