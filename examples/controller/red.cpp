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
    MULT_LOG("****** Red() ******");
}

Red::~Red()
{
    m_waiter.cancel();
    MULT_LOG("Destruct Red");
}
void Red::entry() noexcept
{
    m_context->turn(Mult::ON(), context_type::RedBuble);
}

void Red::doActivity() noexcept
{
    try {
        MULT_LOG("Red on");
        m_waiter.wait_for(3000); //blocked wait
        MULT_LOG("Red off");
    } catch (const Mult::canceled_wait_event& e) {
        m_context->update(SignalEvent::timeout);
        MULT_INFO("==> Canceled in " + std::string(__PRETTY_FUNCTION__));
    } catch (...) {
        MULT_FATAL("====> What happen!! " + std::string(__PRETTY_FUNCTION__));
    }
    m_context->update(SignalEvent::timeout);
}

SignalMachine::state_ptr Red::exit(event_type id) noexcept
{
    m_context->turn(Mult::OFF(), context_type::RedBuble);
    return next(id);
}

