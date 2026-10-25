#include <gtest.h>
#include "tmatrix.h"

#include <sstream>
#include <stdexcept>
#include <limits>
#include <cstddef>
#include <utility>

namespace {

    void fill_matrix(TDynamicMatrix<int>& m, int start = 1)
    {
        int value = start;
        for (size_t i = 0; i < m.size(); ++i)
            for (size_t j = 0; j < m.size(); ++j)
                m[i][j] = value++;
    }

} 

// ---------- Конструирование ----------

TEST(TDynamicMatrixExpanded, default_ctor_has_size_one_and_zero_value)
{
    TDynamicMatrix<int> m;
    EXPECT_EQ(static_cast<size_t>(1), m.size());
    EXPECT_EQ(0, m[0][0]);
}

TEST(TDynamicMatrixExpanded, ctor_zero_size_throws_out_of_range)
{
    EXPECT_THROW(TDynamicMatrix<int> m(0), std::out_of_range);
}

TEST(TDynamicMatrixExpanded, ctor_too_large_throws_length_error)
{
    EXPECT_THROW(
        TDynamicMatrix<int> m(static_cast<size_t>(MAX_MATRIX_SIZE) + 1),
        std::length_error
    );
}

TEST(TDynamicMatrixExpanded, ctor_from_negative_size_t_throws_length_error)
{
    EXPECT_THROW(
        TDynamicMatrix<int> m(static_cast<size_t>(-5)),
        std::length_error
    );
}

// ---------- Копирование / перемещение / присваивание ----------

TEST(TDynamicMatrixExpanded, copy_ctor_equal_and_independent)
{
    TDynamicMatrix<int> m(2);
    fill_matrix(m, 1);

    TDynamicMatrix<int> c(m);

    EXPECT_EQ(m, c);

    c[0][0] = 100;
    EXPECT_NE(m, c);
    EXPECT_EQ(1, m[0][0]);
    EXPECT_EQ(100, c[0][0]);
}

TEST(TDynamicMatrixExpanded, move_ctor_transfers_values)
{
    TDynamicMatrix<int> m(2);
    fill_matrix(m, 1);

    TDynamicMatrix<int> c(std::move(m));

    EXPECT_EQ(static_cast<size_t>(2), c.size());
    EXPECT_EQ(1, c[0][0]);
    EXPECT_EQ(2, c[0][1]);
    EXPECT_EQ(3, c[1][0]);
    EXPECT_EQ(4, c[1][1]);
}

TEST(TDynamicMatrixExpanded, copy_assignment_self_is_safe)
{
    TDynamicMatrix<int> m(2);
    fill_matrix(m, 1);

    m = m;

    EXPECT_EQ(static_cast<size_t>(2), m.size());
    EXPECT_EQ(1, m[0][0]);
    EXPECT_EQ(2, m[0][1]);
    EXPECT_EQ(3, m[1][0]);
    EXPECT_EQ(4, m[1][1]);
}

TEST(TDynamicMatrixExpanded, copy_assignment_different_size_changes_size_and_values)
{
    TDynamicMatrix<int> a(3), b(2);
    fill_matrix(b, 10);

    a = b;

    EXPECT_EQ(static_cast<size_t>(2), a.size());
    EXPECT_EQ(10, a[0][0]);
    EXPECT_EQ(11, a[0][1]);
    EXPECT_EQ(12, a[1][0]);
    EXPECT_EQ(13, a[1][1]);
}

TEST(TDynamicMatrixExpanded, move_assignment_transfers_values)
{
    TDynamicMatrix<int> a(2);
    fill_matrix(a, 1);

    TDynamicMatrix<int> b(1);
    b = std::move(a);

    EXPECT_EQ(static_cast<size_t>(2), b.size());
    EXPECT_EQ(1, b[0][0]);
    EXPECT_EQ(2, b[0][1]);
    EXPECT_EQ(3, b[1][0]);
    EXPECT_EQ(4, b[1][1]);
}

// ---------- Доступ ----------

TEST(TDynamicMatrixExpanded, index_valid_get_and_set)
{
    TDynamicMatrix<int> m(2);

    m[0][0] = 1;
    m[0][1] = 2;
    m[1][0] = 3;
    m[1][1] = 4;

    EXPECT_EQ(1, m[0][0]);
    EXPECT_EQ(2, m[0][1]);
    EXPECT_EQ(3, m[1][0]);
    EXPECT_EQ(4, m[1][1]);
}

// ---------- Сравнение ----------

TEST(TDynamicMatrixExpanded, equality_and_inequality)
{
    TDynamicMatrix<int> a(2), b(2), c(3);
    fill_matrix(a, 1);
    fill_matrix(b, 1);

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);

    b[0][0] = 100;
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);

    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a != c);

    EXPECT_TRUE(a == a);
}

// ---------- Матрично-скалярные ----------

TEST(TDynamicMatrixExpanded, scalar_multiply_returns_new_matrix_and_does_not_modify_original)
{
    TDynamicMatrix<int> m(2);
    fill_matrix(m, 1);

    TDynamicMatrix<int> r = m * 2;

    EXPECT_EQ(2, r[0][0]);
    EXPECT_EQ(4, r[0][1]);
    EXPECT_EQ(6, r[1][0]);
    EXPECT_EQ(8, r[1][1]);

    EXPECT_EQ(1, m[0][0]);
    EXPECT_EQ(2, m[0][1]);
    EXPECT_EQ(3, m[1][0]);
    EXPECT_EQ(4, m[1][1]);
}

// ---------- Матрично-векторные ----------

TEST(TDynamicMatrixExpanded, matrix_vector_multiply_equal_size)
{
    TDynamicMatrix<int> m(2);
    fill_matrix(m, 1); // [[1,2],[3,4]]

    TDynamicVector<int> v(2);
    v[0] = 5;
    v[1] = 6;

    TDynamicVector<int> r = m * v;

    EXPECT_EQ(static_cast<size_t>(2), r.size());
    EXPECT_EQ(17, r[0]); // 1*5 + 2*6
    EXPECT_EQ(39, r[1]); // 3*5 + 4*6
}

TEST(TDynamicMatrixExpanded, matrix_vector_multiply_mismatch_throws_length_error)
{
    TDynamicMatrix<int> m(2);

    TDynamicVector<int> too_large(3);
    TDynamicVector<int> too_small(1);

    EXPECT_THROW(m * too_large, std::length_error);
    EXPECT_THROW(m * too_small, std::length_error);
}

// ---------- Матрично-матричные ----------

TEST(TDynamicMatrixExpanded, matrix_add_sub_equal_size)
{
    TDynamicMatrix<int> a(2), b(2);
    fill_matrix(a, 1);  // [[1,2],[3,4]]
    fill_matrix(b, 10); // [[10,11],[12,13]]

    TDynamicMatrix<int> sum = a + b;
    TDynamicMatrix<int> dif = a - b;

    EXPECT_EQ(11, sum[0][0]);
    EXPECT_EQ(13, sum[0][1]);
    EXPECT_EQ(15, sum[1][0]);
    EXPECT_EQ(17, sum[1][1]);

    EXPECT_EQ(-9, dif[0][0]);
    EXPECT_EQ(-9, dif[0][1]);
    EXPECT_EQ(-9, dif[1][0]);
    EXPECT_EQ(-9, dif[1][1]);

    EXPECT_EQ(1, a[0][0]);
    EXPECT_EQ(2, a[0][1]);
    EXPECT_EQ(3, a[1][0]);
    EXPECT_EQ(4, a[1][1]);
}

TEST(TDynamicMatrixExpanded, matrix_add_sub_mismatch_throws_length_error_in_both_directions)
{
    TDynamicMatrix<int> small(2), large(3);

    EXPECT_THROW(small + large, std::length_error);
    EXPECT_THROW(large + small, std::length_error);

    EXPECT_THROW(small - large, std::length_error);
    EXPECT_THROW(large - small, std::length_error);
}

TEST(TDynamicMatrixExpanded, matrix_multiply_equal_size)
{
    TDynamicMatrix<int> a(2), b(2);
    fill_matrix(a, 1); // [[1,2],[3,4]]
    fill_matrix(b, 5); // [[5,6],[7,8]]

    TDynamicMatrix<int> p = a * b;

    EXPECT_EQ(static_cast<size_t>(2), p.size());

    // [[1*5+2*7, 1*6+2*8], [3*5+4*7, 3*6+4*8]]
    EXPECT_EQ(19, p[0][0]);
    EXPECT_EQ(22, p[0][1]);
    EXPECT_EQ(43, p[1][0]);
    EXPECT_EQ(50, p[1][1]);

    EXPECT_EQ(1, a[0][0]);
    EXPECT_EQ(2, a[0][1]);
    EXPECT_EQ(3, a[1][0]);
    EXPECT_EQ(4, a[1][1]);
}

TEST(TDynamicMatrixExpanded, matrix_multiply_mismatch_throws_length_error_in_both_directions)
{
    TDynamicMatrix<int> small(2), large(3);

    EXPECT_THROW(small * large, std::length_error);
    EXPECT_THROW(large * small, std::length_error);
}

// ---------- Ввод/вывод ----------

TEST(TDynamicMatrixExpanded, input_and_output)
{
    TDynamicMatrix<int> m(2);
    fill_matrix(m, 1); // [[1,2],[3,4]]

    std::stringstream out;
    out << m;
    EXPECT_EQ("1 2 \n3 4 \n", out.str());

    TDynamicMatrix<int> r(2);
    std::stringstream in("5 6 7 8");
    in >> r;

    EXPECT_EQ(5, r[0][0]);
    EXPECT_EQ(6, r[0][1]);
    EXPECT_EQ(7, r[1][0]);
    EXPECT_EQ(8, r[1][1]);
}