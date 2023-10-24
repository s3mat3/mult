/**
 * @file red.cpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief Read state
 *
 * @author s3mat3
 */

#include "signal_controller.hpp"
#include "states.hpp"

Red::Red(event_type id, const std::string& name, context_ptr ptr)
    : state_type(id, name, ptr)
    , m_waiter()
{
    DebugLog("****** Red() ******");
}

Red::~Red()
{
    m_waiter.cancel();
    DebugLog("Destruct Red");
}
void Red::entry() noexcept
{
    m_context->turn(Mult::ON(), context_type::RedBuble);
}

void Red::doActivity() noexcept
{
    try {
        DebugLog("Red on");
        auto to = m_waiter.wait_for(3000); //blocked wait
        DebugLog("Red off");
    } catch (const Mult::canceled_wait_event& e) {
        m_context->update(SignalEvent::timeout);
        InfoLog("==> Canceled in " + std::string(__PRETTY_FUNCTION__));
    } catch (...) {
        FatalLog("====> What happen!! " + std::string(__PRETTY_FUNCTION__));
    }
    m_context->update(SignalEvent::timeout);
}

SignalMachine::state_ptr Red::exit(event_type id) noexcept
{
    m_context->turn(Mult::OFF(), context_type::RedBuble);
    return next(id);
}

