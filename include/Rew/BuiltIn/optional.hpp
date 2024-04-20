#ifndef REW_BUILTIN_OPTIONAL_HPP
#define REW_BUILTIN_OPTIONAL_HPP

#include <optional> // optional, nullopt_t

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

REFLECTABLE_DECLARATION(std::nullopt_t)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(std::nullopt_t)
REFLECTABLE_INIT()


TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ValueType>), (std::optional<ValueType>),
    std::optional<CLEANOF(ValueType)>
)

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ValueType>), std::optional<ValueType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::optional<" + NAMEOF(ValueType) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename ValueType>), std::optional<ValueType>)
    FACTORY(R())
    FACTORY(R(std::nullopt_t))
    FACTORY(R(R const&))
    FACTORY(R(typename R::value_type const&))
    FUNCTION(operator=, R&(std::nullopt_t))
    FUNCTION(operator=, R&(R const&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(operator->, typename R::value_type const*() const)
    FUNCTION(operator->, typename R::value_type*())
    FUNCTION(operator*, typename R::value_type const&() const)
    FUNCTION(operator*, typename R::value_type&())
    FUNCTION(operator bool)
    #endif // REW_CORE_MINIMAL

    FUNCTION(has_value)
    FUNCTION(value, typename R::value_type&())
    FUNCTION(value, typename R::value_type const&() const)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(value_or, typename R::value_type(typename R::value_type const&) const)
    FUNCTION(swap)
    #endif // REW_CORE_MINIMAL

    FUNCTION(reset)
    FUNCTION(emplace, typename R::value_type&(typename R::value_type const&))
REFLECTABLE_INIT()

#endif // REW_BUILTIN_OPTIONAL_HPP
