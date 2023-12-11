#ifndef REW_PARENT_HPP
#define REW_PARENT_HPP

#include <string> // string
#include <map> // map

#include <functional> // function

#include <Rew/Attribute.hpp>

#define PARENT(...)                                                                                     \
    visitor.template parent<__VA_ARGS__, type>({                                                        \
        #__VA_ARGS__,                                                                                   \
        parent_get_handler<__VA_ARGS__, type>(),                                                        \
        registry->find_or_add<__VA_ARGS__>(#__VA_ARGS__)                                                \
    });

namespace rew
{

class type_t;

struct parent_t : public attribute_t<parent_t>
{
    struct meta_t
    {
        const std::string name;
        const std::function<void*(void*)> get = nullptr;
        const type_t *const type = nullptr;
    };

    std::map<std::string, meta_t> all;
};

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
