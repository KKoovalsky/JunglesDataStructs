/**
 * @file	member_isr_enable.hpp
 * @brief	Metaprogramming stuff which allows to call member function from ISR handler
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 *
 * It's intended to be used when the ISR handler is called from a C function invoked from an address put into the
 * interrupt vector table.
 */

#ifndef MEMBER_ISR_ENABLE_HPP
#define MEMBER_ISR_ENABLE_HPP

#include <utility>

namespace jungles {

// --------------------------------------------------------------------------------------------------------------------
// DECLARATION OF PUBLIC STUFF
// --------------------------------------------------------------------------------------------------------------------
template <auto &PointerToIsrHandler, typename ObjectType, auto MemberFunction, typename... ConstructorParams>
ObjectType &make_and_enable_member_as_isr_handler(ConstructorParams &&... params);

// --------------------------------------------------------------------------------------------------------------------
// DEFINITION OF PRIVATE STUFF
// --------------------------------------------------------------------------------------------------------------------
namespace detail {
template <auto &PointerToIsrHandler, auto &Object, auto MemberFunction, typename MemberFunctionType>
struct member_isr_enabler_impl;

template <auto &PointerToIsrHandler,
          auto &Object,
          auto MemberFunction,
          typename ReturnType,
          typename ClassType,
          typename... Args>
struct member_isr_enabler_impl<PointerToIsrHandler, Object, MemberFunction, ReturnType (ClassType::*)(Args...)>
{
    static ReturnType call_on_instance(Args... args)
    {
        return (Object.*MemberFunction)(args...);
    }

    member_isr_enabler_impl()
    {
        PointerToIsrHandler = call_on_instance;
    }
};

template <auto &PointerToIsrHandler, auto &Object, auto MemberFunction>
using member_isr_enabler =
    member_isr_enabler_impl<PointerToIsrHandler, Object, MemberFunction, decltype(MemberFunction)>;
} // namespace detail

// --------------------------------------------------------------------------------------------------------------------
// DEFINITION OF PUBLIC STUFF
// --------------------------------------------------------------------------------------------------------------------
template <auto &PointerToIsrHandler, typename ObjectType, auto MemberFunction, typename... ConstructorParams>
ObjectType &make_and_enable_member_as_isr_handler(ConstructorParams &&... params)
{
    static ObjectType r{std::forward<ConstructorParams>(params)...};
    static detail::member_isr_enabler<PointerToIsrHandler, r, MemberFunction> e;
    return r;
}

} // namespace jungles

#endif /* MEMBER_ISR_ENABLE_HPP */
