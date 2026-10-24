#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include "Order.h"
#include "OrderQueue.h"

OrderQueue counter;
const std::vector<std::string> MENU = {"Biriyani", "Dosa", "Parotta", "Idli", "Pulavu", "Mandhi", "Chappathi", "Butter chicken", "Alfahm", "Vegitable curry"};

void showMenu()
{
    std::cout << "------ MENU ------" << std::endl;
    for (size_t i = 0; i < MENU.size(); i++)
    {
        std::cout << i + 1 << ". " << MENU[i] << std::endl;
    }
    std::cout << "------------------" << std::endl;
}
void waiter()
{
    showMenu();
    int table;
    int nextId = 1;
    while (true)
    {
        std::cout << "Table number? (0 = close): ";
        std::cin >> table;
        if (table == 0)
        {
            break;
        }
        int choice;
        while (true)
        {
            std::cout << "Dish number for table " << table << "? (0 = done): ";
            std::cin >> choice;
            if (choice == 0)
            {
                break;
            }
            if (choice < 0 || choice > (int)MENU.size())
            {
                std::cout << "Invalid choice" << std::endl;
                continue;
            }
            counter.addOrder({nextId, table, MENU[choice - 1]});
            std::cout << "Waiter placed order " << nextId << std::endl;
            nextId++;
        }
    }
    counter.close();
}

void cook(int cookId)
{
    Order o;
    while (counter.takeOrder(o))
    {
        std::cout << "Cook " << cookId << " is cooking order " << o.id << ": " << o.dish << " (table " << o.table << ")" << std::endl;
    }
    std::cout << "Cook " << cookId << " goes home" << std::endl;
}

int main()
{
    std::thread w(waiter);
    std::thread c1(cook, 1);
    std::thread c2(cook, 2);
    w.join();
    c1.join();
    c2.join();
    return 0;
}