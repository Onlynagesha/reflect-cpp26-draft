#ifndef REFLECT_CPP26_ANNOTATIONS_MACROS_H
#define REFLECT_CPP26_ANNOTATIONS_MACROS_H

#if __cplusplus
#define REFLECT_CPP26_ANNOTATE(field, ...) \
  [[ =::reflect_cpp26::annotations::make_##field(__VA_ARGS__) ]]
#else
#define REFLECT_CPP26_ANNOTATE(...) // No-op
#endif

#endif // REFLECT_CPP26_ANNOTATIONS_MACROS_H
