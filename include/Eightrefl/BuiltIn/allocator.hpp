#ifndef EIGHTREFL_BUILTIN_ALLOCATOR_HPP
#define EIGHTREFL_BUILTIN_ALLOCATOR_HPP

#include <memory> // allocator

#include <Eightrefl/Reflectable.hpp>
#include <Eightrefl/Common.hpp>

TEMPLATE_REFLECTABLE_CLEAN
(
    template <typename ValueType>,
    std::allocator<ValueType>, std::allocator<eightrefl::clean_of<ValueType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ValueType>, std::allocator<ValueType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::allocator<" + eightrefl::name_of<ValueType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <typename ValueType>, std::allocator<ValueType>)
    FACTORY(R())
    FACTORY(R(R const&))
REFLECTABLE_INIT()

#endif // EIGHTREFL_BUILTIN_ALLOCATOR_HPP
