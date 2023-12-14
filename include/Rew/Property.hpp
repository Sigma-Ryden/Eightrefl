#ifndef REW_PROPERTY_HPP
#define REW_PROPERTY_HPP

#include <string> // string
#include <map> // map
#include <any> // any

#include <functional> // function

#include <Rew/Attribute.hpp>
#include <Rew/Meta.hpp>

#define PROPERTY(...)                                                                                   \
    visitor.template property<decltype(&reflectable_type::__VA_ARGS__)>({                               \
        #__VA_ARGS__,                                                                                   \
        property_get_handler(&reflectable_type::__VA_ARGS__),                                           \
        property_set_handler(&reflectable_type::__VA_ARGS__)                                            \
    });

namespace rew
{

struct property_meta_t
{
    // TODO:
    // add type_t and reg for primitives
    const std::string name;
    const std::function<void(void*, std::any&)> get = nullptr;
    const std::function<void(void*, const std::any&)> set = nullptr;
    meta_t meta;
};

using property_t = attribute_t<property_meta_t>;

template <class ClassType, typename PropertyType>
auto property_get_handler(PropertyType ClassType::* property)
{
    return [property](void* self, std::any& result)
    {
        result = static_cast<ClassType*>(self)->*property;
    };
}

template <class ClassType, typename PropertyType>
auto property_set_handler(PropertyType ClassType::* property)
{
    return [property](void* self, const std::any& value)
    {
        static_cast<ClassType*>(self)->*property = std::any_cast<const PropertyType&>(value);
    };
}

} // namespace rew

#endif // REW_PROPERTY_HPP
