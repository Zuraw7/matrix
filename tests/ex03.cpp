#include "../includes/operations.hpp"
#include "../includes/vector.hpp"
#include <iostream>
#include <stdexcept>

int main() {
    std::cout << "===== dot product =====\n";

    // subject examples
    mx::Vector<float> u1 = {0.f, 0.f};
    mx::Vector<float> v1 = {1.f, 1.f};
    std::cout << "dot({0,0},{1,1})   = " << mx::dot(u1, v1) << "  (expected 0)\n";

    mx::Vector<float> u2 = {1.f, 1.f};
    mx::Vector<float> v2 = {1.f, 1.f};
    std::cout << "dot({1,1},{1,1})   = " << mx::dot(u2, v2) << "  (expected 2)\n";

    mx::Vector<float> u3 = {-1.f, 6.f};
    mx::Vector<float> v3 = {3.f, 2.f};
    std::cout << "dot({-1,6},{3,2})  = " << mx::dot(u3, v3) << "  (expected 9)\n";

    // orthogonal -> 0
    mx::Vector<float> u4 = {1.f, 0.f};
    mx::Vector<float> v4 = {0.f, 1.f};
    std::cout << "dot({1,0},{0,1})   = " << mx::dot(u4, v4) << "  (expected 0, orthogonal)\n";

    // opposite -> negative
    mx::Vector<float> u5 = {1.f, 0.f};
    mx::Vector<float> v5 = {-1.f, 0.f};
    std::cout << "dot({1,0},{-1,0})  = " << mx::dot(u5, v5) << "  (expected -1, opposite)\n";

    // self dot = length squared
    mx::Vector<float> u6 = {3.f, 4.f};
    std::cout << "dot({3,4},{3,4})   = " << mx::dot(u6, u6) << "  (expected 25, |u|^2)\n";

    // higher dimensions
    mx::Vector<float> u7 = {1.f, 2.f, 3.f};
    mx::Vector<float> v7 = {4.f, 5.f, 6.f};
    std::cout << "dot 3D             = " << mx::dot(u7, v7) << "  (expected 32)\n"; // 4+10+18

    mx::Vector<float> u8 = {1.f, 2.f, 3.f, 4.f};
    mx::Vector<float> v8 = {2.f, 2.f, 2.f, 2.f};
    std::cout << "dot 4D             = " << mx::dot(u8, v8) << "  (expected 20)\n"; // 2+4+6+8

    // method form
    std::cout << "u3.dot(v3)         = " << u3.dot(v3) << "  (expected 9)\n";

    // size mismatch -> throw
    std::cout << "size mismatch: ";
    try {
        mx::Vector<float> a = {1.f, 2.f, 3.f};
        mx::Vector<float> b = {1.f, 2.f};
        mx::dot(a, b);
        std::cout << "NO THROW (wrong)\n";
    } catch (const std::invalid_argument& e) {
        std::cout << "threw invalid_argument (ok)\n";
    }

    return 0;
}
