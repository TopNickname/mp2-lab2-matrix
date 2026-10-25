#include "tmatrix.h"
#include <gtest.h>
#include <sstream>

TEST(TDynamicMatrix, can_create_matrix_with_positive_length)
{
    ASSERT_NO_THROW(TDynamicMatrix<int> m(5));
}

TEST(TDynamicMatrix, cant_create_too_large_matrix)
{
    ASSERT_ANY_THROW(TDynamicMatrix<int> m(MAX_MATRIX_SIZE + 1));
}

TEST(TDynamicMatrix, throws_when_create_matrix_with_negative_length)
{
    ASSERT_ANY_THROW(TDynamicMatrix<int> m(-5));
}

TEST(TDynamicMatrix, can_create_copied_matrix)
{
    TDynamicMatrix<int> m(5);

    ASSERT_NO_THROW(TDynamicMatrix<int> m1(m));
}

TEST(TDynamicMatrix, copied_matrix_is_equal_to_source_one)
{
    TDynamicMatrix<int> m(3);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            m[i][j] = i * 3 + j;

    TDynamicMatrix<int> m1(m);

    EXPECT_EQ(m, m1);
}

TEST(TDynamicMatrix, copied_matrix_has_its_own_memory)
{
    TDynamicMatrix<int> m(3);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            m[i][j] = i * 3 + j;

    TDynamicMatrix<int> m1(m);
    m1[0][0] = 100;

    EXPECT_NE(m, m1);
    EXPECT_EQ(0, m[0][0]);
    EXPECT_EQ(100, m1[0][0]);
}

TEST(TDynamicMatrix, can_get_size)
{
    TDynamicMatrix<int> m(4);

    EXPECT_EQ(4, m.size());
}

TEST(TDynamicMatrix, can_set_and_get_element)
{
    TDynamicMatrix<int> m(3);

    m[0][0] = 1;
    m[1][2] = 5;
    m[2][1] = 9;

    EXPECT_EQ(1, m[0][0]);
    EXPECT_EQ(5, m[1][2]);
    EXPECT_EQ(9, m[2][1]);
}

TEST(TDynamicMatrix, throws_when_set_element_with_negative_index)
{
    TDynamicMatrix<int> m(3);

    ASSERT_ANY_THROW(m[-1][0] = 4);
    ASSERT_ANY_THROW(m[0][-1] = 4);
}

TEST(TDynamicMatrix, throws_when_set_element_with_too_large_index)
{
    TDynamicMatrix<int> m(3);

    ASSERT_ANY_THROW(m[3][0] = 4);
    ASSERT_ANY_THROW(m[0][3] = 4);
    ASSERT_ANY_THROW(m[100][0] = 4);
}

TEST(TDynamicMatrix, can_assign_matrix_to_itself)
{
    TDynamicMatrix<int> m(3);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            m[i][j] = i * 3 + j;

    ASSERT_NO_THROW(m = m);

    ASSERT_EQ(3, m.size());
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            EXPECT_EQ(i * 3 + j, m[i][j]);
}

TEST(TDynamicMatrix, can_assign_matrices_of_equal_size)
{
    TDynamicMatrix<int> m1(3), m2(3);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            m1[i][j] = i * 3 + j;
            m2[i][j] = i * 3 + j + 10;
        }
    }

    m1 = m2;

    ASSERT_EQ(3, m1.size());
    EXPECT_EQ(m2, m1);
}

TEST(TDynamicMatrix, assign_operator_change_matrix_size)
{
    TDynamicMatrix<int> m1(5), m2(3);

    m1 = m2;

    EXPECT_EQ(3, m1.size());
}

TEST(TDynamicMatrix, can_assign_matrices_of_different_size)
{
    TDynamicMatrix<int> m1(5), m2(3);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            m2[i][j] = i * 3 + j + 10;

    m1 = m2;

    ASSERT_EQ(3, m1.size());
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            EXPECT_EQ(i * 3 + j + 10, m1[i][j]);
}

TEST(TDynamicMatrix, compare_equal_matrices_return_true)
{
    TDynamicMatrix<int> m1(3), m2(3);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            m1[i][j] = i * 3 + j;
            m2[i][j] = i * 3 + j;
        }
    }

    EXPECT_TRUE(m1 == m2);
    EXPECT_FALSE(m1 != m2);
}

TEST(TDynamicMatrix, compare_matrix_with_itself_return_true)
{
    TDynamicMatrix<int> m(3);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            m[i][j] = i * 3 + j;

    EXPECT_TRUE(m == m);
}

TEST(TDynamicMatrix, matrices_with_different_size_are_not_equal)
{
    TDynamicMatrix<int> m1(5), m2(3);

    EXPECT_FALSE(m1 == m2);
    EXPECT_TRUE(m1 != m2);
}

TEST(TDynamicMatrix, can_add_matrices_with_equal_size)
{
    TDynamicMatrix<int> m1(3), m2(3);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            m1[i][j] = i * 3 + j;
            m2[i][j] = 10 + i * 3 + j;
        }
    }

    TDynamicMatrix<int> res = m1 + m2;

    ASSERT_EQ(3, res.size());
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            EXPECT_EQ(10 + 2 * (i * 3 + j), res[i][j]);
}

TEST(TDynamicMatrix, cant_add_matrices_with_not_equal_size)
{
    TDynamicMatrix<int> m1(3), m2(5);

    ASSERT_ANY_THROW(m1 + m2);
}

TEST(TDynamicMatrix, can_subtract_matrices_with_equal_size)
{
    TDynamicMatrix<int> m1(3), m2(3);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            m1[i][j] = 10 + i * 3 + j;
            m2[i][j] = i * 3 + j;
        }
    }

    TDynamicMatrix<int> res = m1 - m2;

    ASSERT_EQ(3, res.size());
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            EXPECT_EQ(10, res[i][j]);
}

TEST(TDynamicMatrix, cant_subtract_matrixes_with_not_equal_size)
{
    TDynamicMatrix<int> m1(3), m2(5);

    ASSERT_ANY_THROW(m1 - m2);
}

TEST(TDynamicMatrix, can_read_matrix)
{
    TDynamicMatrix<int> m(3);
    TDynamicMatrix<int> expected(3);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            expected[i][j] = i * 3 + j + 1;   // [1..9]

    std::stringstream sstr;
    sstr << "1 2 3 4 5 6 7 8 9\n";
    sstr >> m;

    EXPECT_EQ(expected, m);
}

TEST(TDynamicMatrix, can_write_matrix)
{
    TDynamicMatrix<int> m(3);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            m[i][j] = i * 3 + j + 1;          // [1..9]
    std::string expected = "1 2 3 \n4 5 6 \n7 8 9 \n";

    std::stringstream sstr;
    sstr << m;

    EXPECT_EQ(expected, sstr.str());
}
