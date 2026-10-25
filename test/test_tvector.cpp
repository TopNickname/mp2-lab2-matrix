#include <gtest.h>
#include "tmatrix.h"

#include <sstream>
#include <stdexcept>
#include <limits>
#include <cstddef>
#include <utility>

// ---------- Конструирование ----------

TEST(TDynamicVectorExpanded, default_ctor_has_size_one_and_zero_value)
{
    TDynamicVector<int> v;
    EXPECT_EQ(static_cast<size_t>(1), v.size());
    EXPECT_EQ(0, v[0]);
}

TEST(TDynamicVectorExpanded, ctor_zero_size_throws_out_of_range)
{
    EXPECT_THROW(TDynamicVector<int> v(0), std::out_of_range);
}

TEST(TDynamicVectorExpanded, ctor_too_large_throws_length_error)
{
    EXPECT_THROW(
        TDynamicVector<int> v(static_cast<size_t>(MAX_VECTOR_SIZE) + 1),
        std::length_error
    );
}

TEST(TDynamicVectorExpanded, ctor_from_negative_size_t_throws_length_error)
{
    // -5 -> огромное size_t; должно упасть в length_error, а не в out_of_range
    EXPECT_THROW(
        TDynamicVector<int> v(static_cast<size_t>(-5)),
        std::length_error
    );
}

TEST(TDynamicVectorExpanded, arr_ctor_copies_values_and_does_not_alias)
{
    int arr[3] = { 1, 2, 3 };
    TDynamicVector<int> v(arr, 3);

    EXPECT_EQ(static_cast<size_t>(3), v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
    EXPECT_EQ(3, v[2]);

    arr[0] = 100;
    EXPECT_EQ(1, v[0]);
}

TEST(TDynamicVectorExpanded, arr_ctor_zero_size_throws_out_of_range)
{
    int arr[1] = { 7 };
    EXPECT_THROW(TDynamicVector<int> v(arr, 0), std::out_of_range);
}

TEST(TDynamicVectorExpanded, arr_ctor_too_large_throws_length_error)
{
    int arr[1] = { 7 };
    EXPECT_THROW(
        TDynamicVector<int> v(arr, std::numeric_limits<size_t>::max()),
        std::length_error
    );
}

// ---------- Копирование / перемещение / присваивание ----------

TEST(TDynamicVectorExpanded, copy_ctor_equal_and_independent)
{
    TDynamicVector<int> v(3);
    v[0] = 1; v[1] = 2; v[2] = 3;

    TDynamicVector<int> c(v);

    EXPECT_EQ(v, c);
    c[0] = 100;
    EXPECT_NE(v, c);
    EXPECT_EQ(1, v[0]);
}

TEST(TDynamicVectorExpanded, move_ctor_transfers_values)
{
    TDynamicVector<int> v(3);
    v[0] = 1; v[1] = 2; v[2] = 3;

    TDynamicVector<int> c(std::move(v));

    EXPECT_EQ(static_cast<size_t>(3), c.size());
    EXPECT_EQ(1, c[0]);
    EXPECT_EQ(2, c[1]);
    EXPECT_EQ(3, c[2]);
}

TEST(TDynamicVectorExpanded, copy_assignment_self_is_safe)
{
    TDynamicVector<int> v(3);
    v[0] = 1; v[1] = 2; v[2] = 3;

    v = v;

    EXPECT_EQ(static_cast<size_t>(3), v.size());
    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
    EXPECT_EQ(3, v[2]);
}

TEST(TDynamicVectorExpanded, copy_assignment_different_size_changes_size_and_values)
{
    TDynamicVector<int> a(5), b(3);
    b[0] = 10; b[1] = 20; b[2] = 30;

    a = b;

    EXPECT_EQ(static_cast<size_t>(3), a.size());
    EXPECT_EQ(10, a[0]);
    EXPECT_EQ(20, a[1]);
    EXPECT_EQ(30, a[2]);
}

TEST(TDynamicVectorExpanded, move_assignment_transfers_values)
{
    TDynamicVector<int> a(3);
    a[0] = 1; a[1] = 2; a[2] = 3;

    TDynamicVector<int> b(1);
    b = std::move(a);

    EXPECT_EQ(static_cast<size_t>(3), b.size());
    EXPECT_EQ(1, b[0]);
    EXPECT_EQ(2, b[1]);
    EXPECT_EQ(3, b[2]);
}

// ---------- Доступ ----------

// Проверяем только корректные индексы; operator[] за границей — UB, не тестируем.
TEST(TDynamicVectorExpanded, index_valid_get_and_set)
{
    TDynamicVector<int> v(3);
    v[0] = 1;
    v[1] = 2;
    v[2] = 3;

    const TDynamicVector<int>& cv = v;
    EXPECT_EQ(1, cv[0]);
    EXPECT_EQ(2, cv[1]);
    EXPECT_EQ(3, cv[2]);
}

TEST(TDynamicVectorExpanded, index_first_and_last_are_valid)
{
    TDynamicVector<int> v(3);
    v[0] = 10;
    v[2] = 30;

    const TDynamicVector<int>& cv = v;
    EXPECT_EQ(10, cv[0]);
    EXPECT_EQ(30, cv[2]);
}

TEST(TDynamicVectorExpanded, at_valid_get_and_set)
{
    TDynamicVector<int> v(3);
    v.at(0) = 1;
    v.at(1) = 2;
    v.at(2) = 3;

    const TDynamicVector<int>& cv = v;
    EXPECT_EQ(1, cv.at(0));
    EXPECT_EQ(2, cv.at(1));
    EXPECT_EQ(3, cv.at(2));
}

TEST(TDynamicVectorExpanded, at_out_of_range_throws_out_of_range)
{
    TDynamicVector<int> v(3);
    const TDynamicVector<int>& cv = v;

    EXPECT_THROW(v.at(3), std::out_of_range);
    EXPECT_THROW(cv.at(3), std::out_of_range);
    EXPECT_THROW(v.at(4), std::out_of_range);
    EXPECT_THROW(cv.at(4), std::out_of_range);
    // огромный индекс — тоже корректно должно бросать (это уже не UB, т.к. at проверяет)
    EXPECT_THROW(v.at(std::numeric_limits<size_t>::max()), std::out_of_range);
    EXPECT_THROW(cv.at(std::numeric_limits<size_t>::max()), std::out_of_range);
}

// ---------- Сравнение ----------

TEST(TDynamicVectorExpanded, equality_and_inequality)
{
    TDynamicVector<int> a(3), b(3), c(4);
    a[0] = 1; a[1] = 2; a[2] = 3;
    b[0] = 1; b[1] = 2; b[2] = 3;

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);

    b[2] = 4;
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);

    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a != c);

    EXPECT_TRUE(a == a);
}

// ---------- Скалярные операции ----------

TEST(TDynamicVectorExpanded, scalar_ops_return_new_vectors_and_do_not_modify_original)
{
    TDynamicVector<int> v(3);
    v[0] = 1; v[1] = 2; v[2] = 3;

    TDynamicVector<int> added = v + 10;
    TDynamicVector<int> subbed = v - 10;
    TDynamicVector<int> muled = v * 2;

    EXPECT_EQ(11, added[0]);
    EXPECT_EQ(12, added[1]);
    EXPECT_EQ(13, added[2]);

    EXPECT_EQ(-9, subbed[0]);
    EXPECT_EQ(-8, subbed[1]);
    EXPECT_EQ(-7, subbed[2]);

    EXPECT_EQ(2, muled[0]);
    EXPECT_EQ(4, muled[1]);
    EXPECT_EQ(6, muled[2]);

    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
    EXPECT_EQ(3, v[2]);
}

// ---------- Векторные операции ----------

TEST(TDynamicVectorExpanded, vector_add_sub_dot_equal_size)
{
    TDynamicVector<int> a(3), b(3);
    a[0] = 1; a[1] = 2; a[2] = 3;
    b[0] = 4; b[1] = 5; b[2] = 6;

    TDynamicVector<int> sum = a + b;
    TDynamicVector<int> dif = a - b;
    int dot = a * b;

    EXPECT_EQ(5, sum[0]);
    EXPECT_EQ(7, sum[1]);
    EXPECT_EQ(9, sum[2]);

    EXPECT_EQ(-3, dif[0]);
    EXPECT_EQ(-3, dif[1]);
    EXPECT_EQ(-3, dif[2]);

    EXPECT_EQ(32, dot);

    EXPECT_EQ(1, a[0]);
    EXPECT_EQ(2, a[1]);
    EXPECT_EQ(3, a[2]);
}

TEST(TDynamicVectorExpanded, vector_ops_mismatch_throw_length_error)
{
    TDynamicVector<int> a(2), b(3);

    EXPECT_THROW(a + b, std::length_error);
    EXPECT_THROW(a - b, std::length_error);
    EXPECT_THROW(a * b, std::length_error);
}

// ---------- swap ----------

TEST(TDynamicVectorExpanded, swap_swaps_contents_and_sizes)
{
    TDynamicVector<int> a(2), b(3);
    a[0] = 1; a[1] = 2;
    b[0] = 10; b[1] = 20; b[2] = 30;

    swap(a, b);

    EXPECT_EQ(static_cast<size_t>(3), a.size());
    EXPECT_EQ(static_cast<size_t>(2), b.size());

    EXPECT_EQ(10, a[0]);
    EXPECT_EQ(20, a[1]);
    EXPECT_EQ(30, a[2]);

    EXPECT_EQ(1, b[0]);
    EXPECT_EQ(2, b[1]);
}

// ---------- Ввод/вывод ----------

TEST(TDynamicVectorExpanded, input_and_output)
{
    TDynamicVector<int> v(3);
    std::stringstream in("1 2 3");
    in >> v;

    EXPECT_EQ(1, v[0]);
    EXPECT_EQ(2, v[1]);
    EXPECT_EQ(3, v[2]);

    std::stringstream out;
    out << v;

    EXPECT_EQ("1 2 3 ", out.str());
}