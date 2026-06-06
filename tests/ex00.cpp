#include "../includes/matrix.hpp"
#include "../includes/vector.hpp"
#include <iostream>

int main() {
    std::cout << "===== Vector =====\n";

    // add: [2,3] + [5,7] = [7,10]
    mx::Vector<float> u1 = {2.f, 3.f};
    mx::Vector<float> v1 = {5.f, 7.f};
    u1.add(v1);
    std::cout << "add:   " << u1 << "  (expected { 7, 10 })\n";

    // subtract: [2,3] - [5,7] = [-3,-4]
    mx::Vector<float> u2 = {2.f, 3.f};
    mx::Vector<float> v2 = {5.f, 7.f};
    u2.subtract(v2);
    std::cout << "sub:   " << u2 << "  (expected { -3, -4 })\n";

    // scale: [2,3] * 2 = [4,6]
    mx::Vector<float> u3 = {2.f, 3.f};
    u3.scale(2.f);
    std::cout << "scale: " << u3 << "  (expected { 4, 6 })\n";

    std::cout << "\n===== Matrix =====\n";

    // add: [[1,2],[3,4]] + [[7,4],[-2,2]] = [[8,6],[1,6]]
    mx::Matrix<float> m1 = {{1.f, 2.f}, {3.f, 4.f}};
    mx::Matrix<float> n1 = {{7.f, 4.f}, {-2.f, 2.f}};
    m1.add(n1);
    std::cout << "add (expected [8,6] / [1,6]):\n" << m1;

    // subtract: [[1,2],[3,4]] - [[7,4],[-2,2]] = [[-6,-2],[5,2]]
    mx::Matrix<float> m2 = {{1.f, 2.f}, {3.f, 4.f}};
    mx::Matrix<float> n2 = {{7.f, 4.f}, {-2.f, 2.f}};
    m2.subtract(n2);
    std::cout << "sub (expected [-6,-2] / [5,2]):\n" << m2;

    // scale: [[1,2],[3,4]] * 2 = [[2,4],[6,8]]
    mx::Matrix<float> m3 = {{1.f, 2.f}, {3.f, 4.f}};
    m3.scale(2.f);
    std::cout << "scale (expected [2,4] / [6,8]):\n" << m3;

    return 0;
}
