#pragma once

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

// Note: Keyword 'not' may be inavailable in MSVC.
#define NOT !

#ifdef ENABLE_STATIC_TEST
#define CONSTEVAL_ON_STATIC_TEST consteval

#define ASSERT_EQ_STATIC(expected, ...) static_assert(expected == (__VA_ARGS__))
#define EXPECT_EQ_STATIC(expected, ...) static_assert(expected == (__VA_ARGS__))
#define ASSERT_NE_STATIC(expected, ...) static_assert(expected != (__VA_ARGS__))
#define EXPECT_NE_STATIC(expected, ...) static_assert(expected != (__VA_ARGS__))
#define ASSERT_FALSE_STATIC(...) static_assert(!(__VA_ARGS__))
#define EXPECT_FALSE_STATIC(...) static_assert(!(__VA_ARGS__))
#define ASSERT_TRUE_STATIC(...) static_assert(__VA_ARGS__)
#define EXPECT_TRUE_STATIC(...) static_assert(__VA_ARGS__)
#else
#define CONSTEVAL_ON_STATIC_TEST

#define ASSERT_EQ_STATIC(expected, ...) ASSERT_EQ(expected, (__VA_ARGS__))
#define EXPECT_EQ_STATIC(expected, ...) EXPECT_EQ(expected, (__VA_ARGS__))
#define ASSERT_NE_STATIC(expected, ...) ASSERT_NE(expected, (__VA_ARGS__))
#define EXPECT_NE_STATIC(expected, ...) EXPECT_NE(expected, (__VA_ARGS__))
#define ASSERT_FALSE_STATIC(...) ASSERT_FALSE((__VA_ARGS__))
#define EXPECT_FALSE_STATIC(...) EXPECT_FALSE((__VA_ARGS__))
#define ASSERT_TRUE_STATIC(...) ASSERT_TRUE((__VA_ARGS__))
#define EXPECT_TRUE_STATIC(...) EXPECT_TRUE((__VA_ARGS__))
#endif
