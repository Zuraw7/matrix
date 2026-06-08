#include "../includes/operations.hpp"
#include "../includes/vector.hpp"
#include <iostream>
#include <stdexcept>

int main() {
    std::cout << "===== cross product =====\n";

    // subject: {0,0,1} x {1,0,0} -> {0,1,0}
    mx::Vector<float> u1 = {0.f, 0.f, 1.f};
    mx::Vector<float> v1 = {1.f, 0.f, 0.f};
    std::cout << "{0,0,1} x {1,0,0} = " << mx::crossProduct(u1, v1) << "  (expected { 0, 1, 0 })\n";

    // subject: {1,2,3} x {4,5,6} -> {-3,6,-3}
    mx::Vector<float> u2 = {1.f, 2.f, 3.f};
    mx::Vector<float> v2 = {4.f, 5.f, 6.f};
    std::cout << "{1,2,3} x {4,5,6} = " << mx::crossProduct(u2, v2) << "  (expected { -3, 6, -3 })\n";

    // subject: {4,2,-3} x {-2,-5,16} -> {17,-58,-16}
    mx::Vector<float> u3 = {4.f, 2.f, -3.f};
    mx::Vector<float> v3 = {-2.f, -5.f, 16.f};
    std::cout << "{4,2,-3} x {-2,-5,16} = " << mx::crossProduct(u3, v3) << "  (expected { 17, -58, -16 })\n";

    // standard basis: x cross y = z
    mx::Vector<float> ex = {1.f, 0.f, 0.f};
    mx::Vector<float> ey = {0.f, 1.f, 0.f};
    std::cout << "x x y             = " << mx::crossProduct(ex, ey) << "  (expected { 0, 0, 1 })\n";

    // anticommutative: y cross x = -z
    std::cout << "y x x             = " << mx::crossProduct(ey, ex) << "  (expected { 0, 0, -1 })\n";

    // parallel vectors -> zero
    mx::Vector<float> u4 = {2.f, 4.f, 6.f};
    mx::Vector<float> v4 = {1.f, 2.f, 3.f};
    std::cout << "{2,4,6} x {1,2,3} = " << mx::crossProduct(u4, v4) << "  (expected { 0, 0, 0 }, parallel)\n";

    // result is orthogonal to both inputs (dot == 0)
    mx::Vector<float> c = mx::crossProduct(u2, v2);
    std::cout << "c.u2 = " << c.dot(u2) << ", c.v2 = " << c.dot(v2) << "  (expected 0, 0)\n";

    // non-3D -> throw
    std::cout << "non-3D: ";
    try {
        mx::Vector<float> a = {1.f, 2.f};
        mx::Vector<float> b = {3.f, 4.f};
        mx::crossProduct(a, b);
        std::cout << "NO THROW (wrong)\n";
    } catch (const std::invalid_argument& e) {
        std::cout << "threw invalid_argument (ok)\n";
    }

    return 0;
}
