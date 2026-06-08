#include "../includes/operations.hpp"
#include "../includes/vector.hpp"
#include <iostream>
#include <stdexcept>

int main() {
    std::cout << "===== Linear combination =====\n";

    // basis vectors: 10*e1 - 2*e2 + 0.5*e3 = {10, -2, 0.5}
    mx::Vector<float> e1 = {1.f, 0.f, 0.f};
    mx::Vector<float> e2 = {0.f, 1.f, 0.f};
    mx::Vector<float> e3 = {0.f, 0.f, 1.f};
    mx::Vector<float> r1 = mx::linearCombination({e1, e2, e3}, {10.f, -2.f, 0.5f});
    std::cout << "r1: " << r1 << "  (expected { 10, -2, 0.5 })\n";

    // 10*v1 - 2*v2 = {10, 0, 230}
    mx::Vector<float> v1 = {1.f, 2.f, 3.f};
    mx::Vector<float> v2 = {0.f, 10.f, -100.f};
    mx::Vector<float> r2 = mx::linearCombination({v1, v2}, {10.f, -2.f});
    std::cout << "r2: " << r2 << "  (expected { 10, 0, 230 })\n";

    // overload taking std::vector (named variables)
    std::vector<mx::Vector<float>> vs = {v1, v2};
    std::vector<float> cs = {10.f, -2.f};
    mx::Vector<float> r3 = mx::linearCombination(vs, cs);
    std::cout << "r3: " << r3 << "  (expected { 10, 0, 230 })\n";

    // count mismatch -> throw
    std::cout << "count mismatch: ";
    try {
        mx::linearCombination({v1, v2}, {1.f});
        std::cout << "NO THROW (wrong)\n";
    } catch (const std::invalid_argument& e) {
        std::cout << "threw invalid_argument (ok)\n";
    }

    return 0;
}
