/*! \file complex_test.cpp
 * \brief
 *
 * \author matsuo@e-mail.jp
 */

#include "result.hpp"
#include "byte_buffer.hpp"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

using namespace Mult;
class Some
{
public:
    using byte_buffer = ByteBuffer;
    using result_ = result<byte_buffer>;
    Some(int i, double d) : a(i), b(d) {}
    Some() : Some(0, 0.0) {}
    auto func1() -> result_
    {
        auto x = from_string(std::to_string(a) + " is int value " + std::to_string(b));
        return ok(x);
    }
    auto func2(int a) -> result_
    {
        ByteBuffer x;
        if (a > 5)
            return func1();
        else
            return error(Mult::to_error(Mult::UNDER_FLOW));
    }
    void aa(int i) { a = i;}
    void bb(double d) { b = d;}
    double bb() {return b;}
private:
    int    a;
    double b;
};

TEST_CASE("Simple")
{
    Some some;
    auto x = some.func1();
    REQUIRE((x));
    CHECK(x.value().to_string() == "0 is int value 0.000000");
    some.aa(3);
    some.bb(3.0);
    auto y = some.func1();
    REQUIRE((y));
    CHECK(y.value().to_string() == "3 is int value 3.000000");
}
TEST_CASE("with failure")
{
    Some some;
    auto x = some.func2(6);
    REQUIRE((x));
    CHECK(x.value().to_string() == "0 is int value 0.000000");
    some.aa(3);
    some.bb(3.0);
    auto y = some.func2(5);
    REQUIRE(! (y));
    CHECK(y.error() == Mult::UNDER_FLOW);
}
TEST_CASE("Single result")
{
    using result = Mult::result<Mult::ByteBuffer>;
    auto x = Mult::ok(Mult::ByteBuffer{"Hello", 5});
    auto y = result(Mult::FAILURE);
    REQUIRE((x));
    CHECK(x.value().to_string() == "Hello");
    REQUIRE(!(y));
    CHECK(y.error() == Mult::FAILURE);
}
