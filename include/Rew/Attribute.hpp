#ifndef REW_ATTRIBUTE_HPP
#define REW_ATTRIBUTE_HPP

#include <string> // string
#include <map> // map

namespace rew
{

template <class MetaType>
struct attribute_t
{
    auto find(const std::string& name)
    {
        auto it = all.find(name);
        return it != all.end() ? &it->second : nullptr;
    }

    bool add(const std::string& name, const MetaType& meta)
    {
        return all.emplace(name, meta).second;
    }

    bool remove(const std::string& name)
    {
        return all.erase(name)>0;
    }

    std::map<std::string, MetaType> all;
};

} // namespace rew

#endif // REW_ATTRIBUTE_HPP
