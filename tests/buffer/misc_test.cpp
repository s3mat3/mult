/**
 * @file measure.cpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief misc check
 *
 * @author s3mat3
 */

# include "byte_buffer.hpp"

struct SomeClass {
    SomeClass() = default;
    SomeClass(int x, int y) : x{x}, y{y} {}
    int x = 1;
    int y = 2;
};
static constexpr Mult::size_type TEST_ROOMS = request_volume(Mult::rooms::V16K) * request_volume(Mult::rooms::V16K);

int main()
{
    SomeClass arr[10] = {{0,0},{1,-1},{2,-2},{3,-3},{4,-4},{5,-1},{6,-2},{7,-3},{8,-4},{9,-4}};
    Mult::ByteBuffer s(1024);
    Mult::ByteBuffer x(s);
    auto y = x;
    auto z = std::move(x);
    Mult::BufferBase<SomeClass> some_buffer(10);
    DUMP(s.capacity());
    DUMP(x.capacity());
    DUMP(y.capacity());
    // y.push_back('H');
    some_buffer.assign(&arr[0], some_buffer.capacity());
    DUMP(some_buffer.size());
    // check resize
    Mult::ByteBuffer t = {0,1,2,3,4,5,6,7,8,9};
    DUMP(t.capacity());
    t.push_back(10);
    DUMP(t.capacity());
    MULT_ASSERT(t.capacity() == 20, "CPACITY check", true);

    Mult::ByteBuffer a(TEST_ROOMS, 0x00);
    MULT_ASSERT(a.capacity() == TEST_ROOMS, "CPACITY check", true);
    a.push_back(0x03);
    MULT_ASSERT(a.capacity() == TEST_ROOMS * 2, "CPACITY check", true);
    MULT_ASSERT(a[TEST_ROOMS] == 0x03, "contents check", true);
    DUMP(TEST_ROOMS);
    DUMP(TEST_ROOMS * 2);
    DUMP(a.capacity());
    VAR_DUMP(a.capacity());
    DUMP(a.size());
    MARK();
    MULT_LOG("Hoge" + VAR_DUMP(a.capacity()));
    MULT_INFO("Hoge" + VAR_DUMP(a.capacity()));
    MULT_WARN("Hoge"  + VAR_DUMP(a.capacity()));
    MULT_NOTICE("Hoge" + VAR_DUMP(a.capacity()));
    MULT_ERROR("Hoge" + VAR_DUMP(a.capacity()));
    MULT_FATAL("Hoge" + VAR_DUMP(a.capacity()));
    return Mult::OK;
}
