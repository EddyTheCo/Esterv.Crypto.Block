cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(testing VERSION 0.1 DESCRIPTION "diferent tests for the library" LANGUAGES CXX)


add_executable(tip24 tip24.cpp)
add_test(NAME tip24 COMMAND tip24)
target_link_libraries(tip24 PRIVATE qblock)

add_executable(serialization serialization.cpp)
add_test(NAME serialization COMMAND serialization)
target_link_libraries(serialization PRIVATE qblock)

