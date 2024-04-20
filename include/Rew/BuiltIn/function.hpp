#ifndef REW_BUILTIN_FUNCTION_HPP
#define REW_BUILTIN_FUNCTION_HPP

#include <functional> // function

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

#ifndef REW_CORE_MINIMAL
// as function return type
#include <Rew/BuiltIn/typeinfo.hpp>
#endif // REW_CORE_MINIMAL

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ReturnType, typename... ArgumentTypes>),
    (std::function<ReturnType(ArgumentTypes...)>),
    std::function<CLEANOF(ReturnType(ArgumentTypes...))>
)

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ReturnType, typename... ArgumentTypes>),
    std::function<ReturnType(ArgumentTypes...)>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::function<" + NAMEOF(ReturnType(ArgumentTypes...)) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE
(
    (template <typename ReturnType, typename... ArgumentTypes>),
    std::function<ReturnType(ArgumentTypes...)>
)
    FACTORY(R())
    FACTORY(R(std::nullptr_t))
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(operator=, R&(std::nullptr_t))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(swap)
    #endif // REW_CORE_MINIMAL

    FUNCTION(operator bool)
    FUNCTION(operator())

    #ifndef REW_CORE_MINIMAL
    FUNCTION(target_type)
    #endif // REW_CORE_MINIMAL
REFLECTABLE_INIT()

#endif // REW_BUILTIN_FUNCTION_HPP
