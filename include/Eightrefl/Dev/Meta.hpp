#ifndef EIGHTREFL_DEV_META_HPP
#define EIGHTREFL_DEV_META_HPP

#ifdef EIGHTREFL_DEV_ENABLE
#include <Eightrefl/Reflectable.hpp>

#include <Eightrefl/Dev/Dev.hpp>

REFLECTABLE_DECLARATION(eightrefl::meta_t)
    REFLECTABLE_REGISTRY(eightrefl::dev())
REFLECTABLE_DECLARATION_INIT()
#endif // EIGHTREFL_DEV_ENABLE

#endif // EIGHTREFL_DEV_META_HPP