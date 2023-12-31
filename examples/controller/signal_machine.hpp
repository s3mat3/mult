/**
 * @file signal_machine.hpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief Signal state machine
 *
 * @author s3mat3
 */

#pragma once

#ifndef SIGNAL_MACHINE_Hpp
# define  SIGNAL_MACHINE_Hpp

# include "event.hpp"
# include "state_machine.hpp"

class TrafficSignalController;

class SignalEvent : public Mult::Event
{
public:
    using name_tbl = std::unordered_map<id_t, std::string>;
    static constexpr Mult::code<id_t> start  {1};
    static constexpr Mult::code<id_t> stop   {2};
    static constexpr Mult::code<id_t> timeout{3};

    SignalEvent()
        : Event()
    {
        m_tbl[SignalEvent::end]     = "terminate";
        m_tbl[SignalEvent::stay]    = "stay";
        m_tbl[SignalEvent::begin]   = "begin";
        m_tbl[SignalEvent::start]   = "start";
        m_tbl[SignalEvent::stop]    = "stop";
        m_tbl[SignalEvent::timeout] = "change";
        MULT_LOG("SignalEvent()");
    }
    virtual ~SignalEvent() = default;
    const std::string to_string() override
    {
        auto id = current();
        std::string ret(" ");
        try {
            ret.append(m_tbl[id]).append(" : ").append(std::to_string(id));
        } catch(...) {
            ret = std::to_string(id).append(" is undefined");
        }
        return ret;
    }
private:
    name_tbl m_tbl;
};

class TrafficSignalController;

class SignalMachine : public  Mult::state_machine<TrafficSignalController>
{
    static_assert(std::is_same<SignalMachine::state_type, Mult::state_base<TrafficSignalController>>::value == true, "Hoge=====");
public:
    SignalMachine(context_ptr ptr);
    virtual ~SignalMachine() = default;
    state_type m_aaa;
private:
    state_ptr idle;
    state_ptr red;
    state_ptr yellow;
    state_ptr green;
};


#endif //<-- macro  SIGNAL_MACHINE_Hpp ends here.
