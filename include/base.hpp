/**
 * @file base.hpp
 *
 * @copyright © 2023 s3mat3 
 *
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief
 *
 * @author s3mat3
 */

#pragma once

#ifndef MULT_BASE_Hpp
# define  MULT_BASE_Hpp

# include "mult.hpp"

namespace Mult {

    /** Mult base class .
     *
     * Abstract Base class for all class
     */
    class Base
    {
    public:
        Base()
            : Base(0, "no-name") {}
        explicit Base(ID_t id)
            : Base(id, "no-name") {}
        explicit Base(const std::string& name)
            : Base(0, name) {}
        explicit Base(ID_t id, const std::string& name)
            : m_id(id)
            , m_name(name) {}
        Base(const Base& lhs)
            : Base(lhs.m_id, lhs.m_name) {}
        Base(Base&& rhs) noexcept
            : Base(rhs.m_id) {m_name.assign(std::move(rhs.m_name));}
        Base& operator=(const Base&) = delete;
        virtual ~Base() = default;

        // getter
        ID_t id() const noexcept {return m_id;}
        const std::string name() const noexcept {return m_name;}
        // setter
        void id(ID_t newID) noexcept {m_id = newID;}
        void name(const std::string& name) noexcept {m_name.assign(name);}
        void name(std::string&& name) noexcept {m_name.assign(std::move(name));}
        // manipulator for data
        void clear() noexcept
        {
            m_id = 0;
            m_name.clear();
        }
    private:
        ID_t        m_id;
        std::string m_name;
    };

} //<-- namespace Mult ends here.

#endif //<-- macro  MULT_BASE_Hpp ends here.
