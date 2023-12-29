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


#undef TRACE_FUNCTION
#undef MULT_TRACE_FUNCTION
#include <optional>
#include <string>
#include "benchmark.h"

#include "result.hpp"
#include "expected.hpp"

using namespace Mult;
/**  .
 *
 *
 */
auto response1(bool s) -> std::string
{
    if (s)
        return std::string("Hello world");
    else
        return std::string("");
}
static void BM_string_only_ok(benchmark::State& state) {
  for (auto _ : state)
      auto x = response1(true);
}
static void BM_string_only_fail(benchmark::State& state) {
  for (auto _ : state)
      auto x = response1(false);
}

BENCHMARK(BM_string_only_ok);
BENCHMARK(BM_string_only_fail);
/**  .
 *
 *
 */
auto response2(bool s) -> std::optional<std::string>
{
    if (s)
        return std::string("Hello world");
    else return std::nullopt;;
}
static void BM_std_optional_ok(benchmark::State& state) {
  for (auto _ : state)
      auto y = response2(true);
}
static void BM_std_optional_fail(benchmark::State& state) {
  for (auto _ : state)
      auto y = response2(false);
}

BENCHMARK(BM_std_optional_ok);
BENCHMARK(BM_std_optional_fail);
/**  .
 *
 *
 */
auto response3(bool s) -> tl::expected<std::string, return_code>
{
    using result_ = tl::expected<std::string, return_code>;
    if (s)
        return result_(std::string("Hello world"));
    else
        return tl::make_unexpected(-1);
}
static void BM_expected_ok(benchmark::State& state) {
  for (auto _ : state)
      auto x = response3(true);
}
static void BM_expected_fail(benchmark::State& state) {
  for (auto _ : state)
      auto x = response3(false);
}

BENCHMARK(BM_expected_ok);
BENCHMARK(BM_expected_fail);
/**  .
 *
 *
 */
auto response4(bool s) -> Mult::Result<std::string>
{
    using result = Mult::Result<std::string>;
    if (s)
        return result("Hello world");
        // return std::string("Hello world");
    else
        return result(Mult::error_type(-1));
}

static void BM_mult_result_ok_direct(benchmark::State& state) {
  for (auto _ : state)
      auto x = response4(true);
}
static void BM_mult_result_fail_direct(benchmark::State& state) {
  for (auto _ : state)
      auto x = response4(false);
}

BENCHMARK(BM_mult_result_ok_direct);
BENCHMARK(BM_mult_result_fail_direct);

/**  .
 *
 *
 */
std::string str("Hello world");

auto response5(bool s) -> Mult::Result<std::string>
{
    using result = Mult::Result<std::string>;
    if (s)
        return result(str);
    else
        return result(Mult::error_type(-1));
}

static void BM_mult_result_ok_copy(benchmark::State& state) {
  for (auto _ : state)
      auto x = response5(true);
}
static void BM_mult_result_fail_copy(benchmark::State& state) {
  for (auto _ : state)
      auto x = response5(false);
}

BENCHMARK(BM_mult_result_ok_copy);
BENCHMARK(BM_mult_result_fail_copy);
/**  .
 *
 *
 */
auto response6(bool s) -> Mult::Result<std::string>
{
    using result = Mult::Result<std::string>;
    if (s)
        return result(std::string("Hello world"));
    else
        return result(Mult::error_type(-1));
}

static void BM_mult_result_ok_move(benchmark::State& state) {
  for (auto _ : state)
      auto x = response6(true);
}
static void BM_mult_result_fail_move(benchmark::State& state) {
  for (auto _ : state)
      auto x = response6(false);
}

BENCHMARK(BM_mult_result_ok_move);
BENCHMARK(BM_mult_result_fail_move);

BENCHMARK_MAIN();
