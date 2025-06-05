//
// Created by Halberg, Spencer on 6/4/25.
//

#include "gtest/gtest.h"
#include "../Matrix.h"


class MatrixTest : public ::testing::Test
{
protected:
void
SetUp() override
{
    matrix = new Matrix(3);
    matrix->setElement(0, 0, 1.0);
    matrix->setElement(0, 1, 2.0);
    matrix->setElement(0, 2, 3.0);
    matrix->setElement(1, 0, 4.0);
    matrix->setElement(1, 1, 5.0);
    matrix->setElement(1, 2, 6.0);
    matrix->setElement(2, 0, 7.0);
    matrix->setElement(2, 1, 8.0);
    matrix->setElement(2, 2, 9.0);
}

void
TearDown() override
{
    delete matrix;
}

Matrix *matrix;
};

TEST_F(MatrixTest, ConstructorTest)
{
    EXPECT_NO_THROW(Matrix(3));
    EXPECT_NO_THROW(Matrix());
}

TEST_F(MatrixTest, SetElementTest)
{
    EXPECT_NO_THROW(matrix->setElement(0, 0, 1.0));
    EXPECT_NO_THROW(matrix->setElement(2, 2, 1.0));
    EXPECT_THROW(matrix->setElement(3, 0, 1.0), std::out_of_range);
    EXPECT_THROW(matrix->setElement(0, 3, 1.0), std::out_of_range);
    EXPECT_THROW(matrix->setElement(-1, 0, 1.0), std::out_of_range);
}

TEST_F(MatrixTest, GetElementTest)
{
    matrix->setElement(1, 1, 5.0);
    EXPECT_DOUBLE_EQ(matrix->getElement(1, 1), 5.0);
    EXPECT_THROW(matrix->getElement(3, 0), std::out_of_range);
    EXPECT_THROW(matrix->getElement(0, 3), std::out_of_range);
    EXPECT_THROW(matrix->getElement(-1, 0), std::out_of_range);
}

TEST_F(MatrixTest, SwapRowsTest)
{
    matrix->setElement(0, 0, 1.0);
    matrix->setElement(1, 0, 2.0);
    EXPECT_NO_THROW(matrix->swapRows(0, 1));
    EXPECT_DOUBLE_EQ(matrix->getElement(0, 0), 2.0);
    EXPECT_DOUBLE_EQ(matrix->getElement(1, 0), 1.0);
    EXPECT_THROW(matrix->swapRows(0, 3), std::out_of_range);
}

TEST_F(MatrixTest, SwapColsTest)
{
    matrix->setElement(0, 0, 1.0);
    matrix->setElement(0, 1, 2.0);
    EXPECT_THROW(matrix->swapCols(0, 3), std::out_of_range);
    EXPECT_NO_THROW(matrix->swapCols(0, 1));
    EXPECT_DOUBLE_EQ(matrix->getElement(0, 0), 2.0);
    EXPECT_DOUBLE_EQ(matrix->getElement(0, 1), 1.0);
    EXPECT_DOUBLE_EQ(matrix->getElement(0, 2), 3.0);
    EXPECT_DOUBLE_EQ(matrix->getElement(1, 0), 5.0);
    EXPECT_DOUBLE_EQ(matrix->getElement(1, 1), 4.0);
    EXPECT_DOUBLE_EQ(matrix->getElement(1, 2), 6.0);
    EXPECT_DOUBLE_EQ(matrix->getElement(2, 0), 8.0);
    EXPECT_DOUBLE_EQ(matrix->getElement(2, 1), 7.0);
    EXPECT_DOUBLE_EQ(matrix->getElement(2, 2), 9.0);
}

TEST_F(MatrixTest, ReorderRowsTest)
{
    std::map<int, int> validPerm = {{0, 2}, {1, 1}, {2, 0}};
    std::map<int, int> invalidPerm = {{0, 3}};
    EXPECT_NO_THROW(matrix->reorderRows(&validPerm));
    EXPECT_DOUBLE_EQ(matrix->getElement(0, 0), 7.0);
    EXPECT_DOUBLE_EQ(matrix->getElement(0, 1), 8.0);
    EXPECT_DOUBLE_EQ(matrix->getElement(0, 2), 9.0);
    EXPECT_DOUBLE_EQ(matrix->getElement(1, 0), 4.0);
    EXPECT_DOUBLE_EQ(matrix->getElement(1, 1), 5.0);
    EXPECT_DOUBLE_EQ(matrix->getElement(1, 2), 6.0);
    EXPECT_DOUBLE_EQ(matrix->getElement(2, 0), 1.0);
    EXPECT_DOUBLE_EQ(matrix->getElement(2, 1), 2.0);
    EXPECT_DOUBLE_EQ(matrix->getElement(2, 2), 3.0);
    EXPECT_THROW(matrix->reorderRows(&invalidPerm), std::out_of_range);
    EXPECT_THROW(matrix->reorderRows(nullptr), std::invalid_argument);

}

TEST_F(MatrixTest, ReorderColsTest)
{
    std::map<int, int> validPerm = {{0, 2}, {1, 1}, {2, 0}};
    std::map<int, int> invalidPerm = {{0, 3}};
    EXPECT_NO_THROW(matrix->reorderCols(&validPerm));
    EXPECT_DOUBLE_EQ(matrix->getElement(0, 0), 3.0);
    EXPECT_DOUBLE_EQ(matrix->getElement(0, 1), 2.0);
    EXPECT_DOUBLE_EQ(matrix->getElement(0, 2), 1.0);
    EXPECT_DOUBLE_EQ(matrix->getElement(1, 0), 6.0);
    EXPECT_DOUBLE_EQ(matrix->getElement(1, 1), 5.0);
    EXPECT_DOUBLE_EQ(matrix->getElement(1, 2), 4.0);
    EXPECT_DOUBLE_EQ(matrix->getElement(2, 0), 9.0);
    EXPECT_DOUBLE_EQ(matrix->getElement(2, 1), 8.0);
    EXPECT_DOUBLE_EQ(matrix->getElement(2, 2), 7.0);

    EXPECT_THROW(matrix->reorderCols(&invalidPerm), std::out_of_range);
    EXPECT_THROW(matrix->reorderCols(nullptr), std::invalid_argument);
}

