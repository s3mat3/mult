/**
 * @file example.cpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief StateMacine sample
 *
 * @author s3mat3
 */

// #include <memory>
// #include <unordered_map>

#include <unordered_map>
#include <iostream>

#include "signal_controller.hpp"





int main(void)
{
    TrafficSignalController ts;
    ts.dispatch(nullptr);
    return 0;
}

