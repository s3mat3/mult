/**
 * @file signal_controller.hpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief Signal controller
 *
 * @author s3mat3
 */

#pragma once

#ifndef SIGNAL_CONTROLLER_Hpp
# define  SIGNAL_CONTROLLER_Hpp

# include <memory>
# include "debug.hpp"
# include "event.hpp"
# include "mult.hpp"

class SignalMachine;
class SignalEvent;

class TrafficSignalController {
public:
    using state_machine_ptr = std::shared_ptr<SignalMachine>;
    using event_type        = SignalEvent;
    using event_ptr         = std::shared_ptr<event_type>;
    enum Device {
        RedBuble,
        YelloBuble,
        GreenBuble
    };
    TrafficSignalController();
    ~TrafficSignalController();
    void turn(Mult::LogicCode l, Device d);
    bool update(Mult::Event::id_t event);

    void dispatch([[maybe_unused]]void* ptr);
private:
    state_machine_ptr m_ptr;
    int               m_event;
}; //<-- class TrafficSignalController ends here.



#endif //<-- macro  SIGNAL_CONTROLLER_Hpp ends here.
