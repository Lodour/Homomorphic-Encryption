cmake_minimum_required(VERSION 3.10)
project(Homomorphic_Encryption)

set(CMAKE_CXX_STANDARD 17)

# The following flags are copied from HElib's Makefile
set(CMAKE_CXX_FLAGS "-O2 -std=c++11 -pthread -DFHE_THREADS -DFHE_BOOT_THREADS -fmax-errors=2")

include_directories(include ext/HElib/src ext/ntl-11.3.2/include)
link_directories(/usr/local/lib)

add_executable(Homomorphic_Encryption main.cpp include/timer.h include/random.h src/random.cpp include/he.h src/he.cpp)
target_link_libraries(Homomorphic_Encryption ${CMAKE_SOURCE_DIR}/ext/HElib/src/fhe.a ntl gmp m)

# TODO: Integrate HElib's CMakeLists.txt