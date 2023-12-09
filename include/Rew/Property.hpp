#ifndef REW_PROPERTY_HPP
#define REW_PROPERTY_HPP

#include <string> // string
#include <map> // map
#include <any> // any

#include <functional> // function

#include <Rew/Attribute.hpp>

#define PROPERTY(...)                                                                                   \
    visitor.template property<decltype(&type::__VA_ARGS__)>({                                           \
        #__VA_ARGS__,                                                                                   \
        property_get_handler(&type::__VA_ARGS__),                                                       \
        property_set_handler(&type::__VA_ARGS__)                                                        \
    });

namespace rew
{

struct property_t : attribute_t<property_t>
{
    struct meta_t
    {
        const std::string name;

        const std::function<void(void*, std::any&)> get = nullptr;
        const std::function<void(void*, const std::any&)> set = nullptr;
        // TODO:
        // add type_t and reg for primitives
    };

    std::map<std::string, meta_t> all;
};

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
