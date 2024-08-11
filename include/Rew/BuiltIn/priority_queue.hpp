#ifndef REW_BUILTIN_PRIORITY_QUEUE_HPP
#define REW_BUILTIN_PRIORITY_QUEUE_HPP

#include <queue> // priority_queue

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

// default container for priority_queue
#include <Rew/BuiltIn/vector.hpp>

// less - default comparator for priority_queue
#include <Rew/BuiltIn/functional.hpp>

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ValueType, class ContainerType, class ComparatorType>),
    (std::priority_queue<ValueType, ContainerType, ComparatorType>),
    std::priority_queue<rew::cleanof<ValueType>, rew::cleanof<ContainerType>, rew::cleanof<ComparatorType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ValueType>, std::priority_queue<ValueType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::priority_queue<" + rew::nameof<ValueType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ValueType, class ContainerType>), std::priority_queue<ValueType, ContainerType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::priority_queue<" + rew::nameof<ValueType>() + ", " + rew::nameof<ContainerType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ValueType, class ContainerType, class ComparatorType>),
    std::priority_queue<ValueType, ContainerType, ComparatorType>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::priority_queue<" + rew::nameof<ValueType>() + ", " + rew::nameof<ContainerType>() + ", "
                                            + rew::nameof<ComparatorType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE
(
    (template <typename ValueType, class ContainerType, class ComparatorType>),
    std::priority_queue<ValueType, ContainerType, ComparatorType>
)
    FACTORY(R())
    FACTORY(R(typename R::value_compare const&, typename R::container_type const&))
    FACTORY(R(typename R::value_compare const&))
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(top, typename R::const_reference() const)
    FUNCTION(empty)
    FUNCTION(size)
    FUNCTION(push, void(typename R::const_reference))
    FUNCTION(pop)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(swap)
    #endif // REW_CORE_MINIMAL
REFLECTABLE_INIT()

#endif // REW_BUILTIN_PRIORITY_QUEUE_HPP
