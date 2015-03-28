// A set of simple testing macros:
//
//    EXPECT_EQ(fingers_per_hand(), toes_per_foot())
//    EXPECT_FALSE(v.empty())
//    etc.

#ifndef H_TESTING
#define H_TESTING

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cinttypes>
#include <iostream>
#include <type_traits>

#define EXPECT_OP(op, A, B) do { \
        auto && a = (A);                                              \
    auto && b = (B);                                                  \
    if (!(a op b)) {                                                  \
        std::cout << "Expected " #A " " #op " " #B " but got: '"      \
                  << std::boolalpha << a << "' vs '" << b << "'.\n";  \
       std::cout << "Line: " __FILE__ ":" << __LINE__ << "\n";        \
    }                                                                 \
} while (false)

#define EXPECT_EQ(A, B) EXPECT_OP(==, A, B)
#define EXPECT_NE(A, B) EXPECT_OP(==, A, B)
#define EXPECT_TRUE(A) EXPECT_OP(==, A, true)
#define EXPECT_FALSE(A) EXPECT_OP(==, A, false)

// Floating point comparison tolerates one ULP difference.
#define EXPECT_FLOAT_EQ(A, B) EXPECT_TRUE(float_eq(A, B, 2))

template <typename T, typename U>
T bitcopy(U && u)
{
    static_assert(sizeof(T) == sizeof(U),
                  "Source and destination types have different sizes.");

    static_assert(std::is_trivial<T>::value,
                  "Destination type is not trivial.");

    // (Compiler support for this trait is lacking at the moment.)
    // static_assert(std::is_trivially_copyable<std::decay<U>::type>::value,
    //               "Source type is not trivially copyable.");

    T t;
    std::copy_n(reinterpret_cast<unsigned char const *>(&u),   // from
                sizeof(T),                                     // how much
                reinterpret_cast<unsigned char *>(&t));        // to
    return t;
}

bool float_eq(double a, double b, std::uint64_t ulp = 1, std::uint64_t * ulpdiff = nullptr)
{
    if (ulpdiff) { *ulpdiff = 0; }

    // Handle NaN.
    if (std::isunordered(a, b)) { return false; }

    // Handle +/- zero.
    if ((a < 0 && 0 <= b) || (b < 0 && 0 <= a)) { return a == b; }

    // Handle infinity (by now, a and b must have the same sign).
    if (std::isinf(a) && std::isinf(b)) { return true; }

    auto ia = bitcopy<std::uint64_t>(a), ib = bitcopy<std::uint64_t>(b);
    std::uint64_t diff = ia < ib ? ib - ia : ia - ib;
    if (ulpdiff) { *ulpdiff = diff; }
    return diff < ulp;
}

#endif