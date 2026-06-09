#include "../includes/matrix.hpp"
#include "../includes/vector.hpp"
#include <iostream>
#include <stdexcept>

int main() {
    std::cout << "===== matrix * vector =====\n";

    // subject: identity * {4,2} -> {4,2}
    mx::Matrix<float> i2 = {{1.f, 0.f}, {0.f, 1.f}};
    mx::Vector<float> v = {4.f, 2.f};
    std::cout << "I * {4,2}     = " << i2.mulVec(v) << "  (expected { 4, 2 })\n";

    // subject: 2*identity * {4,2} -> {8,4}
    mx::Matrix<float> s2 = {{2.f, 0.f}, {0.f, 2.f}};
    std::cout << "2I * {4,2}    = " << s2.mulVec(v) << "  (expected { 8, 4 })\n";

    // subject: {{2,-2},{-2,2}} * {4,2} -> {4,-4}
    mx::Matrix<float> m1 = {{2.f, -2.f}, {-2.f, 2.f}};
    std::cout << "{{2,-2},{-2,2}} * {4,2} = " << m1.mulVec(v) << "  (expected { 4, -4 })\n";

    // non-square: 3x2 matrix * vec(2) -> vec(3)  (regression for result size)
    mx::Matrix<float> m32 = {{1.f, 2.f}, {3.f, 4.f}, {5.f, 6.f}};
    mx::Vector<float> w = {1.f, 1.f};
    std::cout << "3x2 * {1,1}   = " << m32.mulVec(w) << "  (expected { 3, 7, 11 })\n";

    // mulVec size mismatch -> throw
    std::cout << "mulVec mismatch: ";
    try {
        mx::Vector<float> bad = {1.f, 2.f, 3.f};
        i2.mulVec(bad);
        std::cout << "NO THROW (wrong)\n";
    } catch (const std::invalid_argument& e) {
        std::cout << "threw invalid_argument (ok)\n";
    }

    std::cout << "\n===== matrix * matrix =====\n";

    // subject: identity * identity -> identity
    std::cout << "I * I =\n" << i2.mulMat(i2) << "(expected [1,0] / [0,1])\n";

    // subject: I * {{2,1},{4,2}} -> {{2,1},{4,2}}
    mx::Matrix<float> b = {{2.f, 1.f}, {4.f, 2.f}};
    std::cout << "I * B =\n" << i2.mulMat(b) << "(expected [2,1] / [4,2])\n";

    // subject: {{3,-5},{6,8}} * {{2,1},{4,2}} -> {{-14,-7},{44,22}}
    mx::Matrix<float> c = {{3.f, -5.f}, {6.f, 8.f}};
    std::cout << "C * B =\n" << c.mulMat(b) << "(expected [-14,-7] / [44,22])\n";

    // non-square: (2x3) * (3x2) -> (2x2)
    mx::Matrix<float> a23 = {{1.f, 2.f, 3.f}, {4.f, 5.f, 6.f}};
    mx::Matrix<float> b32 = {{7.f, 8.f}, {9.f, 10.f}, {11.f, 12.f}};
    std::cout << "A(2x3) * B(3x2) =\n" << a23.mulMat(b32) << "(expected [58,64] / [139,154])\n";

    // mulMat shape mismatch -> throw
    std::cout << "mulMat mismatch: ";
    try {
        mx::Matrix<float> bad = {{1.f, 2.f, 3.f}};  // 1x3
        i2.mulMat(bad);                              // 2x2 * 1x3, cols(2) != rows(1)
        std::cout << "NO THROW (wrong)\n";
    } catch (const std::invalid_argument& e) {
        std::cout << "threw invalid_argument (ok)\n";
    }

    return 0;
}
