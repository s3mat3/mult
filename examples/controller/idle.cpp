/**
 * @file idle.cpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief Idle state
 *
 * @author s3mat3
 */

#include "signal_controller.hpp"
#include "states.hpp"

Idle::Idle(event_type id, const std::string& name, context_ptr ptr)
                : state_type(id, name, ptr)
{
    MULT_LOG("****** Idle() ******");
}

Idle::~Idle()
{
    MULT_LOG("Destruct Idle");
}

void Idle::entry() noexcept
{
    m_context->turn(Mult::OFF(), context_type::RedBuble);
    m_context->turn(Mult::OFF(), context_type::YelloBuble);
    m_context->turn(Mult::OFF(), context_type::GreenBuble);
}
void Idle::doActivity() noexcept
{
    MULT_LOG("***** Idle::do *****");
}
SignalMachine::state_ptr Idle::exit(SignalMachine::event_type id) noexcept
{
    return next(id);
}


