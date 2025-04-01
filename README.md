# Prerequisites
[Experimental LLVM compiler with C++26 Reflection support](https://github.com/bloomberg/clang-p2996/tree/p2996) should be installed.

# Build & Run Test Cases
This project uses [XMake](https://xmake.io) as build system.
```
xmake f -m <debug|release> \
        --sdk=<llvm-root> \
        --cxxflags="-stdlib=libc++" \
        --ldflags="-stdlib=libc++" \
        --toolchain=llvm

xmake build --group=tests/**
LD_LIBRARY_PATH=<path-to-libc++> xmake run --group=tests/**
```

# TODO
* Enum functions or types not implemented (compared to [magic_enum](https://github.com/Neargye/magic_enum)):
  * `enum_fusion`
  * Functions for enum flags
  * IOStream operators
  * Bitwise operators
  * containers
* Improvements to `constant`:
  * `map`, `filter` with index
  * `index_of`, `last_index_of`, `includes` with start index
* `tests/type_traits/class_types/test_flattened_accessible_nsdm_X.cpp`
  * Class types with virtual member functions

# Test Cases Failed Due to Compiler Bug

* `tests/type_traits/class_types/test_flattened_accessible_nsdm_1.cpp`
  * `std::meta::offset_of()` is not usable for base relationship.
