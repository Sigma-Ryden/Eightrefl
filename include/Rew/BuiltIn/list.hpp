#ifndef REW_BUILTIN_LIST_HPP
#define REW_BUILTIN_LIST_HPP

#include <list> // list

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

// default allocator for list
#include <Rew/BuiltIn/allocator.hpp>

#ifndef REW_CORE_MINIMAL
// as function argument type
#include <Rew/BuiltIn/initializer_list.hpp>
#include <Rew/BuiltIn/function.hpp>
#include <Rew/BuiltIn/iterator.hpp>
#endif // REW_CORE_MINIMAL

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ValueType, typename AllocatorType>),
    (std::list<ValueType, AllocatorType>), std::list<rew::cleanof<ValueType>, rew::cleanof<AllocatorType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ValueType>, std::list<ValueType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::list<" + rew::nameof<ValueType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ValueType, typename AllocatorType>), std::list<ValueType, AllocatorType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::list<" + rew::nameof<ValueType>() + ", " + rew::nameof<AllocatorType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename ValueType, typename AllocatorType>), std::list<ValueType, AllocatorType>)
    FACTORY(R())

    #ifndef REW_CORE_MIMIMAL
    FACTORY(R(typename R::allocator_type const&))
    FACTORY(R(typename R::size_type, typename R::const_reference, typename R::allocator_type const&))
    FACTORY(R(typename R::size_type, typename R::const_reference))
    FACTORY(R(typename R::size_type, typename R::allocator_type const&))
    FACTORY(R(typename R::size_type))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::allocator_type const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>))
    FACTORY(R(R const&, typename R::allocator_type const&))
    #endif // REW_CORE_MINIMAL

    FACTORY(R(R const&))

    #ifndef REW_CORE_MINIMAL
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::allocator_type const&))
    FACTORY(R(std::initializer_list<typename R::value_type>))
    #endif // REW_CORE_MINIMAL

    FUNCTION(operator=, R&(R const&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(operator=, R&(std::initializer_list<typename R::value_type>))
    FUNCTION(assign, void(typename R::size_type, typename R::const_reference))
    FUNCTION(assign, void(std_const_iterator<R>, std_const_iterator<R>))
    FUNCTION(assign, void(std::initializer_list<typename R::value_type>))
    FUNCTION(get_allocator)
    FUNCTION(front, typename R::const_reference() const)
    FUNCTION(front, typename R::reference())
    FUNCTION(back, typename R::const_reference() const)
    FUNCTION(back, typename R::reference())
    FUNCTION(begin, std_const_iterator<R>() const)
    FUNCTION(begin, std_iterator<R>())
    FUNCTION(cbegin, std_const_iterator<R>() const)
    FUNCTION(end, std_const_iterator<R>() const)
    FUNCTION(end, std_iterator<R>())
    FUNCTION(cend, std_const_iterator<R>() const)
    FUNCTION(rbegin, std_const_reverse_iterator<R>() const)
    FUNCTION(rbegin, std_reverse_iterator<R>())
    FUNCTION(crbegin, std_const_reverse_iterator<R>() const)
    FUNCTION(rend, std_const_reverse_iterator<R>() const)
    FUNCTION(rend, std_reverse_iterator<R>())
    FUNCTION(crend, std_const_reverse_iterator<R>() const)
    #endif // REW_CORE_MINIMAL

    FUNCTION(empty)
    FUNCTION(size)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(max_size)
    #endif // REW_CORE_MINIMAL

    FUNCTION(clear)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, typename R::const_reference))
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, typename R::size_type, typename R::const_reference))
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, std_const_iterator<R>, std_const_iterator<R>))
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, std::initializer_list<typename R::value_type>))
    FUNCTION(erase, std_iterator<R>(std_const_iterator<R>))
    FUNCTION(erase, std_iterator<R>(std_const_iterator<R>, std_const_iterator<R>))
    #endif // REW_CORE_MINIMAL

    FUNCTION(push_back, void(typename R::const_reference))
    FUNCTION(push_front, void(typename R::const_reference))
    FUNCTION(pop_back)
    FUNCTION(pop_front)
    FUNCTION(resize, void(typename R::size_type))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(resize, void(typename R::size_type, typename R::const_reference))
    FUNCTION(swap)
    #endif // REW_CORE_MINIMAL

    FUNCTION(merge, void(R&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(merge, void(R&, std::function<bool(typename R::const_reference, typename R::const_reference)>))
    FUNCTION(splice, void(std_const_iterator<R>, R&))
    FUNCTION(splice, void(std_const_iterator<R>, R&, std_const_iterator<R>))
    FUNCTION(splice, void(std_const_iterator<R>, R&, std_const_iterator<R>, std_const_iterator<R>))
    #endif // REW_CORE_MINIMAL

#if __cplusplus < 202002L
    FUNCTION(remove, void(typename R::const_reference))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(remove_if, void(std::function<bool(typename R::const_reference)>))
    #endif // REW_CORE_MINIMAL

    FUNCTION(unique, void())

    #ifndef REW_CORE_MINIMAL
    FUNCTION(unique, void(std::function<bool(typename R::const_reference, typename R::const_reference)>))
    #endif // REW_CORE_MINIMAL
#else
    FUNCTION(remove, typename R::size_type(typename R::const_reference))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(remove_if, typename R::size_type(std::function<bool(typename R::const_reference)>))
    #endif // REW_CORE_MINIMAL

    FUNCTION(unique, typename R::size_type())

    #ifndef REW_CORE_MINIMAL
    FUNCTION(unique, typename R::size_type(std::function<bool(typename R::const_reference, typename R::const_reference)>))
    #endif // REW_CORE_MINIMAL
#endif // if

    FUNCTION(reverse)
    FUNCTION(sort, void())

    #ifndef REW_CORE_MINIMAL
    FUNCTION(sort, void(std::function<bool(typename R::const_reference, typename R::const_reference)>))
    #endif // REW_CORE_MINIMAL
REFLECTABLE_INIT()

#endif // REW_BUILTIN_LIST_HPP
