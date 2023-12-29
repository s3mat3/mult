/*! \file unit_test.cpp
 *
 * \brief
 *
 */

#include "storage.hpp"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

using namespace Mult;



TEST_CASE("StorageBase construct1") {
    auto x = StorageBase<char>();
    CHECK((x) == true);
    CHECK(x.capacity() == default_volume());
    CHECK(x.size() == 0);
}

TEST_CASE("StorageBase construct2") {
    auto x = StorageBase<int>(request_volume(rooms::V128));
    CHECK((x) == true);
    CHECK(x.capacity() == request_volume(rooms::V128));
    CHECK(x.size() == 0);
}
TEST_CASE("StorageBase construct3") {
    auto x = StorageBase<int>(request_volume(rooms::V128), 0xdeadbeef);
    CHECK((x) == true);
    CHECK(x.capacity() == request_volume(rooms::V128));
    CHECK(x.size() == request_volume(rooms::V128));
    int c = 0;
    auto p = x.ptr();
    for (auto i = ZERO; i < request_volume(rooms::V128); ++p, ++i) {
        c = *p;
    }
    CHECK(c == 0xdeadbeef);
    // copy construct / copy assign
    SUBCASE("Copy construct/assign") {
        auto x_ptr = x.ptr();
        // copy construct
        StorageBase<int>y(x);
        CHECK((x) == true);
        CHECK(y.capacity() == request_volume(rooms::V128));
        CHECK(y.size() == request_volume(rooms::V128));
        int c = 0;
        auto p = y.ptr();
        auto y_ptr = y.ptr();
        CHECK(y_ptr != x_ptr);
        for (auto i = ZERO; i < request_volume(rooms::V128); ++p, ++i) {
            c = *p;
        }
        CHECK(c == 0xdeadbeef);
        // copy assign
        auto z = x;
        CHECK((x) == true);
        CHECK(z.capacity() == request_volume(rooms::V128));
        CHECK(z.size() == request_volume(rooms::V128));
        c = 0;
        auto pp = z.ptr();
        auto z_ptr = z.ptr();
        CHECK(z_ptr != x_ptr);
        CHECK(z_ptr != y_ptr);
        for (auto i = ZERO; i < request_volume(rooms::V128); ++pp, ++i) {
            c = *pp;
        }
        CHECK(c == 0xdeadbeef);
    }
    // move
    SUBCASE("Move construct/assign") {
        auto x_ptr = x.ptr();
        // construct
        StorageBase<int>y(std::move(x));
        CHECK((x) == false);
        CHECK(y.capacity() == request_volume(rooms::V128));
        CHECK(y.size() == request_volume(rooms::V128));
        int c = 0;
        auto p = y.ptr();
        auto y_ptr = p;
        CHECK(y_ptr == x_ptr);
        for (auto i = ZERO; i < request_volume(rooms::V128); ++p, ++i) {
            c = *p;
        }
        CHECK(c == 0xdeadbeef);
        // move assign
        auto z = std::move(y);
        CHECK((y) == false);
        CHECK(y_ptr == z.ptr());
        CHECK(z.capacity() == request_volume(rooms::V128));
        CHECK(z.size() == request_volume(rooms::V128));
    }
}
struct SomeClass {
    int a = 1000;
    std::string b = {"Hello world"};
};

TEST_CASE("Direct construct") {
    StorageBase<SomeClass> x(4, {10, "DEAD_BEEF"});
    CHECK((x) == true);
    for (auto v : x) {
        CHECK(v.a == 10);
        CHECK(v.b == "DEAD_BEEF");
    }
}
