//
// Created by Roll on 2018-11-28.
//
#ifndef HOMOMORPHIC_ENCRYPTION_RANDOM_H
#define HOMOMORPHIC_ENCRYPTION_RANDOM_H

#include <random>
#include <functional>
#include <chrono>
#include <cstdint>
#include <vector>

class Random {
 public:
  static std::vector<int> Vector(std::size_t size);

 protected:
  static std::int64_t seed;
  static std::default_random_engine random_engine;
  static std::function<int()> dice_int;
};

#endif //HOMOMORPHIC_ENCRYPTION_RANDOM_H
