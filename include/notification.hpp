/**
 * @file notification.hpp
 *
 * @copyright © 2023 s3mat3 
 *
 * This code is licensed under the MIT License, see the LICENSE.txt file for details
 *
 * @brief notification class for callback
 *
 * @warning You need a c++11 or higher compiler. For gcc, if it is not g++4.9 or higher, there is a mistake in the functional implementation.
 * @warning Be careful when crossing threads because resource protection is not performed!
 * @todo Register multiple callbacks using a list (Well, almost one to one, so it's less important ...)
 * @author s3mat3
 */

#pragma once

#ifndef MULT_NOTIFICATION_Hpp
# define  MULT_NOTIFICATION_Hpp

# include <functional>
# include <memory>

# include "internal/logger.hpp"

/*!
 * \def MULT_ARGC
 * \brief argument place holder
 *
 *  Argument object of bind () function system such as Mult::Notification class
 *  The macro's argument "x" is expanded to std::placeholder::_ x.
 *  "x" must be a number from 1 to the number of arguments in sequential.
 *  e.g. if there are two arguments x takes 1 and 2. Therefore, the specified macro is MULT_ARGC (1), MULT_ARGC (2)
 *  MULT_ARGC(1) => std::placeholders::_1
 *  MULT_ARGC(2) => std::placeholders::_2
 *  MULT_ARGC(N) => std::placeholders::_N (N is under 10 cause \see https://cpprefjp.github.io/reference/functional/placeholders.html#備考)
 */
# ifndef MULT_ARG
#  define MULT_ARGC(x) std::placeholders::_##x
# endif

namespace Mult {
    /*!
     *  \tparam Args Argument type of a member function (callback) that receives an event (do not write it if variable length is void)
     */
    template <typename... Args>
    /*! \class Notification
     * notification class
     *
     * This class can notify even if the delivery destination (callback) is unknown.
     * Even in this state, without crashing, display an error message and continue the program.
     * A little connect can be used as a complicated and difficult but flexible event delivery system.
     */
    class Notification
    {
    public:
        using reciver_t = std::function<int(Args...)>; //!< callback
    public:
        Notification() : cb(nullptr) {}
        /*! Connect notify destination (callback) (raw pointer version)
         *
         *  \tparam C Class to receive event (class which defined callback)
         *  \tparam T Member function (callback) that receives an event of class C
         *  \tparam ...A Argument type of a member function (callback) that receives an event (do not write it if variable length is void)
         *
         *  \param[in] obj Instance of class pointer in which connection destination is defined
         *  \param[in] mf Connection destination (callback) by member function of obj
         *  \param[in] a Arguments of member functions to be called back (variable length) Perhaps you can pass arguments at runtime by using the MULT_ARG (x) macro
         */
        template <class C, typename T, typename...A>
        void connect(C* obj, T&& mf, A... a)
        {
            cb = std::bind(mf, obj,  a...);
        }

        /*! Connect notify destination (callback) shared pointer version
         *
         *  \tparam C Class to receive event (class which defined callback)
         *  \tparam T Member function (callback) that receives an event of class C
         *  \tparam ...A Argument type of a member function (callback) that receives an event (do not write it if variable length is void)
         *
         *  \param[in] obj Instance of class pointer in which connection destination is defined
         *  \param[in] mf Connection destination (callback) by member function of obj
         *  \param[in] a Arguments of member functions to be called back (variable length) Perhaps you can pass arguments at runtime by using the MULT_ARG (x) macro
         */
        template <class C, typename T, typename...A>
        void connect(std::shared_ptr<C> obj, T&& mf, A... a)
        {
            cb = std::bind(mf, obj,  a...);
        }
        template <class C, typename T, typename...A>
        const Notification& operator()(C* obj, T&& mf, A... a)
        {
            cb = std::bind(mf, obj,  a...);
            return *this;
        }
        template <class C, typename T, typename...A>
        const Notification& operator()(std::shared_ptr<C> obj, T&& mf, A... a)
        {
            cb = std::bind(mf, obj,  a...);
            return *this;
        }
        /*! Notify (execute callback)
         *
         *  connectされたコールバックをここで呼びだす
         *
         *  \param[inout] args コールバック先の実引数で入力、出力または引数が有る、無しはコールバック先に依存する
         *  \retval コールバック先は返却値があろうがなかろうが必ずintである必要がある
         */
        int notify(Args... args)
        {
            int ret = 0;
            if (cb) { // 純粋に割り当てられているか
                try {
                    ret = cb(args...);
                } catch (std::bad_function_call& e) { // 多分ここでcatchされるとコールバックのスコープが無いって事だと思う
                    MULTFatal("=====> Occure Exception std::bad_function_call");
                } catch (...) {
                    MULTFatal("=====> Occure Exception!! What huppen???");
                }
            } else {
                MULTError("====> Notification :: No assignment receiver function(no callback)!! please setup me!!");
            }
            return ret;
        }

    private:
        reciver_t cb; //!< callback
    }; //<-- class Notification ends here.
    //
    // none member functions
    //

    /*! Notification connect helper function
     *
     *  \tparam C Class to receive event (class which defined callback)
     *  \tparam T Member function (callback) that receives an event of class C
     *  \tparam ...A Argument type of notify function of Notification class of connection source
     *  \tparam ...AA Argument type of a member function (callback) that receives an event (do not write it if variable length is void)
     *
     *  \param[in] n Notification class references of connection source (not const because cheange to source of Notification member)
     *  \param[in] obj Instance of class pointer in which connection destination is defined
     *  \param[in] mf Connection destination (callback) by member function of obj
     *  \param[in] a Arguments of member functions to be called back (variable length) Perhaps you can pass arguments at runtime by using the MULT_ARG (x) macro
     */
    template <class C, typename T, typename...A, typename...AA>
    inline void connect(Notification<A...>& n, C* obj, T&& mf, AA... a)
    {
        n.connect(obj, mf, a...);
    }
    /*! Notification connect helper function
     *
     *  \tparam C Class to receive event (class which defined callback)
     *  \tparam T Member function (callback) that receives an event of class C
     *  \tparam ...A Argument type of notify function of Notification class of connection source
     *  \tparam ...AA Argument type of a member function (callback) that receives an event (do not write it if variable length is void)
     *
     *  \param[in] n Notification class pointer of connection source
     *  \param[in] obj Instance of class pointer in which connection destination is defined
     *  \param[in] mf Connection destination (callback) by member function of obj
     *  \param[in] a Arguments of member functions to be called back (variable length) Perhaps you can pass arguments at runtime by using the MULT_ARG (x) macro
     */
    template <class C, typename T, typename...A, typename...AA>
    inline void connect( Notification<A...>* n, C* obj, T&& mf, AA... a)
    {
        n->connect(obj, mf, a...);
    }

    /*! Notification connect helper function
     *  
     *  \tparam C Class to receive event (class which defined callback)
     *  \tparam T Member function (callback) that receives an event of class C
     *  \tparam ...A Argument type of notify function of Notification class of connection source
     *  \tparam ...AA Argument type of a member function (callback) that receives an event (do not write it if variable length is void)
     *
     *  \param[in] n Notification class pointer of connection source
     *  \param[in] obj Instance of class pointer in which connection destination is defined
     *  \param[in] mf Connection destination (callback) by member function of obj
     *  \param[in] a Arguments of member functions to be called back (variable length) Perhaps you can pass arguments at runtime by using the MULT_ARG (x) macro
     */
    template <class C, typename T, typename...A, typename...AA>
    inline void connect( Notification<A...>* n, std::shared_ptr<C> obj, T&& mf, AA... a)
    {
        n->connect(obj, mf, a...);
    }

    /*! Notification connect helper function
     *  
     *  \tparam C Class to receive event (class which defined callback)
     *  \tparam T Member function (callback) that receives an event of class C
     *  \tparam ...A Argument type of notify function of Notification class of connection source
     *  \tparam ...AA Argument type of a member function (callback) that receives an event (do not write it if variable length is void)
     *
     *  \param[in] n Notification class pointer of connection source
     *  \param[in] obj Instance of class pointer in which connection destination is defined
     *  \param[in] mf Connection destination (callback) by member function of obj
     *  \param[in] a Arguments of member functions to be called back (variable length) Perhaps you can pass arguments at runtime by using the MULT_ARG (x) macro
     */
    template <class C, typename T, typename...A,typename...AA>
    inline void connect(std::shared_ptr<Notification<A...>> n, C* obj, T&& mf, AA... a)
    {
        n->connect(obj, mf, a...);
    }

    /*! Notification connect helper function
     *  
     *  \tparam C Class to receive event (class which defined callback)
     *  \tparam T Member function (callback) that receives an event of class C
     *  \tparam ...A Argument type of notify function of Notification class of connection source
     *  \tparam ...AA Argument type of a member function (callback) that receives an event (do not write it if variable length is void)
     *
     *  \param[in] n Notification class pointer of connection source
     *  \param[in] obj Instance of class pointer in which connection destination is defined
     *  \param[in] mf Connection destination (callback) by member function of obj
     *  \param[in] a Arguments of member functions to be called back (variable length) Perhaps you can pass arguments at runtime by using the MULT_ARG (x) macro
     */
    template <class C, typename T, typename...A,typename...AA>
    inline void connect(std::shared_ptr<Notification<A...>> n, std::shared_ptr<C> obj, T&& mf, AA... a)
    {
        n->connect(obj, mf, a...);
    }

/*! \class Notification 
 *
 * Usage
\code
using notification_int_t = Mult::Notification<int>;
using notification_string_t = Mult::Notification<std::string&>;

class SomeCaller
{
public:
    SomeCaller() : run(true) {}
    void stop() {run = false;}

    void function1(Mult::void_ptr)
    {
        size_t l = 0;
        while(run) {
            std::string hello(__FUNCTION__);
            hello.append(std::to_string(l++));
            DebugLog("notify " +  hello);
            aaa.notify(l);
            bbb.notify(hello);
            Mult::Thread::sleep(100);
            if (l > 20) break;
        }
    }
public:
    notification_int_t    aaa;
    notification_string_t bbb;
private:
    bool run;
};

class SomeCallee
{
public:
    SomeCallee() : run(true) {}
    void stop() {run = false;}

    void function1(Mult::void_ptr)
    {
        size_t l = 0;
        while(run) {
            std::string hello(__FUNCTION__);
            DebugLog("Hello " +  hello + std::to_string(l++));
            Mult::Thread::sleep(100);
            if (l > 20) break;
        }
    }

    int function2(int a)
    {
        std::string x = std::to_string(a) + " get from Caller";
        DebugLog(x);
        return a;
    }

    int function3(std::string a)
    {
        DebugLog(a);
        return 0;
    }
public:
private:
    bool run;
};

int main()
{
    MULTLoggerInternalLevel(Mult::LogLevel::debug);
    MULTLoggerLevel(Mult::LogLevel::debug);
    using runnable_t = Mult::RunnableAdapter<SomeCaller>;
    using runnable_p = std::shared_ptr<runnable_t>();
    // setup caller Thread
    auto caller = std::make_shared<SomeCaller>();
    auto runner1 = std::make_shared<runnable_t>(caller, &SomeCaller::function1);
    Mult::Thread th1(runner1, "TH1");
    // setup callee object
    auto callee  = std::make_shared<SomeCallee>();
    // connect callback on 2way
    caller->aaa.connect(callee, &SomeCallee::function2, MULT_ARGC(1));
    caller->bbb(callee, &SomeCallee::function3, std::placeholders::_1);

    th1.start(nullptr);
    return 0;
}
\endcode
\verbatim
0000004237012830 (APP ) [140132747589376]  [Debug ]: notify function116 example.cpp in 29
0000004237012954 (APP ) [140132747589376]  [Debug ]: 17 get from Caller example.cpp in 63
0000004237012985 (APP ) [140132747589376]  [Debug ]: function116 example.cpp in 69
0000004237113334 (APP ) [140132747589376]  [Debug ]: notify function117 example.cpp in 29
0000004237113433 (APP ) [140132747589376]  [Debug ]: 18 get from Caller example.cpp in 63
0000004237113463 (APP ) [140132747589376]  [Debug ]: function117 example.cpp in 69
0000004237213620 (APP ) [140132747589376]  [Debug ]: notify function118 example.cpp in 29
0000004237213746 (APP ) [140132747589376]  [Debug ]: 19 get from Caller example.cpp in 63
0000004237213808 (APP ) [140132747589376]  [Debug ]: function118 example.cpp in 
\endverbatim
*/


} //<-- namespace Mult ends here.

#endif //<-- macro  MULT_NOTIFICATION_Hpp ends here.
