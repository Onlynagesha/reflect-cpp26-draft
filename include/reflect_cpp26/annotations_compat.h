#ifndef REFLECT_CPP26_ANNOTATIONS_COMPAT_H
#define REFLECT_CPP26_ANNOTATIONS_COMPAT_H

#if __cplusplus
#include <reflect_cpp26/annotations.hpp>
#else
#define REFLECT_CPP26_ANNOTATE(...) /* no-op */
#define REFLECT_CPP26_ANNOTATIONS(...) /* no-op */
#endif

#endif // REFLECT_CPP26_ANNOTATIONS_COMPAT_H
