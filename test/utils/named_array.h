#pragma once
#include <type_traits>
#include <gtest/gtest.h>
#include "../../header/utils/named_array.h"

namespace vkm_test::utils::NamedArray {
    TEST(NamedArray, small_array)
    {
        enum class Index {
            A, B,
            ArrayMax
        };
        vkm::utils::NamedArray<Index, int> x;
        EXPECT_EQ(sizeof(x), 2 * sizeof(int));
    }

    TEST(NamedArray, big_array)
    {
        enum class Index {
            ArrayMax = 1000
        };
        vkm::utils::NamedArray<Index, char> x;
        EXPECT_EQ(sizeof(x), 1000u);
    }

    TEST(NamedArray, init)
    {
        enum class Index {
            A, B, C, D, E, F, G, H, I, J,
            ArrayMax
        };
        alignas(vkm::utils::NamedArray<Index, int>) unsigned char buffer[sizeof(vkm::utils::NamedArray<Index, int>)];
        std::memset(buffer, 0xff, sizeof(buffer));
        auto* y = new(buffer) vkm::utils::NamedArray<Index, int>;
        for (int i = 0; i < static_cast<int>(Index::ArrayMax); ++i) {
            EXPECT_EQ((*y)[static_cast<Index>(i)], 0);
        }
        y->~NamedArray();
    }

    TEST(NamedArray, basic_attr)
    {
        enum class Index {
            A, B, C, D, E, F, G, H, I, J,
            ArrayMax
        };
        using NamedArrayType = vkm::utils::NamedArray<Index, int>;

        EXPECT_TRUE(std::is_default_constructible_v<NamedArrayType>);
        EXPECT_TRUE(std::is_copy_constructible_v<NamedArrayType>);
        EXPECT_TRUE(std::is_copy_assignable_v<NamedArrayType>);
        EXPECT_TRUE(std::is_move_constructible_v<NamedArrayType>);
        EXPECT_TRUE(std::is_move_assignable_v<NamedArrayType>);
        EXPECT_TRUE(std::is_destructible_v<NamedArrayType>);

        EXPECT_TRUE(std::is_trivially_copyable_v<NamedArrayType>);

        EXPECT_TRUE(std::is_nothrow_default_constructible_v<NamedArrayType>);
        EXPECT_TRUE(std::is_nothrow_copy_constructible_v<NamedArrayType>);
        EXPECT_TRUE(std::is_nothrow_copy_assignable_v<NamedArrayType>);
        EXPECT_TRUE(std::is_nothrow_move_constructible_v<NamedArrayType>);
        EXPECT_TRUE(std::is_nothrow_move_assignable_v<NamedArrayType>);
        EXPECT_TRUE(std::is_nothrow_destructible_v<NamedArrayType>);

        EXPECT_TRUE(std::is_standard_layout_v<NamedArrayType>);
    }

    TEST(NamedArray, op_eq)
    {
        enum class Index {
            A, B, C, D, E, F, G, H, I, J,
            ArrayMax
        };
        vkm::utils::NamedArray<Index, int> x, y;

        EXPECT_TRUE(x == y);
        x[Index::A] = 1;
        EXPECT_TRUE(x != y);
        y[Index::A] = 1;
        EXPECT_TRUE(x == y);
    }

    TEST(NamedArray, write)
    {
        enum class Index {
            A, B, C, D, E, F, G, H, I, J,
            ArrayMax
        };
        vkm::utils::NamedArray<Index, int> x;
        x[Index::A] = 10;
        EXPECT_EQ(x[Index::A], 10);
        x[Index::B] = 20;
        EXPECT_EQ(x[Index::B], 20);
        x[Index::H] = 80;
        EXPECT_EQ(x[Index::H], 80);
        x[Index::I] = 90;
        EXPECT_EQ(x[Index::I], 90);
        x[Index::J] = 100;
        EXPECT_EQ(x[Index::A], 10);
        EXPECT_EQ(x[Index::B], 20);
        EXPECT_EQ(x[Index::H], 80);
        EXPECT_EQ(x[Index::I], 90);
        EXPECT_EQ(x[Index::J], 100);

        auto [a, b] = x[{Index::A, Index::J}];
        a = 1000;
        b = 2000;
        EXPECT_EQ(x[Index::A], 1000);
        EXPECT_EQ(x[Index::J], 2000);
    }

    TEST(NamedArray, batch_write)
    {
        enum class Index {
            A, B, C, D, E, F, G, H, I, J,
            ArrayMax
        };
        vkm::utils::NamedArray<Index, int> x;
        x[{Index::A, Index::B, Index::H, Index::I, Index::J}] = { 1, 2, 8, 9, 10 };
        EXPECT_EQ(x[Index::A], 1);
        EXPECT_EQ(x[Index::B], 2);
        EXPECT_EQ(x[Index::H], 8);
        EXPECT_EQ(x[Index::I], 9);
        EXPECT_EQ(x[Index::J], 10);

        x[{Index::A, Index::J}] = { -1, -10 };
        EXPECT_EQ(x[Index::A], -1);
        EXPECT_EQ(x[Index::B], 2);
        EXPECT_EQ(x[Index::H], 8);
        EXPECT_EQ(x[Index::I], 9);
        EXPECT_EQ(x[Index::J], -10);
    }

    TEST(NamedArray, read)
    {
        enum class Index {
            A, B, C, D, E, F, G, H, I, J,
            ArrayMax
        };
        vkm::utils::NamedArray<Index, int> x;
        x[{Index::A, Index::B, Index::H, Index::I, Index::J}] = { 1, 2, 8, 9, 10 };
        const vkm::utils::NamedArray<Index, int>& y = const_cast<const vkm::utils::NamedArray<Index, int>&>(x);
        auto [a, b] = y[{Index::A, Index::C}];
        EXPECT_EQ(a, 1);
        EXPECT_EQ(b, 0);
        EXPECT_TRUE((std::is_same_v<std::remove_cvref_t<decltype(a)>, int>));
        auto [c, d] = x[{Index::A, Index::C}];
        EXPECT_TRUE((std::is_same_v<std::remove_cvref_t<decltype(c)>, int>));
    }

    template<typename>
    constexpr bool t = false;
    template<typename T>
        requires requires{ std::declval<vkm::utils::NamedArray<T, int>>(); }
    constexpr bool t<T> = true;

    TEST(NamedArray, compiling_wrong_parameter)
    {
        class Index1 {};
        EXPECT_FALSE(t<Index1>);
        enum class Index2 {
            A
        };
        EXPECT_FALSE(t<Index2>);
        enum class Index3 {
            ArrayMax
        };
        EXPECT_FALSE(t<Index3>);
        enum class Index4 {
            A, B, C, D, E, F, G, H, I, J,
            ArrayMax
        };
        EXPECT_TRUE(t<Index4>);
    }

    TEST(NamedArray, hash)
    {
        enum class Index {
            A, B, C, D, E, F, G, H, I, J,
            ArrayMax
        };
        vkm::utils::NamedArray<Index, int> x;
        auto a = std::hash<decltype(x)>{}(x);
        x[Index::A] = 1;
        auto b = std::hash<decltype(x)>{}(x);
        x[Index::A] = 0;
        x[Index::I] = 9;
        auto c = std::hash<decltype(x)>{}(x);
        EXPECT_NE(a, b);
        EXPECT_NE(b, c);
        auto y = x;
        EXPECT_EQ(std::hash<decltype(x)>{}(x), std::hash<decltype(y)>{}(y));
    }

    TEST(NamedArray, proxy_array_assign)
    {
        enum class Index {
            A, B, C, D, E, F, G, H, I, J,
            ArrayMax
        };
        vkm::utils::NamedArray<Index, int> x;
        x[Index::A] = 1;
        x[Index::I] = 9;
        x[{Index::A, Index::I}] = x[{Index::I, Index::A}];
        EXPECT_EQ(x[Index::A], 9);
        EXPECT_EQ(x[Index::I], 1);
    }
}