/**
 * @file signal.hpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief Signal sender/reciver in blocked
 *
 * @author s3mat3
 */

#pragma once

#ifndef MULT_SIGNAL_Hpp
# define  MULT_SIGNAL_Hpp

# include <chrono>
# include <condition_variable>
# include <exception>
# include <mutex>

# include "mult.hpp"

namespace Mult {
    /** signal wait cancel exception .
     *
     *
     */
    struct canceled_wait_event : std::exception {};

    /** Signal .
     *  \class Signal
     *
     *  ブロッキングタイプのシグナル配送を行う。updaterとreader2つのスレッドを想定している
     *
     */
    class Signal {
        using signal_id = Mult::return_code;
        using lock_type = std::mutex;
        using locker    = std::unique_lock<lock_type>;
        using cv        = std::condition_variable;
        static constexpr signal_id TIMEOUT = Mult::TIMEOUT;
    public:
        /** Constructor 1 .
         *
         * nessesary default constructable SignalCode as m_id
         */
        Signal()
            : m_updated(false)
            , m_canceled(false)
            , m_id(Mult::OK)
            , m_guard()
            , m_monitor()
        {}
        ~Signal() = default;
        /**  Update new signal code.
         *
         *
         */
        void update(const signal_id& x)
        {
            locker guard(m_guard);
            m_updated = true;
            m_id = x;
            m_monitor.notify_all();
        }
        /** Wait update .
         *
         *
         */
        signal_id wait_update() noexcept(false)
        {
            locker guard(m_guard);
            m_monitor.wait(guard, [&] {return m_updated || m_canceled;});
            if (m_canceled) {
                m_canceled = false;
                throw canceled_wait_event();
            }
            if (m_updated) m_updated = false;
            return m_id;
        }
        /** Wait update with timeout.
         *
         *
         */
        signal_id wait_for(millisec_interval tout) noexcept(false)
        {
            locker guard(m_guard);
            auto ret = m_monitor.wait_for(guard
                                       , std::chrono::milliseconds(tout)
                                       , [&] {return m_updated || m_canceled;});
            if (m_canceled) {
                m_canceled = false;
                throw canceled_wait_event();
            }
            if (m_updated) m_updated = false;
            if (! ret) return TIMEOUT;
            return m_id;
        }
        void clear()
        {
            locker guard(m_guard);
            m_updated  = false;
            m_canceled = false;
        }
        /** cancel for wait .
         *
         *
         */
        void cancel()
        {
            locker guard(m_guard);
            m_canceled = true;
            m_monitor.notify_all();
        }
    private:
        bool      m_updated;  //!< updated flag true: update occurred, false: no update
        bool      m_canceled; //!< cancel flag true: cancel occurrerd, false: no action
        signal_id m_id;       //!< signal id
        lock_type m_guard;    //!< resource guard
        cv        m_monitor;  //!< monitor with condition variable
    }; //<-- class Signal ends here.

    /** \class Signal
     *\code
     * 使用例
     *    #include <iostream>
     *    #include "Signal.hpp"
     *    #include "Thread.hpp"
     *    const int N = 100;
     *
     *    int main()
     *    {
     *        Mult::Signal s;
     *        std::thread th1([&]{
     *            try {
     *                for (int i = 1; i <= N; ++i) {
     *                    s.update(i + 10000);
     *                    Mult::Thread::sleep(100);
     *                }
     *                s.cancel();
     *            } catch (canceled_wait_event&) {
     *                std::cout << "Canceled by Thread 2" << std::endl;
     *            }
     *        });
     *        std::thread th2([&]{
     *            try {
     *                while (1) {
     *                    std::cout << s.wait_update() << std::endl;
     *                }
     *            } catch (canceled_wait_event&) {
     *                s.cancel();
     *                std::cout << "canceled by Thread 1" << std::endl;
     *            }
     *        });
     *        Mult::Thread::sleep(100);
     *        //s.cancel();
     *        th1.join();
     *        th2.join();
     *        return 0;
     *    }
     *\endcode
     */
} //<-- namespace Mult ends here.

#endif //<-- macro  MULT_SIGNAL_Hpp ends here.
