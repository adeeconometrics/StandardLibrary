#include "../include/Iterator.hpp"
#include <gtest/gtest.h>

namespace sl {
namespace test {

// Test fixture for Iterator tests
class IteratorTest : public ::testing::Test {
protected:
  // Sample array to iterate over
  static constexpr size_t test_size = 5;
  int test_array[test_size] = {1, 2, 3, 4, 5};

  // Helper methods
  auto create_iterator(size_t pos = 0) -> ConcreteIterator<int> {
    return ConcreteIterator<int>(&test_array[pos]);
  }
};

// Test Category 1: Construction and Basic Operations
TEST_F(IteratorTest, Constructor_CreatesValidIterator) {
  auto it = create_iterator();
  EXPECT_EQ(*it, 1);
}

TEST_F(IteratorTest, CopyConstruction_CreatesIdenticalIterator) {
  auto it1 = create_iterator();
  auto it2(it1);
  EXPECT_TRUE(it1 == it2);
  EXPECT_EQ(*it1, *it2);
}

// Test Category 2: Forward Iterator Operations
TEST_F(IteratorTest, Increment_MovesToNextElement) {
  auto it = create_iterator();
  ++it;
  EXPECT_EQ(*it, 2);
}

TEST_F(IteratorTest, PostIncrement_ReturnsOldValueAndIncrements) {
  auto it = create_iterator();
  auto old_it = it++;
  EXPECT_EQ(*old_it, 1);
  EXPECT_EQ(*it, 2);
}

// Test Category 3: Bidirectional Iterator Operations
TEST_F(IteratorTest, Decrement_MovesToPreviousElement) {
  auto it = create_iterator(1); // Start at second element
  --it;
  EXPECT_EQ(*it, 1);
}

TEST_F(IteratorTest, PostDecrement_ReturnsOldValueAndDecrements) {
  auto it = create_iterator(1); // Start at second element
  auto old_it = it--;
  EXPECT_EQ(*old_it, 2);
  EXPECT_EQ(*it, 1);
}

// Test Category 4: Comparison Operations
TEST_F(IteratorTest, EqualityComparison_ReturnsTrueForSamePosition) {
  auto it1 = create_iterator();
  auto it2 = create_iterator();
  EXPECT_TRUE(it1 == it2);
}

TEST_F(IteratorTest, InequalityComparison_ReturnsTrueForDifferentPositions) {
  auto it1 = create_iterator();
  auto it2 = create_iterator(1);
  EXPECT_TRUE(it1 != it2);
}

// Test Category 5: Dereferencing Operations
TEST_F(IteratorTest, Dereference_ReturnsCorrectValue) {
  auto it = create_iterator();
  EXPECT_EQ(*it, 1);
  EXPECT_EQ(*(++it), 2);
}

TEST_F(IteratorTest, ArrowOperator_AccessesMemberCorrectly) {
  struct TestType {
    int value;
    explicit TestType(int v) : value(v) {}
  };

  TestType test_obj_array[1] = {TestType(42)};
  ConcreteIterator<TestType> it(&test_obj_array[0]);
  EXPECT_EQ(it->value, 42);
}

// Test Category 6: Edge Cases
TEST_F(IteratorTest, IncrementToEnd_HandlesCorrectly) {
  auto it = create_iterator();
  for (size_t i = 0; i < test_size - 1; ++i) {
    ++it;
  }
  EXPECT_EQ(*it, 5); // Last element
}

TEST_F(IteratorTest, DecrementFromEnd_HandlesCorrectly) {
  auto it = create_iterator(test_size - 1);
  EXPECT_EQ(*it, 5); // Last element
  --it;
  EXPECT_EQ(*it, 4); // Second to last element
}

} // namespace test
} // namespace sl
