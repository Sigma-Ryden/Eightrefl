#ifndef REW_TYPE_HPP
#define REW_TYPE_HPP

#include <string> // string

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
};

} // namespace rew

#endif // REW_TYPE_HPP
