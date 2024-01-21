#ifndef REW_BUILT_IN_UNIQUE_PTR_HPP
#define REW_BUILT_IN_UNIQUE_PTR_HPP

#include <memory> // unique_ptr

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

// as function argument type
#include <Rew/BuiltIn/nullptr_t.hpp>

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ElementType>), (std::default_delete<ElementType>))
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::default_delete<" + NAMEOF(ElementType) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ElementType, typename DeleterType>),
    (std::unique_ptr<ElementType, DeleterType>)
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::unique_ptr<" + NAMEOF(ElementType) + ", " + NAMEOF(DeleterType) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE
(
    (template <typename ElementType, typename DeleterType>),
    (std::unique_ptr<ElementType, DeleterType>)
)
    FUNCTION(operator=, R&(std::nullptr_t))
    FUNCTION(release)
    FUNCTION(reset, void(typename R::pointer))
    FUNCTION(swap)
    FUNCTION(get)
    FUNCTION(get_deleter, typename R::deleter_type&())
    FUNCTION(get_deleter, typename R::deleter_type const&() const)
    FUNCTION(operator bool)
    FUNCTION(operator*)
    FUNCTION(operator->)
REFLECTABLE_INIT()

#endif // REW_BUILT_IN_UNIQUE_PTR_HPP
