#ifndef REW_BUILTIN_UNIQUE_PTR_HPP
#define REW_BUILTIN_UNIQUE_PTR_HPP

#include <memory> // unique_ptr

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ElementType, typename DeleterType>),
    (std::unique_ptr<ElementType, DeleterType>), std::unique_ptr<rew::cleanof<ElementType>, rew::cleanof<DeleterType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ElementType>, std::default_delete<ElementType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::default_delete<" + rew::nameof<ElementType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <typename ElementType>, std::default_delete<ElementType>)
REFLECTABLE_INIT()

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ElementType>, std::unique_ptr<ElementType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::unique_ptr<" + rew::nameof<ElementType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ElementType, typename DeleterType>),
    std::unique_ptr<ElementType, DeleterType>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::unique_ptr<" + rew::nameof<ElementType>() + ", " + rew::nameof<DeleterType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE
(
    (template <typename ElementType, typename DeleterType>),
    std::unique_ptr<ElementType, DeleterType>
)
    FUNCTION(operator=, R&(std::nullptr_t))
    FUNCTION(release)
    FUNCTION(reset, void(typename R::pointer))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(swap)
    #endif // REW_CORE_MINIMAL

    FUNCTION(get)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(get_deleter, typename R::deleter_type&())
    FUNCTION(get_deleter, typename R::deleter_type const&() const)
    #endif // REM_CORE_MINIMAL

    FUNCTION(operator bool)
    FUNCTION(operator*)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(operator->)
    #endif // REW_CORE_MINIMAL
REFLECTABLE_INIT()

#endif // REW_BUILTIN_UNIQUE_PTR_HPP
