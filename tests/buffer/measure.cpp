/**
 * @file measure.cpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief Check for buffer perfomance
 *
 * @warning using google benchmark
 *
 * @author s3mat3
 */

#undef TRACE_FUNCTION

#include "benchmark/benchmark.h"

#include "byte_buffer.hpp"
#include "measure_time.hpp"

using namespace Mult;
char a[2048];
constexpr long LOOP_MAX = 100000;

static void BM_StringCreation(benchmark::State& state) {
  for (auto _ : state)
    std::string empty_string;
}
// Register the function as a benchmark
BENCHMARK(BM_StringCreation);

static void BM_bbCreation(benchmark::State& state) {
  for (auto _ : state)
    ByteBuffer empty_string;
}
// Register the function as a benchmark
BENCHMARK(BM_bbCreation);

// Define another benchmark
static void BM_StringCopy(benchmark::State& state) {
  std::string x = "hello";
  for (auto _ : state)
    std::string copy(x);
}
BENCHMARK(BM_StringCopy);

static void BM_bbCopy(benchmark::State& state) {
    ByteBuffer x("hello", 5);
    for (auto _ : state)
        ByteBuffer copy(x);
}
BENCHMARK(BM_bbCopy);

BENCHMARK_MAIN();
// template<typename BT>
// class Benchmark
// {
// public:
// private:
// };

// int main()
// {
//     MeasureTime timer;
//     long y = 0;
//     long i = 0;
//     long max = 0;
//     long min = 100000;
//     for (i = 0; i < LOOP_MAX; i++) {
//         timer.start_acquire();
//         for (auto j = 0; j < ByteBuffer::SIZE_2K; j++) {
//             a[j] = j & 0x7f;
//         }
//         auto x = timer.acquire();
//         y += x;
//         max = (max >= x) ? max : x;
//         min = (min >= x) ? x : min;
//     }
//     std::cout << "Naitive array char[2048] sequencial write\n\t>>> Total: " << y << " Min " << min << " Avarage " << y / LOOP_MAX << " Max " << max << std::endl;
//     y = 0;
//     i = 0;
//     max = 0;
//     min = 10000;
//     for (i = 0; i < LOOP_MAX; i++) {
//         timer.start_acquire();
//         for (auto j = 0; j < ByteBuffer::SIZE_2K; j++) {
//             auto val = a[j];
//         }
//         auto x = timer.acquire();
//         y += x;
//         max = (max >= x) ? max : x;
//         min = (min >= x) ? x : min;
//     }
//     std::cout << "Naitive array char[2048] sequencial read\n\t>>> Total: " << y << " Min " << min << " Avarage " << y / LOOP_MAX << " Max " << max << std::endl;

//     y = 0;
//     i = 0;
//     max = 0;
//     min = 100000;
//     for (i = 0; i < LOOP_MAX; i++) {
//         std::string b(ByteBuffer::SIZE_2K, '0');
//         timer.start_acquire();
//         for (auto j = 0; j < ByteBuffer::SIZE_2K; j++) {
//             b.push_back(j & 0x7f);
//         }
//         auto x = timer.acquire();
//         y += x;
//         //std::cout << "Stop  : "<<  x << std::endl;
//         max = (max >= x) ? max : x;
//         min = (min >= x) ? x : min;
//     }
//     std::cout << "std::string(SIZE_2K,'0')\n\t>>> Total " << y << " Min " << min << " Avarage " << y / LOOP_MAX << " Max " << max << std::endl;

//     y = 0;
//     i = 0;
//     max = 0;
//     min = 100000;
//     for (i = 0; i < LOOP_MAX; i++) {
//         ByteBuffer b{ByteBuffer::SIZE_2K};
//         timer.start_acquire();
//         for (auto j = 0; j < ByteBuffer::SIZE_2K; j++) {
//             b.push_back(j & 0x7f);
//         }
//         auto x = timer.acquire();
//         y += x;
//         //std::cout << "Stop  : "<<  x << std::endl;
//         max = (max >= x) ? max : x;
//         min = (min >= x) ? x : min;
//     }
//     std::cout << "ByteBuffer(SIZE_2K)\n\t>>> Total " << y << " Min " << min << " Avarage " << y / LOOP_MAX << " Max " << max << std::endl;
//     return 0;
// }
