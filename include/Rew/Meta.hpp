#ifndef REW_META_HPP
#define REW_META_HPP

// .meta(name, expr)
#define META(name_str, ...)                                                                             \
    {                                                                                                   \
        auto __meta = rew::find_or_add_meta(__reflection, name_str, __VA_ARGS__);                       \
        injection.template meta<CleanR, decltype(__VA_ARGS__)>(name_str, *__meta);                      \
    }

#endif // REW_META_HPP
