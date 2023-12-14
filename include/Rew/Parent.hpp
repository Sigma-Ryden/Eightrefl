#ifndef REW_PARENT_HPP
#define REW_PARENT_HPP

#include <string> // string
#include <map> // map
#include <vector> // vector

#include <functional> // function

#include <Rew/Attribute.hpp>
#include <Rew/Meta.hpp>

#define PARENT(...)                                                                                     \
    visitor.template parent<__VA_ARGS__, reflectable_type>({                                            \
        #__VA_ARGS__,                                                                                   \
        parent_get_handler<__VA_ARGS__, reflectable_type>(),                                            \
        registry->find_or_add<__VA_ARGS__>(#__VA_ARGS__)                                                \
    });

namespace rew
{

class type_t;

struct parent_meta_t
{
    const std::string name;
    const std::function<void*(void*)> get = nullptr;
    const type_t *const type = nullptr;
    meta_t meta;
};

using parent_t = attribute_t<parent_meta_t>;

template <class BaseClassType, class DerivedClassType>
auto parent_get_handler()
{
    return [](void* self)
    {
        return static_cast<BaseClassType*>(static_cast<DerivedClassType*>(self));
    };
}

} // namespace rew

#endif // REW_PARENT_HPP
