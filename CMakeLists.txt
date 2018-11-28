cmake_minimum_required(VERSION 3.12)
project(Homomorphic_Encryption)

set(CMAKE_CXX_STANDARD 17)

include_directories(include ext/HElib/src ext/ntl-11.3.2/include)

add_executable(Homomorphic_Encryption main.cpp include/timer.h)
