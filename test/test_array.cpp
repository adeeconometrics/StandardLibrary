#include "../include/Array.hpp"
#include <gtest/gtest.h>

class ArrayTest : public ::testing::Test {
protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(ArrayTest, DefaultConstructor) {
  Array<int, 5> array;
  EXPECT_FALSE(array.empty());
  EXPECT_EQ(array.size(), 5);
}

TEST_F(ArrayTest, InitializerListConstructor) {
  Array<int, 3> array{1, 2, 3};
  EXPECT_EQ(array[0], 1);
  EXPECT_EQ(array[1], 2);
  EXPECT_EQ(array[2], 3);
}

TEST_F(ArrayTest, CopyConstructor) {
  Array<int, 3> array1{1, 2};

  Array<int, 3> array2(array1);
  EXPECT_EQ(array2[0], 1);
  EXPECT_EQ(array2[1], 2);
}

TEST_F(ArrayTest, MoveConstructor) {
  Array<std::string, 2> array1;
  array1.add("test1");
  array1.add("test2");

  Array<std::string, 2> array2(std::move(array1));
  EXPECT_EQ(array2[0], "test1");
  EXPECT_EQ(array2[1], "test2");
}

TEST_F(ArrayTest, AssignmentOperator) {
  Array<int, 3> array1;
  array1.add(1);
  array1.add(2);

  Array<int, 3> array2;
  array2 = array1;
  EXPECT_EQ(array2[0], 1);
  EXPECT_EQ(array2[1], 2);
}

TEST_F(ArrayTest, MoveAssignmentOperator) {
  Array<std::string, 2> array1;
  array1.add("test1");
  array1.add("test2");

  Array<std::string, 2> array2;
  array2 = std::move(array1);
  EXPECT_EQ(array2[0], "test1");
  EXPECT_EQ(array2[1], "test2");
}

TEST_F(ArrayTest, InitializerListAssignment) {
  Array<int, 3> array;
  array = {1, 2, 3};
  EXPECT_EQ(array[0], 1);
  EXPECT_EQ(array[1], 2);
  EXPECT_EQ(array[2], 3);
}

TEST_F(ArrayTest, AddAndAccessElements) {
  Array<int, 3> array;
  array.add(1);
  array.add(2);
  array.add(3);

  EXPECT_EQ(array[0], 1);
  EXPECT_EQ(array[1], 2);
  EXPECT_EQ(array[2], 3);
}

TEST_F(ArrayTest, IteratorTraversal) {
  Array<int, 3> array{1, 2, 3};

  int expected = 1;
  for (const auto &value : array) {
    EXPECT_EQ(value, expected++);
  }
}

TEST_F(ArrayTest, ConstIteratorTraversal) {
  Array<int, 3> array{1, 2, 3};

  const Array<int, 3> &const_array = array;
  int expected = 1;
  for (auto it = const_array.cbegin(); it != const_array.cend(); ++it) {
    EXPECT_EQ(*it, expected++);
  }
}

TEST_F(ArrayTest, ExceptionHandling) {
  Array<int, 2> array;

  // Test array bounds checking
  EXPECT_THROW(static_cast<void>(array[2]), std::out_of_range);

  // Test array capacity
  array.add(1);
  array.add(2);
  EXPECT_THROW(array.add(3), std::length_error);

  // Test initializer list size checking - constructor
  EXPECT_THROW((Array<int, 2>{1, 2, 3}), std::length_error);

  // Test initializer list size checking - assignment
  EXPECT_THROW((array = {1, 2, 3}), std::length_error);
}

TEST_F(ArrayTest, SFINAEConstraints) {
  // This should compile
  Array<int, 1> valid_array;

  // These should not compile (tested through static_assert):
  // Array<int, 0> zero_size_array;  // Size must be > 0
  // Array<void, 1> void_type_array; // Type must be default constructible
}

TEST_F(ArrayTest, ModifyThroughIterator) {
  Array<int, 3> array{1, 2, 3};

  // Modify through iterator
  for (auto &value : array) {
    value *= 2;
  }

  EXPECT_EQ(array[0], 2);
  EXPECT_EQ(array[1], 4);
  EXPECT_EQ(array[2], 6);
}

TEST_F(ArrayTest, SizeConsistency) {
  Array<int, 3> array;

  EXPECT_EQ(array.size(), 3);

  array.add(1);
  EXPECT_EQ(array.size(), 3); // Size should remain constant for Array

  array.add(2);
  array.add(3);
  EXPECT_EQ(array.size(), 3);
}

TEST_F(ArrayTest, BoundsChecking) {
  Array<int, 3> array{1, 2, 3};

  // Testing operator[] bounds
  EXPECT_NO_THROW(static_cast<void>(array[0]));
  EXPECT_NO_THROW(static_cast<void>(array[2]));
  EXPECT_THROW(static_cast<void>(array[3]), std::out_of_range);
}

TEST_F(ArrayTest, ComplexTypeOperations) {
  struct ComplexType {
    std::string data;
    int value;

    ComplexType() : data("default"), value(0) {}
    ComplexType(const std::string &d, int v) : data(d), value(v) {}
  };

  Array<ComplexType, 2> array;
  array.add(ComplexType("first", 1));
  array.add(ComplexType("second", 2));

  EXPECT_EQ(array[0].data, "first");
  EXPECT_EQ(array[0].value, 1);
  EXPECT_EQ(array[1].data, "second");
  EXPECT_EQ(array[1].value, 2);
}
