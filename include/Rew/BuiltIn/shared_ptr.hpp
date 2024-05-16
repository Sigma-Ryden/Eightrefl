#ifndef REW_BUILTIN_SHARED_PTR_HPP
#define REW_BUILTIN_SHARED_PTR_HPP

#include <memory> // shared_ptr

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

TEMPLATE_REFLECTABLE_CLEAN
(
    template <typename ElementType>,
    std::shared_ptr<ElementType>, std::shared_ptr<rew::cleanof<ElementType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ElementType>, std::shared_ptr<ElementType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::shared_ptr<" + rew::nameof<ElementType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <typename ElementType>, std::shared_ptr<ElementType>)
    FACTORY(R())

    #ifndef REW_CORE_MINIMAL
    FACTORY(R(std::nullptr_t))
    #endif // REW_CORE_MINIMAL

    FACTORY(R(typename R::element_type*))
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(reset, void())

    #ifndef REW_CORE_MINIMAL
    FUNCTION(reset, void(typename R::element_type*))
//  FUNCTION(swap, void(R&))
    #endif // REW_CORE_MINIMAL

    FUNCTION(get)
    FUNCTION(operator*)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(operator->)
    FUNCTION(use_count)
    #endif // REW_CORE_MINIMAL

    FUNCTION(operator bool)
REFLECTABLE_INIT()

#endif // REW_BUILTIN_SHARED_PTR_HPP
