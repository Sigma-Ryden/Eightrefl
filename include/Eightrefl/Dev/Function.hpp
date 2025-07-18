#ifndef EIGHTREFL_DEV_FUNCTION_HPP
#define EIGHTREFL_DEV_FUNCTION_HPP

#ifdef EIGHTREFL_DEV_ENABLE
#include <Eightrefl/Reflectable.hpp>

#include <Eightrefl/Dev/Dev.hpp>

REFLECTABLE_DECLARATION(eightrefl::function_t)
    REFLECTABLE_REGISTRY(eightrefl::dev())
REFLECTABLE_DECLARATION_INIT()
#endif // EIGHTREFL_DEV_ENABLE

#endif // EIGHTREFL_DEV_FUNCTION_HPP
