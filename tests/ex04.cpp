#include "../includes/vector.hpp"
#include <iostream>

int main() {
    std::cout << "===== norms =====\n";

    // subject example: {0,0,0} -> 0, 0, 0
    mx::Vector<float> u0 = {0.f, 0.f, 0.f};
    std::cout << "{0,0,0}   norm_1=" << u0.norm_1()
              << "  norm=" << u0.norm()
              << "  norm_inf=" << u0.norm_inf()
              << "  (expected 0, 0, 0)\n";

    // subject example: {1,2,3} -> 6, 3.74165738, 3
    mx::Vector<float> u1 = {1.f, 2.f, 3.f};
    std::cout << "{1,2,3}   norm_1=" << u1.norm_1()
              << "  norm=" << u1.norm()
              << "  norm_inf=" << u1.norm_inf()
              << "  (expected 6, 3.741657, 3)\n";

    // subject example: {-1,-2} -> 3, 2.236067977, 2
    mx::Vector<float> u2 = {-1.f, -2.f};
    std::cout << "{-1,-2}   norm_1=" << u2.norm_1()
              << "  norm=" << u2.norm()
              << "  norm_inf=" << u2.norm_inf()
              << "  (expected 3, 2.236068, 2)\n";

    // classic 3-4-5 triangle: L2 must be exactly 5
    mx::Vector<float> u3 = {3.f, 4.f};
    std::cout << "{3,4}     norm_1=" << u3.norm_1()
              << "  norm=" << u3.norm()
              << "  norm_inf=" << u3.norm_inf()
              << "  (expected 7, 5, 4)\n";

    // negatives must use absolute value
    mx::Vector<float> u4 = {-5.f, 3.f, -1.f};
    std::cout << "{-5,3,-1} norm_1=" << u4.norm_1()
              << "  norm=" << u4.norm()
              << "  norm_inf=" << u4.norm_inf()
              << "  (expected 9, 5.91608, 5)\n";

    // single element
    mx::Vector<float> u5 = {-42.f};
    std::cout << "{-42}     norm_1=" << u5.norm_1()
              << "  norm=" << u5.norm()
              << "  norm_inf=" << u5.norm_inf()
              << "  (expected 42, 42, 42)\n";

    return 0;
}
