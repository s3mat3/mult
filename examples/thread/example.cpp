/**
 * @file example.cpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief Example for thread
 *
 * @author s3mat3
 */

#include <iostream>

#include "debug.hpp"
#include "thread.hpp"

class Some
{
public:
    Some() : run(true) {}
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
    void function2(Mult::void_ptr)
    {
        size_t l = 0;
        while(run) {
            std::string hello(__FUNCTION__);
            MULT_LOG("Fugoo " +  hello + std::to_string(l++));
            Mult::Thread::sleep(100);
            if (l > 20) break;
        }
    }
private:
    bool run;
};

int main()
{
    using runnable_t = Mult::RunnableAdapter<Some>;
    //using runnable_p = std::shared_ptr<runnable_t>();
    auto runner1 = std::make_shared<runnable_t>(std::make_shared<Some>(), &Some::function1);
    auto runner2 = std::make_shared<runnable_t>(std::make_shared<Some>(), &Some::function2);
    Mult::Thread th1(runner1, "TH1");
    Mult::Thread th2(runner2, "TH2");

    th1.start(nullptr);
    th2.start(nullptr);

    return 0;
}
