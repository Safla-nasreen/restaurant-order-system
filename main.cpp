#include <iostream>
#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>        // 1. the bell's header
#include <chrono>
#include"Order.h"

std::queue<Order> counter;
std::mutex m;
std::condition_variable cv;
bool closed = false;

void waiter() {
    std::string dishes[5] = {"Biriyani", "Dosa", "Parotta", "Idli", "pulavu"};
    for (int i = 0; i < 5; i++) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        {
            std::lock_guard<std::mutex> lock(m);   
            counter.push({i + 1, dishes[i]});
            std::cout << "Waiter placed order " << i + 1 << std::endl;
        }
        cv.notify_one();                          
    }
   std::lock_guard<std::mutex> lock(m);
   closed = true;
   cv.notify_all();
}

void cook(int cookId) {
    while(true) {
        std::unique_lock<std::mutex> lock(m);       
        while (counter.empty() && !closed) {                      
            cv.wait(lock);
        }
        if (counter.empty() && closed) {
            std::cout << "Cook " << cookId << " goes home" << std::endl;
            break;
        }    
        Order o = counter.front();
        counter.pop();
        std::cout <<"Cook: "<<cookId<< " is cooking order " << o.id << ": " << o.dish << std::endl;
    }
}

int main() {
    std::thread w(waiter);
    std::thread c1(cook, 1);   
    std::thread c2(cook, 2);                 
    w.join();
    c1.join();  
    c2.join();                                 
    return 0;
}