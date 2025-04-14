#pragma once

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

// Note: Keyword 'not' may be inavailable in MSVC.
#define NOT !

#ifdef ENABLE_STATIC_CHECK
#define ASSERT_EQ_STATIC(expected, actual) static_assert(expected == actual)
#define EXPECT_EQ_STATIC(expected, actual) static_assert(expected == actual)
#define ASSERT_NE_STATIC(expected, actual) static_assert(expected != actual)
#define EXPECT_NE_STATIC(expected, actual) static_assert(expected != actual)
#define ASSERT_FALSE_STATIC(cond) static_assert(!(cond))
#define EXPECT_FALSE_STATIC(cond) static_assert(!(cond))
#define ASSERT_TRUE_STATIC(cond) static_assert(cond)
#define EXPECT_TRUE_STATIC(cond) static_assert(cond)
#else
#define ASSERT_EQ_STATIC(expected, actual) ASSERT_EQ(expected, actual)
#define EXPECT_EQ_STATIC(expected, actual) EXPECT_EQ(expected, actual)
#define ASSERT_NE_STATIC(expected, actual) ASSERT_NE(expected, actual)
#define EXPECT_NE_STATIC(expected, actual) EXPECT_NE(expected, actual)
#define ASSERT_FALSE_STATIC(cond) ASSERT_FALSE(cond)
#define EXPECT_FALSE_STATIC(cond) EXPECT_FALSE(cond)
#define ASSERT_TRUE_STATIC(cond) ASSERT_TRUE(cond)
#define EXPECT_TRUE_STATIC(cond) EXPECT_TRUE(cond)
#endif
