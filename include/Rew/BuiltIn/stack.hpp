#ifndef REW_BUILTIN_STACK_HPP
#define REW_BUILTIN_STACK_HPP

#include <stack> // stack

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

// default container for stack
#include <Rew/BuiltIn/deque.hpp>

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ValueType, class ContainerType>),
    (std::stack<ValueType, ContainerType>), std::stack<rew::cleanof<ValueType>, rew::cleanof<ContainerType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ValueType>, std::stack<ValueType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::stack<" + rew::nameof<ValueType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ValueType, class ContainerType>), std::stack<ValueType, ContainerType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::stack<" + rew::nameof<ValueType>() + ", " + rew::nameof<ContainerType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename ValueType, class ContainerType>), std::stack<ValueType, ContainerType>)
    FACTORY(R())
    FACTORY(R(typename R::container_type const&))
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(top, typename R::reference())
    FUNCTION(top, typename R::const_reference() const)
    FUNCTION(empty)
    FUNCTION(size)
    FUNCTION(push, void(typename R::const_reference))
    FUNCTION(pop)

    #ifdef REW_FULLY_ENABLE
    FUNCTION(swap)
    #endif // REW_FULLY_ENABLE
REFLECTABLE_INIT()

#endif // REW_BUILTIN_STACK_HPP
