#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
//
// Created by Roll on 2018-11-28.
//

#include "random.h"

std::int64_t Random::seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine Random::random_engine = std::default_random_engine(Random::seed);
std::function<int()> Random::dice_int = std::bind(std::uniform_int_distribution(0, 1000), Random::random_engine);

std::vector<int> Random::Vector(std::size_t size) {
  std::vector<int> vec(size);
  std::generate(vec.begin(), vec.end(), Random::dice_int);
  return vec;
}

#pragma clang diagnostic pop