#include "../includes/operations.hpp"
#include "../includes/vector.hpp"
#include "../includes/matrix.hpp"
#include <iostream>

int main() {
    std::cout << "===== lerp: scalars =====\n";
    std::cout << "lerp(0, 1, 0)    = " << mx::lerp(0.f, 1.f, 0.f)   << "  (expected 0)\n";
    std::cout << "lerp(0, 1, 1)    = " << mx::lerp(0.f, 1.f, 1.f)   << "  (expected 1)\n";
    std::cout << "lerp(0, 1, 0.5)  = " << mx::lerp(0.f, 1.f, 0.5f)  << "  (expected 0.5)\n";
    std::cout << "lerp(21, 42, 0.3)= " << mx::lerp(21.f, 42.f, 0.3f) << "  (expected 27.3)\n";

    // clamp: t outside [0,1]
    std::cout << "lerp(0, 1, -2)   = " << mx::lerp(0.f, 1.f, -2.f)  << "  (expected 0, clamped)\n";
    std::cout << "lerp(0, 1, 5)    = " << mx::lerp(0.f, 1.f, 5.f)   << "  (expected 1, clamped)\n";

    std::cout << "\n===== lerp: vectors =====\n";
    mx::Vector<float> a = {2.f, 1.f};
    mx::Vector<float> b = {4.f, 2.f};
    std::cout << "lerp(a, b, 0.3) = " << mx::lerp(a, b, 0.3f) << "  (expected { 2.6, 1.3 })\n";

    std::cout << "\n===== lerp: matrices =====\n";
    mx::Matrix<float> m = {{2.f, 1.f}, {3.f, 4.f}};
    mx::Matrix<float> n = {{20.f, 10.f}, {30.f, 40.f}};
    std::cout << "lerp(m, n, 0.5) = (expected [11,5.5] / [16.5,22]):\n"
              << mx::lerp(m, n, 0.5f);

    return 0;
}
