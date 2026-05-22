#pragma once
#include <gtest/gtest.h>
#include "../../header/utils/func_table.h"

namespace vkm_test::utils::FuncTable{
    // 测试用的枚举类
    enum class TestFuncEnum {
        Func0,
        Func1, 
        Func2,
        FuncMax = 3
    };

    // 测试用的函数类型
    using TestFuncType = vkm::utils::FuncTableType<
        void(*)(),
        int(*)(int),
        const char*(*)(const char*)
    >;

    // 模拟的测试函数
    void testFunc0() {}
    int testFunc1(int x) { return x + 1; }
    const char* testFunc2(const char* s) { return s; }

    TEST(FuncTableTest, DefaultConstruction) {
        vkm::utils::FuncTable<TestFuncEnum, TestFuncType> table;
        EXPECT_EQ(table.size(), size_t(3));
        
        // 默认构造的表应该都是nullptr
        for (size_t i = 0; i < table.size(); ++i) {
            EXPECT_EQ(table[static_cast<TestFuncEnum>(i)], static_cast<void*>(nullptr));
        }
    }

    TEST(FuncTableTest, AssignmentAndAccess) {
        vkm::utils::FuncTable<TestFuncEnum, TestFuncType> table;
        
        // 使用Proxy赋值
        table[TestFuncEnum::Func0] = testFunc0;
        table[TestFuncEnum::Func1] = testFunc1;
        table[TestFuncEnum::Func2] = testFunc2;
        
        // 验证赋值结果
        EXPECT_EQ(table[TestFuncEnum::Func0], reinterpret_cast<void*>(testFunc0));
        EXPECT_EQ(table[TestFuncEnum::Func1], reinterpret_cast<void*>(testFunc1));
        EXPECT_EQ(table[TestFuncEnum::Func2], reinterpret_cast<void*>(testFunc2));
        
        // 使用get方法访问
        auto& func0 = table.template get<TestFuncEnum::Func0>();
        auto& func1 = table.template get<TestFuncEnum::Func1>();
        auto& func2 = table.template get<TestFuncEnum::Func2>();
        
        EXPECT_EQ(func0, testFunc0);
        EXPECT_EQ(func1, testFunc1);
        EXPECT_EQ(func2, testFunc2);
        
        // 测试函数调用
        func0();
        EXPECT_EQ(func1(5), 6);
        EXPECT_STREQ(func2("hello"), "hello");
    }

    TEST(FuncTableTest, CopyAndMoveSemantics) {
        vkm::utils::FuncTable<TestFuncEnum, TestFuncType> original;
        original[TestFuncEnum::Func0] = testFunc0;
        original[TestFuncEnum::Func1] = testFunc1;
        
        // 测试拷贝构造
        vkm::utils::FuncTable<TestFuncEnum, TestFuncType> copy(original);
        EXPECT_EQ(copy[TestFuncEnum::Func0], original[TestFuncEnum::Func0]);
        EXPECT_EQ(copy[TestFuncEnum::Func1], original[TestFuncEnum::Func1]);
        
        // 测试拷贝赋值
        vkm::utils::FuncTable<TestFuncEnum, TestFuncType> copy_assign;
        copy_assign = original;
        EXPECT_EQ(copy_assign[TestFuncEnum::Func0], original[TestFuncEnum::Func0]);
        EXPECT_EQ(copy_assign[TestFuncEnum::Func1], original[TestFuncEnum::Func1]);
        
        // 测试移动构造
        vkm::utils::FuncTable<TestFuncEnum, TestFuncType> move(std::move(original));
        EXPECT_EQ(move[TestFuncEnum::Func0], reinterpret_cast<void*>(testFunc0));
        EXPECT_EQ(move[TestFuncEnum::Func1], reinterpret_cast<void*>(testFunc1));
        
        // 原对象应该仍然有效（因为是默认移动语义）
        EXPECT_EQ(original[TestFuncEnum::Func0], reinterpret_cast<void*>(testFunc0));
    }

    TEST(FuncTableTest, EqualityComparison) {
        vkm::utils::FuncTable<TestFuncEnum, TestFuncType> table1;
        vkm::utils::FuncTable<TestFuncEnum, TestFuncType> table2;
        
        // 两个默认构造的表应该相等
        EXPECT_TRUE(table1 == table2);
        
        // 给table1赋值
        table1[TestFuncEnum::Func0] = testFunc0;
        table1[TestFuncEnum::Func1] = testFunc1;
        
        // 现在应该不相等
        EXPECT_FALSE(table1 == table2);
        
        // 给table2赋相同的值
        table2[TestFuncEnum::Func0] = testFunc0;
        table2[TestFuncEnum::Func1] = testFunc1;
        
        // 现在应该相等
        EXPECT_TRUE(table1 == table2);
    }

    TEST(FuncTableTest, IteratorInterface) {
        vkm::utils::FuncTable<TestFuncEnum, TestFuncType> table;
        table[TestFuncEnum::Func0] = testFunc0;
        table[TestFuncEnum::Func1] = testFunc1;
        table[TestFuncEnum::Func2] = testFunc2;
        
        // 测试begin/end迭代器
        size_t count = 0;
        for (auto it = table.begin(); it != table.end(); ++it) {
            EXPECT_NE(*it, static_cast<void*>(nullptr));
            count++;
        }
        EXPECT_EQ(count, size_t(3));
        
        // 测试const迭代器
        const auto& const_table = table;
        count = 0;
        for (auto it = const_table.begin(); it != const_table.end(); ++it) {
            EXPECT_NE(*it, static_cast<void*>(nullptr));
            count++;
        }
        EXPECT_EQ(count, size_t(3));
    }

    TEST(FuncTableTest, HashFunction) {
        vkm::utils::FuncTable<TestFuncEnum, TestFuncType> table1;
        vkm::utils::FuncTable<TestFuncEnum, TestFuncType> table2;
        
        // 相同内容应该有相同哈希值
        std::hash<vkm::utils::FuncTable<TestFuncEnum, TestFuncType>> hasher;
        EXPECT_EQ(hasher(table1), hasher(table2));
        
        // 不同内容应该有不同的哈希值（大概率）
        table1[TestFuncEnum::Func0] = testFunc0;
        EXPECT_NE(hasher(table1), hasher(table2));
    }

    TEST(FuncTableTest, TupleInterface) {
        vkm::utils::FuncTable<TestFuncEnum, TestFuncType> table;
        table[TestFuncEnum::Func0] = testFunc0;
        table[TestFuncEnum::Func1] = testFunc1;
        
        // 测试tuple_size
        EXPECT_EQ(std::tuple_size_v<decltype(table)>, size_t(3));
        
        // 测试tuple_element
        using ElementType0 = std::tuple_element_t<0, decltype(table)>;
        using ElementType1 = std::tuple_element_t<1, decltype(table)>;
        using ElementType2 = std::tuple_element_t<2, decltype(table)>;
        
        EXPECT_TRUE((std::is_same_v<ElementType0, void(*)()>));
        EXPECT_TRUE((std::is_same_v<ElementType1, int(*)(int)>));
        EXPECT_TRUE((std::is_same_v<ElementType2, const char*(*)(const char*)>));
    }
}