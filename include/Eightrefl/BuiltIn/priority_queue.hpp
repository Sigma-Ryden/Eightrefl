#ifndef EIGHTREFL_BUILTIN_PRIORITY_QUEUE_HPP
#define EIGHTREFL_BUILTIN_PRIORITY_QUEUE_HPP

#include <queue> // priority_queue

#include <Eightrefl/Reflectable.hpp>
#include <Eightrefl/Common.hpp>

// default container for priority_queue
#include <Eightrefl/BuiltIn/vector.hpp>

// less - default comparator for priority_queue
#include <Eightrefl/BuiltIn/functional.hpp>

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ValueType, class ContainerType, class ComparatorType>),
    (std::priority_queue<ValueType, ContainerType, ComparatorType>),
    std::priority_queue<eightrefl::clean_of<ValueType>, eightrefl::clean_of<ContainerType>, eightrefl::clean_of<ComparatorType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ValueType>, std::priority_queue<ValueType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::priority_queue<" + eightrefl::name_of<ValueType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ValueType, class ContainerType>), std::priority_queue<ValueType, ContainerType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::priority_queue<" + eightrefl::name_of<ValueType>() + ", " + eightrefl::name_of<ContainerType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ValueType, class ContainerType, class ComparatorType>),
    std::priority_queue<ValueType, ContainerType, ComparatorType>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::priority_queue<" + eightrefl::name_of<ValueType>() + ", " + eightrefl::name_of<ContainerType>() + ", "
                                            + eightrefl::name_of<ComparatorType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE
(
    (template <typename ValueType, class ContainerType, class ComparatorType>),
    std::priority_queue<ValueType, ContainerType, ComparatorType>
)
    FACTORY(R())

    #ifdef EIGHTREFL_FULLY_ENABLE
    FACTORY(R(typename R::value_compare const&, typename R::container_type const&))
    FACTORY(R(typename R::value_compare const&))
    #endif // EIGHTREFL_FULLY_ENABLE

    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(top, typename R::const_reference() const)
    FUNCTION(empty)
    FUNCTION(size)
    FUNCTION(push, void(typename R::const_reference))
    FUNCTION(pop)

    #ifdef EIGHTREFL_FULLY_ENABLE
    FUNCTION(swap)
    #endif // EIGHTREFL_FULLY_ENABLE
REFLECTABLE_INIT()

#endif // EIGHTREFL_BUILTIN_PRIORITY_QUEUE_HPP
