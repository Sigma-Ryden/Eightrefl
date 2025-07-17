#ifndef EIGHTREFL_BUILTIN_SHARED_PTR_HPP
#define EIGHTREFL_BUILTIN_SHARED_PTR_HPP

#include <memory> // shared_ptr

#include <Eightrefl/Reflectable.hpp>
#include <Eightrefl/Common.hpp>

TEMPLATE_REFLECTABLE_CLEAN
(
    template <typename ElementType>,
    std::shared_ptr<ElementType>, std::shared_ptr<eightrefl::clean_of<ElementType>>
)

REFLECTABLE_DECLARATION(std::shared_ptr<void>)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ElementType>, std::shared_ptr<ElementType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::shared_ptr<" + eightrefl::name_of<ElementType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <typename ElementType>, std::shared_ptr<ElementType>)
    FACTORY(R())

    #ifdef EIGHTREFL_FULLY_ENABLE
    FACTORY(R(std::nullptr_t))
    #endif // EIGHTREFL_FULLY_ENABLE

    FACTORY(R(typename R::element_type*))
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(reset, void())

    #ifdef EIGHTREFL_FULLY_ENABLE
    FUNCTION(reset, void(typename R::element_type*))
//  FUNCTION(swap, void(R&))
    #endif // EIGHTREFL_FULLY_ENABLE

    FUNCTION(get)
    FUNCTION(operator*)

    #ifdef EIGHTREFL_FULLY_ENABLE
    FUNCTION(operator->)
    FUNCTION(use_count)
    #endif // EIGHTREFL_FULLY_ENABLE

    FUNCTION(operator bool)
REFLECTABLE_INIT()

#endif // EIGHTREFL_BUILTIN_SHARED_PTR_HPP
