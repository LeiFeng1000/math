/**
 * @file    numerical_table.cpp
 * @brief   单元测试，@class numerical_table
 */

#include <gtest/gtest.h>
#include "numerical_table.hpp"

using namespace std;
using namespace math;

TEST(numerical,math)
{
  numerical_table<float> nt();
  //EXPECT_EQ(1,nt.get_M());
  // EXPECT_EQ(1,nt.get_N());

  {
    
    numerical_table<float> nt1(2,3,{2.0f});
    EXPECT_EQ(2,nt1.get_M());
    EXPECT_EQ(3,nt1.get_N());
  }
  
}
