#pragma once
#include<mutex>
#include<queue>
#include<condition_variable>
#include"Order.h"

class OrderQueue{
    public:
    void addOrder(const Order& order);  
    bool takeOrder(Order& order);       
    void close();                      

    private:
    std::queue<Order> orders;           
    std::mutex m;                       
    std::condition_variable cv;        
    bool closed = false;                
};