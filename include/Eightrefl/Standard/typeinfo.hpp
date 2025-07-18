#ifndef EIGHTREFL_STANDARD_TYPE_INFO_HPP
#define EIGHTREFL_STANDARD_TYPE_INFO_HPP

#include <typeinfo> // type_info

#include <Eightrefl/Reflectable.hpp>

#include <Eightrefl/Standard/Standard.hpp>

REFLECTABLE_DECLARATION(std::type_info)
    REFLECTABLE_REGISTRY(eightrefl::standard())
REFLECTABLE_DECLARATION_INIT()

#endif // EIGHTREFL_STANDARD_TYPE_INFO_HPP
