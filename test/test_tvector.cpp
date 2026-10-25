#include <sstream>
#include "gtest.h"
#include "tmatrix.h"

TEST(TDynamicVector, can_create_vector_with_positive_length)
{
    ASSERT_NO_THROW(TDynamicVector<int> v(5));
}

TEST(TDynamicVector, cant_create_too_large_vector)
{
    ASSERT_ANY_THROW(TDynamicVector<int> v(MAX_VECTOR_SIZE + 1));
}

TEST(TDynamicVector, throws_when_create_vector_with_negative_length)
{
    ASSERT_ANY_THROW(TDynamicVector<int> v(-5));
}

TEST(TDynamicVector, can_create_copied_vector)
{
    TDynamicVector<int> v(10);

    ASSERT_NO_THROW(TDynamicVector<int> v1(v));
}

TEST(TDynamicVector, can_move_vector_using_move_constructor)
{
    TDynamicVector<int> v(5);
    for (int i = 0; i < 5; i++) v[i] = i;

    TDynamicVector<int> v1(std::move(v));

    ASSERT_EQ(5, v1.size());
    for (int i = 0; i < 5; i++) EXPECT_EQ(i, v1[i]);
}

TEST(TDynamicVector, can_move_vector_using_move_operator)
{
    TDynamicVector<int> v(5);
    for (int i = 0; i < 5; i++) v[i] = i;

    TDynamicVector<int> v1;
    v1 = std::move(v);

    ASSERT_EQ(5, v1.size());
    for (int i = 0; i < 5; i++) EXPECT_EQ(i, v1[i]);
}

TEST(TDynamicVector, copied_vector_is_equal_to_source_one)
{
    TDynamicVector<int> v(5);
    for (int i = 0; i < 5; i++) v[i] = i;

    TDynamicVector<int> v1(v);

    EXPECT_EQ(v, v1);
}

TEST(TDynamicVector, copied_vector_has_its_own_memory)
{
    TDynamicVector<int> v(5);
    for (int i = 0; i < 5; i++) v[i] = i;

    TDynamicVector<int> v1(v);
    v1[0] = 100;

    EXPECT_NE(v, v1);
    EXPECT_EQ(0, v[0]);
    EXPECT_EQ(100, v1[0]);
}

TEST(TDynamicVector, can_get_size)
{
    TDynamicVector<int> v(4);

    EXPECT_EQ(4, v.size());
}

TEST(TDynamicVector, can_set_and_get_element)
{
  TDynamicVector<int> v(4);
  v[0] = 4;

  EXPECT_EQ(4, v[0]);
}

TEST(TDynamicVector, throws_when_set_element_with_negative_index)
{
    TDynamicVector<int> v(5);

    ASSERT_ANY_THROW(v[-1] = 4);
}

TEST(TDynamicVector, throws_when_set_element_with_too_large_index)
{
    TDynamicVector<int> v(5);

    ASSERT_ANY_THROW(v[5] = 4);
    ASSERT_ANY_THROW(v[100] = 4);
}

TEST(TDynamicVector, can_assign_vector_to_itself)
{
    TDynamicVector<int> v(5);
    for (int i = 0; i < 5; i++) v[i] = i;

    ASSERT_NO_THROW(v = v);

    ASSERT_EQ(5, v.size());
    for (int i = 0; i < 5; i++) EXPECT_EQ(i, v[i]);
}

TEST(TDynamicVector, can_assign_vectors_of_equal_size)
{
    TDynamicVector<int> v1(5), v2(5);
    for (int i = 0; i < 5; i++) {
        v1[i] = i;
        v2[i] = i + 10;
    }

    v1 = v2;

    ASSERT_EQ(5, v1.size());
    EXPECT_EQ(v2, v1);
}

TEST(TDynamicVector, assign_operator_change_vector_size)
{
    TDynamicVector<int> v1(5), v2(3);

    v1 = v2;

    EXPECT_EQ(3, v1.size());
}

TEST(TDynamicVector, can_assign_vectors_of_different_size)
{
    TDynamicVector<int> v1(5), v2(3);
    for (int i = 0; i < 3; i++) v2[i] = i + 10;

    v1 = v2;

    ASSERT_EQ(3, v1.size());
    for (int i = 0; i < 3; i++) EXPECT_EQ(i + 10, v1[i]);
}

TEST(TDynamicVector, compare_equal_vectors_return_true)
{
    TDynamicVector<int> v1(5), v2(5);
    for (int i = 0; i < 5; i++) {
        v1[i] = i;
        v2[i] = i;
    }

    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 != v2);
}

TEST(TDynamicVector, compare_vector_with_itself_return_true)
{
    TDynamicVector<int> v(5);
    for (int i = 0; i < 5; i++) v[i] = i;

    EXPECT_TRUE(v == v);
}

TEST(TDynamicVector, vectors_with_different_size_are_not_equal)
{
    TDynamicVector<int> v1(5), v2(3);

    EXPECT_FALSE(v1 == v2);
    EXPECT_TRUE(v1 != v2);
}

TEST(TDynamicVector, can_add_scalar_to_vector)
{
    TDynamicVector<int> v(3);
    v[0] = 1; v[1] = 2; v[2] = 3;

    TDynamicVector<int> res = v + 10;

    ASSERT_EQ(3, res.size());
    EXPECT_EQ(11, res[0]);
    EXPECT_EQ(12, res[1]);
    EXPECT_EQ(13, res[2]);
}

TEST(TDynamicVector, can_subtract_scalar_from_vector)
{
    TDynamicVector<int> v(3);
    v[0] = 11; v[1] = 12; v[2] = 13;

    TDynamicVector<int> res = v - 10;

    ASSERT_EQ(3, res.size());
    EXPECT_EQ(1, res[0]);
    EXPECT_EQ(2, res[1]);
    EXPECT_EQ(3, res[2]);
}

TEST(TDynamicVector, can_multiply_scalar_by_vector)
{
    TDynamicVector<int> v(3);
    v[0] = 1; v[1] = 2; v[2] = 3;

    TDynamicVector<int> res = v * 2;

    ASSERT_EQ(3, res.size());
    EXPECT_EQ(2, res[0]);
    EXPECT_EQ(4, res[1]);
    EXPECT_EQ(6, res[2]);
}

TEST(TDynamicVector, can_add_vectors_with_equal_size)
{
    TDynamicVector<int> v1(3), v2(3);
    v1[0] = 1; v1[1] = 2; v1[2] = 3;
    v2[0] = 4; v2[1] = 5; v2[2] = 6;

    TDynamicVector<int> res = v1 + v2;

    ASSERT_EQ(3, res.size());
    EXPECT_EQ(5, res[0]);
    EXPECT_EQ(7, res[1]);
    EXPECT_EQ(9, res[2]);
}

TEST(TDynamicVector, cant_add_vectors_with_not_equal_size)
{
    TDynamicVector<int> v1(3), v2(5);

    ASSERT_ANY_THROW(v1 + v2);
}

TEST(TDynamicVector, can_subtract_vectors_with_equal_size)
{
    TDynamicVector<int> v1(3), v2(3);
    v1[0] = 5; v1[1] = 7; v1[2] = 9;
    v2[0] = 4; v2[1] = 5; v2[2] = 6;

    TDynamicVector<int> res = v1 - v2;

    ASSERT_EQ(3, res.size());
    EXPECT_EQ(1, res[0]);
    EXPECT_EQ(2, res[1]);
    EXPECT_EQ(3, res[2]);
}

TEST(TDynamicVector, cant_subtract_vectors_with_not_equal_size)
{
    TDynamicVector<int> v1(3), v2(5);

    ASSERT_ANY_THROW(v1 - v2);
}

TEST(TDynamicVector, can_multiply_vectors_with_equal_size)
{
    TDynamicVector<int> v1(3), v2(3);
    v1[0] = 1; v1[1] = 2; v1[2] = 3;
    v2[0] = 4; v2[1] = 5; v2[2] = 6;

    int res = v1 * v2;

    EXPECT_EQ(32, res);
}

TEST(TDynamicVector, cant_multiply_vectors_with_not_equal_size)
{
    TDynamicVector<int> v1(3), v2(5);

    ASSERT_ANY_THROW(v1 * v2);
}

TEST(TDynamicVector, can_read_vector)
{
    TDynamicVector<int> v(5);
    TDynamicVector<int> expected(5);
    for (int i = 0; i < 5; i++) expected[i] = i+1;

    std::stringstream sstr;
    sstr << "1 2 3 4 5\n";
    sstr >> v;

    EXPECT_EQ(expected, v);
}

TEST(TDynamicVector, can_write_vector)
{
    TDynamicVector<int> v(5);
    for (int i = 0; i < 5; i++) v[i] = i+1;
    std::string expected = "1 2 3 4 5 ";

    std::stringstream sstr;
    sstr << v;

    EXPECT_EQ(expected, sstr.str());
}