#ifndef REW_BUILTIN_BITSET_HPP
#define REW_BUILTIN_BITSET_HPP

#include <string> // to_string

#include <bitset> // bitest

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

// as function argument tye
#include <Rew/BuiltIn/string.hpp>

TEMPLATE_REFLECTABLE_USING
(
    (template <class StdBitsetType>), std_bitset_reference,
    (std_bitset_reference<StdBitsetType>), typename CLEANOF(StdBitsetType)::reference
)

TEMPLATE_REFLECTABLE_DECLARATION((template <class StdBitsetType>), std_bitset_reference<StdBitsetType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(NAMEOF(StdBitsetType) + "::reference")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <class StdBitsetType>), std_bitset_reference<StdBitsetType>)
    FUNCTION(operator=, R&(bool))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(operator bool)
    FUNCTION(operator~)
    FUNCTION(flip, R&())
REFLECTABLE_INIT()


TEMPLATE_REFLECTABLE_DECLARATION((template <std::size_t BitsetSize>), std::bitset<BitsetSize>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::bitset<" + std::to_string(BitsetSize) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <std::size_t BitsetSize>), std::bitset<BitsetSize>)
    FACTORY(R())
    FACTORY(R(R const&))

    #ifndef REW_CORE_MINIMAL
    FACTORY(R(unsigned long))
    #endif // REW_CORE_MINIMAL

    FACTORY(R(unsigned long long))

    #ifndef REW_CORE_MINIMAL
    FACTORY(R(std::string const&, std_size_t))
    #endif // REW_CORE_MINIMAL

    FACTORY(R(std::string const&))

    #ifndef REW_CORE_MINIMAL
    FACTORY(R(char const*, std_size_t))
    FACTORY(R(char const*))
    #endif // REW_CORE_MINIMAL

    FUNCTION(operator=, R&(R const&))
    FUNCTION(operator==)
    FUNCTION(operator[], bool(std_size_t) const)
    FUNCTION(operator[], std_bitset_reference<R>(std_size_t))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(test)
    FUNCTION(all)
    FUNCTION(any)
    FUNCTION(none)
    FUNCTION(count)
    FUNCTION(size)
    FUNCTION(operator&=)
    FUNCTION(operator|=)
    FUNCTION(operator^=)
    FUNCTION(operator~)
    FUNCTION(operator<<)
    FUNCTION(operator<<=)
    FUNCTION(operator>>)
    FUNCTION(operator>>=)
    FUNCTION(set, R&())
    #endif // REW_CORE_MINIMAL

    FUNCTION(set, R&(std_size_t, bool))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(reset, R&())
    #endif // REW_CORE_MINIMAL

    FUNCTION(reset, R&(std_size_t))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(flip, R&())
    #endif // REW_CORE_MINIMAL

    FUNCTION(flip, R&(std_size_t))
    FUNCTION(to_string, std::string(char, char) const)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(to_ulong)
    #endif // REW_CORE_MINIMAL

    FUNCTION(to_ullong)
REFLECTABLE_INIT()

#endif // REW_BUILTIN_BITSET_HPP
