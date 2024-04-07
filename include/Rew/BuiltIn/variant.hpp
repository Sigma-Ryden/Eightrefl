#ifndef REW_BUILTIN_VARIANT_HPP
#define REW_BUILTIN_VARIANT_HPP

#include <variant> // variant, monostate

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

REFLECTABLE_DECLARATION(std::monostate)
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(std::monostate)
REFLECTABLE_INIT()


TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ArgumentType, typename... ArgumentTypes>), (std::variant<ArgumentType, ArgumentTypes...>),
    std::variant<CLEANOF(ArgumentType), CLEANOF(ArgumentTypes)...>
)

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ArgumentType, typename... ArgumentTypes>),
    std::variant<ArgumentType, ArgumentTypes...>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::variant<" + ( NAMEOF(ArgumentType) + ... + (", " + NAMEOF(ArgumentTypes)) ) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE
(
    (template <typename ArgumentType, typename... ArgumentTypes>),
    std::variant<ArgumentType, ArgumentTypes...>
)
    FACTORY(R())
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(index)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(valueless_by_exception)
    FUNCTION(swap)
    #endif // REW_CORE_MINIMAL
REFLECTABLE_INIT()

#endif // REW_BUILTIN_VARIANT_HPP
