# TODO
* Member pointer facilities in `type_traits/class_types.cpp` fails to work
  with reference members: "cannot form a pointer-to-member to member 'i' of
  reference type 'int &'".
  * Failed test cases:
    * `tests/type_traits/class_types/test_flattenable_types.cpp`
      * TypeTraitsClassTypes.IsFlattenableClassTypesReferenceMembers

# Test cases failed due to compiler bug

* `tests/type_traits/class_types/test_flattened_accessible_nsdm_1.cpp`
  * `std::meta::offset_of()` is not usable for base relationship.
