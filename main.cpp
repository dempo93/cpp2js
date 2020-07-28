#include <array>
#include <future>
#include <iostream>

#ifdef IS_EMSCRIPTEN
#include <emscripten.h>
#define THREEYD_LIB extern "C" EMSCRIPTEN_KEEPALIVE
#else
#define THREEYD_LIB
#endif

namespace threeyd {

bool check_prime_chunk(int chunk_begin, int chunk_end, int number) {
  for (auto i = chunk_begin; i < chunk_end; ++i) {
    if (number % i == 0) {
      return false;
    }
  }
  return true;
}

struct Future {
  bool ready{false};
  bool is_prime{false};
  std::future<bool> future;
};

THREEYD_LIB const char *hello_world_string() { return "Hello World"; }

THREEYD_LIB int get_42() { return 42; }

THREEYD_LIB int is_prime(int n) {
  constexpr auto THREADS = 8;

  if (n == 1) {
    return 0;
  } else if (n == 2) {
    return 1;
  } else {
    std::array<Future,THREADS> futures;
    const auto ELEMENT_PER_CHUNK = n / (2 * THREADS);
    for (int i = 1; i < THREADS - 1; ++i) {
      futures[i].future =
          std::async(std::launch::async, check_prime_chunk,
                     i * ELEMENT_PER_CHUNK, (i + 1) * ELEMENT_PER_CHUNK, n);
    }
    futures[0].future = std::async(std::launch::async, check_prime_chunk, 2,
                                   ELEMENT_PER_CHUNK, n);
    futures[THREADS - 1].future =
        std::async(std::launch::async, check_prime_chunk,
                   (THREADS - 1) * ELEMENT_PER_CHUNK, (n / 2) + 1, n);

    while (true) {

      for (auto &fut : futures) {
        if (fut.ready) {
          continue;
        }
        if (fut.future.wait_for(std::chrono::seconds(0)) ==
            std::future_status::ready) {
          fut.is_prime = fut.future.get();
          fut.ready = true;
        }
      }

      if (std::any_of(futures.begin(), futures.end(), [](const auto &fut) {
            return fut.ready && !fut.is_prime;
          })) {
        return 0;
      }

      if (std::all_of(futures.begin(), futures.end(), [](const auto &fut) {
            return fut.ready && fut.is_prime;
          })) {
        return 1;
      }

      std::this_thread::sleep_for(std::chrono::milliseconds (10));
    }

    return 0;
  }
}
} // namespace threeyd

int main() {
  std::cout << threeyd::hello_world_string() << " " << threeyd::get_42()
            << std::endl;

  for (int i = 1000; i < 1020; ++i) {
    std::cout << i << " " << threeyd::is_prime(i) << std::endl;
  }

  return 0;
}
