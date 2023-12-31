/**
 * @file yellow.cpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief Yellow state
 *
 * @author s3mat3
 */

#include "signal_controller.hpp"
#include "states.hpp"

Yellow::Yellow(event_type id, const std::string& name, context_ptr ptr)
    : state_type(id, name, ptr)
    , m_waiter()
{
    MULT_LOG("****** Yellow() ******");
}
Yellow::~Yellow()
{
    m_waiter.cancel();
    MULT_LOG("Destruct yellow state");
}

void Yellow::entry() noexcept
{
    m_context->turn(Mult::ON(), context_type::YelloBuble);
}

void Yellow::doActivity() noexcept
{
    try {
        MULT_LOG("Yellow on");
        m_waiter.wait_for(2000); //blocked wait
        MULT_LOG("Yellow off");
    } catch (const Mult::canceled_wait_event& e) {
        m_context->update(SignalEvent::timeout);
        MULT_INFO("==> Canceled in " + std::string(__PRETTY_FUNCTION__));
    } catch (...) {
        MULT_FATAL("====> What happen!! " + std::string(__PRETTY_FUNCTION__));
    }
    m_context->update(SignalEvent::timeout);
}

SignalMachine::state_ptr Yellow::exit(event_type id) noexcept
{
    m_context->turn(Mult::OFF(), context_type::YelloBuble);
    return next(id);
}
