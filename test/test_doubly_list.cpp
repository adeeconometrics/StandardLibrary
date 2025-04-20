#include "../include/DoublyList.hpp"
#include <gtest/gtest.h>
#include <string>
#include <type_traits>

class DoublyListTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// Construction Tests
TEST_F(DoublyListTest, DefaultConstructor_CreatesEmptyList) {
    DoublyList<int> list;
    EXPECT_TRUE(list.is_empty());
    EXPECT_EQ(list.size(), 0);
}

TEST_F(DoublyListTest, InitializerListConstructor_CreatesPopulatedList) {
    DoublyList<int> list{1, 2, 3};
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.top(), 1);
    EXPECT_EQ(list.bottom(), 3);
}

TEST_F(DoublyListTest, CopyConstructor) {
    DoublyList<int> list1;
    list1.add(1);
    list1.add(2);
    
    DoublyList<int> list2(list1);
    list1.add(3);  // Modifying original shouldn't affect copy

    EXPECT_EQ(list2.size(), 2);
    EXPECT_EQ(list2.top(), 1);
    EXPECT_EQ(list2.bottom(), 2);
}

TEST_F(DoublyListTest, MoveConstructor) {
    DoublyList<std::string> list1;
    list1.add("test1");
    list1.add("test2");
    
    DoublyList<std::string> list2(std::move(list1));
    
    EXPECT_TRUE(list1.is_empty());  // Original should be empty
    EXPECT_EQ(list2.size(), 2);
    EXPECT_EQ(list2.top(), "test1");
    EXPECT_EQ(list2.bottom(), "test2");
}

// Assignment Tests
TEST_F(DoublyListTest, CopyAssignment) {
    DoublyList<int> list1;
    list1.add(1);
    list1.add(2);
    
    DoublyList<int> list2;
    list2 = list1;
    list1.add(3);  // Modifying original shouldn't affect copy
    
    EXPECT_EQ(list2.size(), 2);
    EXPECT_EQ(list2.top(), 1);
    EXPECT_EQ(list2.bottom(), 2);
}

TEST_F(DoublyListTest, MoveAssignment) {
    DoublyList<std::string> list1;
    list1.add("test1");
    list1.add("test2");
    
    DoublyList<std::string> list2;
    list2 = std::move(list1);
    
    EXPECT_TRUE(list1.is_empty());
    EXPECT_EQ(list2.size(), 2);
    EXPECT_EQ(list2.top(), "test1");
    EXPECT_EQ(list2.bottom(), "test2");
}

// Element Addition Tests
TEST_F(DoublyListTest, AddAndAccessElements) {
    DoublyList<int> list;
    
    list.add(1);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.top(), 1);
    EXPECT_EQ(list.bottom(), 1);
    
    list.add(2);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.top(), 1);
    EXPECT_EQ(list.bottom(), 2);
}

TEST_F(DoublyListTest, AddFrontAndBack) {
    DoublyList<int> list;
    
    list.add_front(1);  // [1]
    list.add_back(2);   // [1, 2]
    list.add_front(0);  // [0, 1, 2]
    
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.top(), 0);
    EXPECT_EQ(list.bottom(), 2);
}

// Element Removal Tests
TEST_F(DoublyListTest, RemoveElements) {
    DoublyList<int> list{1, 2, 3, 4};
    
    list.remove(2);  // [1, 3, 4]
    EXPECT_EQ(list.size(), 3);
    
    list.remove(1);  // [3, 4]
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.top(), 3);
    
    list.remove(4);  // [3]
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.top(), 3);
    EXPECT_EQ(list.bottom(), 3);
}

// Iterator Tests
TEST_F(DoublyListTest, ForwardIteratorTraversal) {
    DoublyList<int> list{1, 2, 3};
    
    int expected = 1;
    for (const auto& value : list) {
        EXPECT_EQ(value, expected++);
    }
    EXPECT_EQ(expected, 4);
}

TEST_F(DoublyListTest, ConstIteratorBehavior) {
    const DoublyList<int> list{1, 2, 3};
    
    auto it = list.cbegin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
    
    // Verify const correctness
    static_assert(std::is_same_v<decltype(*list.cbegin()), const int&>,
                 "Const iterator should return const reference");
}

TEST_F(DoublyListTest, BidirectionalIteratorRequirements) {
    // Type requirements verification
    using Iterator = typename DoublyList<int>::iterator;
    using ConstIterator = typename DoublyList<int>::const_iterator;
    
    static_assert(std::is_same_v<
        typename std::iterator_traits<Iterator>::iterator_category,
        std::bidirectional_iterator_tag
    >, "Iterator must be bidirectional");
    
    static_assert(std::is_same_v<
        typename std::iterator_traits<ConstIterator>::iterator_category,
        std::bidirectional_iterator_tag
    >, "Const iterator must be bidirectional");
}

// TEST_F(DoublyListTest, BidirectionalIteratorOperations) {
//     DoublyList<int> list{1, 2, 3, 4, 5};
    
//     // Forward iteration
//     auto it = list.begin();
//     EXPECT_EQ(*it, 1);
//     ++it;
//     EXPECT_EQ(*it, 2);
//     it++;
//     EXPECT_EQ(*it, 3);
    
//     // Backward iteration from middle
//     --it;
//     EXPECT_EQ(*it, 2);
//     it--;
//     EXPECT_EQ(*it, 1);
    
//     // Move to end then backwards
//     it = list.end();
//     --it;
//     EXPECT_EQ(*it, 5);
//     --it;
//     EXPECT_EQ(*it, 4);
// }

TEST_F(DoublyListTest, ConstBidirectionalIteratorOperations) {
    const DoublyList<int> list{1, 2, 3, 4, 5};
    
    // Forward iteration with const_iterator
    auto it = list.cbegin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
    
    // Backward iteration with const_iterator
    auto end_it = list.cend();
    --end_it;
    EXPECT_EQ(*end_it, 5);
    --end_it;
    EXPECT_EQ(*end_it, 4);
    
    // Verify const correctness
    static_assert(std::is_same_v<decltype(*list.cbegin()), const int&>,
                 "Const iterator must return const reference");
}

TEST_F(DoublyListTest, BidirectionalIteratorEdgeCases) {
    DoublyList<int> list;
    
    // Empty list iteration
    EXPECT_EQ(list.begin(), list.end());
    
    // Single element bidirectional iteration
    list.add(1);
    auto it = list.begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(it, list.end());
    --it;
    EXPECT_EQ(*it, 1);
}

// Clear and Empty Tests
TEST_F(DoublyListTest, ClearList) {
    DoublyList<int> list{1, 2, 3};
    EXPECT_EQ(list.size(), 3);
    
    list.clear();
    EXPECT_TRUE(list.is_empty());
    EXPECT_EQ(list.size(), 0);
}

TEST_F(DoublyListTest, EmptyListOperations) {
    DoublyList<int> list;
    
    EXPECT_TRUE(list.is_empty());
    EXPECT_EQ(list.size(), 0);
    EXPECT_THROW(static_cast<void>(list.remove(1)), std::out_of_range);
    EXPECT_THROW(static_cast<void>(list.top()), std::out_of_range);
    EXPECT_THROW(static_cast<void>(list.bottom()), std::out_of_range);
}

// Complex Type Tests
TEST_F(DoublyListTest, ComplexTypeOperations) {
    struct ComplexType {
        std::string data;
        int value;
        
        ComplexType() = default;
        ComplexType(const ComplexType&) = default;
        ComplexType(const std::string& d, int v) : data(d), value(v) {}
        
        bool operator==(const ComplexType& other) const {
            return data == other.data && value == other.value;
        }
    };
    
    DoublyList<ComplexType> list;
    list.add(ComplexType("first", 1));
    list.add(ComplexType("second", 2));
    
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.top(), (ComplexType("first", 1)));
    EXPECT_EQ(list.bottom(), (ComplexType("second", 2)));
}

// Type Requirements Tests
TEST_F(DoublyListTest, TypeRequirements) {
    static_assert(std::is_default_constructible_v<DoublyList<int>>,
                 "DoublyList must be default constructible");
    static_assert(std::is_copy_constructible_v<DoublyList<int>>,
                 "DoublyList must be copy constructible");
    static_assert(std::is_move_constructible_v<DoublyList<int>>,
                 "DoublyList must be move constructible");
    static_assert(std::is_copy_assignable_v<DoublyList<int>>,
                 "DoublyList must be copy assignable");
    static_assert(std::is_move_assignable_v<DoublyList<int>>,
                 "DoublyList must be move assignable");
}
