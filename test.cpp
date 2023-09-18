#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING
#include "gtest/gtest.h"

extern "C" {
#include "dose.h"
#include "types.h"
#include "parameters.h"
#include "tables.h"
}



//TEST(TestCaseName, TestName) {
//  EXPECT_EQ(1, 1);
//  EXPECT_TRUE(true);
//}
////In the next three test cases, I am testing my self_created functions 
////created with the aim of refactoring the assignment.
//TEST(CalcInterpolateTest, TestwithValidInputs) {
//    // Set up input values
//    u8 p = 5;
//    u8 limited_mm = 30;
//    u8 mm_index[9] = { 25 }; 
//    u32 result = calcInterpolate(p, limited_mm, mm_index);
//    // Assert the result
//    EXPECT_EQ(result, (1000 * (limited_mm - mm_index[p])) / (mm_index[p + 1] - mm_index[p]));
//}
//
//TEST(CalcInterpolateTest, TestWithInvalidInputPZero) {
//    // Set up input values that will lead to p being 0
//    u8 p = 0;
//    u8 limited_mm = 30;
//    u8 mm_index[1] = { 25 }; 
//    u32 result = calcInterpolate(p, limited_mm, mm_index);
//
//    // Assert the result
//    EXPECT_EQ(result, 0);
//}
//
//TEST(CalcMgdIedAnodeTest, TestWithValidInputs) { // I can also test for invalid inputs at were p=0 and kv =0
//    u8 kv = 80;
//    u8 p = 3;
//    u32 interpol = 2000;
//    u8 filter = 2;
//
//    u16 result = calcMgdIedAnode(MGD_Mo, kv, p, interpol);
//    u16 expected_result = MGD_Mo[p][kv - 20] - ((interpol * (MGD_Mo[p][kv - 20] - MGD_Mo[p + 1][kv - 20]) + 500) / 1000);
//
//    // Assert the result
//    EXPECT_EQ(result, expected_result);
//}
//
//// Now to test for calcIED
////First test for valid input and test for invalid thickness since the thickness is alway confined to be between 10 and 100;
//TEST(CalcIEDAndMGDTest, CalcIED_ValidInputs) {
//    // Define valid inputs
//    // Call calcIED with these inputs
//    // Assert the result matches the expected IED value
//}
//
//// Tests for calcMGD WITH an INVALID thickness
//TEST(CalcMGDTest, CalcMGD_ValidInputs) {
//    //Again, define some valide input, THIS time with an invalid thickness say 0 or any number greater that 100
//    //Then call the calcMGD function into a results.
//    // Then assert if the result derived is equal to an expected outcome. just as in the case of calcIED
//}