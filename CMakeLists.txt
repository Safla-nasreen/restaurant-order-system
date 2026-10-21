cmake_minimum_required(VERSION 3.16)
project(RestaurantOrderSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(restaurant main.cpp src/OrderQueue.cpp)
target_include_directories(restaurant PRIVATE include)
target_link_libraries(restaurant PRIVATE Threads::Threads)