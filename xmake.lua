add_rules("mode.debug", "mode.release")

add_requires("gtest", {
    config = { main = true }
})

-- Ordered by dependency of tested components
meta_test_cases = {
    -- Utility
    "tests/utils/test_preprocessors",
    "tests/utils/test_utils_misc",
    "tests/utils/test_constant",
    "tests/utils/test_to_string",
    "tests/utils/test_meta_tuple",
    -- Type Traits
    "tests/type_traits/class_types/test_function_types",
    "tests/type_traits/class_types/test_member_pointers",
    "tests/type_traits/class_types/test_member_reflections",
    "tests/type_traits/class_types/test_tuple_like_types",
    -- "tests/type_traits/class_types/test_flattened_accessible_nsdm_1",
    "tests/type_traits/class_types/test_flattened_accessible_nsdm_2",
    "tests/type_traits/class_types/test_flattenable_types",
    "tests/type_traits/class_types/test_structured_types",
    "tests/type_traits/class_types/test_class_memberwise_predicate",
    "tests/type_traits/class_types/test_class_memberwise_zip_transform",
    -- Type Operations
    "tests/type_operations/test_comparison",
    "tests/type_operations/test_to_structured",
    -- Annotations
    -- "tests/annotations/test_predicates",
    "tests/annotations/test_properties",
    -- Enum
    "tests/enum/impl/test_hash_collision_check",
    "tests/enum/test_enum_misc",
    "tests/enum/test_enum_switch",
    "tests/enum/test_enum_json",
    "tests/enum/test_enum_json_static",
    "tests/enum/test_enum_type_name",
    "tests/enum/test_enum_cast_from_integer",
    "tests/enum/test_enum_name",
    "tests/enum/test_enum_name_binary_search",
    "tests/enum/test_enum_name_static",
    "tests/enum/test_enum_cast_from_string",
    "tests/enum/test_enum_cast_from_string_binary_search",
    "tests/enum/test_enum_cast_from_string_linear_search",
    "tests/enum/test_enum_cast_from_string_static",
    "tests/enum/test_enum_contains_integer",
    "tests/enum/test_enum_contains_string",
    "tests/enum/test_enum_index"
}
for i, path in ipairs(meta_test_cases) do
    local _, last_slash_index = string.find(path, ".*/")
    case_group = string.sub(path, 1, last_slash_index - 1)
    case_target_name = string.gsub(path, "/", "-")
    case_src_file_path = path .. ".cpp"

    target(case_target_name, function ()
        set_kind("binary")
        set_group(case_group)
        add_files(case_src_file_path)
        set_languages("c++26")
        add_includedirs("include")
        add_cxxflags("-freflection-latest")
        add_packages("gtest")
    end)
end
