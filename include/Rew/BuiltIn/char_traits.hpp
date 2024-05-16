#ifndef REW_BUILTIN_CHAR_TRAITS_HPP
#define REW_BUILTIN_CHAR_TRAITS_HPP

#include <string> // char_traits

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

TEMPLATE_REFLECTABLE_CLEAN
(
    template <typename CharType>,
    std::char_traits<CharType>, std::char_traits<rew::cleanof<CharType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename CharType>, std::char_traits<CharType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::char_traits<" + rew::nameof<CharType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <typename CharType>, std::char_traits<CharType>)
    #ifndef REW_CORE_MINIMAL
    FUNCTION(assign, void(typename R::char_type&, typename R::char_type const&))
    FUNCTION(assign, typename R::char_type*(typename R::char_type*, std::size_t, typename R::char_type))
    FUNCTION(eq)
    FUNCTION(lt)
    FUNCTION(move)
    FUNCTION(copy)
    FUNCTION(compare)
    FUNCTION(length)
    FUNCTION(find)
    FUNCTION(to_char_type)
    FUNCTION(to_int_type)
    FUNCTION(eq_int_type)
    FUNCTION(eof)
    FUNCTION(not_eof)
    #endif // REW_CORE_MINIMAL
REFLECTABLE_INIT()

#endif // REW_BUILTIN_CHAR_TRAITS_HPP
