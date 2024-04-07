#ifndef REW_BUILTIN_TUPLE_HPP
#define REW_BUILTIN_TUPLE_HPP

#include <tuple> // tuple

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename... ArgumentTypes>), (std::tuple<ArgumentTypes...>),
    std::tuple<CLEANOF(ArgumentTypes)...>
)

TEMPLATE_REFLECTABLE_DECLARATION((template <>), std::tuple<>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::tuple<>")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ArgumentType, typename... ArgumentTypes>),
    std::tuple<ArgumentType, ArgumentTypes...>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::tuple<" + ( NAMEOF(ArgumentType) + ... + (", " + NAMEOF(ArgumentTypes)) ) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename... ArgumentTypes>), std::tuple<ArgumentTypes...>)
    FACTORY(R())
    FACTORY(R(ArgumentTypes...))
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(swap)
    #endif // REW_CORE_MINIMAL
REFLECTABLE_INIT()

#endif // REW_BUILTIN_TUPLE_HPP
