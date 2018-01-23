cmake_minimum_required(VERSION 3.9)
project(concurrent_linked_list C)

set(CMAKE_C_STANDARD 99)

add_executable(concurrent_linked_list main.c mutex_code.c)