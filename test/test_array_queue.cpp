#include "../include/ArrayQueue.hpp"
#include <gtest/gtest.h>

class ArrayQueueTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(ArrayQueueTest, DefaultConstructor) {
    ArrayQueue<int, 5> queue;
    EXPECT_TRUE(queue.is_empty());
    EXPECT_FALSE(queue.is_full());
}

TEST_F(ArrayQueueTest, EnqueueDequeue) {
    ArrayQueue<int, 3> queue;
    
    queue.enqueue(1);
    EXPECT_EQ(queue.top(), 1);
    
    queue.enqueue(2);
    queue.enqueue(3);
    EXPECT_TRUE(queue.is_full());
    
    queue.dequeue();
    EXPECT_EQ(queue.top(), 2);
    EXPECT_FALSE(queue.is_full());
}

TEST_F(ArrayQueueTest, CopyConstructor) {
    ArrayQueue<int, 3> queue1;
    queue1.enqueue(1);
    queue1.enqueue(2);
    
    ArrayQueue<int, 3> queue2(queue1);
    EXPECT_EQ(queue2.top(), 1);
    queue2.dequeue();
    EXPECT_EQ(queue2.top(), 2);
}

TEST_F(ArrayQueueTest, MoveConstructor) {
    ArrayQueue<int, 3> queue1;
    queue1.enqueue(1);
    queue1.enqueue(2);
    
    ArrayQueue<int, 3> queue2(std::move(queue1));
    EXPECT_EQ(queue2.top(), 1);
    queue2.dequeue();
    EXPECT_EQ(queue2.top(), 2);
}

TEST_F(ArrayQueueTest, Iterator) {
    ArrayQueue<int, 3> queue;
    queue.enqueue(1);
    queue.enqueue(2);
    queue.enqueue(3);
    
    int expected = 1;
    for (const auto& value : queue) {
        EXPECT_EQ(value, expected++);
    }
}

TEST_F(ArrayQueueTest, ConstIterator) {
    ArrayQueue<int, 3> queue;
    queue.enqueue(1);
    queue.enqueue(2);
    queue.enqueue(3);
    
    const ArrayQueue<int, 3>& const_queue = queue;
    int expected = 1;
    for (auto it = const_queue.cbegin(); it != const_queue.cend(); ++it) {
        EXPECT_EQ(*it, expected++);
    }
}

TEST_F(ArrayQueueTest, ExceptionHandling) {
    ArrayQueue<int, 2> queue;
    
    queue.enqueue(1);
    queue.enqueue(2);
    EXPECT_THROW(queue.enqueue(3), std::length_error);
    
    queue.dequeue();
    queue.dequeue();
    EXPECT_THROW(queue.dequeue(), std::out_of_range);
}
