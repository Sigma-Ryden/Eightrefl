#ifndef REW_BUILTIN_ALLOCATOR_HPP
#define REW_BUILTIN_ALLOCATOR_HPP

#include <memory> // allocator

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ValueType>), std::allocator<ValueType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::allocator<" + NAMEOF(ValueType) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename ValueType>), std::allocator<ValueType>)
    FACTORY(R())
    FACTORY(R(R const&))
    FUNCTION(allocate, typename R::value_type*(std::size_t, void const*))
    FUNCTION(deallocate, void(typename R::value_type*, std::size_t))
REFLECTABLE_INIT()

#endif // REW_BUILTIN_ALLOCATOR_HPP
