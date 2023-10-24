/**
 * @file unit_test.cpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief Unit test for buffer class
 *
 * @author s3mat3
 */

//#undef TRACE_FUNCTION
#include "byte_buffer.hpp"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define DOCTEST_CONFIG_SUPER_FAST_ASSERTS
#include "doctest.h"

std::string str("Hello world");
std::string rts("Wellcaome to the hell");

using namespace Mult;

struct Some {
    Some() : x{0}, y{0} {}
    ~Some() = default;
    int x;
    int y;
};

const char data4[] = {0x02,0x00,0x00,0x03};
const char test4[] = {0x01, 0x02, 0x03, 0x04};

TEST_CASE("constructor-ByteBuffer") {
    ByteBuffer x(test4, 4);
    REQUIRE(x);
    MESSAGE("Set up ByteBuffer default size AND Specify byte array");
    SUBCASE("sized") {
        ByteBuffer a(ByteBuffer::SIZE_1K);
        REQUIRE(a);
        CHECK(a.capacity() == ByteBuffer::SIZE_1K);
        MESSAGE("Sized constructor");
    }
    SUBCASE("Specify objects") {
        ByteBuffer a(test4, 4);
        REQUIRE(a);
        CHECK(a.wp() == 4);
        for (auto i = ByteBuffer::ZERO; i < a.wp(); ++i) {
            CHECK(a[i] == i + 1);
        }
        MESSAGE("Specify objects constructor");
    }
    SUBCASE("Copy construct") {
        MESSAGE("*** Copy start ***");
        ByteBuffer z(test4, 4);
        ByteBuffer y(z);
        MESSAGE("*** Copied ***");
        REQUIRE(z);
        REQUIRE(y);
        CHECK(x.capacity() == y.capacity());
        CHECK(x.wp() == y.wp());
        CHECK(x.wp() == 4);
        MESSAGE("*** Check for contents ***");
        for (auto i = ByteBuffer::ZERO; i < y.wp(); ++i) {
            CHECK(x[i] == y[i]);
        }
        MESSAGE("Copy constructor");
    }
    SUBCASE("Copy operator") {
        ByteBuffer y;
        y = x;
        REQUIRE(x);
        REQUIRE(y);
        CHECK(x.capacity() == y.capacity());
        CHECK(x.wp() == y.wp());
        for (auto i = ByteBuffer::ZERO; i < y.wp(); ++i) {
            CHECK(x[i] == y[i]);
        }
        MESSAGE("Copy operator");
    }
    SUBCASE("Move construct") {
        ByteBuffer z(std::move(x));
        REQUIRE(!(x));
        CHECK(x.capacity() == 0);
        CHECK(x.wp() == 0);
        REQUIRE(z);
        for (auto i = ByteBuffer::ZERO; i < z.wp(); ++i) {
             CHECK(z[i] == i + 1);
        }
        MESSAGE("Move constructor");
    }
    SUBCASE("Move operator") {
        ByteBuffer z;
        z = std::move(x);
        REQUIRE(!(x));
        CHECK(x.capacity() == 0);
        CHECK(x.wp() == 0);
        REQUIRE(z);
        for (auto i = ByteBuffer::ZERO; i < z.wp(); ++i) {
             CHECK(z[i] == i + 1);
        }
        MESSAGE("Move operator");
    }
}

TEST_CASE("Array access")
{
    const size_t size = 16;
    ByteBuffer x(size);
    REQUIRE(x);
    CHECK(x.capacity() == size);
    MESSAGE("Set up object to 16byte");

    SUBCASE("Index out of range rhs version") {
        for (auto i = ByteBuffer::ZERO; i < x.capacity(); ++i) {
            x.push_back(i);
        }
        CHECK_THROWS_WITH_AS(x[16], "index is out of range" ,std::out_of_range&);
    }
    SUBCASE("Index out of range lhs reference version") {
        for (auto i = ByteBuffer::ZERO; i < x.capacity(); ++i) {
            x.push_back(i);
        }
        CHECK_THROWS_WITH_AS(auto y = x[16], "index is out of range" ,std::out_of_range&);
    }
}

const char* asd = "Hello"; //5 byte
SCENARIO("buffer can be sized and resized")
{
    GIVEN("A vector with some items") {
        ByteBuffer v(5);
        REQUIRE(v.capacity() == 5);
        MESSAGE("Setup");

        WHEN("the size is reduced") {
            v.resize(2);

            THEN("the size changes but not capacity") {
                CHECK(v.capacity() == 2);
            }
        }
        WHEN("the size is increased") {
            v.resize(10);

            THEN("the size and capacity change") {
                CHECK(v.capacity() == 10);
            }
            AND_THEN("Write safe") {
                for (size_t i = 0; i < v.capacity(); ++i) {
                    v.push_back(i);
                }
                CHECK(v.wp() == v.capacity());
            }
            AND_THEN("Shrink") {
                v.assign(asd, 5);
                CHECK(v.wp() == 5);
                CHECK(v.capacity() == 10);
                CHECK(v.to_string() == "Hello");
                v.shrink();
                CHECK(v.capacity() == 5);
                CHECK(v.wp() == 5); // check write pointer
                CHECK(v.to_string() == "Hello");
                v.resize(2);
                CHECK(v.wp() == 2); // check write pointer
                CHECK(v.to_string() == "He");
            }
        }
    }
}

TEST_CASE("Helper functions")
{
    MESSAGE("Check helpers");
    using buffer = Mult::ByteBuffer;
    SUBCASE("make helper size over") {
        auto x = Mult::make_buffer(buffer::limit_size() + 1);
        REQUIRE(x);
        CHECK(x.capacity() == buffer::limit_size());
    }
    SUBCASE("make helper size 0") {
        auto x = Mult::make_buffer(0);
        REQUIRE(x);
        CHECK(x.capacity() == buffer::default_size());
    }
    SUBCASE("make helper size -1") {
        auto x = Mult::make_buffer(-1);
        REQUIRE(x);
        CHECK(x.capacity() == buffer::limit_size());
    }
}

TEST_CASE("UTF-8")
{
    MESSAGE("utf8");
    std::string x("Hello **日本語** -~$");
    ByteBuffer y{x};
    REQUIRE(y);
    // for (auto c : x) y.push_back(c);
    CHECK(x.size() == y.size());
    CHECK(x == y.to_string());
    MESSAGE(y.to_string());
    MESSAGE(x);
}
/// exception

// TEST_CASE("construct") {
//     SUBCASE("default") {
//         Mult::ByteArray x;
//         REQUIRE((x));
//         CHECK(x.get() != nullptr);
//         CHECK(x.capacity() == Mult::ByteArray::default_size());
//         CHECK(x.size() == 0);
//     }
//     SUBCASE("sized") {
//         Mult::ByteArray x(Mult::ByteArray::SIZE_512);
//         CHECK(x.capacity() != Mult::ByteArray::default_size());
//         CHECK(x.capacity() == Mult::ByteArray::SIZE_512);
//         CHECK(x.size() == 0);

//         SUBCASE("Boundary safe") {
//             MESSAGE("Boundary safe");
//             std::string str(512, 'x');
//             CHECK(x.assign(str) == Mult::OK);
//         }

//         SUBCASE("range over") {
//             MESSAGE("Boundary range over");
//             std::string str(513, 'x');
//             CHECK(x.assign(str) == Mult::RANGE_OVER);
//         }
//     }
//     SUBCASE("iniitalize byte array") {
//         Mult::ByteArray x(Mult::ByteArray::SIZE_1K, null4, 4);
//         CHECK(x.capacity() == Mult::ByteArray::SIZE_1K);
//         CHECK(x.size() == 4);
//         CHECK(x.to_debug() == "[NUL][NUL][NUL][NUL]");
//     }
//     SUBCASE("iniitalize byte string") {
//         Mult::ByteArray x(Mult::ByteArray::SIZE_1K, str);
//         CHECK(x.size() != 4);
//         CHECK(x.to_string() == str);
//     }
//     SUBCASE("iniitalize byte MOVE1") {
//         Mult::ByteArray y(Mult::ByteArray{str});
//         CHECK(y.size() != 4);
//         CHECK(y.to_string() == str);
//     }
//     SUBCASE("iniitalize byte MOVE2") {
//         Mult::ByteArray x(str);
//         Mult::ByteArray y(std::move(x));
//         CHECK(y.size() != 4);
//         CHECK(y.to_string() == str);
//     }
//     SUBCASE("iniitalize byte MOVE3") {
//         Mult::ByteArray y = Mult::ByteArray{str};
//         CHECK(y.size() != 4);
//         CHECK(y.to_string() == str);
//     }
//     SUBCASE("iniitalize byte MOVE4") {
//         auto y = func();
//         CHECK(y.size() != 4);
//         CHECK(y.to_string() == rts);
//     }
// }

// TEST_CASE("append") {
//     SUBCASE("first append string class") {
//         Mult::ByteArray x;
//         x.append(str);
//         CHECK(x.to_string() == str);
//     }
//     SUBCASE("first append char pointer") {
//         Mult::ByteArray x;
//         x.append(null4, 4);
//         CHECK(x.to_debug() == "[NUL][NUL][NUL][NUL]");
//     }
//     SUBCASE("first append string class UTF8") {
//         std::string utf("ほげ帆下ヘロＡＢＣＤ");
//         Mult::ByteArray x;
//         x.append(utf);
//         CHECK(x.to_string() == utf);
//         SUBCASE("more add1") {
//             x.append(str);
//             CHECK(x.to_string() == utf + str);
//             std::cout << x.to_string() << std::endl;
//         }
//         SUBCASE("more add2") {
//             x.append(null4, 4);
//             std::string n({0x00,0x00,0x00,0x00});
//             CHECK(x.to_string() == utf + n);
//             std::cout << x.to_debug() << std::endl;
//         }
//     }

// }

// TEST_CASE("extract") {
//     SUBCASE("first append string class") {
//         Mult::ByteArray x;
//         x.append(str);
//         auto y = x.extract(6, 5);
//         CHECK(y.to_string() == "world");
//         SUBCASE("get one char1") {
//             CHECK(y[0] == 'w');
//             CHECK(y[4] == 'd');
//         }
//         SUBCASE("get one char2 (with ranged FOR)") {
//             int i = 0;
//             for (auto c : y) {
//                 switch (i++) {
//                 case 0: CHECK(c == 'w'); break;
//                 case 1: CHECK(c == 'o'); break;
//                 case 2: CHECK(c == 'r'); break;
//                 case 3: CHECK(c == 'l'); break;
//                 case 4: CHECK(c == 'd'); break;
//                 }
//             }
//         }
//     }
// }



