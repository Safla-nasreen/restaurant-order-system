#include <gtest/gtest.h>
#include "OrderQueue.h"

TEST(OrderQueueTest, TakeReturnsSameOrder)
{
    OrderQueue q;
    q.addOrder({1, 5, "Dosa"});

    Order o;
    ASSERT_TRUE(q.takeOrder(o)); // we must get an order

    EXPECT_EQ(o.id, 1);
    EXPECT_EQ(o.table, 5);
    EXPECT_EQ(o.dish, "Dosa");
}

TEST(OrderQueueTest, AddRefusedAfterClose)
{
    OrderQueue q;
    q.close(); // Arrange: close the kitchen

    bool accepted = q.addOrder({2, 3, "Tea"}); // Act: late customer

    EXPECT_FALSE(accepted); // Check: it must be refused
}

TEST(OrderQueueTest, TakeReturnsFalseWhenClosedAndEmpty)
{
    OrderQueue q;
    q.close(); // Arrange: closed, no orders

    Order o;
    bool gotOrder = q.takeOrder(o); // Act: a cook asks for work

    EXPECT_FALSE(gotOrder); // Check: no work, go home
}