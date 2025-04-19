#include "../include/Array.hpp"
#include <gtest/gtest.h>

class ArrayTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(ArrayTest, RandomAccessTest) {
    // Test array creation and random access
    Array<int, 5> arr;
    
    // Test adding elements
    arr.add(1);
    arr.add(2);
    arr.add(3);
    arr.add(4);
    arr.add(5);
    
    // Test random access operator[]
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[2], 3);
    EXPECT_EQ(arr[4], 5);
    
    // Test out of bounds access throws
    EXPECT_THROW(arr[5], std::out_of_range);
    
    // Test const access
    const Array<int, 5>& const_arr = arr;
    EXPECT_EQ(const_arr[1], 2);
    EXPECT_EQ(const_arr[3], 4);
}

TEST_F(ArrayTest, IteratorTest) {
    Array<int, 3> arr;
    arr.add(1);
    arr.add(2);
    arr.add(3);
    
    // Test iterator traversal
    auto it = arr.begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_EQ(*it, 3);
    
    // Test const iterator
    const Array<int, 3>& const_arr = arr;
    auto const_it = const_arr.cbegin();
    EXPECT_EQ(*const_it, 1);
}
