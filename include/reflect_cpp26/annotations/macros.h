#ifndef REFLECT_CPP26_ANNOTATIONS_MACROS_H
#define REFLECT_CPP26_ANNOTATIONS_MACROS_H

#if __cplusplus
#define REFLECT_CPP26_PROPERTY(field, ...) \
  [[ =::reflect_cpp26::annotations::make_##field(__VA_ARGS__) ]]
#define REFLECT_CPP26_VALIDATOR(field, ...)         \
  [[ =[] {                                          \
    constexpr auto maker = []() {                   \
      using namespace ::reflect_cpp26::annotations; \
      return field;                                 \
    }();                                            \
    return maker(__VA_ARGS__);                      \
  }() ]]
#else
#define REFLECT_CPP26_PROPERTY(...) // No-op
#define REFLECT_CPP26_VALIDATOR(...) // No-op
#endif

#endif // REFLECT_CPP26_ANNOTATIONS_MACROS_H
