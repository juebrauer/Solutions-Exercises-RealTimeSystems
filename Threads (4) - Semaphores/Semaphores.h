#pragma once

/// A counting semaphore class in C++
/// using mutexes and conditional variables
///
/// code is from
/// http://stackoverflow.com/questions/4792449/c0x-has-no-semaphores-how-to-synchronize-threads

#include <thread>
#include <mutex>


class Semaphore {
public:
  Semaphore(int count_ = 0)
    : count(count_) {}

  inline void V()
  {
    std::unique_lock<std::mutex> lock(mtx);
    count++;
    cv.notify_one();
  }

  inline void P()
  {
    std::unique_lock<std::mutex> lock(mtx);

    while (count == 0) {
      cv.wait(lock);
    }
    count--;
  }

private:
  std::mutex mtx;
  std::condition_variable cv;
  int count;
};