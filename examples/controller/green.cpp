/**
 * @file green.cpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief Green stte
 *
 * @author s3mat3
 */

#include "signal_controller.hpp"
#include "states.hpp"

Green::Green(event_type id, const std::string& name, context_ptr ptr)
    : state_type(id, name, ptr)
    , m_waiter() {}
Green::~Green() { m_waiter.cancel(); }

void Green::entry() noexcept
{
    m_context->turn(Mult::ON(), context_type::GreenBuble);
}
void Green::doActivity() noexcept
{
    try {
        MULT_LOG("Green on");
        m_waiter.wait_for(3000); //blocked wait
        MULT_LOG("Green off");
    } catch (const Mult::canceled_wait_event& e) {
        m_context->update(SignalEvent::timeout);
        MULT_INFO("==> Canceled in " + std::string(__PRETTY_FUNCTION__));
    } catch (...) {
        MULT_FATAL("====> What happen!! " + std::string(__PRETTY_FUNCTION__));
    }
    m_context->update(SignalEvent::timeout);
}

SignalMachine::state_ptr Green::exit(event_type id) noexcept
{
    m_context->turn(Mult::OFF(), context_type::GreenBuble);
    return next(id);
}
