#ifndef REW_BUILT_IN_TUPLE_HPP
#define REW_BUILT_IN_TUPLE_HPP

#include <tuple> // tuple

#include <Rew/Reflectable.hpp>

TEMPLATE_REFLECTABLE_DECLARATION
((template <typename ArgumentType, typename... ArgumentTypes>), (std::tuple<ArgumentType, ArgumentTypes...>))
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::tuple<" + NAMEOF(ArgumentType) + (", " + ... + NAMEOF(ArgumentTypes)) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE
((template <typename ArgumentType, typename... ArgumentTypes>), (std::tuple<ArgumentType, ArgumentTypes...>))
    FACTORY(R())
    FACTORY(R(ArgumentType, ArgumentTypes...))
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(swap)
REFLECTABLE_INIT()

#endif // REW_BUILT_IN_TUPLE_HPP
