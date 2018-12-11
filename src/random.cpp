//
// Created by Roll on 2018-11-28.
//

#include <random.h>

std::vector<long> Random::Vector(std::size_t size) {
  std::vector<long> vec(size);
  std::generate(vec.begin(), vec.end(), Random::dice_long);
  return vec;
}

long Random::dice_long() {
  static auto _dice = std::bind(std::uniform_int_distribution<long>(0, 1000), Random::random_engine());
  return _dice();
}

std::int64_t Random::seed() {
  static auto _seed = std::chrono::system_clock::now().time_since_epoch().count();
  return _seed;
}

const std::default_random_engine &Random::random_engine() {
  static auto _engine = std::default_random_engine(static_cast<unsigned int>(Random::seed()));
  return _engine;
}

