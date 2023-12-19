#ifndef REW_TYPE_HPP
#define REW_TYPE_HPP

#include <cstddef> // size_t

#include <string> // string
#include <any> // any

#include <utility> // ref
#include <functional> // function

namespace rew
{

struct reflection_t;
struct visitor_t;

struct type_t
{
    const std::string name;
    reflection_t *const reflection = nullptr;
    const std::function<void(visitor_t&)> evaluate = nullptr;
    const std::function<std::any(void*)> ref = nullptr;
    const std::size_t size = 0;
};

template <typename ReflectableType>
auto type_ref_handler()
{
    return [](void* object) -> std::any
    {
        return std::ref(*static_cast<ReflectableType*>(object));
    };
}

} // namespace rew

#endif // REW_TYPE_HPP
