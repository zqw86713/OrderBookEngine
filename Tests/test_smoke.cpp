// #include <gtest/gtest.h>
//
// TEST(SmokeTest, TrueIsTrue) { EXPECT_TRUE(true); }
//
// int main(int argc, char** argv) {
//	::testing::InitGoogleTest(&argc, argv);
//	return RUN_ALL_TESTS();
//
// }

#include <gtest/gtest.h>

TEST(SampleTest, Add2) { EXPECT_EQ(2 + 1, 3); }
