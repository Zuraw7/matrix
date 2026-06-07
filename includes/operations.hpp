#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP

#include "vector.hpp"
#include <cmath>
#include <initializer_list>
#include <vector>

namespace mx {

	/**
	 * @brief Linear combination of vectors weighted by coefficients.
	 *
	 * Computes @c coeffs[0]*vecs[0] + coeffs[1]*vecs[1] + ... All vectors must
	 * share the same dimension, and there must be exactly one coefficient per
	 * vector.
	 *
	 * Uses @c std::fma for each multiply-add: one fused operation, one rounding,
	 * giving better accuracy and mapping to a hardware FMA instruction where
	 * available. Runs in O(n), where n is the total number of coordinates
	 * across all input vectors.
	 *
	 * @tparam K Scalar (field) type.
	 * @param vecs Vectors to combine; all of equal dimension.
	 * @param coeffs Coefficients, one per vector; @c coeffs.size() == @c vecs.size().
	 * @return The combined vector, or an empty vector if the inputs are empty,
	 *         the counts differ, or the vectors are not all the same size.
	 *
	 * @code
	 * mx::Vector<float> v1{1,2,3}, v2{0,10,-100};
	 * auto r = mx::linearCombination({v1, v2}, {10.f, -2.f}); // { 10, 0, 230 }
	 * @endcode
	 */
	template<typename K>
	Vector<K> linearCombination(const std::vector<Vector<K>> &vecs, const std::vector<K> &coeffs) {
		if (vecs.size() != coeffs.size() || vecs.empty())
			return Vector<K>(0);

		size_t n = vecs.front().size();
		Vector<K> result(n);

		for (size_t i = 0; i < vecs.size(); i++) {
			if (vecs[i].size() != n)
				return Vector<K>(0);
			for (size_t j = 0; j < n; j++) {
				result[j] = std::fma(coeffs[i], vecs[i][j], result[j]);
			}
		}
		return result;
	}

	/// @brief Overload to allow calling with brace lists; see the definition above.
	template<typename K>
	Vector<K> linearCombination(std::initializer_list<Vector<K>> vecs, const std::initializer_list<K> coeffs) {
		return linearCombination(std::vector<Vector<K>>(vecs),std::vector<K>(coeffs));
	}

}

#endif
