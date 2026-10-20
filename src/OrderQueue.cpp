#include "OrderQueue.h"

void OrderQueue::addOrder(const Order& order) {
    {
        std::lock_guard<std::mutex> lock(m);   
        orders.push(order);                      
    }
    cv.notify_one();                                   
}

void OrderQueue::close() {
    {
        std::lock_guard<std::mutex> lock(m);
        closed = true;
    }
    cv.notify_all();      
}

bool OrderQueue::takeOrder(Order& order) {
    std::unique_lock<std::mutex> lock(m);          
    while (orders.empty() && !closed) {                          
        cv.wait(lock);
    }
    if (orders.empty() && closed) {
        return false;                        
    }
    order = orders.front();                
    orders.pop();
    return true;                            // got an order
}