/**
 * @file sl_thread.hpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief Stack less thread
 *
 * @defgroup StackLessThread
 * @{
 * Protothread-cppを改変したlicenseはlicenses/protothreads-cppを参照の事 <br>
 * https://github.com/benhoyt/protothreads-cpp <br>
 * さらに、大元は <br>
 * @verbatim
  This was "ported" to C++ from Adam Dunkels' protothreads C library at:
      http://www.sics.se/~adam/pt/

  Originally ported for use by Hamilton Jet (www.hamiltonjet.co.nz) by
  Ben Hoyt, but stripped down for public release. See his blog entry about
  it for more information:
      http://blog.brush.co.nz/2008/07/protothreads/
 * @endverbatim
 *
 * @author s3mat3
 */

#pragma once

#ifndef MULT_SL_THREAD_Hpp
# define  MULT_SL_THREAD_Hpp

# include <cstdint>
# include <list>

namespace Mult {
    /**
     * \brief Stack less thread .
     *
     * メンバ関数runをoverrideして使って
     */
    class SLThread
    {
        // Used to store a protothread's position (what Dunkels calls a
        // "local continuation").
        using line_number_type = std::uint_fast16_t;

        // An invalid line number, used to mark the protothread has ended.
        static constexpr line_number_type LineNumberInvalid = static_cast<line_number_type>(-1);

    public:
        // Construct a new protothread that will start from the beginning
        // of its Run() function.
        SLThread()
            : m_saved_line{0}
            , m_ready{false}
        {}

        // Restart protothread.
        void restart() noexcept { m_saved_line = 0; }

        // Stop the protothread from running. Happens automatically at PT_END.
        // Note: this differs from the Dunkels' original protothread behaviour
        // (his restart automatically, which is usually not what you want).
        void stop() noexcept { m_saved_line = LineNumberInvalid; }

        // Return true if the protothread is running or waiting, false if it has
        // ended or exited.
        bool isRunning() const noexcept { return m_saved_line != LineNumberInvalid; }

        bool is_ready() const noexcept {return m_ready;}
        void ready(bool s) noexcept {m_ready = s;}
        void exit() noexcept {m_ready = false;}
        // Run next part of protothread or return immediately if it's still
        // waiting. Return true if protothread is still running, false if it
        // has finished. Implement this method in your Protothread subclass.
        virtual bool run() noexcept = 0;
    protected:
        // Stores the protothread's position (by storing the line number of
        // the last PT_WAIT, which is then switched on at the next Run).
        line_number_type m_saved_line;
    private:
        bool m_ready;
    }; //<-- class SLThread ends here.

//! Declare start of protothread (use at start of Run() implementation).
# define SLT_BEGIN() bool slt_yielded = true; (void) slt_yielded; switch (m_saved_line) { case 0:

//! Stop protothread and end it (use at end of Run() implementation).
# define SLT_END() default: ; } stop(); return false;

//! Cause protothread to wait until given condition is true.
# define SLT_WAIT_UNTIL(condition) \
    do { m_saved_line = __LINE__; case __LINE__: \
    if (!(condition)) return true; } while (0)

//! Cause protothread to wait while given condition is true.
# define SLT_WAIT_WHILE(condition) SLT_WAIT_UNTIL(!(condition))

//! Cause protothread to wait until given child protothread completes.
# define SLT_WAIT_THREAD(child) SLT_WAIT_WHILE((child).run())

//! Restart and spawn given child protothread and wait until it completes.
# define SLT_SPAWN(child) \
    do { (child).restart(); SLT_WAIT_THREAD(child); } while (0)

//! Restart protothread's execution at its SLT_BEGIN.
# define SLT_RESTART() do { restart(); return true; } while (0)

//! Stop and exit from protothread.
# define SLT_EXIT() do { stop(); return false; } while (0)

//! Yield protothread till next call to its Run().
# define SLT_YIELD() \
    do { slt_yielded = false; m_saved_line = __LINE__; case __LINE__: \
    if (!slt_yielded) return true; } while (0)

//! Yield protothread until given condition is true.
#define SLT_YIELD_UNTIL(condition)                                             \
  do {                                                                         \
    slt_yielded = false;                                                       \
    m_saved_line = __LINE__;                                                   \
  case __LINE__:                                                               \
    if (!slt_yielded || !(condition))                                          \
      return true;                                                             \
  } while (0)

    /** Scheduler for SLThread .
     *
     * SLThreadのスケジューラ(蛇足クラス)
     */
    class Sched {
    public:
        Sched()
            : m_flag{false}
            , m_list{}
        {}
        virtual ~Sched() = default;
        virtual int start() noexcept {m_flag = true; return 0;}
        virtual int stop() noexcept {m_flag = false; return 0;}
        virtual void entry(SLThread* thread, bool ready=true) noexcept {(ready) ? thread->ready(true) : thread->ready(false); m_list.push_back(thread);}
        virtual void go()
        {
            while(m_flag) {
                for (auto thread : m_list) {
                    if (thread && thread->is_ready()) {
                        thread->run();
                    }
                }
            }
        }
    private:
        bool m_flag;
        std::list<SLThread*> m_list;
    }; //<-- class sched ends here.
/** \class SLThread
 *
\code {.cpp}
以下に使用例を示す

using Mult;
#include <iostream>
#include <chrono>

class ExpiryTimer {
public:
    using system_clock = std::chrono::steady_clock;
    using time_point = std::chrono::time_point<system_clock>;
   void start() noexcept
   {
       m_time = system_clock::now();
   }
   bool isExpire(int msec) noexcept
   {
        bool ret = false;
        auto now = system_clock::now();
        ret = (std::chrono::milliseconds(msec) < (now - m_time));
        return ret;
    }
private:
    time_point m_time;
};

class LEDFlasher : public SLThread
{
public:
    bool run() noexcept override;
    void set(bool s)
    {
        std::string msg = (s) ? "TURN ON" : "TURN OFF";
        std::cout << msg << std::endl;
    }
private:
    ExpiryTimer _timer;
};

bool LEDFlasher::run() noexcept
{
    SLT_BEGIN();
    while (true) {
        set(true);
        _timer.start();
        SLT_WAIT_UNTIL(_timer.isExpire(250));

        set(false);
        _timer.start();
        SLT_WAIT_UNTIL(_timer.isExpire(750));
    }

    SLT_END();
}

int main()
{
    LEDFlasher led;
    Sched sched;
    sched.entry(&led, true);
    sched.start();
    sched.go();
    return 0;
}

\endcode
 */
/** @} */
} //<-- namespace Mult ends here.

#endif //<-- macro  MULT_SL_THREAD_Hpp ends here.
