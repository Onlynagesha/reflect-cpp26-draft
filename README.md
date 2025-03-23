# Test cases failed due to compiler bug

* `tests/type_traits/class_types/test_flattened_accessible_nsdm_1.cpp`
  * Compiler crashes.
* `tests/type_traits/class_types/test_tuple_like_types.cpp`
  * Static assertion `rfl::is_tuple_like_v<rfl_meta_tuple>` fails since
    neither `t.get<I>()` nor `get<I>(t)` is compilable.
* `tests/type_traits/utils/test_meta_tuple.cpp`
  * Compiler crashes with member-get expressions like `t.get<0>()`.
