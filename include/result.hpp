/**
 * @file result.hpp
 *
 * @copyright © 2023 s3mat3
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief Result type for function
 *
 * @author s3mat3
 */
#pragma once

#ifndef MULT_RESULT_Hpp
# define  MULT_RESULT_Hpp

# include <type_traits>
# include "mult.hpp"
# if 0
# define MULT_TRACE 1
#endif
# include "debug.hpp"

namespace Mult {
    class bad_result_access : public std::exception {
    public:
        explicit bad_result_access(std::string r) : m_reason(std::move(r)) {}
        virtual auto what() const noexcept(true) -> const char* override
        {
            return m_reason.c_str();
        }
    private:
        std::string m_reason;
    };

    using return_code = std::int64_t;
    struct error_type final
    {
        constexpr error_type() = default;
        constexpr explicit error_type(return_code e) : m_error(e) {}
        constexpr explicit error_type(const error_type&) = default;
        constexpr explicit error_type(error_type&&) = default;
        constexpr error_type& operator=(const error_type& rhs) = default;
        constexpr error_type& operator=(const error_type&& rhs) noexcept {if (this != &rhs) {m_error = rhs.m_error;} return *this;}
        ~error_type() = default;
        constexpr auto code() const noexcept ->return_code {return m_error;}
        constexpr auto operator()() const noexcept -> return_code {return m_error;}
        return_code m_error {-1};
    }; //<-- struct error_type ends here.

    template <typename T>
    using is_result_requirement = std::conjunction<
        std::is_copy_constructible<T>
        , std::is_move_constructible<T>
        , std::negation<std::is_reference<T>>
        , std::negation<std::is_pointer<T>>
        , std::negation<std::is_same<error_type, T>>
        >;
    /** Result class .
     *
     * This class can propagate value_type or error_type to caller as appropriate.
     *
     *  \tparam T value_type has constraint, that is movable, copyable, not error_type, not pointer and not reference.
     */
    template <typename T>
    class Result
    {
        static_assert(is_result_requirement<T>::value, "Necessary the typename T can be copy and move constructible and Not pointer and reference");
        using value_type = T;
        using reference = Result<value_type>&;
        using const_reference = const Result<value_type>&;
        using rvalue_reference = Result<value_type>&&;
    public:
        /** constructor 1 .
         *
         * No effect...
         */
        constexpr Result() = default;
        /** constructor 2 .
         *
         * for create result value from value_type reference
         */
        constexpr explicit Result(const value_type& v) : m_has_value(true) {construct_value(v);}
        /** constructor 3 .
         *
         * for create result value from value_type rvalue reference
         */
        constexpr explicit Result(value_type&& v) : m_has_value(true) {construct_value(v);}
        /** constructor 4 .
         *
         * for create result value with the result object constructing
         */
        template <class... Args> constexpr Result(Args... args) : m_has_value(true) {construct_value(args ...);}
        // error_type result
        /** constructor 5 .
         *
         * for create error value from error_type reference
         */
        constexpr explicit Result(const error_type& e) : m_has_value(false) {construct_error(e);}
        /** constructor 6 .
         *
         * for create error value from error_type rvalue reference
         */
        constexpr explicit Result(error_type&& e) : m_has_value(false) {construct_error(e);}
        // /** constructor 7 .
        //  *
        //  * for create result value with the result object constructing
        //  */
        // template <class Arg> constexpr Result(Arg arg) : m_has_value(false) {construct_error(arg);}
        // copy move delete
        //Result(const_reference) = delete;
        //Result(rvalue_reference) = delete;
        // reference operator=(const_reference) = delete;
        // reference operator=(rvalue_reference) = delete;
        ~Result()
        {
            if constexpr (! std::is_trivially_destructible_v<value_type>) {
                // if !m_has_value maybe not constructed
                if (m_has_value) m_value.~T();
            }
            // error_type class allways trivially desutructible
            static_assert(std::is_trivially_destructible_v<error_type>, "All way true!!");
        }
        // accssessors
        /**  resut value getter.
         *
         * if has_value is true
         *  \retval value in value_type (T)
         */
        constexpr auto value() const noexcept -> value_type {return this->m_value;}
        /** error Result getter .
         *
         * if !has_value
         *  \retval error_type
         * if you need value, use error_type::code() function
         */
        constexpr auto error() const noexcept {return this->m_error();}
        /** Check for existence of value.
         *
         *  \retval ture value existence
         *  \retval false error existence
         */
        constexpr auto has_value() const noexcept -> bool {return m_has_value;}
        /**  Check for existance of value.
         *
         *
         */
        constexpr operator bool() const noexcept {return m_has_value;}
    private:
        //////////////////////////////////////////////
        // helper value_type / error_type constructors
        //////////////////////////////////////////////
        // value
        // direct construct
        template <class... Args>
        constexpr auto construct_value(Args&&... args) noexcept -> void {new (std::addressof(m_value)) T(std::forward<Args>(args)...);}
        // copy construct
        constexpr auto construct_value(const T& v) noexcept -> void {new (std::addressof(m_value)) T(v);}
        // move construct
        constexpr auto construct_value(T&& v) noexcept -> void {new (std::addressof(m_value)) T(std::move(v));}
        // error
        // // direct construct
        // template <class Arg>
        // constexpr auto construct_error(Arg&& arg) noexcept -> void {new (std::addressof(m_error)) error_type(std::forward<Arg>(arg));}
        // copy construct
        constexpr auto construct_error(const error_type& v) noexcept -> void {new (std::addressof(m_error)) error_type(v);}
        // move construct
        constexpr auto construct_error(error_type&& v) noexcept -> void {new (std::addressof(m_error)) error_type(std::move(v));}
    private:
        bool m_has_value       = false;
        union {
            bool         dummy = false;
            value_type m_value;
            error_type m_error;
        };
    }; //<-- class Result ends here.
} //<-- namespace Mult ends here.

    // using error_code = return_code;
    // inline constexpr error_code to_error(return_code i) {return static_cast<error_code>(i);}
    // /**  .
    //  *
    //  *
    //  */
    // template <typename T, typename E>
    // union result_storage {
    //     // struct dummy {};
    //     T m_value;
    //     E m_error;
    //     constexpr result_storage(const T& value) : m_value(value) {}
    //     constexpr result_storage(const E& error) : m_error(error) {}
    //     ~result_storage() {}
    // };
    // /**  result base class.
    //  *
    //  *  \tparam T any class/type default_constructible, copy_constructible, move_constructible
    //  *  \tparam E any class/type default_constructible, copy_constructible, move_constructible
    //  */
    // template<typename T, typename E>
    // class result_base
    // {
    //     static_assert(std::is_trivially_destructible<E>::value == true, "Non constructible, necessary the class T can be constructible");
    //     using storage = result_storage<T, E>;
    // public:
    //     bool    m_has_value;
    //     storage m_storage;
    // public:
    //     constexpr result_base()
    //         : m_has_value(false)
    //         , m_storage()
    //     {}
    //     constexpr result_base(const T& value)
    //         : m_has_value(true)
    //         , m_storage(value)
    //     {}
    //     constexpr result_base(const E& error)
    //         : m_has_value(false)
    //         , m_storage(error)
    //     {}
    //     ~result_base()
    //     {
    //         if (m_has_value) {
    //             m_storage.m_value.~T(); // call non trivial destructor
    //         }
    //     }
    // };
    // /**  .
    //  *
    //  *
    //  */
    // template<typename T, typename E>
    // class destructible_result_base
    // {
    //     static_assert(std::is_trivially_destructible<E>::value == true, "Non constructible, necessary the class T can be constructible");
    //     using storage = result_storage<T, E>;
    // public:
    //     bool    m_has_value;
    //     storage m_storage;
    // public:
    //     constexpr destructible_result_base()
    //         : m_has_value(false)
    //         , m_storage()
    //     {}
    //     constexpr destructible_result_base(const T& value)
    //         : m_has_value(true)
    //         , m_storage(value)
    //     {}
    //     constexpr destructible_result_base(const E& error)
    //         : m_has_value(false)
    //         , m_storage(error)
    //     {}
    //     ~destructible_result_base() = default;
    // };
    // /**  result base class selector.
    //  *
    //  *  When T is trivially_destructible, selected_result_base select destructible_result_base another case select result_base.
    //  */
    // template <typename T, typename E>
    // using selected_result_base = std::conditional_t<
    //     std::is_trivially_destructible<T>::value,
    //     destructible_result_base<T, E>, // value == true, T is trivially_destructible
    //     result_base<T, E>               // value == false, T is not trivially_destructible
    //     >;
    // /**  Result class.
    //  *
    //  *  \tparam T any class/type default_constructible, copy_constructible, move_constructible
    //  *  \tparam E any class/type trivially_destructible, default_constructible, copy_constructible, move_constructible
    //  *  \note private inherit
    //  */
    // template <typename T, typename E>
    // class Result : selected_result_base<T, E>
    // {
    //     static_assert(std::is_default_constructible<T>::value  == true, "Non constructible, necessary the class T can be constructible");
    //     static_assert(std::is_copy_constructible<T>::value     == true, "Non copy constructible, necessary the class T can be copy constructible");
    //     static_assert(std::is_move_constructible<T>::value     == true, "Non move constructible, necessary the class T can be move constructible");
    //     static_assert(std::is_trivially_destructible<E>::value == true, "Non trivially destructible, necessary the class E can be trivially destrucible");
    //     static_assert(std::is_default_constructible<E>::value  == true, "Non constructible, necessary the class E can be constructible");
    //     static_assert(std::is_copy_constructible<E>::value     == true, "Non copy constructible, necessary the class E can be copy constructible");
    //     static_assert(std::is_move_constructible<E>::value     == true, "Non move constructible, necessary the class E can be move constructible");
    // public:
    //     // success
    //     constexpr Result(const T& v)
    //         : selected_result_base<T, E>(v)
    //     {}
    //     // Result(const Result& rhs) noexcept = default;
    //     // Result(Result&& rhs) noexcept = default;
    //     constexpr Result(const E& e) noexcept(true)
    //         : selected_result_base<T, E>(e)
    //     {}
    //     operator bool() const noexcept(true) {return this->m_has_value;}
    //     constexpr bool has_value() const noexcept(true) {return this->m_has_value;}
    //     const T& value() const noexcept(false)
    //     {
    //         if (this->m_has_value) return selected_result_base<T, E>::m_storage.m_value;
    //         else throw "invalid operation";
    //     }
    //     const E& error() const noexcept(false)
    //     {
    //         if (! this->m_has_value) return selected_result_base<T, E>::m_storage.m_error;
    //         else throw "invalid operation";
    //     }
    // };
    // /** Specialized Result<T, error_code> .
    //  *
    //  * It's named result.
    //  */
    // template <typename T>
    // using result_ = Result<T, error_code>;
    // /** ok helper function for specialized result.
    //  *
    //  *  Only template type name E is fixed erro_code
    //  *  \tparam T
    //  */
    // template <typename T>
    // result_<T> ok(const T& t) {return result_<T>(t);}
    // /**  .
    //  * for type hint error() function
    //  *
    //  */
    // template <typename Ex>
    // struct error_value {
    //     explicit error_value(Ex e): e(e) {}
    //     template <typename T, typename E>
    //     operator Result<T, E> () const;
    // private:
    //     Ex e;
    // };

    // template <typename Ex>
    // template <typename T, typename E>
    // error_value<Ex>::operator Result<T, E>() const {return Result<T, E>(e);}
    // /** error helper function .
    //  *
    //  *
    //  */
    // template <typename T>
    // error_value<T> error(T t) {return error_value<T>(t);}


#endif //<-- macro  MULT_RESULT_Hpp ends here.
