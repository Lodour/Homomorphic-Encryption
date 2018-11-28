//
// Created by Roll on 2018-11-28.
//

#ifndef HOMOMORPHIC_ENCRYPTION_TIMER_H
#define HOMOMORPHIC_ENCRYPTION_TIMER_H

#include <chrono>

class Timer {
 public:
  void tic() { tp1 = std::chrono::system_clock::now(); }
  void toc() { tp2 = std::chrono::system_clock::now(); }
  auto ms() -> std::chrono::milliseconds::rep {
    return std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1).count();
  }

 protected:
  std::chrono::time_point<std::chrono::system_clock> tp1, tp2;
};

#endif //HOMOMORPHIC_ENCRYPTION_TIMER_H
