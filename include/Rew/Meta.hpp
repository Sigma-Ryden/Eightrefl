#ifndef REW_META_HPP
#define REW_META_HPP

// .meta(name, expr)
#define META(name_str, ...)                                                                             \
    {                                                                                                   \
        auto xxmeta = rew::find_or_add_meta(xxreflection, name_str, __VA_ARGS__);                       \
        injection.template meta<CleanR, decltype(__VA_ARGS__)>(name_str, *xxmeta);                      \
    }

#endif // REW_META_HPP
