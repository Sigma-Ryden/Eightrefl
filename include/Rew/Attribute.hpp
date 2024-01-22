#ifndef REW_ATTRIBUTE_HPP
#define REW_ATTRIBUTE_HPP

#include <string> // string
#include <unordered_map> // unordered_map

namespace rew
{

template <class MetaType, typename KeyType = std::string>
struct attribute_t
{
    auto find(const KeyType& key)
    {
        auto it = all.find(key);
        return it != all.end() ? &it->second : nullptr;
    }

    decltype(auto) add(const KeyType& key, const MetaType& meta)
    {
        return (all.emplace(key, meta).first->second);
    }

    bool remove(const KeyType& key)
    {
        return all.erase(key)>0;
    }

    std::unordered_map<KeyType, MetaType> all;
};

} // namespace rew

#endif // REW_ATTRIBUTE_HPP
