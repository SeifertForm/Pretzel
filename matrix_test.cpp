#include "matrix.hpp"
#include "testing.hpp"

void TestConstruct()
{
    matrix<float> m(2, 3);
    EXPECT_EQ(m.rows(), 2U);
    EXPECT_EQ(m.cols(), 3U);
    EXPECT_EQ(m(0, 0), 0U);
}

void TestTranspose()
{
    matrix<double> m(2, 3);
    m(0, 0) = 1; m(0, 1) = 2; m(0, 2) = 3;
    m(1, 0) = 9; m(1, 1) = 8; m(1, 2) = 7;

    matrix<double> mt = m.transpose();

    for (std::size_t i = 0; i != m.rows(); ++i)
        for (std::size_t j = 0; j != m.cols(); ++j)
            EXPECT_EQ(m(i, j), mt(j, i));
}

void TestVandermonde()
{
    square_matrix<int> m = vandermonde<int>({2, 3, 5});
    EXPECT_EQ(m(1, 0), 1);
    EXPECT_EQ(m(1, 1), 3);
    EXPECT_EQ(m(1, 2), 9);
}

void TestDeterminant()
{
    square_matrix<float> m(2);
    m(0, 0) = 2; m(0, 1) = 7;
    m(1, 0) = -3; m(1, 1) = -1;
    EXPECT_EQ(m.determinant(), 19);
}

void TestGaussJordanElimination()
{
    // Data from Wikipedia (http://en.wikipedia.org/wiki/Gaussian_elimination)
    matrix<double> m(3, 4);
    m(0, 0) =  2; m(0, 1) =  1; m(0, 2) = -1; m(0, 3) =   8;
    m(1, 0) = -3; m(1, 1) = -1; m(1, 2) =  2; m(1, 3) = -11;
    m(2, 0) = -2; m(2, 1) =  1; m(2, 2) =  2; m(2, 3) =  -3;

    matrix<double> mgj = m.gauss_jordan();

    EXPECT_FLOAT_EQ(mgj(0, 3),  2);
    EXPECT_FLOAT_EQ(mgj(1, 3),  3);
    EXPECT_FLOAT_EQ(mgj(2, 3), -1);
}

int main()
{
    TestConstruct();
    TestTranspose();
    TestVandermonde();
    TestDeterminant();
    TestGaussJordanElimination();
}