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

	/**
	 * @brief Linear interpolation between two values.
	 *
	 * Returns @c u + (v - u) * t, i.e. the point a fraction @p t of the way
	 * from @p u to @p v: @c t==0 yields @p u, @c t==1 yields @p v, and @c t==0.5
	 * the midpoint.
	 *
	 * Generic over any type @p K that supports @c +, @c - and @c * by
	 * a @c float (scalars, @ref Vector, @ref Matrix). @p t is clamped to [0, 1].
	 *
	 * @tparam K Interpolated type (a scalar, vector or matrix).
	 * @param u Value at @c t==0.
	 * @param v Value at @c t==1.
	 * @param t Interpolation factor; clamped to the range [0, 1].
	 * @return The interpolated value of type @p K.
	 *
	 * @code
	 * lerp(0.0f, 1.0f, 0.5f);	// 0.5
	 * lerp(mx::Vector<float>{2.0f, 1.0f}, mx::Vector<float>{4.0f, 2.0f}, 0.3f);	// { 2.6, 1.3 }
	 * @endcode
	 */
	template<typename K>
	K lerp(K u, K v, float t) {
		if (t < 0.0f) t = 0.0f;
		if (t > 1.0f) t = 1.0f;
		return u + (v - u) * t;
	}

}

#endif
