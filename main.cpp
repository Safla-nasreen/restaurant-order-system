#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include"Order.h"
#include "OrderQueue.h"

OrderQueue counter;
const std::string MENU[10] = {"Biriyani", "Dosa", "Parotta", "Idli", "Pulavu","Mandhi","Chappathi","Butter chicken", "Alfahm","Vegitable curry"};

void showMenu() {
    std::cout << "------ MENU ------" << std::endl;
    for (int i = 0; i < 10; i++) {
        std::cout << i + 1 << ". " << MENU[i] << std::endl;
    }
    std::cout << "------------------" << std::endl;
}
void waiter() {
    showMenu();
    for (int i = 0; i < 5; i++) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        counter.addOrder({i + 1, 1, MENU[i]});
        std::cout << "Waiter placed order " << i + 1 << std::endl;                         
    }
   counter.close();
}

void cook(int cookId) {
    Order o;
while (counter.takeOrder(o)) {
    std::cout << "Cook " << cookId << " is cooking order " << o.id << ": " << o.dish << " (table " << o.table << ")" << std::endl;
}
std::cout << "Cook " << cookId << " goes home" << std::endl;
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