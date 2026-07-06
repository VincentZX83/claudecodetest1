// ============================================================
// test_typelist.cpp — Compile-time assertion tests for TypeList
// ============================================================

#include <gtest/gtest.h>
#include <type_traits>
#include "typelist.hpp"

using namespace cccs;

using EmptyList      = TypeList<>;
using IntFloatDouble = TypeList<int, float, double>;
using MixedTypes     = TypeList<int, double, char, short, long>;
using AllInts        = TypeList<int, short, long, unsigned>;

// ========== Basic Operations ==========

TEST(TypeListTest, Size) {
    static_assert(EmptyList::size == 0);
    static_assert(IntFloatDouble::size == 3);
    static_assert(MixedTypes::size == 5);
    SUCCEED();
}

TEST(TypeListTest, Front) {
    static_assert(std::is_same_v<Front_t<IntFloatDouble>, int>);
    static_assert(std::is_same_v<Front_t<MixedTypes>, int>);
    SUCCEED();
}

TEST(TypeListTest, PopFront) {
    using AfterPop = PopFront_t<IntFloatDouble>;
    static_assert(AfterPop::size == 2);
    static_assert(std::is_same_v<Front_t<AfterPop>, float>);
    SUCCEED();
}

TEST(TypeListTest, PushFront) {
    using WithBool = PushFront_t<IntFloatDouble, bool>;
    static_assert(WithBool::size == 4);
    static_assert(std::is_same_v<Front_t<WithBool>, bool>);
    SUCCEED();
}

TEST(TypeListTest, PushBack) {
    using Extended = PushBack_t<IntFloatDouble, char>;
    static_assert(Extended::size == 4);
    using Reversed = Reverse_t<Extended>;
    static_assert(std::is_same_v<Front_t<Reversed>, char>);
    SUCCEED();
}

// ========== Advanced: NthType, Contains, IndexOf ==========

TEST(TypeListTest, NthType) {
    static_assert(std::is_same_v<NthType_t<MixedTypes, 0>, int>);
    static_assert(std::is_same_v<NthType_t<MixedTypes, 1>, double>);
    static_assert(std::is_same_v<NthType_t<MixedTypes, 2>, char>);
    static_assert(std::is_same_v<NthType_t<MixedTypes, 4>, long>);
    SUCCEED();
}

TEST(TypeListTest, Contains) {
    static_assert(Contains_v<MixedTypes, double>);
    static_assert(!Contains_v<MixedTypes, bool>);
    static_assert(!Contains_v<EmptyList, int>);
    SUCCEED();
}

TEST(TypeListTest, IndexOf) {
    static_assert(IndexOf_v<MixedTypes, int>    == 0);
    static_assert(IndexOf_v<MixedTypes, double> == 1);
    static_assert(IndexOf_v<MixedTypes, long>   == 4);
    static_assert(IndexOf_v<MixedTypes, bool>   == -1);
    static_assert(IndexOf_v<EmptyList, int>     == -1);
    SUCCEED();
}

// ========== Predicates: AllOf, AnyOf ==========

TEST(TypeListTest, AllOf_Integral) {
    static_assert(AllOf_v<TypeList<int, short>, IsIntegral>);
    static_assert(!AllOf_v<TypeList<int, float>, IsIntegral>);
    static_assert(AllOf_v<EmptyList, IsIntegral>);  // vacuous truth
    SUCCEED();
}

TEST(TypeListTest, AnyOf) {
    static_assert(AnyOf_v<MixedTypes, IsFloatingPoint>);
    static_assert(!AnyOf_v<AllInts, IsFloatingPoint>);
    static_assert(!AnyOf_v<EmptyList, IsIntegral>);
    SUCCEED();
}

// ========== Transform, Filter, Reverse ==========

TEST(TypeListTest, Transform_AddConst) {
    using ConstList = Transform_t<IntFloatDouble, AddConst>;
    static_assert(std::is_same_v<Front_t<ConstList>, const int>);
    SUCCEED();
}

TEST(TypeListTest, Filter_Integral) {
    using OnlyIntegral = Filter_t<MixedTypes, IsIntegral>;
    static_assert(OnlyIntegral::size == 4);  // int, char, short, long
    static_assert(!Contains_v<OnlyIntegral, double>);
    SUCCEED();
}

TEST(TypeListTest, Reverse_List) {
    using Reversed = Reverse_t<MixedTypes>;
    static_assert(std::is_same_v<Front_t<Reversed>, long>);
    static_assert(std::is_same_v<NthType_t<Reversed, 4>, int>);
    SUCCEED();
}

// ========== Concatenate, GetSize ==========

TEST(TypeListTest, Concatenate) {
    using Combined = Concatenate_t<TypeList<int, float>, TypeList<char, double>>;
    static_assert(Combined::size == 4);
    static_assert(std::is_same_v<Front_t<Combined>, int>);
    static_assert(std::is_same_v<NthType_t<Combined, 3>, double>);
    SUCCEED();
}

TEST(TypeListTest, GetSize) {
    static_assert(GetSize_v<TypeList<int, char>> == sizeof(int) + sizeof(char));
    static_assert(GetSize_v<TypeList<int, double, char>> ==
                  sizeof(int) + sizeof(double) + sizeof(char));
    SUCCEED();
}

// ========== Real-world use: Tuple from TypeList ==========

TEST(TypeListTest, TypeListToTuple) {
    // Demonstrates bridging compile-time TypeList to runtime std::tuple
    using MyTypes = TypeList<int, double, char>;
    using MyTuple = std::tuple<int, double, char>;

    static_assert(std::tuple_size_v<MyTuple> == MyTypes::size);
    static_assert(std::is_same_v<std::tuple_element_t<0, MyTuple>, int>);
    static_assert(std::is_same_v<std::tuple_element_t<1, MyTuple>, double>);
    SUCCEED();
}
