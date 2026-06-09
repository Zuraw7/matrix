#include "../includes/matrix.hpp"
#include <iostream>

int main() {
    std::cout << "===== transpose =====\n";

    // square 2x2: {{1,2},{3,4}} -> {{1,3},{2,4}}
    mx::Matrix<float> m1 = {{1.f, 2.f}, {3.f, 4.f}};
    std::cout << "{{1,2},{3,4}}^T =\n" << m1.transpose() << "(expected [1,3] / [2,4])\n";

    // non-square 2x3 -> 3x2
    mx::Matrix<float> m2 = {{1.f, 2.f, 3.f}, {4.f, 5.f, 6.f}};
    std::cout << "2x3 transposed (expect 3x2) =\n" << m2.transpose()
              << "(expected [1,4] / [2,5] / [3,6])\n";

    // non-square 3x1 (column) -> 1x3 (row)
    mx::Matrix<float> m3 = {{7.f}, {8.f}, {9.f}};
    std::cout << "3x1 transposed (expect 1x3) =\n" << m3.transpose()
              << "(expected [7,8,9])\n";

    // double transpose is identity
    mx::Matrix<float> tt = m2.transpose().transpose();
    std::cout << "(m2^T)^T =\n" << tt << "(expected original [1,2,3] / [4,5,6])\n";

    // shape check: 2x3 -> transposed shape is {3,2}
    auto s = m2.transpose().shape();
    std::cout << "transposed shape = {" << s.first << ", " << s.second
              << "}  (expected {3, 2})\n";

    return 0;
}
