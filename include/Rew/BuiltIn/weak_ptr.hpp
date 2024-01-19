#ifndef REW_BUILT_IN_WEAK_PTR_HPP
#define REW_BUILT_IN_WEAK_PTR_HPP

#include <memory> // weak_ptr

#include <Rew/Reflectable.hpp>

// as return type of the lock() function
#include <Rew/BuiltIn/shared_ptr.hpp>

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ElementType>), (std::weak_ptr<ElementType>))
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::weak_ptr<" + NAMEOF(ElementType) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename ElementType>), (std::weak_ptr<ElementType>))
    FACTORY(R())
    FACTORY(R(R const&))
    FACTORY(R(std::shared_ptr<typename R::element_type> const&))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(operator=, R&(std::shared_ptr<typename R::element_type> const&))
    FUNCTION(reset)
    FUNCTION(swap)
    FUNCTION(use_count)
    FUNCTION(expired)
    FUNCTION(lock)
    FUNCTION(owner_before, bool(R const&))
    FUNCTION(owner_before, bool(std::shared_ptr<typename R::value_type> const&))
REFLECTABLE_INIT()

#endif // REW_BUILT_IN_WEAK_PTR_HPP
