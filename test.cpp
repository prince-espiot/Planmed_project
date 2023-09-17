// File: test_dose.cpp

#include <gtest/gtest.h>
#include "dose.h"

TEST(CalcInterpolateTest, Test1) {
    u8 mm_index[9] = { 0, 10, 20, 30, 40, 50, 60, 70, 80 };
    u32 result = calcInterpolate(7, 35, mm_index);
    EXPECT_EQ(result, 750);
}

TEST(CalcInterpolateTest, Test2) {
    u8 mm_index[9] = { 0, 10, 20, 30, 40, 50, 60, 70, 80 };
    u32 result = calcInterpolate(5, 55, mm_index);
    EXPECT_EQ(result, 500);
}

// Add more tests as needed

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
