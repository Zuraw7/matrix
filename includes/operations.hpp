#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP

#include "vector.hpp"
#include <cmath>
#include <initializer_list>
#include <stdexcept>
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
	 * @return The combined vector.
	 * @throws std::invalid_argument if the inputs are empty, the counts differ,
	 *         or the vectors are not all the same size.
	 *
	 * @code
	 * mx::Vector<float> v1{1,2,3}, v2{0,10,-100};
	 * auto r = mx::linearCombination({v1, v2}, {10.f, -2.f}); // { 10, 0, 230 }
	 * @endcode
	 */
	template<typename K>
	Vector<K> linearCombination(const std::vector<Vector<K>> &vecs, const std::vector<K> &coeffs) {
		if (vecs.size() != coeffs.size() || vecs.empty())
			throw std::invalid_argument("linearCombination: need one coeff per vector, non-empty");

		size_t n = vecs.front().size();
		Vector<K> result(n);

		for (size_t i = 0; i < vecs.size(); i++) {
			if (vecs[i].size() != n)
				throw std::invalid_argument("linearCombination: all vectors must have equal size");
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

	/**
	 * @brief Dot (inner) product of two vectors.
	 *
	 * Free-function form of @ref Vector::dot, so callers can write
	 * @c mx::dot(u, v). Returns the scalar @c sum(v[i] * u[i]).
	 *
	 * @tparam K Scalar (field) type.
	 * @param v First vector.
	 * @param u Second vector, of the same size as @p v.
	 * @return The scalar dot product.
	 * @throws std::invalid_argument if the sizes differ.
	 */
	template<typename K>
	K dot(const Vector<K> &v, const Vector<K> &u) {
		return v.dot(u);
	}

	/**
	 * @brief Cosine of the angle between two vectors.
	 *
	 * Returns @c dot(v, u) / (||v|| * ||u||), the cosine of the angle theta
	 * between the vectors. The result lies in [-1, 1]: @c 1 means same
	 * direction (theta = 0), @c 0 means orthogonal (theta = 90 deg) and
	 * @c -1 means opposite direction (theta = 180 deg).
	 *
	 * @tparam K Scalar (field) type.
	 * @param v First vector.
	 * @param u Second vector, of the same size as @p v.
	 * @return The cosine of the angle, in [-1, 1].
	 * @throws std::invalid_argument if either vector is empty, the sizes
	 *         differ, or either vector has zero length (angle undefined).
	 */
	template<typename K>
	float angleCos(const Vector<K> &v, const Vector<K> &u) {
		if (v.size() == 0 || u.size() == 0)
			throw std::invalid_argument("angleCos: vectors cannot be empty");

		if (v.size() != u.size())
			throw std::invalid_argument("angleCos: vectors must have equal size");

		float nv = v.norm(), nu = u.norm();
		if (nv == 0.0f || nu == 0.0f)
			throw std::invalid_argument("angleCos: zero-length vector has no angle");

		return v.dot(u) / (nv * nu);
	}

	/**
	 * @brief Cross product of two 3-dimensional vectors.
	 *
	 * Returns the vector @c v x u, perpendicular to both @p v and @p u,
	 * with orientation given by the right-hand rule. Its length equals
	 * @c ||v|| * ||u|| * sin(theta), the area of the parallelogram they span,
	 * so parallel inputs yield the zero vector. The operation is
	 * anticommutative: @c v x u == -(u x v).
	 *
	 * Defined only in 3D.
	 *
	 * @tparam K Scalar (field) type.
	 * @param v First vector (size 3).
	 * @param u Second vector (size 3).
	 * @return The cross product @c v x u, a vector of size 3.
	 * @throws std::invalid_argument if either vector is not 3-dimensional.
	 */
	template<typename K>
	Vector<K> crossProduct(const Vector<K> &v, const Vector<K> &u) {
		if (v.size() != 3 || u.size() != 3)
			throw std::invalid_argument("crossProduct: vectors must be 3-dimensional");

		return {	v[1] * u[2] - v[2] * u[1],
					v[2] * u[0] - v[0] * u[2],
					v[0] * u[1] - v[1] * u[0] };
	}

}

#endif
