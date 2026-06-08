#include "../includes/operations.hpp"
#include "../includes/vector.hpp"
#include <iostream>
#include <stdexcept>

int main() {
    std::cout << "===== angle cosine =====\n";

    // subject: {1,0},{1,0} -> 1 (same direction)
    mx::Vector<float> u1 = {1.f, 0.f};
    mx::Vector<float> v1 = {1.f, 0.f};
    std::cout << "cos({1,0},{1,0})     = " << mx::angleCos(u1, v1) << "  (expected 1)\n";

    // subject: {1,0},{0,1} -> 0 (orthogonal)
    mx::Vector<float> u2 = {1.f, 0.f};
    mx::Vector<float> v2 = {0.f, 1.f};
    std::cout << "cos({1,0},{0,1})     = " << mx::angleCos(u2, v2) << "  (expected 0)\n";

    // subject: {-1,1},{1,-1} -> -1 (opposite)
    mx::Vector<float> u3 = {-1.f, 1.f};
    mx::Vector<float> v3 = {1.f, -1.f};
    std::cout << "cos({-1,1},{1,-1})   = " << mx::angleCos(u3, v3) << "  (expected -1)\n";

    // subject: {2,1},{4,2} -> 1 (colinear, same orientation)
    mx::Vector<float> u4 = {2.f, 1.f};
    mx::Vector<float> v4 = {4.f, 2.f};
    std::cout << "cos({2,1},{4,2})     = " << mx::angleCos(u4, v4) << "  (expected 1)\n";

    // subject: {1,2,3},{4,5,6} -> 0.974631846
    mx::Vector<float> u5 = {1.f, 2.f, 3.f};
    mx::Vector<float> v5 = {4.f, 5.f, 6.f};
    std::cout << "cos({1,2,3},{4,5,6}) = " << mx::angleCos(u5, v5) << "  (expected 0.974632)\n";

    // 60 degrees: {1,0},{1,sqrt(3)} -> 0.5
    mx::Vector<float> u6 = {1.f, 0.f};
    mx::Vector<float> v6 = {1.f, 1.7320508f};
    std::cout << "cos 60 deg           = " << mx::angleCos(u6, v6) << "  (expected 0.5)\n";

    // size mismatch -> throw
    std::cout << "size mismatch: ";
    try {
        mx::Vector<float> a = {1.f, 2.f, 3.f};
        mx::Vector<float> b = {1.f, 2.f};
        mx::angleCos(a, b);
        std::cout << "NO THROW (wrong)\n";
    } catch (const std::invalid_argument& e) {
        std::cout << "threw invalid_argument (ok)\n";
    }

    // zero-length vector -> throw
    std::cout << "zero vector:   ";
    try {
        mx::Vector<float> a = {0.f, 0.f};
        mx::Vector<float> b = {1.f, 1.f};
        mx::angleCos(a, b);
        std::cout << "NO THROW (wrong)\n";
    } catch (const std::invalid_argument& e) {
        std::cout << "threw invalid_argument (ok)\n";
    }

    return 0;
}
