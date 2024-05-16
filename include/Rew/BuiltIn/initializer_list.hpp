#ifndef REW_BUILTIN_INITIALIZER_LIST_HPP
#define REW_BUILTIN_INITIALIZER_LIST_HPP

#include <initializer_list> // initializer_list

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

TEMPLATE_REFLECTABLE_CLEAN
(
    template <typename ValueType>,
    std::initializer_list<ValueType>, std::initializer_list<rew::cleanof<ValueType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ValueType>, std::initializer_list<ValueType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::initializer_list<" + rew::nameof<ValueType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <typename ValueType>, std::initializer_list<ValueType>)
    FACTORY(R())
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))
    
    #ifndef REW_CORE_MINIMAL
    FUNCTION(begin)
    FUNCTION(end)
    #endif // REW_CORE_MINIMAL

    FUNCTION(size)
REFLECTABLE_INIT()

#endif // REW_BUILTIN_INITIALIZER_LIST_HPP
