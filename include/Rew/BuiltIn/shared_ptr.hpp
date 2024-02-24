#ifndef REW_BUILTIN_SHARED_PTR_HPP
#define REW_BUILTIN_SHARED_PTR_HPP

#include <memory> // shared_ptr

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

// as function argument type
#include <Rew/BuiltIn/nullptr_t.hpp>

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ElementType>), (std::shared_ptr<ElementType>),
    std::shared_ptr<CLEANOF(ElementType)>
)

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ElementType>), std::shared_ptr<ElementType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::shared_ptr<" + NAMEOF(ElementType) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename ElementType>), std::shared_ptr<ElementType>)
    FACTORY(R())
    FACTORY(R(std::nullptr_t))
    FACTORY(R(typename R::element_type*))
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(reset, void())
    FUNCTION(reset, void(typename R::element_type*))
    //FUNCTION(swap, void(R&))
    FUNCTION(get)
    FUNCTION(operator*)
    FUNCTION(operator->)
    FUNCTION(use_count)
    FUNCTION(operator bool)
REFLECTABLE_INIT()

#endif // REW_BUILTIN_SHARED_PTR_HPP
