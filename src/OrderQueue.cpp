#include "OrderQueue.h"

bool OrderQueue::addOrder(const Order &order)
{
    {
        std::lock_guard<std::mutex> lock(m);
        if (closed)
        {
            return false; 
        }
        orders.push(order);
    }
    cv.notify_one();
    return true; 
}

void OrderQueue::close()
{
    {
        std::lock_guard<std::mutex> lock(m);
        closed = true;
    }
    cv.notify_all();
}

bool OrderQueue::takeOrder(Order &order)
{
    std::unique_lock<std::mutex> lock(m);
    while (orders.empty() && !closed)
    {
        cv.wait(lock);
    }
    if (orders.empty() && closed)
    {
        return false;
    }
    order = orders.front();
    orders.pop();
    return true; // got an order
}