#ifndef REW_BUILTIN_WEAK_PTR_HPP
#define REW_BUILTIN_WEAK_PTR_HPP

#include <memory> // weak_ptr

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

// as return type of the lock() function
#include <Rew/BuiltIn/shared_ptr.hpp>

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ElementType>), (std::weak_ptr<ElementType>),
    std::weak_ptr<CLEANOF(ElementType)>
)

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ElementType>), std::weak_ptr<ElementType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::weak_ptr<" + NAMEOF(ElementType) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename ElementType>), std::weak_ptr<ElementType>)
    FACTORY(R())
    FACTORY(R(R const&))
    FACTORY(R(std::shared_ptr<typename R::element_type> const&))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(operator=, R&(std::shared_ptr<typename R::element_type> const&))
    FUNCTION(reset)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(swap)
    FUNCTION(use_count)
    #endif // REW_CORE_MINIMAL

    FUNCTION(expired)
    FUNCTION(lock)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(owner_before, bool(R const&))
    FUNCTION(owner_before, bool(std::shared_ptr<typename R::value_type> const&))
    #endif // REW_CORE_MINIMAL
REFLECTABLE_INIT()

#endif // REW_BUILTIN_WEAK_PTR_HPP
