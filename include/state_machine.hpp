/**
 * @file state_machine.hpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief UMLのState machine diagramを模倣してみた(ネスト(合成状態)は未考慮)
 *
 * このファイル内に2つのクラスを定義し
 * - state_base class 状態を保持するクラス
 * - state_macine class 状態の切り替えを行うクラス
 *
 *  とした
 *@startuml
 * state state_base
 *
 * state_base : entry()
 * state_base : do() (function doActivity)
 * state_base : exit()
 *@enduml
 *
 * state_machineクラスでは開始疑似状態と終了疑似状態をそれぞれ設けている
 *
 * @author s3mat3
 */
#pragma once

#ifndef MULT_STATE_MACHINE_Hpp
# define  MULT_STATE_MACHINE_Hpp
# include <unordered_map>
# include <memory>
# include "base.hpp"
# include "debug.hpp"
# include "event.hpp"

namespace Mult {
    /** State base class .
     *
     * entry, do(doActivity), exitをオーバーライドして使用する
     * 実行コンテキストを触れる様テンプレートからContextを嵌めてやる
     * このステート自体に次状態を登録(state_holder型)し、この状態を脱出する(exitする)時に次状態をStatemachineにくれてやる
     * \todo state_holder型が現在unordered_mapだがpolicyでコンテナをvectorやarrayまたはスカラに変更出来る様にしたい
     *  \tparam Context このクラスの状態を変更する
     */
    template<class Context>
    class state_base : public Base {
    public:
        using context_type = Context;
        using context_ptr  = context_type*;
        using event_type   = Event::id_t;
        using state_type   = state_base<context_type>;
        using state_ptr    = std::shared_ptr<state_type>;
        using state_wptr   = std::weak_ptr<state_type>;
        using state_holder = std::unordered_map<event_type, state_wptr>;

        state_base() : state_base(0, "", nullptr) {}
        explicit state_base(context_ptr context) : state_base(0, "", context) {}
        explicit state_base(ID_t id, const name_type& name) : state_base(id, name, nullptr) {}

        explicit state_base(ID_t id, const name_type& name, context_ptr context)
            : Base(id, name)
            , m_context(context)
            , m_destination()
        {}
        state_base(const state_base&)                  = delete;
        state_base(state_base&&)                       = delete;
        const state_base& operator=(const state_base&) = delete;
        const state_base& operator=(state_base&&)      = delete;

        virtual ~state_base() = default;

        void context(context_ptr c) noexcept {m_context = c;}
        virtual void destination(event_type id, state_wptr ptr) noexcept
        {
            m_destination[id] = ptr;
        }
        /** 次状態の取出し .
         *
         *
         */
        virtual state_ptr next(event_type id) noexcept
        {
            state_ptr ret;
            MULT_LOG("Next index is " + std::to_string(id));
            try {
                if (! m_destination.at(id).expired()) {
                    ret = m_destination.at(id).lock();
                } else {
                    MULT_FATAL("======> State pointer is Expired!!");
                    return nullptr;
                }
            } catch (const std::out_of_range& e) {
                MULT_FATAL("=====> We have not next state => " + std::string(e.what()));
            } catch (...) {
                MULT_FATAL("=====> We have not next state =====> No more info!!");
            }
            return ret;
        }
        /** 入場処理.
         *
         *
         */
        virtual void entry() noexcept {}
        /** アクティビティ .
         *
         *
         */
        virtual void doActivity() noexcept {}
        /** 退場処理.
         *
         * Stateを脱出する時に次状態を取得しstate_machineへ提供する
         */
        virtual state_ptr exit([[maybe_unused]]event_type id) noexcept {return nullptr;}
    protected:
        context_ptr  m_context;
        state_holder m_destination;
    }; //<-- class state_base ends here.
    /** State machine .
     *
     * state_base<Context>なステートの遷移器
     * state_base<Context>な現状態と一番最初に実行される状態を保持する変数の構築上
     * templateクラスにした、
     *  \tparam Context 状態遷移したいクラスでstate_baseのテンプレートパラメータと同じ必要が有る(また基本Contextクラスは全ての関数にアクセス出来る様にしておいてね)
     */
    template<class Context>
    class state_machine {
        // static_assert(std::is_same<>::value == true, "Parameter named Context is nessesary same same the parameter named Context using for state_base");
    public:
        using self_type    = state_machine<Context>; //!< state_machine<Context>
        using machine_type = state_machine<Context>; //!< ailias
        using context_type = Context;
        using context_ptr  = context_type*;
        using event_type   = Event::id_t;
        using state_type   = state_base<context_type>; 
        using state_ptr    = std::shared_ptr<state_type>;
        state_machine() : state_machine(nullptr) {}
        explicit state_machine(state_ptr initial)
            : m_current(std::make_shared<pseudo_start>(this))
            , m_initial(initial)
            , m_pseudo_stop(std::make_shared<pseudo_stop>())
        {}
        virtual ~state_machine() = default;
        void initial(state_ptr initial) noexcept {m_initial = initial;}
        auto id() const noexcept {return m_current->id();}
        auto name() const noexcept {return m_current->name();}
        /**  Event IDによる状態遷移の実行部.
         *
         *  \param[in] id is the event ID for drive state machine.
         *  \retval true is not detect terminal state (null state or pesudo STOP state)
         *  \retval false is detected terminal state (null state or pesudo STOP state)
         */
        bool onAnyEvent(event_type id) noexcept
        {
            if (m_current) {
                auto next = m_current->exit(id); // exit function
                if (next && (next != m_pseudo_stop)) {
                    MULT_LOG("**Change state** from " + m_current->name() + " to " + next->name() + " by event id = " + std::to_string(id));
                    m_current = next;
                } else {
                    MULT_INFO("=> Detect pseudo_stop or null STATE return to FALSE");
                    return false; // next is nullptr OR m_stop
                }
                m_current->entry();      // entry function
                m_current->doActivity(); // do function
            } else {
                MULT_FATAL("=====> No current state");
                return false;
            }
            return true;
        }
        /** 遷移表の構築用ヘルパ関数 .
         *
         * 遷移元のstate_holderにイベントIDと共に保管する
         *  \param[in] from 遷移元状態変数
         *  \param[in] id イベントトリガID
         *  \param[in] to 遷移先
         */
        void addTransition(state_ptr from,  event_type id, state_ptr to) const noexcept
        {
            from->destination(id, to);
        }
    protected:
        state_ptr m_current;     //!< 実行中のステート
        state_ptr m_initial;     //!< 疑似開始状態で起動される最初の状態(ステート)
        state_ptr m_pseudo_stop; //!< 終了疑似状態 (状態遷移表を構築する時必要なら利用してねって事で準備している)
    private:
        /** Pseudo start state .
         *
         * ステートマシン用の疑似開始状態
         * このステートマシンのエントリポイントとなっている
         * onAnyEventでイベント処理で最初に実行される関数はcurrent状態のexit関数である
         * この関数は最初のみの実行で、2度以上使うな、また此れが呼ばれる前に必ずconstructorまたはinitialで初期状態をセットする事
         */
        class pseudo_start : public state_type {
        public:
            explicit pseudo_start(machine_type* machine)
                : state_type(0, "pseudo-start")
                , state_machine(machine)
            {}
            state_ptr exit([[maybe_unused]]event_type id) noexcept override {return state_machine->m_initial;}
        private:
            machine_type* state_machine;
        }; //<-- class machine_type::pseudo_start ends here.
        /** Pseudo stop state .
         *
         *
         */
        class pseudo_stop : public state_type {
        public:
            pseudo_stop() : state_type(10000, "pseudo-stop") {}
            state_ptr exit([[maybe_unused]]event_type id) noexcept override {return nullptr;}
        }; //<-- class machine_type::pseudo_stop ends here.
    }; //<-- class state_machine ends here.
} //<-- namespace Mult ends here.
#endif //<-- macro  MULT_STATE_MACHINE_Hpp ends here.
