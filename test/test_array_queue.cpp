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

TEST_F(ArrayQueueTest, InitializerListConstructor) {
  ArrayQueue<int, 3> queue{1, 2, 3};

  EXPECT_EQ(queue.size(), 3);
  EXPECT_EQ(queue.top(), queue[0]);
  EXPECT_EQ(queue.bottom(), queue[2]);
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
  ArrayQueue<int, 3> queue{1, 2, 3};

  int expected = 1;
  for (const auto &value : queue) {
    EXPECT_EQ(value, expected++);
  }
}

TEST_F(ArrayQueueTest, ConstIterator) {
  ArrayQueue<int, 3> queue;
  queue.enqueue(1);
  queue.enqueue(2);
  queue.enqueue(3);

  const ArrayQueue<int, 3> &const_queue = queue;
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

TEST_F(ArrayQueueTest, AssignmentOperator) {
  ArrayQueue<int, 3> queue1;
  queue1.enqueue(1);
  queue1.enqueue(2);

  ArrayQueue<int, 3> queue2;
  queue2 = queue1;
  EXPECT_EQ(queue2.top(), 1);
  queue2.dequeue();
  EXPECT_EQ(queue2.top(), 2);
}

TEST_F(ArrayQueueTest, MoveAssignmentOperator) {
  ArrayQueue<int, 3> queue1;
  queue1.enqueue(1);
  queue1.enqueue(2);

  ArrayQueue<int, 3> queue2;
  queue2 = std::move(queue1);
  EXPECT_EQ(queue2.top(), 1);
  queue2.dequeue();
  EXPECT_EQ(queue2.top(), 2);
}

TEST_F(ArrayQueueTest, SizeConsistency) {
  ArrayQueue<int, 3> queue;
  EXPECT_EQ(queue.size(), 0);

  queue.enqueue(1);
  EXPECT_EQ(queue.size(), 1);

  queue.enqueue(2);
  queue.enqueue(3);
  EXPECT_EQ(queue.size(), 3);

  queue.dequeue();
  EXPECT_EQ(queue.size(), 2);
}

TEST_F(ArrayQueueTest, MixedOperations) {
  ArrayQueue<int, 3> queue;

  queue.enqueue(1);
  queue.enqueue(2);
  EXPECT_EQ(queue.size(), 2);

  queue.dequeue();
  EXPECT_EQ(queue.size(), 1);
  EXPECT_EQ(queue.top(), 2);

  queue.enqueue(3);
  queue.enqueue(4);
  EXPECT_TRUE(queue.is_full());

  std::vector<int> expected = {2, 3, 4};
  int idx = 0;
  for (const auto &value : queue) {
    EXPECT_EQ(value, expected[idx++]);
  }
}

TEST_F(ArrayQueueTest, CustomType) {
  struct MoveOnlyType {
    int value;
    MoveOnlyType() : value() {}
    MoveOnlyType(int v) : value(v) {}
    MoveOnlyType(const MoveOnlyType &) = delete;
    MoveOnlyType &operator=(const MoveOnlyType &) = delete;
    MoveOnlyType(MoveOnlyType &&) = default;
    MoveOnlyType &operator=(MoveOnlyType &&) = default;
  };

  ArrayQueue<MoveOnlyType, 2> queue;
  queue.enqueue((MoveOnlyType(1)));
  queue.enqueue(MoveOnlyType(2));

  EXPECT_EQ(queue.top().value, 1);
  queue.dequeue();
  EXPECT_EQ(queue.top().value, 2);
}
