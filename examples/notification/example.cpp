/**
 * @file example.cpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief Example for notification
 *
 * @author s3mat3
 */

#include <iostream>

#include "debug.hpp"
#include "notification.hpp"
#include "thread.hpp"


using notification_int_t = Mult::Notification<int>;
using notification_string_t = Mult::Notification<std::string&>;

class SomeCaller
{
public:
    SomeCaller() : run(true) {}
    void stop() {run = false;}

    void function1(Mult::void_ptr)
    {
        size_t l = 0;
        while(run) {
            std::string hello(__FUNCTION__);
            hello.append(std::to_string(l++));
            MULT_LOG("notify " +  hello);
            aaa.notify(l);
            bbb.notify(hello);
            Mult::Thread::sleep(100);
            if (l > 20) break;
        }
    }
public:
    notification_int_t    aaa;
    notification_string_t bbb;
private:
    bool run;
};

class SomeCallee
{
public:
    SomeCallee() : run(true) {}
    void stop() {run = false;}

    void function1(Mult::void_ptr)
    {
        size_t l = 0;
        while(run) {
            std::string hello(__FUNCTION__);
            MULT_LOG("Hello " +  hello + std::to_string(l++));
            Mult::Thread::sleep(100);
            if (l > 20) break;
        }
    }

    int function2(int a)
    {
        std::string x = std::to_string(a) + " get from Caller";
        MULT_LOG(x);
        return a;
    }

    int function3(std::string a)
    {
        MULT_LOG(a);
        return 0;
    }
public:
private:
    bool run;
};

int main()
{
    using runnable_t = Mult::RunnableAdapter<SomeCaller>;
    // setup caller Thread
    auto caller = std::make_shared<SomeCaller>();
    auto runner1 = std::make_shared<runnable_t>(caller, &SomeCaller::function1);
    Mult::Thread th1(runner1, "TH1");
    // setup callee object
    auto callee  = std::make_shared<SomeCallee>();
    // connect callback on 2way
    caller->aaa.connect(callee, &SomeCallee::function2, MULT_ARGC(1));
    caller->bbb(callee, &SomeCallee::function3, std::placeholders::_1);

    th1.start(nullptr);
    return 0;
}
