/**
 * @file event.hpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief
 *
 * @author s3mat3
 */

#pragma once

#ifndef MULT_EVENT_Hpp
# define  MULT_EVENT_Hpp

# include <mutex>
# include "code.hpp"
# include "internal/logger.hpp"
namespace Mult {
    /**  Event class.
     *
     * ステートマシンFsmを駆動するイベントで各実装に合せ導出する。
     * イベントコードはcodeヘルパクラスより生成する。baseでvoid_event=INT32_MIN,begin=-10,end=-1,stay=0が定義されているので
     * 導出クラスでは1以上で定義する事
     * \warning Eventのread/writeはmutexでロックされる
     * \warning イベントコードは登録済のEvent::void_event,stay,begin,end(0以下)があり、拡張する場合1以上のid_tで実装する事
     */
    class Event {
    public:
        using id_t = std::int32_t;

        static constexpr code<id_t> void_event{INT32_MIN}; //!< 無効イベント
        static constexpr code<id_t> begin{-10};            //!< スタート疑似状態からinitialStateへ
        static constexpr code<id_t> end{-1};               //!< 終了疑似状態へ
        static constexpr code<id_t> stay{0};               //!< 特殊イベント(do activityを繰替えす)

        Event()
            : m_current(Event::void_event)
            , m_guard()
        {
            MULTDebug("Event base construct");
        }

        virtual ~Event()
        {
            MULTDebug("~Event() process start");
            std::unique_lock<std::mutex> checker(m_guard, std::defer_lock);
            if (checker) {
                m_guard.unlock();
            }
            MULTDebug("~Event and unlock");
        }

        /** Event writer with lock.
         *
         *  \param[in] newEvent new event code
         */
        virtual void write(id_t newEvent) noexcept
        {
            MULTDebug("> Write new event");
            try {
                //std::lock_guard<std::mutex> lock(m_guard);
                m_current = newEvent;
                MULTDebug("> Write done!!!!!!");
            } catch (...) {
                MULTDebug("> Detect lock exception");
            }
        }

        /** Event writer.
         *
         * Assign operator version.
         *  \param[in] newEvent new event code
         */
        void operator= (id_t newEvent) noexcept {write(newEvent);}
        /** Event reader with lock.
         *
         *  \retval event id code
         */
        virtual id_t read() const noexcept
        {
            std::lock_guard<std::mutex> lock(m_guard);
            return m_current;
        }
        /** Functor for event reader .
         *
         *
         */
        id_t operator() () const noexcept {return read();}
        /** Current event getter .
         *
         *  \retval event id code
         */
        virtual id_t current() const noexcept {return m_current;}
        /** For debug .
         *
         * Event code to string
         */
        virtual const std::string to_string() { return std::to_string(m_current); }
    protected:
    private:
        id_t                m_current; //!< now event code
        mutable std::mutex  m_guard;   //!< guard
    };

} //<-- namespace Mult ends here.

#endif //<-- macro  MULT_EVENT_Hpp ends here.
