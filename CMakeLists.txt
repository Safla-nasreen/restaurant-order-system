cmake_minimum_required(VERSION 3.16)
project(RestaurantOrderSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(restaurant main.cpp src/OrderQueue.cpp)
target_include_directories(restaurant PRIVATE include)
target_link_libraries(restaurant PRIVATE Threads::Threads)

find_package(GTest REQUIRED)
add_executable(tests tests/test_order_queue.cpp src/OrderQueue.cpp)
target_include_directories(tests PRIVATE include)
target_link_libraries(tests PRIVATE GTest::gtest GTest::gtest_main Threads::Threads)