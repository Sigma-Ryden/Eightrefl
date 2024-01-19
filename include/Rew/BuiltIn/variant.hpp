#ifndef REW_BUILT_IN_VARIANT_HPP
#define REW_BUILT_IN_VARIANT_HPP

#include <variant> // variant

#include <Rew/Reflectable.hpp>

TEMPLATE_REFLECTABLE_DECLARATION
((template <typename ArgumentType, typename... ArgumentTypes>), (std::variant<ArgumentType, ArgumentTypes...>))
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::variant<" + NAMEOF(ArgumentType) + (", " + ... + NAMEOF(ArgumentTypes)) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE
((template <typename ArgumentType, typename... ArgumentTypes>), (std::variant<ArgumentType, ArgumentTypes...>))
    FACTORY(R())
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(index)
    FUNCTION(valueless_by_exception)
    FUNCTION(swap)
REFLECTABLE_INIT()

#endif // REW_BUILT_IN_VARIANT_HPP
