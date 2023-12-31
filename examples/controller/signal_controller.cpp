/**
 * @file signal_controller.cpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief Signal controller
 *
 * @author s3mat3
 */

#include "signal_controller.hpp"
#include "signal_machine.hpp"

TrafficSignalController::TrafficSignalController()
    : m_ptr(std::make_shared<SignalMachine>(this))
    , m_event(0)
{
}

TrafficSignalController::~TrafficSignalController() {}

void TrafficSignalController::turn(Mult::LogicCode l, Device d)
{
    std::string msg;
    if (l == Mult::ON()) {
        msg.assign("Turn on **P** -> ");
    } else {
        msg.assign("Turn off zzz!! -> ");
    }
    switch (d) {
    case RedBuble: msg.append("RED");     break;
    case YelloBuble: msg.append("YELLO"); break;
    case GreenBuble: msg.append("GREEN"); break;
    default: msg.append("What???");
    }
    MULT_LOG(msg);
}

bool TrafficSignalController::update(Mult::Event::id_t event)
{
    m_event = event;
    MULT_LOG("Update to " + std::to_string(m_event));
    return true;
}

void TrafficSignalController::dispatch([[maybe_unused]]void *ptr)
{
    bool x = true;
    size_t y = 0;
    while (x) {
        if (++y == 3) update(1);
        if (y == 10) update(2);
        if (y == 12) update(4);
        MULT_LOG("----- Change STATE -----");
        x = m_ptr->onAnyEvent(m_event);
        MULT_LOG("----- Change STATE done -----" + std::to_string(y));
    }
}
