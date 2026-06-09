#include "../includes/matrix.hpp"
#include <iostream>
#include <stdexcept>

int main() {
    std::cout << "===== trace =====\n";

    // subject: identity 3x3 -> 3
    mx::Matrix<float> i3 = {{1.f, 0.f, 0.f}, {0.f, 1.f, 0.f}, {0.f, 0.f, 1.f}};
    std::cout << "trace(I3)            = " << i3.trace() << "  (expected 3)\n";

    // subject: {{2,-5,0},{4,3,7},{-2,3,4}} -> 9
    mx::Matrix<float> m1 = {{2.f, -5.f, 0.f}, {4.f, 3.f, 7.f}, {-2.f, 3.f, 4.f}};
    std::cout << "trace(m1)            = " << m1.trace() << "  (expected 9)\n";

    // subject: {{-2,-8,4},{1,-23,4},{0,6,4}} -> -21
    mx::Matrix<float> m2 = {{-2.f, -8.f, 4.f}, {1.f, -23.f, 4.f}, {0.f, 6.f, 4.f}};
    std::cout << "trace(m2)            = " << m2.trace() << "  (expected -21)\n";

    // 2x2
    mx::Matrix<float> m3 = {{5.f, 1.f}, {2.f, 7.f}};
    std::cout << "trace({{5,1},{2,7}}) = " << m3.trace() << "  (expected 12)\n";

    // 1x1
    mx::Matrix<float> m4 = {{42.f}};
    std::cout << "trace({{42}})        = " << m4.trace() << "  (expected 42)\n";

    // non-square -> throw
    std::cout << "non-square: ";
    try {
        mx::Matrix<float> bad = {{1.f, 2.f, 3.f}, {4.f, 5.f, 6.f}};  // 2x3
        bad.trace();
        std::cout << "NO THROW (wrong)\n";
    } catch (const std::invalid_argument& e) {
        std::cout << "threw invalid_argument (ok)\n";
    }

    return 0;
}
