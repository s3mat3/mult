/**
 * @file thread.hpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief Thread helpers
 *
 * @author s3mat3
 */

#pragma once

#ifndef MULT_THREAD_Hpp
# define  MULT_THREAD_Hpp

# include <memory>
# include <thread>

# include "base.hpp"
# include "internal/logger.hpp"

namespace Mult{
    /**  Runnable Interface class.
     *
     * スレッドのエントリーポイント
     */
    class Runnable
    {
    public:
        virtual ~Runnable() {}
        virtual void run(void*) noexcept = 0;
        virtual void stop(void) noexcept = 0;
    }; //<-- class Runnable ends here.

    /**  Runnableのアダプタ.
     *
     * アタッチの条件はクラスメンバ関数にvoid stop()が実装されている必要が有る。又、run(VP)で実行されるentrypoint_tに該当するメンバ関数が実装されている必要が有る。
     * このクラスにアタッチされた任意のクラスのfunctionをThreadクラスが別スレッドとして起動する。
     */
    template <class C>
    class RunnableAdapter : public Runnable
    {
    public:
        // typedef void (C::*entrypoint_t)(void*);

        using entrypoint_t = void (C::*)(void_ptr);
        using instance_p = std::shared_ptr<C>;

        explicit RunnableAdapter(instance_p target, entrypoint_t func) noexcept
            : m_instance(std::move(target))
            , m_entrypoint(func)
        {}
        RunnableAdapter() : RunnableAdapter(nullptr, nullptr) {}

        RunnableAdapter(const RunnableAdapter&) = delete;

        RunnableAdapter(RunnableAdapter&& rhs) noexcept
            : RunnableAdapter(std::move(rhs.m_instance), rhs.m_entrypoint) {}

        virtual ~RunnableAdapter() = default;

        virtual void run(void_ptr vp) noexcept override
        {
            if (m_instance && m_entrypoint) {
                (*m_instance.*m_entrypoint)(vp);
            } else {
                MULTFatal("=====> Hasn't runnable entrypoint object");
            }
        }

        virtual void stop() noexcept override
        {
            m_instance->stop();
            MULTDebug("stoped");
        }

        void attach(instance_p obj, entrypoint_t entrypoint)
        {
            m_instance   = obj;
            m_entrypoint = entrypoint;
        }

        void operator ()(instance_p obj, entrypoint_t function)
        {
            attach(obj, function);
        }
        Runnable& operator =(RunnableAdapter&& rhs)
        {
            if (this != rhs) {
                m_entrypoint = rhs.m_entrypoint;
                m_instance   = std::move(rhs.m_instance);
            }
            return *this;
        }
        Runnable& operator =(const RunnableAdapter& o)
        {
            if (this != &o) {
                attach(o.m_instance, o.m_function);
            }
            return *this;
        }
    private:
        instance_p   m_instance;    //!< target class instance
        entrypoint_t m_entrypoint;  //!< adapted runner
    }; //<-- class RunnableAdapter ends here.

    /**  \class Thread
     * \brief Thread class for evry class
     *
     * このクラスは導出不可
     * 単純にあるクラスのvoid name-function(void_ptr)なメンバ関数を簡単にスレッド空間へ移行させる為のヘルパクラスに過ぎない為
     * 高機能なThread classは勝手に作って頂戴
     * 
     */
    class Thread final : public Base
    {
    public:
        using runnable_p = std::shared_ptr<Runnable>;
        using runnable_w = std::weak_ptr<Runnable>;
        using thread_u   = std::unique_ptr<std::thread>;

        Thread(runnable_p runnable, ID_t id, const std::string& name) noexcept
            : Base{id, name}
            , m_runnable{runnable}
            , m_thread{nullptr}
            , m_started{false} {}
        explicit Thread(runnable_p runnable, const std::string& name) noexcept : Thread(runnable, 0, name) {}
        explicit Thread(runnable_p runnable) noexcept : Thread(runnable, "some thread") {}
        Thread() noexcept : Thread(nullptr) {}

        Thread(const Thread&) = delete;

        Thread& operator =(const Thread&) = delete;

        ~Thread() noexcept
        {
            auto c = this->join();
            MULTDebug(name() + " Thread object deleting : " + std::to_string(c));
            m_thread.reset();
            MULTDebug(name() + "::~Thread");
        }
        /*! start threading
         *
         *  runnnable runner go to other thread space
         *  to move another memory context
         *
         *  \param[inout] vp thread argument(s)
         *  \retval OK thread lunched
         *  \retval NO_RESOURCE We have not the RUNNABLE
         */
        auto start(void* vp) noexcept
        {
            return_code ret = NO_RESOURCE;
            if (m_runnable) {
                m_started = true;
                MULTDebug(name() + " start thread");
                m_thread.reset(new std::thread([this,vp]{m_runnable->run(vp);}));
                return OK;
            } else {
                MULTFatal("=====> No setup Runnable object < " + name());
                return ret; // NO_RESOURCE
            }
            return ret; // but naver here.
        }
        /*! set runnable for new thread
         *
         *  \param[in] r for threading target
         */
        void runnable(runnable_p r) noexcept {m_runnable = r;}
        /*! set runnable for new thread
         *
         *  \param[in] r for threading target
         */
        void operator ()(runnable_p r) noexcept {m_runnable = r;}
        /** 有効なrunnableを保持しているか .
         *
         *  \retval true leagal runnable
         *  \retval false hasn't runnable
         */
        operator bool() const noexcept {return (m_runnable) ? true : false;}
        /*! wait for join terminate thread
         *
         *  \retval OK joined
         *  \retval FAILURE maybe already joined
         *  \retval FAIL_JOIN when catch std::system_error or other
         */
        auto join() noexcept -> decltype(Mult::OK)
        {
            if (m_thread->joinable() && m_started) {
                try {
                    m_started = false;
                    m_thread->join();
                } catch (std::system_error& e) {
                    MULTFatal(name() + " =====> fail join : " + e.what());
                    return FAIL_JOIN;
                } catch (...) {
                    MULTFatal(name() + " =====> fail join : Catch unknown EXCEPTION");
                    return FAIL_JOIN;
                }
                MULTInfo(name() + " => Joined thread");
                return OK;
            } else {
                MULTDebug(name() + " is No joinable maybe alrady joined OR **Not RUN**" );
                return FAILURE;
            }
            return OK; // maybe never here.
        }
        /*! started thread checker
         *
         *  \retval true The thead started
         *  \retval false The thread was joined or Not start
         */
        auto started() const noexcept -> bool {return m_started;}
        /*! sleep current thread
         *
         *  not thread class started thread
         *  only just call current thread
         *  制限は2^31ミリ秒が最大値
         *  \param[in] milliseconds in milliseconds unit ([ms])
         */
        static inline void sleep(millisec_interval milliseconds)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
        }

        /*! sleep current thread
         *
         *  not thread class started thread
         *  only just call current thread
         *  制限は2^31マイクロ秒が最大値
         *  \param[in] microseconds in microseconds unit ([us])
         */
        static inline void usleep(time_interval microseconds)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
        }
        /*! yield current thread
         *
         */
        static inline void yield()
        {
            std::this_thread::yield();
        }
    protected:
        runnable_p  m_runnable; //!< real thread runner
        thread_u    m_thread;   //!< thread holder
        bool        m_started;  //!< running flag
    }; // class Thread

/*! \class Thread
 *
 * RunnableAdapterを含めた使い方を以下に例示する。
 * \code
[example]
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
            DebugLog("Hello " +  hello + std::to_string(l++));
            Mult::Thread::sleep(100);
            if (l > 20) break;
        }
    }
    void function2(Mult::void_ptr)
    {
        size_t l = 0;
        while(run) {
            std::string hello(__FUNCTION__);
            DebugLog("Fugoo " +  hello + std::to_string(l++));
            Mult::Thread::sleep(100);
            if (l > 20) break;
        }
    }
private:
    bool run;
};

int main()
{
    MULTLoggerInternalLevel(Mult::LogLevel::debug);
    MULTLoggerLevel(Mult::LogLevel::debug);
    using runnable_t = Mult::RunnableAdapter<Some>;
    using runnable_p = std::shared_ptr<runnable_t>();
    auto runner1 = std::make_shared<runnable_t>(std::make_shared<Some>(), &Some::function1);
    auto runner2 = std::make_shared<runnable_t>(std::make_shared<Some>(), &Some::function2);
    Mult::Thread th1(runner1, "TH1");
    Mult::Thread th2(runner2, "TH2");

    th1.start(nullptr);
    th2.start(nullptr);

    return 0;
}
*\endcode
*
*/

} //<-- namespace Mult ends here.

#endif //<-- macro  MULT_THREAD_Hpp ends here.
