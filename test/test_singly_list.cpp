#include "../include/SinglyList.hpp"
#include <gtest/gtest.h>
#include <string>

class SinglyListTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(SinglyListTest, DefaultConstructor_CreatesEmptyList) {
    SinglyList<int> list;
    EXPECT_TRUE(list.is_empty());
    EXPECT_EQ(list.size(), 0);
}

TEST_F(SinglyListTest, InitializerListConstructor) {
    SinglyList<int> list{1, 2, 3};
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.top(), 1);
    EXPECT_EQ(list.bottom(), 3);
}

TEST_F(SinglyListTest, CopyConstructor) {
    SinglyList<int> list1;
    list1.add(1);
    list1.add(2);
    
    SinglyList<int> list2(list1);
    EXPECT_EQ(list2.size(), 2);
    EXPECT_EQ(list2.top(), 1);
    EXPECT_EQ(list2.bottom(), 2);
}

TEST_F(SinglyListTest, MoveConstructor) {
    SinglyList<std::string> list1;
    list1.add("test1");
    list1.add("test2");
    
    SinglyList<std::string> list2(std::move(list1));
    EXPECT_EQ(list2.size(), 2);
    EXPECT_EQ(list2.top(), "test1");
    EXPECT_EQ(list2.bottom(), "test2");
}

TEST_F(SinglyListTest, AddAndAccessElements) {
    SinglyList<int> list;
    
    list.add(1);
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.top(), 1);
    EXPECT_EQ(list.bottom(), 1);
    
    list.add(2);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.top(), 1);
    EXPECT_EQ(list.bottom(), 2);
}

TEST_F(SinglyListTest, AddFrontAndBack) {
    SinglyList<int> list;
    
    list.add_front(1);  // [1]
    list.add_back(2);   // [1, 2]
    list.add_front(0);  // [0, 1, 2]
    
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.top(), 0);
    EXPECT_EQ(list.bottom(), 2);
}

TEST_F(SinglyListTest, RemoveElements) {
    SinglyList<int> list{1, 2, 3, 4};
    
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

TEST_F(SinglyListTest, ForwardIteratorTraversal) {
    SinglyList<int> list{1, 2, 3};
    
    int expected = 1;
    for (const auto& value : list) {
        EXPECT_EQ(value, expected++);
    }
    EXPECT_EQ(expected, 4);
}

TEST_F(SinglyListTest, ClearList) {
    SinglyList<int> list{1, 2, 3};
    EXPECT_EQ(list.size(), 3);
    
    list.clear();
    EXPECT_TRUE(list.is_empty());
    EXPECT_EQ(list.size(), 0);
}

TEST_F(SinglyListTest, ComplexTypeOperations) {
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
    
    SinglyList<ComplexType> list;
    list.add(ComplexType("first", 1));
    list.add(ComplexType("second", 2));
    
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.top(), (ComplexType("first", 1)));
    EXPECT_EQ(list.bottom(), (ComplexType("second", 2)));
}

TEST_F(SinglyListTest, EmptyListOperations) {
    SinglyList<int> list;
    
    EXPECT_TRUE(list.is_empty());
    EXPECT_EQ(list.size(), 0);

    EXPECT_THROW(list.remove(1), std::out_of_range);
}

TEST_F(SinglyListTest, IteratorOperations) {
    SinglyList<int> list{1, 2, 3};
    
    // Test iterator increment
    auto it = list.begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
    it++;
    EXPECT_EQ(*it, 3);
    
    // Test iterator comparison
    auto it2 = list.begin();
    EXPECT_TRUE(it2 != it);
    EXPECT_TRUE(it2 != list.end());
}
