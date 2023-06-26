cmake_minimum_required(VERSION 3.12)
project(CRC16)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Add the executable
add_executable(${PROJECT_NAME} crc16.cpp)
