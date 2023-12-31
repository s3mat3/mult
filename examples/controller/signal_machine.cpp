/**
 * @file signal_machine.cpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief Signal state machine
 *
 * @author s3mat3
 */

#include "signal_controller.hpp"
#include "states.hpp"

SignalMachine::SignalMachine(context_ptr ptr)
    : self_type()
    , idle(std::make_shared<Idle>     (1, "Idle",  ptr))
    , red(std::make_shared<Red>       (2, "red",   ptr))
    , yellow(std::make_shared<Yellow> (3, "yello", ptr))
    , green(std::make_shared<Green>   (4, "green", ptr))
{
    MULT_LOG("StateMachine()");
    // addTransition(idle,  SignalEvent::start,   red);
    // addTransition(red,   SignalEvent::timeout, green);
    // addTransition(red,   SignalEvent::stop,    idle);
    // addTransition(green, SignalEvent::timeout, yellow);
    // addTransition(green, SignalEvent::stop,    idle);
    // addTransition(yellow, SignalEvent::timeout, red);
    // addTransition(yellow, SignalEvent::stop,    idle);

    addTransition(idle,   0, idle);
    addTransition(idle,   1, red);
    addTransition(idle,   2, idle);
    addTransition(idle,   4, m_pseudo_stop);
    addTransition(red,    3, green);
    addTransition(red,    2, idle);
    addTransition(green,  3, yellow);
    addTransition(green,  2, idle);
    addTransition(yellow, 3, red);
    addTransition(yellow, 2, idle);
    initial(idle);
    MULT_LOG("current State name is " + name());
    MULT_LOG("Done constructor StateMachine()");
}
