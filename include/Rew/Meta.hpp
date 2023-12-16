#ifndef REW_META_HPP
#define REW_META_HPP

#include <string> // string
#include <map> // map
#include <any> // any

#include <Rew/Attribute.hpp>

#define META(name, ...)                                                                                 \
    visitor.template meta<info_t::type>(name, __VA_ARGS__);

namespace rew
{

// TODO:
// improve meta class for better registration info
using meta_t = attribute_t<std::any>;

} // namespace rew

#endif // REW_META_HPP
