#ifdef EIGHTREFL_DEV_ENABLE
#include <Eightrefl/Dev/Meta.hpp>
#include <Eightrefl/Dev/Attribute.hpp>

#include <Eightrefl/Standard/string.hpp>
#include <Eightrefl/Standard/any.hpp>

REFLECTABLE(eightrefl::meta_t)
    PROPERTY(name)
    PROPERTY(value)
    PROPERTY(meta)
REFLECTABLE_INIT()
#endif // EIGHTREFL_DEV_ENABLE
