#ifndef EIGHTREFL_STANDARD_ANY_HPP
#define EIGHTREFL_STANDARD_ANY_HPP

#include <any> // any

#include <Eightrefl/Reflectable.hpp>

#include <Eightrefl/Standard/Standard.hpp>

REFLECTABLE_DECLARATION(std::any)
    REFLECTABLE_REGISTRY(eightrefl::standard())
REFLECTABLE_DECLARATION_INIT()

#endif // EIGHTREFL_STANDARD_ANY_HPP
