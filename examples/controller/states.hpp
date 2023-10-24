/**
 * @file state.cpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief Define signal states
 *
 * @author s3mat3
 */

#pragma once

#ifndef STATES_Hpp
# define  STATES_Hpp

# include "signal.hpp"
# include "signal_machine.hpp"

class Idle : public SignalMachine::state_type {
public:
    Idle(SignalMachine::event_type id, const std::string& name, context_ptr ptr);
    virtual ~Idle();
    void entry() noexcept override;
    void doActivity() noexcept override;
    SignalMachine::state_ptr exit(SignalMachine::event_type id) noexcept override;
private:
};

class Red : public SignalMachine::state_type {
public:
    Red(event_type id, const std::string& name, context_ptr ptr);
    virtual ~Red();
    void entry() noexcept override;
    void doActivity() noexcept override;
    state_ptr exit(event_type id) noexcept override;
private:
    Mult::Signal m_waiter;
};

class Yellow : public SignalMachine::state_type {
public:
    Yellow(event_type id, const std::string& name, context_ptr ptr);
    virtual ~Yellow();
    void entry() noexcept override;
    void doActivity() noexcept override;
    state_ptr exit(event_type id) noexcept override;
private:
    Mult::Signal m_waiter;
};


class Green : public SignalMachine::state_type {
public:
    Green(event_type id, const std::string& name, context_ptr ptr);
    virtual ~Green();
    void entry() noexcept override;
    void doActivity() noexcept override;
    state_ptr exit(event_type id) noexcept override;
private:
    Mult::Signal m_waiter;
};


#endif //<-- macro  STATES_Hpp ends here.
