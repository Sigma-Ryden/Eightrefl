#ifndef REW_BUILT_IN_BITSET_HPP
#define REW_BUILT_IN_BITSET_HPP

#include <string> // to_string

#include <bitset> // bitest

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

TEMPLATE_REFLECTABLE_DECLARATION((template <std::size_t BitsetSize>), (std::bitset<BitsetSize>))
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::bitset<" + std::to_string(BitsetSize) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <std::size_t BitsetSize>), (std::bitset<BitsetSize>))
    FACTORY(R())
    FACTORY(R(R const&))
    FACTORY(R(unsigned long))
    FACTORY(R(unsigned long long))
    FACTORY(R(std::string const&, std::size_t))
    FACTORY(R(std::string const&))
    FACTORY(R(char const*, std::size_t))
    FACTORY(R(char const*))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(operator==)
    FUNCTION(operator[], bool(std::size_t) const)
    FUNCTION(operator[], typename R::reference(std::size_t))
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
    FUNCTION(set, R&(std::size_t, bool))
    FUNCTION(reset, R&())
    FUNCTION(reset, R&(std::size_t))
    FUNCTION(flip, R&())
    FUNCTION(flip, R&(std::size_t))
    FUNCTION(to_string, std::string(char, char) const)
    FUNCTION(to_ulong)
    FUNCTION(to_ullong)
REFLECTABLE_INIT()

#endif // REW_BUILT_IN_BITSET_HPP
