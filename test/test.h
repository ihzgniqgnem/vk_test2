#pragma once
#include <gtest/gtest.h>
#include "../header/static_assertions.h"
#include "../header/utils/named_array.h"
#include "../header/utils/is_enum_class.h"

// Test enum for NamedArray
enum class TestIndex : size_t {
    Zero,
    One,
    Two,
    Three,
    Four,
    ArrayMax  // Must be last
};

// Another test enum with different size
enum class ColorIndex : size_t {
    Red,
    Green,
    Blue,
    ArrayMax
};

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}