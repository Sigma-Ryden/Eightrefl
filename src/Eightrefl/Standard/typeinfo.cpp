#include <Eightrefl/Standard/typeinfo.hpp>

#include <Eightrefl/BuiltIn/Core.hpp>

REFLECTABLE(std::type_info)
    FUNCTION(operator==)

    #ifdef EIGHTREFL_FULLY_ENABLE
    FUNCTION(before)
    #endif // EIGHTREFL_CRE_MINIMAL

    FUNCTION(hash_code)
    FUNCTION(name)
REFLECTABLE_INIT()
