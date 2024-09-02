#ifndef TEST_REFLECTION_SELF_TEST_ATTRIBUTE
#define TEST_REFLECTION_SELF_TEST_ATTRIBUTE

#include <RewTestingBase.hpp>

#include <Rew/BuiltIn/string.hpp>
#include <Rew/BuiltIn/unordered_map.hpp>

TEMPLATE_REFLECTABLE_DECLARATION((template <typename MetaType>), rew::attribute_t<MetaType>)
    REFLECTABLE_NAME("rew::attribute_t<" + rew::nameof<MetaType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename MetaType>), rew::attribute_t<MetaType>)
    FUNCTION(find)
    FUNCTION(add)
    PROPERTY(all)
REFLECTABLE_INIT()

#endif // TEST_REFLECTION_SELF_TEST_ATTRIBUTE
