#ifndef REW_BUILTIN_FUNCTION_HPP
#define REW_BUILTIN_FUNCTION_HPP

#include <functional> // function

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

// as function argument type
#include <Rew/BuiltIn/nullptr_t.hpp>

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ReturnType>), (std::function<ReturnType()>))
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::function<" + NAMEOF(ReturnType) + "()>")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ReturnType, typename ArgumentType, typename... ArgumentTypes>),
    (std::function<ReturnType(ArgumentType, ArgumentTypes...)>)
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::function<" + NAMEOF(ReturnType) + "(" + NAMEOF(ArgumentType) + (", " + ... + NAMEOF(ArgumentTypes)) + ")>")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE
(
    (template <typename ReturnType, typename... ArgumentTypes>),
    (std::function<ReturnType(ArgumentTypes...)>)
)
    FACTORY(R())
    FACTORY(R(std::nullptr_t))
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(operator=, R&(std::nullptr_t))
    FUNCTION(swap)
    FUNCTION(operator bool)
    FUNCTION(operator())
    FUNCTION(target_type)
REFLECTABLE_INIT()

#endif // REW_BUILTIN_FUNCTION_HPP
