#ifndef REW_BUILTIN_QUEUE_HPP
#define REW_BUILTIN_QUEUE_HPP

#include <queue> // queue

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

// default container for queue
#include <Rew/BuiltIn/deque.hpp>

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ValueType, class ContainerType>), (std::queue<ValueType, ContainerType>))
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::queue<" + NAMEOF(ValueType) + ", " + NAMEOF(ContainerType) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename ValueType, class ContainerType>), (std::queue<ValueType, ContainerType>))
    FACTORY(R())
    FACTORY(R(typename R::container_type const&))
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(front, typename R::reference())
    FUNCTION(front, typename R::const_reference() const)
    FUNCTION(back, typename R::reference())
    FUNCTION(back, typename R::const_reference() const)
    FUNCTION(empty)
    FUNCTION(size)
    FUNCTION(push, void(typename R::const_reference))
    FUNCTION(pop)
    FUNCTION(swap)
REFLECTABLE_INIT()

#endif // REW_BUILTIN_QUEUE_HPP
