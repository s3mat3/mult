/**
 * @file bench.cpp
 *
 * @copylight © 2023 Matsuo Shin
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief bench mark for std::optional VS Mult::Result
 *
 * @warning using google benchmark
 *
 * @author matsuo.shin@gmail.com
 */


#undef MULT_TRACE_FUNCTION
#include <string>
#include <vector>
#include "benchmark.h"

#include "storage.hpp"

using namespace Mult;
/**  .
 *
 *
 */
static void BM_StringCreation(benchmark::State& state) {
  for (auto _ : state)
      std::string empty_string(128,0x00);
}
// Register the function as a benchmark

static void BM_StorageCreation(benchmark::State& state) {
  for (auto _ : state)
      StorageBase<char> empty_strage(128, '0');
}
BENCHMARK(BM_StringCreation);
BENCHMARK(BM_StorageCreation);

/**  .
 *
 *
 */
static constexpr std::size_t volume = 1024*1024;
static void BM_StringCreationSize(benchmark::State& state) {
  for (auto _ : state)
      std::string empty_string(volume, 'c');
}

static void BM_VectorCreationSize(benchmark::State& state) {
  for (auto _ : state)
      std::vector<char> empty_vector(volume, 'c');
}

static void BM_StorageCreationSize(benchmark::State& state) {
  for (auto _ : state)
      StorageBase<char> empty_strage(volume, 'c');
}

BENCHMARK(BM_StringCreationSize);
BENCHMARK(BM_VectorCreationSize);
BENCHMARK(BM_StorageCreationSize);

static void BM_StringCopy(benchmark::State& state) {
    std::string str(volume, 'c');
    for (auto _ : state)
        std::string empty_string = str;
}
static void BM_StorageCopy(benchmark::State& state) {
    StorageBase<char> storage(volume, 'c');
    for (auto _ : state)
        StorageBase<char> empty_storage = storage;
}
BENCHMARK(BM_StringCopy);
BENCHMARK(BM_StorageCopy);

BENCHMARK_MAIN();
