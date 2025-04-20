#include "../include/ArrayStack.hpp"
#include <gtest/gtest.h>
#include <string>

// Test fixture for ArrayStack
class ArrayStackTest : public ::testing::Test {
protected:
    static constexpr size_t test_size = 5;
    ArrayStack<int, test_size> int_stack;
    ArrayStack<std::string, test_size> string_stack;
};

// Construction and Assignment Tests
TEST_F(ArrayStackTest, DefaultConstructorCreatesEmptyStack) {
    EXPECT_TRUE(int_stack.empty());
    EXPECT_EQ(int_stack.size(), 0);
    EXPECT_EQ(int_stack.capacity(), test_size);
}

TEST_F(ArrayStackTest, InitializerListConstructor) {
    ArrayStack<int, 3> stack{1, 2, 3};
    EXPECT_EQ(stack.size(), 3);
    EXPECT_EQ(stack.top(), 3);
}

TEST_F(ArrayStackTest, CopyConstructorDeepCopies) {
    int_stack.push(1);
    int_stack.push(2);
    
    ArrayStack<int, test_size> copy(int_stack);
    EXPECT_EQ(copy.size(), int_stack.size());
    EXPECT_EQ(copy.top(), int_stack.top());
    
    // Modifying copy shouldn't affect original
    copy.pop();
    EXPECT_NE(copy.size(), int_stack.size());
}

TEST_F(ArrayStackTest, MoveConstructorTransfersOwnership) {
    int_stack.push(1);
    int_stack.push(2);
    size_t original_size = int_stack.size();
    
    ArrayStack<int, test_size> moved(std::move(int_stack));
    EXPECT_EQ(moved.size(), original_size);
    EXPECT_EQ(moved.top(), 2);
}

// Element Access Tests
TEST_F(ArrayStackTest, TopAccessesLastElement) {
    string_stack.push("first");
    string_stack.push("second");
    EXPECT_EQ(string_stack.top(), "second");
}

TEST_F(ArrayStackTest, TopThrowsOnEmptyStack) {
    int_stack.clear();
    EXPECT_THROW(int_stack.top(), std::out_of_range);
}

// Modifier Tests
TEST_F(ArrayStackTest, PushIncreasesSize) {
    EXPECT_EQ(int_stack.size(), 0);
    int_stack.push(1);
    EXPECT_EQ(int_stack.size(), 1);
}

TEST_F(ArrayStackTest, PushThrowsWhenFull) {
    for(std::size_t i = 0; i < test_size; ++i) {
        int_stack.push(i);
    }
    EXPECT_THROW(int_stack.push(test_size), std::length_error);
}

TEST_F(ArrayStackTest, PopDecreasesSize) {
    int_stack.push(1);
    int_stack.push(2);
    size_t original_size = int_stack.size();
    
    int_stack.pop();
    EXPECT_EQ(int_stack.size(), original_size - 1);
}

TEST_F(ArrayStackTest, PopThrowsOnEmptyStack) {
    EXPECT_THROW(int_stack.pop(), std::out_of_range);
}

TEST_F(ArrayStackTest, ClearResetsSize) {
    int_stack.push(1);
    int_stack.push(2);
    int_stack.clear();
    EXPECT_TRUE(int_stack.empty());
    EXPECT_EQ(int_stack.size(), 0);
}

// Capacity Tests
TEST_F(ArrayStackTest, CapacityChecks) {
    EXPECT_TRUE(int_stack.empty());
    EXPECT_FALSE(int_stack.full());
    
    for(std::size_t i = 0; i < test_size; ++i) {
        int_stack.push(i);
    }
    
    EXPECT_FALSE(int_stack.empty());
    EXPECT_TRUE(int_stack.full());
}

// Iterator Tests
TEST_F(ArrayStackTest, IteratorTraversal) {
    for(std::size_t i = 0; i < 3; ++i) {
        int_stack.push(i);
    }
    
    int expected = 0;
    for(const auto& value : int_stack) {
        EXPECT_EQ(value, expected++);
    }
}

TEST_F(ArrayStackTest, ConstIteratorTraversal) {
    for(std::size_t i = 0; i < 3; ++i) {
        int_stack.push(i);
    }
    
    const auto& const_stack = int_stack;
    int expected = 0;
    for(auto it = const_stack.cbegin(); it != const_stack.cend(); ++it) {
        EXPECT_EQ(*it, expected++);
    }
}

// Move Assignment Tests
TEST_F(ArrayStackTest, MoveAssignmentTransfersOwnership) {
    int_stack.push(1);
    int_stack.push(2);
    
    ArrayStack<int, test_size> other;
    other = std::move(int_stack);
    
    EXPECT_EQ(other.size(), 2);
    EXPECT_EQ(other.top(), 2);
}

// Template Constraint Tests
TEST_F(ArrayStackTest, StaticAsserts) {
    // These should fail to compile if uncommented
    // ArrayStack<int, 0> invalid_size; // Size must be > 0
    // struct NonCopyable { NonCopyable(const NonCopyable&) = delete; };
    // ArrayStack<NonCopyable, 1> invalid_type; // Type must be copy constructible
    
    // These should compile
    struct Movable {
        Movable() = default;
        Movable(const Movable&) = default;
    };
    ArrayStack<Movable, 1> valid_stack;
    EXPECT_TRUE(valid_stack.empty());
}

// Emplace Tests
TEST_F(ArrayStackTest, EmplaceConstructsInPlace) {
    struct ComplexType {
        int x;
        std::string y;
        ComplexType(): x(int{}), y(std::string{}) {}
        ComplexType(int x_, std::string y_) : x(x_), y(std::move(y_)) {}
    };
    
    ArrayStack<ComplexType, 2> complex_stack;
    complex_stack.emplace(42, "test");
    
    EXPECT_EQ(complex_stack.top().x, 42);
    EXPECT_EQ(complex_stack.top().y, "test");
}