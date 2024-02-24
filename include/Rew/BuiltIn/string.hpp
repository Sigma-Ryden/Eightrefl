// TODO: add std::char_traits
#ifndef REW_BUILTIN_STRING_HPP
#define REW_BUILTIN_STRING_HPP

#include <string> // basic_string

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

// default allocator for basic_string
#include <Rew/BuiltIn/allocator.hpp>

// as function argument type
#include <Rew/BuiltIn/initializer_list.hpp>
#include <Rew/BuiltIn/iterator.hpp>

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename CharType, typename Traits, typename AllocatorType>),
    (std::basic_string<CharType, Traits, AllocatorType>),
    std::basic_string<CLEANOF(CharType), CLEANOF(Traits), CLEANOF(AllocatorType)>
)

TEMPLATE_REFLECTABLE_DECLARATION((template <>), std::string)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::string")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION((template <>), std::wstring)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::wstring")
REFLECTABLE_DECLARATION_INIT()

#if __cplusplus >= 202002L
TEMPLATE_REFLECTABLE_DECLARATION((template <>), std::u8string)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::u8string")
REFLECTABLE_DECLARATION_INIT()
#endif // if

TEMPLATE_REFLECTABLE_DECLARATION((template <>), std::u16string)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::u16string")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION((template <>), std::u32string)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::u32string")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION((template <typename CharType>), std::basic_string<CharType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::basic_string<" + NAMEOF(CharType) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION((template <typename CharType, typename Traits>), std::basic_string<CharType, Traits>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::basic_string<" + NAMEOF(CharType) + ", " + NAMEOF(Traits) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename CharType, typename Traits, typename AllocatorType>),
    std::basic_string<CharType, Traits, AllocatorType>
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::basic_string<" + NAMEOF(CharType) + ", " + NAMEOF(Traits) + ", " + NAMEOF(AllocatorType) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE
(
    (template <typename CharType, typename Traits, typename AllocatorType>),
    std::basic_string<CharType, Traits, AllocatorType>
)
    FACTORY(R())
    FACTORY(R(typename R::allocator_type const&))
    FACTORY(R(typename R::size_type, typename R::value_type, typename R::allocator_type const&))
    FACTORY(R(typename R::size_type, typename R::value_type))
    FACTORY(R(R const&, typename R::size_type, typename R::size_type, typename R::allocator_type const&))
    FACTORY(R(R const&, typename R::size_type, typename R::size_type))
    FACTORY(R(typename R::value_type const*, typename R::size_type, typename R::allocator_type const&))
    FACTORY(R(typename R::value_type const*, typename R::size_type))
    FACTORY(R(typename R::value_type const*, typename R::allocator_type const&))
    FACTORY(R(typename R::value_type const*))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>, typename R::allocator_type const&))
    FACTORY(R(std_const_iterator<R>, std_const_iterator<R>))
    FACTORY(R(R const&, typename R::allocator_type const&))
    FACTORY(R(R const&))
    FACTORY(R(std::initializer_list<typename R::value_type>, typename R::allocator_type const&))
    FACTORY(R(std::initializer_list<typename R::value_type>))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(operator=, R&(typename R::value_type const*))
    FUNCTION(operator=, R&(typename R::value_type))
    FUNCTION(operator=, R&(std::initializer_list<typename R::value_type>))
    FUNCTION(assign, R&(typename R::size_type, typename R::value_type))
    FUNCTION(assign, R&(R const&))
    FUNCTION(assign, R&(R const&, typename R::size_type, typename R::size_type))
    FUNCTION(assign, R&(typename R::value_type const*, typename R::size_type))
    FUNCTION(assign, R&(typename R::value_type const*))
    FUNCTION(assign, R&(std_const_iterator<R>, std_const_iterator<R>))
    FUNCTION(assign, R&(std::initializer_list<typename R::value_type>))
    FUNCTION(get_allocator)
    FUNCTION(at, typename R::reference(typename R::size_type))
    FUNCTION(at, typename R::const_reference(typename R::size_type) const)
    FUNCTION(operator[], typename R::reference(typename R::size_type))
    FUNCTION(operator[], typename R::const_reference(typename R::size_type) const)
    FUNCTION(front, typename R::reference())
    FUNCTION(front, typename R::const_reference() const)
    FUNCTION(back, typename R::reference())
    FUNCTION(back, typename R::const_reference() const)
    FUNCTION(data, typename R::value_type const*() const)
#if __cplusplus >= 201703L
    FUNCTION(data, typename R::value_type*())
#endif // if
    FUNCTION(c_str)
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
    FUNCTION(empty)
    FUNCTION(size)
    FUNCTION(length)
    FUNCTION(max_size)
    FUNCTION(reserve, void(typename R::size_type))
    FUNCTION(capacity)
    FUNCTION(shrink_to_fit)
    FUNCTION(clear)
    FUNCTION(insert, R&(typename R::size_type, typename R::size_type, typename R::value_type))
    FUNCTION(insert, R&(typename R::size_type, typename R::value_type const*))
    FUNCTION(insert, R&(typename R::size_type, typename R::value_type const*, typename R::size_type))
    FUNCTION(insert, R&(typename R::size_type, R const&))
    FUNCTION(insert, R&(typename R::size_type, R const&, typename R::size_type, typename R::size_type))
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, typename R::value_type))
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, typename R::size_type, typename R::value_type))
    FUNCTION(insert, std_iterator<R>(std_const_iterator<R>, std::initializer_list<typename R::value_type>))
    FUNCTION(erase, R&(typename R::size_type, typename R::size_type))
    FUNCTION(erase, std_iterator<R>(std_const_iterator<R>))
    FUNCTION(erase, std_iterator<R>(std_const_iterator<R>, std_const_iterator<R>))
    FUNCTION(push_back)
    FUNCTION(pop_back)
    FUNCTION(append, R&(typename R::size_type, typename R::value_type))
    FUNCTION(append, R&(R const&))
    FUNCTION(append, R&(R const&, typename R::size_type, typename R::size_type))
    FUNCTION(append, R&(typename R::value_type const*, typename R::size_type))
    FUNCTION(append, R&(typename R::value_type const*))
    FUNCTION(append, R&(std_const_iterator<R>, std_const_iterator<R>))
    FUNCTION(append, R&(std::initializer_list<typename R::value_type>))
    FUNCTION(operator+=, R&(R const&))
    FUNCTION(operator+=, R&(typename R::value_type))
    FUNCTION(operator+=, R&(typename R::value_type const*))
    FUNCTION(operator+=, R&(std::initializer_list<typename R::value_type>))
    FUNCTION(replace, R&(typename R::size_type, typename R::size_type, R const&))
    FUNCTION(replace, R&(std_const_iterator<R>, std_const_iterator<R>, R const&))
    FUNCTION(replace, R&(typename R::size_type, typename R::size_type, R const&, typename R::size_type, typename R::size_type))
    FUNCTION(replace, R&(typename R::size_type, typename R::size_type, typename R::value_type const*, typename R::size_type))
    FUNCTION(replace, R&(std_const_iterator<R>, std_const_iterator<R>, typename R::value_type const*, typename R::size_type))
    FUNCTION(replace, R&(typename R::size_type, typename R::size_type, typename R::value_type const*))
    FUNCTION(replace, R&(std_const_iterator<R>, std_const_iterator<R>, typename R::value_type const*))
    FUNCTION(replace, R&(typename R::size_type, typename R::size_type, typename R::size_type, typename R::value_type))
    FUNCTION(replace, R&(std_const_iterator<R>, std_const_iterator<R>, typename R::size_type, typename R::value_type))
    FUNCTION(replace, R&(std_const_iterator<R>, std_const_iterator<R>, std_const_iterator<R>, std_const_iterator<R>))
    FUNCTION(replace, R&(std_const_iterator<R>, std_const_iterator<R>, std::initializer_list<typename R::value_type>))
    FUNCTION(copy)
    FUNCTION(resize, void(typename R::size_type))
    FUNCTION(resize, void(typename R::size_type, typename R::value_type))
    FUNCTION(swap)
    FUNCTION(find, typename R::size_type(R const&, typename R::size_type) const)
    FUNCTION(find, typename R::size_type(typename R::value_type const*, typename R::size_type, typename R::size_type) const)
    FUNCTION(find, typename R::size_type(typename R::value_type const*, typename R::size_type) const)
    FUNCTION(find, typename R::size_type(typename R::value_type, typename R::size_type) const)
    FUNCTION(rfind, typename R::size_type(R const&, typename R::size_type) const)
    FUNCTION(rfind, typename R::size_type(typename R::value_type const*, typename R::size_type, typename R::size_type) const)
    FUNCTION(rfind, typename R::size_type(typename R::value_type const*, typename R::size_type) const)
    FUNCTION(rfind, typename R::size_type(typename R::value_type, typename R::size_type) const)
    FUNCTION(find_first_of, typename R::size_type(R const&, typename R::size_type) const)
    FUNCTION(find_first_of, typename R::size_type(typename R::value_type const*, typename R::size_type, typename R::size_type) const)
    FUNCTION(find_first_of, typename R::size_type(typename R::value_type const*, typename R::size_type) const)
    FUNCTION(find_first_of, typename R::size_type(typename R::value_type, typename R::size_type) const)
    FUNCTION(find_first_not_of, typename R::size_type(R const&, typename R::size_type) const)
    FUNCTION(find_first_not_of, typename R::size_type(typename R::value_type const*, typename R::size_type, typename R::size_type) const)
    FUNCTION(find_first_not_of, typename R::size_type(typename R::value_type const*, typename R::size_type) const)
    FUNCTION(find_first_not_of, typename R::size_type(typename R::value_type, typename R::size_type) const)
    FUNCTION(find_last_of, typename R::size_type(R const&, typename R::size_type) const)
    FUNCTION(find_last_of, typename R::size_type(typename R::value_type const*, typename R::size_type, typename R::size_type) const)
    FUNCTION(find_last_of, typename R::size_type(typename R::value_type const*, typename R::size_type) const)
    FUNCTION(find_last_of, typename R::size_type(typename R::value_type, typename R::size_type) const)
    FUNCTION(find_last_not_of, typename R::size_type(R const&, typename R::size_type) const)
    FUNCTION(find_last_not_of, typename R::size_type(typename R::value_type const*, typename R::size_type, typename R::size_type) const)
    FUNCTION(find_last_not_of, typename R::size_type(typename R::value_type const*, typename R::size_type) const)
    FUNCTION(find_last_not_of, typename R::size_type(typename R::value_type, typename R::size_type) const)
    FUNCTION(compare, int(R const&) const)
    FUNCTION(compare, int(typename R::size_type, typename R::size_type, R const&) const)
    FUNCTION(compare, int(typename R::size_type, typename R::size_type, R const&, typename R::size_type, typename R::size_type) const)
    FUNCTION(compare, int(typename R::value_type const*) const)
    FUNCTION(compare, int(typename R::size_type, typename R::size_type, typename R::value_type const*) const)
    FUNCTION(compare, int(typename R::size_type, typename R::size_type, typename R::value_type const*, typename R::size_type) const)
#if __cplusplus >= 202002L
    FUNCTION(starts_with, bool(typename R::value_type) const)
    FUNCTION(starts_with, bool(typename R::value_tyoe const*) const)
    FUNCTION(ends_wtih, bool(typename R::value_type) const)
    FUNCTION(ends_with, bool(typename R::value_tyoe const*) const)
#endif // if
    FUNCTION(substr, R(typename R::size_type, typename R::size_type) const)
REFLECTABLE_INIT()

#endif // REW_BUILTIN_STRING_HPP
