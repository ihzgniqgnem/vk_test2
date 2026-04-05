#include "test.h"

using namespace vkm::utils;

// ============================================================================
// Basic Construction Tests
// ============================================================================

TEST(NamedArrayTest, DefaultConstructor) {
    NamedArray<TestIndex, int> arr;
    EXPECT_EQ(arr.size(), 5u);
    EXPECT_FALSE(arr.empty());
    for (size_t i = 0; i < arr.size(); ++i) {
        EXPECT_EQ(arr[TestIndex(i)], 0);
    }
}

TEST(NamedArrayTest, InitializerListConstructor) {
    NamedArray<TestIndex, int> arr{10, 20, 30, 40, 50};
    EXPECT_EQ(arr[TestIndex::Zero], 10);
    EXPECT_EQ(arr[TestIndex::One], 20);
    EXPECT_EQ(arr[TestIndex::Two], 30);
    EXPECT_EQ(arr[TestIndex::Three], 40);
    EXPECT_EQ(arr[TestIndex::Four], 50);
}

TEST(NamedArrayTest, InitializerListPartial) {
    NamedArray<TestIndex, int> arr{1, 2};
    EXPECT_EQ(arr[TestIndex::Zero], 1);
    EXPECT_EQ(arr[TestIndex::One], 2);
    EXPECT_EQ(arr[TestIndex::Two], 0);
    EXPECT_EQ(arr[TestIndex::Three], 0);
    EXPECT_EQ(arr[TestIndex::Four], 0);
}

TEST(NamedArrayTest, CopyConstructor) {
    NamedArray<TestIndex, int> arr1{1, 2, 3, 4, 5};
    NamedArray<TestIndex, int> arr2(arr1);
    EXPECT_EQ(arr1, arr2);
}

TEST(NamedArrayTest, MoveConstructor) {
    NamedArray<TestIndex, int> arr1{1, 2, 3, 4, 5};
    NamedArray<TestIndex, int> arr2(std::move(arr1));
    EXPECT_EQ(arr2[TestIndex::Zero], 1);
    EXPECT_EQ(arr2[TestIndex::One], 2);
}

TEST(NamedArrayTest, CopyAssignment) {
    NamedArray<TestIndex, int> arr1{1, 2, 3, 4, 5};
    NamedArray<TestIndex, int> arr2;
    arr2 = arr1;
    EXPECT_EQ(arr1, arr2);
}

TEST(NamedArrayTest, MoveAssignment) {
    NamedArray<TestIndex, int> arr1{1, 2, 3, 4, 5};
    NamedArray<TestIndex, int> arr2;
    arr2 = std::move(arr1);
    EXPECT_EQ(arr2[TestIndex::Zero], 1);
}

// ============================================================================
// Element Access Tests
// ============================================================================

TEST(NamedArrayTest, OperatorBracket) {
    NamedArray<TestIndex, int> arr{10, 20, 30, 40, 50};
    EXPECT_EQ(arr[TestIndex::Zero], 10);
    EXPECT_EQ(arr[TestIndex::One], 20);
    
    // Modify through operator[]
    arr[TestIndex::Zero] = 100;
    EXPECT_EQ(arr[TestIndex::Zero], 100);
}

TEST(NamedArrayTest, AtWithBoundsChecking) {
    NamedArray<TestIndex, int> arr{10, 20, 30, 40, 50};
    
    #ifndef NDEBUG
    // Should throw in debug mode
    EXPECT_THROW(arr.at(static_cast<TestIndex>(10)), std::out_of_range);
    #endif
    
    // Should work fine in all modes for valid indices
    EXPECT_EQ(arr.at(TestIndex::Two), 30);
}

TEST(NamedArrayTest, ConstAt) {
    const NamedArray<TestIndex, int> arr{10, 20, 30, 40, 50};
    EXPECT_EQ(arr.at(TestIndex::Three), 40);
}

TEST(NamedArrayTest, CompileTimeGet) {
    NamedArray<TestIndex, int> arr{10, 20, 30, 40, 50};
    EXPECT_EQ(arr.get<0>(), 10);
    EXPECT_EQ(arr.get<1>(), 20);
    EXPECT_EQ(arr.get<4>(), 50);
    
    // Modify through get
    arr.get<2>() = 300;
    EXPECT_EQ(arr[TestIndex::Two], 300);
}

TEST(NamedArrayTest, ConstCompileTimeGet) {
    const NamedArray<TestIndex, int> arr{10, 20, 30, 40, 50};
    EXPECT_EQ(arr.get<3>(), 40);
}

// ============================================================================
// Multi-element Access (ProxyArray) Tests
// ============================================================================

TEST(NamedArrayTest, ProxyArrayAccess) {
    NamedArray<TestIndex, int> arr{10, 20, 30, 40, 50};
    TestIndex indices[] = {TestIndex::Zero, TestIndex::Two, TestIndex::Four};
    
    auto proxy = arr[indices];
    EXPECT_EQ(proxy[0], 10);
    EXPECT_EQ(proxy[1], 30);
    EXPECT_EQ(proxy[2], 50);
}

TEST(NamedArrayTest, ProxyArrayModification) {
    NamedArray<TestIndex, int> arr{10, 20, 30, 40, 50};
    TestIndex indices[] = {TestIndex::Zero, TestIndex::Two};
    
    auto proxy = arr[indices];
    proxy[0] = 100;
    proxy[1] = 300;
    
    EXPECT_EQ(arr[TestIndex::Zero], 100);
    EXPECT_EQ(arr[TestIndex::Two], 300);
}

TEST(NamedArrayTest, ProxyArrayAssignmentFromArray) {
    NamedArray<TestIndex, int> arr{10, 20, 30, 40, 50};
    TestIndex indices[] = {TestIndex::Zero, TestIndex::One};
    
    int values[] = {100, 200};
    arr[indices] = values;
    
    EXPECT_EQ(arr[TestIndex::Zero], 100);
    EXPECT_EQ(arr[TestIndex::One], 200);
}

TEST(NamedArrayTest, ProxyArrayAssignmentFromInitializerList) {
    NamedArray<TestIndex, int> arr{10, 20, 30, 40, 50};
    TestIndex indices[] = {TestIndex::Zero, TestIndex::One, TestIndex::Two};
    
    arr[indices] = {100, 200, 300};
    
    EXPECT_EQ(arr[TestIndex::Zero], 100);
    EXPECT_EQ(arr[TestIndex::One], 200);
    EXPECT_EQ(arr[TestIndex::Two], 300);
}

TEST(NamedArrayTest, ProxyArrayCopyAssignment) {
    NamedArray<TestIndex, int> arr1{10, 20, 30, 40, 50};
    NamedArray<TestIndex, int> arr2{1, 2, 3, 4, 5};
    
    TestIndex indices1[] = {TestIndex::Zero, TestIndex::Two};
    TestIndex indices2[] = {TestIndex::One, TestIndex::Three};
    
    arr1[indices1] = arr2[indices2];
    
    EXPECT_EQ(arr1[TestIndex::Zero], 2);
    EXPECT_EQ(arr1[TestIndex::Two], 4);
}

TEST(NamedArrayTest, ProxyArrayFill) {
    NamedArray<TestIndex, int> arr{10, 20, 30, 40, 50};
    TestIndex indices[] = {TestIndex::Zero, TestIndex::One, TestIndex::Two};
    
    arr[indices].fill(999);
    
    EXPECT_EQ(arr[TestIndex::Zero], 999);
    EXPECT_EQ(arr[TestIndex::One], 999);
    EXPECT_EQ(arr[TestIndex::Two], 999);
    EXPECT_EQ(arr[TestIndex::Three], 40);  // Unchanged
    EXPECT_EQ(arr[TestIndex::Four], 50);   // Unchanged
}

TEST(NamedArrayTest, ProxyArraySize) {
    NamedArray<TestIndex, int> arr{10, 20, 30, 40, 50};
    TestIndex indices[] = {TestIndex::Zero, TestIndex::Two, TestIndex::Four};
    
    auto proxy = arr[indices];
    EXPECT_EQ(proxy.size(), 3u);
}

TEST(NamedArrayTest, ProxyArrayComparison) {
    NamedArray<TestIndex, int> arr1{10, 20, 30, 40, 50};
    NamedArray<TestIndex, int> arr2{10, 20, 30, 40, 50};
    NamedArray<TestIndex, int> arr3{1, 2, 3, 4, 5};
    
    TestIndex indices[] = {TestIndex::Zero, TestIndex::One};
    
    EXPECT_EQ(arr1[indices], arr2[indices]);
    EXPECT_NE(arr1[indices], arr3[indices]);
}

TEST(NamedArrayTest, ProxyArrayTemplateGet) {
    NamedArray<TestIndex, int> arr{10, 20, 30, 40, 50};
    TestIndex indices[] = {TestIndex::Zero, TestIndex::Two};
    
    auto proxy = arr[indices];
    EXPECT_EQ(proxy.get<0>(), 10);
    EXPECT_EQ(proxy.get<1>(), 30);
    
    proxy.get<0>() = 100;
    EXPECT_EQ(arr[TestIndex::Zero], 100);
}

// ============================================================================
// Fill and Data Pointer Tests
// ============================================================================

TEST(NamedArrayTest, Fill) {
    NamedArray<TestIndex, int> arr;
    arr.fill(42);
    
    for (size_t i = 0; i < arr.size(); ++i) {
        EXPECT_EQ(arr[TestIndex(i)], 42);
    }
}

TEST(NamedArrayTest, DataPointer) {
    NamedArray<TestIndex, int> arr{10, 20, 30, 40, 50};
    int* ptr = arr.data_ptr();
    
    EXPECT_EQ(ptr[0], 10);
    EXPECT_EQ(ptr[1], 20);
    EXPECT_EQ(ptr[4], 50);
    
    // Modify through pointer
    ptr[2] = 300;
    EXPECT_EQ(arr[TestIndex::Two], 300);
}

TEST(NamedArrayTest, ConstDataPointer) {
    const NamedArray<TestIndex, int> arr{10, 20, 30, 40, 50};
    const int* ptr = arr.data_ptr();
    EXPECT_EQ(ptr[3], 40);
}

// ============================================================================
// Iteration Tests
// ============================================================================

TEST(NamedArrayTest, Iteration) {
    NamedArray<TestIndex, int> arr{10, 20, 30, 40, 50};
    
    int expected = 10;
    for (auto& val : arr) {
        EXPECT_EQ(val, expected);
        expected += 10;
    }
}

TEST(NamedArrayTest, ConstIteration) {
    const NamedArray<TestIndex, int> arr{10, 20, 30, 40, 50};
    
    int sum = 0;
    for (auto it = arr.cbegin(); it != arr.cend(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 150);
}

TEST(NamedArrayTest, IteratorArithmetic) {
    NamedArray<TestIndex, int> arr{10, 20, 30, 40, 50};
    
    EXPECT_EQ(*arr.begin(), 10);
    EXPECT_EQ(*(arr.end() - 1), 50);
    EXPECT_EQ(arr.end() - arr.begin(), 5);
}

// ============================================================================
// Comparison Tests
// ============================================================================

TEST(NamedArrayTest, EqualityOperator) {
    NamedArray<TestIndex, int> arr1{10, 20, 30, 40, 50};
    NamedArray<TestIndex, int> arr2{10, 20, 30, 40, 50};
    NamedArray<TestIndex, int> arr3{1, 2, 3, 4, 5};
    
    EXPECT_EQ(arr1, arr2);
    EXPECT_NE(arr1, arr3);
}

TEST(NamedArrayTest, InequalityOperator) {
    NamedArray<TestIndex, int> arr1{10, 20, 30, 40, 50};
    NamedArray<TestIndex, int> arr2{1, 2, 3, 4, 5};
    
    EXPECT_TRUE(arr1 != arr2);
    EXPECT_FALSE(arr1 != arr1);
}

// ============================================================================
// Size and Empty Tests
// ============================================================================

TEST(NamedArrayTest, Size) {
    NamedArray<TestIndex, int> arr;
    EXPECT_EQ(arr.size(), 5u);
}

TEST(NamedArrayTest, Empty) {
    NamedArray<TestIndex, int> arr;
    EXPECT_FALSE(arr.empty());
}

// Test with zero-sized array (edge case)
enum class EmptyIndex : size_t {
    ArrayMax
};

TEST(NamedArrayTest, ZeroSizedArray) {
    // This should compile but ArrayMax must be > 0 per constraint
    // So we test with minimum size instead
    NamedArray<ColorIndex, double> arr;
    EXPECT_EQ(arr.size(), 3u);
}

// ============================================================================
// Different Type Tests
// ============================================================================

TEST(NamedArrayTest, DoubleType) {
    NamedArray<ColorIndex, double> arr{1.1, 2.2, 3.3};
    EXPECT_DOUBLE_EQ(arr[ColorIndex::Red], 1.1);
    EXPECT_DOUBLE_EQ(arr[ColorIndex::Green], 2.2);
    EXPECT_DOUBLE_EQ(arr[ColorIndex::Blue], 3.3);
}

TEST(NamedArrayTest, StringType) {
    NamedArray<ColorIndex, std::string> arr;
    arr[ColorIndex::Red] = "red";
    arr[ColorIndex::Green] = "green";
    arr[ColorIndex::Blue] = "blue";
    
    EXPECT_EQ(arr[ColorIndex::Red], "red");
    EXPECT_EQ(arr[ColorIndex::Green], "green");
    EXPECT_EQ(arr[ColorIndex::Blue], "blue");
}

// ============================================================================
// Hash Function Tests
// ============================================================================

TEST(NamedArrayTest, HashFunction) {
    NamedArray<TestIndex, int> arr1{10, 20, 30, 40, 50};
    NamedArray<TestIndex, int> arr2{10, 20, 30, 40, 50};
    NamedArray<TestIndex, int> arr3{1, 2, 3, 4, 5};
    
    std::hash<NamedArray<TestIndex, int>> hasher;
    
    EXPECT_EQ(hasher(arr1), hasher(arr2));
    EXPECT_NE(hasher(arr1), hasher(arr3));
}

// ============================================================================
// Edge Cases and Stress Tests
// ============================================================================

TEST(NamedArrayTest, AllElementsAccess) {
    NamedArray<TestIndex, int> arr;
    
    // Write to all elements
    for (size_t i = 0; i < arr.size(); ++i) {
        arr[TestIndex(i)] = static_cast<int>(i * 100);
    }
    
    // Verify all elements
    for (size_t i = 0; i < arr.size(); ++i) {
        EXPECT_EQ(arr[TestIndex(i)], static_cast<int>(i * 100));
    }
}

TEST(NamedArrayTest, MultipleProxyArrays) {
    NamedArray<TestIndex, int> arr{1, 2, 3, 4, 5};
    
    TestIndex indices1[] = {TestIndex::Zero, TestIndex::One};
    TestIndex indices2[] = {TestIndex::Two, TestIndex::Three};
    
    auto proxy1 = arr[indices1];
    auto proxy2 = arr[indices2];
    
    proxy1[0] = 10;
    proxy2[1] = 40;
    
    EXPECT_EQ(arr[TestIndex::Zero], 10);
    EXPECT_EQ(arr[TestIndex::Three], 40);
}

TEST(NamedArrayTest, ChainedOperations) {
    NamedArray<TestIndex, int> arr{1, 2, 3, 4, 5};
    
    TestIndex indices[] = {TestIndex::Zero, TestIndex::One, TestIndex::Two};
    
    // Chain fill and verify
    arr[indices].fill(999);
    EXPECT_EQ(arr[TestIndex::Zero], 999);
    EXPECT_EQ(arr[TestIndex::One], 999);
    EXPECT_EQ(arr[TestIndex::Two], 999);
}

// ============================================================================
// Const Correctness Tests
// ============================================================================

TEST(NamedArrayTest, ConstCorrectness) {
    const NamedArray<TestIndex, int> arr{10, 20, 30, 40, 50};
    
    // These should compile with const methods
    [[maybe_unused]] auto val1 = arr[TestIndex::Zero];
    [[maybe_unused]] auto val2 = arr.at(TestIndex::One);
    [[maybe_unused]] auto val3 = arr.get<2>();
    [[maybe_unused]] auto ptr = arr.data_ptr();
    [[maybe_unused]] auto sz = arr.size();
    [[maybe_unused]] auto empty = arr.empty();
    
    // Verify values
    EXPECT_EQ(val1, 10);
    EXPECT_EQ(val2, 20);
    EXPECT_EQ(val3, 30);
    EXPECT_EQ(ptr[3], 40);
    EXPECT_EQ(sz, 5u);
    EXPECT_FALSE(empty);
}
