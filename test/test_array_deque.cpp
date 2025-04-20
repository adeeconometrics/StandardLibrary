#include "../include/ArrayDeque.hpp"
#include <gtest/gtest.h>

class ArrayDequeTest : public ::testing::Test {
protected:
    static constexpr size_t test_size = 5;
    ArrayDeque<int, test_size> deque;

    void SetUp() override {}
    void TearDown() override {}

    void fill_deque() {
        for(size_t i = 0; i < test_size; ++i) {
            deque.push_back(i);
        }
    }
};

// Category 1: Construction and Assignment Tests
TEST_F(ArrayDequeTest, DefaultConstructor_CreatesEmptyDeque) {
    ArrayDeque<int, test_size> d;
    EXPECT_TRUE(d.empty());
    EXPECT_EQ(d.size(), 0);
    EXPECT_EQ(d.capacity(), test_size);
}

TEST_F(ArrayDequeTest, InitializerListConstructor_CreatesDequeWithElements) {
    ArrayDeque<int, 3> d{1, 2, 3};
    EXPECT_EQ(d.size(), 3);
    EXPECT_EQ(d.front(), 1);
    EXPECT_EQ(d.back(), 3);
}

TEST_F(ArrayDequeTest, CopyConstructor_CreatesDeepCopy) {
    deque.push_back(1);
    deque.push_back(2);
    
    ArrayDeque<int, test_size> copy(deque);
    EXPECT_EQ(copy.size(), deque.size());
    EXPECT_EQ(copy.front(), deque.front());
    EXPECT_EQ(copy.back(), deque.back());
    
    // Modify original, copy should be unchanged
    deque.push_back(3);
    EXPECT_NE(copy.size(), deque.size());
}

TEST_F(ArrayDequeTest, MoveConstructor_TransfersOwnership) {
    deque.push_back(1);
    deque.push_back(2);
    
    ArrayDeque<int, test_size> moved(std::move(deque));
    EXPECT_EQ(moved.size(), 2);
    EXPECT_EQ(moved.front(), 1);
    EXPECT_EQ(moved.back(), 2);
    EXPECT_TRUE(deque.empty());
}

// Category 2: Core Operations Tests
TEST_F(ArrayDequeTest, PushFront_AddsElementsToFront) {
    deque.push_front(1);
    EXPECT_EQ(deque.front(), 1);
    EXPECT_EQ(deque.back(), 1);
    
    deque.push_front(2);
    EXPECT_EQ(deque.front(), 2);
    EXPECT_EQ(deque.back(), 1);
    EXPECT_EQ(deque.size(), 2);
}

TEST_F(ArrayDequeTest, PushBack_AddsElementsToBack) {
    deque.push_back(1);
    EXPECT_EQ(deque.front(), 1);
    EXPECT_EQ(deque.back(), 1);
    
    deque.push_back(2);
    EXPECT_EQ(deque.front(), 1);
    EXPECT_EQ(deque.back(), 2);
    EXPECT_EQ(deque.size(), 2);
}

TEST_F(ArrayDequeTest, PopFront_RemovesElementsFromFront) {
    deque.push_back(1);
    deque.push_back(2);
    deque.push_back(3);
    
    deque.pop_front();
    EXPECT_EQ(deque.front(), 2);
    EXPECT_EQ(deque.size(), 2);
}

TEST_F(ArrayDequeTest, PopBack_RemovesElementsFromBack) {
    deque.push_back(1);
    deque.push_back(2);
    deque.push_back(3);
    
    deque.pop_back();
    EXPECT_EQ(deque.back(), 2);
    EXPECT_EQ(deque.size(), 2);
}

TEST_F(ArrayDequeTest, Clear_RemovesAllElements) {
    fill_deque();
    EXPECT_FALSE(deque.empty());
    
    deque.clear();
    EXPECT_TRUE(deque.empty());
    EXPECT_EQ(deque.size(), 0);
}

// Category 3: Iterator Tests
TEST_F(ArrayDequeTest, Iterator_AllowsForwardTraversal) {
    deque.push_back(1);
    deque.push_back(2);
    deque.push_back(3);
    
    int expected = 1;
    for (const auto& value : deque) {
        EXPECT_EQ(value, expected++);
    }
}

TEST_F(ArrayDequeTest, ConstIterator_AllowsConstTraversal) {
    deque.push_back(1);
    deque.push_back(2);
    deque.push_back(3);
    
    const ArrayDeque<int, test_size>& const_deque = deque;
    int expected = 1;
    for (auto it = const_deque.cbegin(); it != const_deque.cend(); ++it) {
        EXPECT_EQ(*it, expected++);
    }
}

TEST_F(ArrayDequeTest, Iterator_AllowsBidirectionalTraversal) {
    deque.push_back(1);
    deque.push_back(2);
    deque.push_back(3);
    
    auto it = deque.end();
    int expected = 3;
    while (it != deque.begin()) {
        --it;
        EXPECT_EQ(*it, expected--);
    }
}

// Category 4: Edge Cases Tests
TEST_F(ArrayDequeTest, EmptyDeque_ThrowsOnAccess) {
    EXPECT_THROW(static_cast<void>(deque.front()), std::out_of_range);
    EXPECT_THROW(static_cast<void>(deque.back()), std::out_of_range);
    EXPECT_THROW(deque.pop_front(), std::out_of_range);
    EXPECT_THROW(deque.pop_back(), std::out_of_range);
}

TEST_F(ArrayDequeTest, FullDeque_ThrowsOnPush) {
    fill_deque();
    EXPECT_THROW(deque.push_front(42), std::length_error);
    EXPECT_THROW(deque.push_back(42), std::length_error);
}

TEST_F(ArrayDequeTest, CircularBuffer_HandlesWraparound) {
    // Fill the deque
    fill_deque();
    
    // Remove some elements from front
    deque.pop_front();
    deque.pop_front();
    
    // Add elements to back (should wrap around)
    deque.push_back(5);
    deque.push_back(6);
    
    std::vector<int> expected = {2, 3, 4, 5, 6};
    int idx = 0;
    for (const auto& value : deque) {
        EXPECT_EQ(value, expected[idx++]);
    }
}

// Category 5: Type Requirements Tests
// TEST_F(ArrayDequeTest, CustomType_SupportsComplexTypes) {
//     struct TestType {
//         int value;
//         explicit TestType(int v) : value(v) {}
//         bool operator==(const TestType& other) const { return value == other.value; }
//     };

//     ArrayDeque<TestType, 3> custom_deque;
//     EXPECT_NO_THROW(custom_deque.push_back(TestType{1}));
//     EXPECT_NO_THROW(custom_deque.push_front(TestType{2}));
//     EXPECT_EQ(custom_deque.front().value, 2);
//     EXPECT_EQ(custom_deque.back().value, 1);
// }

// TEST_F(ArrayDequeTest, MoveOnlyType_SupportsMovableTypes) {
//     struct MoveOnly {
//         int value;
//         explicit MoveOnly(int v) : value(v) {}
//         MoveOnly(const MoveOnly&) = delete;
//         MoveOnly& operator=(const MoveOnly&) = delete;
//         MoveOnly(MoveOnly&&) = default;
//         MoveOnly& operator=(MoveOnly&&) = default;
//     };

//     ArrayDeque<MoveOnly, 3> move_deque;
//     EXPECT_NO_THROW(move_deque.push_back(MoveOnly{1}));
//     EXPECT_NO_THROW(move_deque.push_front(MoveOnly{2}));
//     EXPECT_EQ(move_deque.front().value, 2);
//     EXPECT_EQ(move_deque.back().value, 1);
// }

// TEST_F(ArrayDequeTest, EmplaceOperations_ConstructsInPlace) {
//     struct Empaceable {
//         int x, y;
//         Empaceable(int x_, int y_) : x(x_), y(y_) {}
//     };

//     ArrayDeque<Empaceable, 3> emplace_deque;
//     EXPECT_NO_THROW(emplace_deque.emplace_back(1, 2));
//     EXPECT_NO_THROW(emplace_deque.emplace_front(3, 4));
    
//     EXPECT_EQ(emplace_deque.front().x, 3);
//     EXPECT_EQ(emplace_deque.front().y, 4);
//     EXPECT_EQ(emplace_deque.back().x, 1);
//     EXPECT_EQ(emplace_deque.back().y, 2);
// }

// Additional Tests for Complete Coverage
TEST_F(ArrayDequeTest, AssignmentOperator_CopiesElements) {
    deque.push_back(1);
    deque.push_back(2);
    
    ArrayDeque<int, test_size> other;
    other = deque;
    
    EXPECT_EQ(other.size(), deque.size());
    EXPECT_EQ(other.front(), deque.front());
    EXPECT_EQ(other.back(), deque.back());
}

TEST_F(ArrayDequeTest, MoveAssignment_TransfersOwnership) {
    deque.push_back(1);
    deque.push_back(2);
    
    ArrayDeque<int, test_size> other;
    other = std::move(deque);
    
    EXPECT_EQ(other.size(), 2);
    EXPECT_EQ(other.front(), 1);
    EXPECT_EQ(other.back(), 2);
    EXPECT_TRUE(deque.empty());
}

TEST_F(ArrayDequeTest, SizeAndCapacity_TracksCorrectly) {
    EXPECT_EQ(deque.size(), 0);
    EXPECT_EQ(deque.capacity(), test_size);
    
    deque.push_back(1);
    EXPECT_EQ(deque.size(), 1);
    
    deque.push_front(2);
    EXPECT_EQ(deque.size(), 2);
    
    deque.pop_back();
    EXPECT_EQ(deque.size(), 1);
    EXPECT_EQ(deque.capacity(), test_size);
}
