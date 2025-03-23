add_rules("mode.debug", "mode.release")

add_requires("gtest", {
    config = { main = true }
})

meta_test_cases = {
    -- Ordered by dependency of tested components
    "tests/utils/test_preprocessors",
    "tests/utils/test_utils_misc",
    "tests/utils/test_to_string",
    "tests/utils/test_to_string_overloads",
    "tests/utils/test_meta_tuple",

    "tests/type_traits/class_types/test_member_pointers",
    "tests/type_traits/class_types/test_tuple_like_types",
    "tests/type_traits/class_types/test_flattened_accessible_nsdm_1",
    "tests/type_traits/class_types/test_flattened_types",
    "tests/type_traits/class_types/test_structured_types",

    "tests/annotations/test_predicates",
    "tests/annotations/test_properties",

    "tests/enum/impl/test_hash_collision_check",
    "tests/enum/test_enum_misc",
}
for i, path in ipairs(meta_test_cases) do
    case_target_name = string.gsub(path, "/", "-")
    case_src_file_path = path .. ".cpp"
    target(case_target_name, function ()
        set_kind("binary")
        add_files(case_src_file_path)
        set_languages("c++26")
        add_includedirs("include")
        add_cxxflags("-freflection-latest")
        add_packages("gtest")
    end)
end
