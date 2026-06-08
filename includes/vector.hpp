#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cmath>
#include <initializer_list>
#include <iostream>
#include <ostream>
#include <cstddef>
#include <stdexcept>
#include <vector>

namespace mx {

	/**
	 * @brief Vector over a field K (the scalar type).
	 *
	 * Stores @c n scalars of type @p K contiguously. Models an element of the
	 * vector space K^n. Operations (add, subtract, scale) mutate the vector
	 * in place.
	 *
	 * @tparam K Scalar (field) type, e.g. @c float, @c double, @c int.
	 */
	template<typename K>
	class Vector {
	public:
		/**
		 * @brief Construct a zero-filled vector of a given length.
		 * @param len Number of scalars; every element is value-initialized (K{}).
		 */
		explicit Vector(size_t len) : m_scalars(len, K{}) { }

		/**
		 * @brief Construct a vector from a list of scalars.
		 * @param vals Scalars in order; the vector's size is @c vals.size().
		 *
		 * @code
		 * mx::Vector<float> v{1.0f, 2.0f, 3.0f}; // [1.0, 2.0, 3.0]
		 * @endcode
		 */
		Vector(std::initializer_list<K> vals) : m_scalars(vals) { }

		/**
		 * @brief Access the i-th scalar (mutable).
		 * @param i Zero-based index, must be < size().
		 * @return Reference to the element, allowing read and write.
		 */
		K& operator[](size_t i) { return m_scalars[i]; }

		/**
		 * @brief Access the i-th scalar (read-only).
		 * @param i Zero-based index, must be < size().
		 * @return Const reference to the element.
		 */
		const K& operator[](size_t i) const { return m_scalars[i]; }

		/**
		 * @brief Element-wise sum, as a new vector.
		 * @param other Vector of the same size.
		 * @return A new vector equal to @c *this + @p other; operands unchanged.
		 */
		Vector<K> operator+(const Vector<K> &other) const {
			Vector<K> retVec = *this;
			retVec.add(other);
			return retVec;
		}

		/**
		 * @brief Element-wise difference, as a new vector.
		 * @param other Vector of the same size.
		 * @return A new vector equal to @c *this - @p other; operands unchanged.
		 */
		Vector<K> operator-(const Vector<K> &other) const {
			Vector<K> retVec = *this;
			retVec.subtract(other);
			return retVec;
		}

		/**
		 * @brief Scalar multiple, as a new vector.
		 * @param scalar Factor applied to every component.
		 * @return A new vector equal to @c *this * @p scalar; @c *this unchanged.
		 * @note Member form, so the vector must be the left operand
		 *       (@c vec * s, not @c s * vec).
		 */
		Vector<K> operator*(const K &scalar) const {
			Vector<K> retVec = *this;
			retVec.scale(scalar);
			return retVec;
		}

		/**
		 * @brief Stream the vector as @c { a, b, c }.
		 * @param out Output stream.
		 * @param vec Vector to print.
		 * @return The same stream, for chaining.
		 */
		friend std::ostream& operator<<(std::ostream &out, const Vector<K>& vec) {
			out << "{ ";
			for (size_t i = 0; i < vec.m_scalars.size(); i++) {
				out << vec.m_scalars[i];
				if (i + 1 < vec.m_scalars.size())
					out << ", ";
			}
			out << " }";
			return out;
		}

		/**
		 * @brief Add another vector element-wise, in place.
		 * @param other Vector of the same size; sizes must match.
		 * @note If sizes differ the call returned.
		 */
		void add(const Vector<K>& other) {
			if (m_scalars.size() != other.m_scalars.size())
				return;
			for (size_t i = 0; i < m_scalars.size(); i++)
				m_scalars[i] += other.m_scalars[i];
		}

		/**
		 * @brief Subtract another vector element-wise, in place.
		 * @param other Vector of the same size; sizes must match.
		 * @note If sizes differ the call is returned.
		 */
		void subtract(const Vector<K>& other) {
			if (m_scalars.size() != other.m_scalars.size())
				return;
			for (size_t i = 0; i < m_scalars.size(); i++)
				m_scalars[i] -= other.m_scalars[i];
		}

		/**
		 * @brief Scale every element by a scalar, in place.
		 * @param scalar Factor multiplying each component.
		 */
		void scale(const K& scalar) {
			for (K& s: m_scalars)
				s *= scalar;
		}

		/**
		 * @brief Dot (inner) product with another vector.
		 *
		 * Returns the scalar @c sum(this[i] * other[i]), computed with
		 * @c std::fma for accuracy. Equals @c |this|*|other|*cos(theta): its
		 * sign tells direction (positive: same side, zero: orthogonal,
		 * negative: opposite) while its magnitude also scales with the lengths.
		 * The dot of a vector with itself is its squared length.
		 *
		 * @param other Vector of the same size.
		 * @return The scalar dot product.
		 * @throws std::invalid_argument if the sizes differ.
		 * @note Runs in O(n).
		 */
		K dot(const Vector<K> &other) const {
			if (size() != other.size())
				throw std::invalid_argument("Vector: dot product requires vectors with same size");
			K result{0};

			for (size_t i = 0; i < size(); i++) {
				result = std::fma(m_scalars[i], other.m_scalars[i], result);
			}
			return result;
		}

		/**
		 * @brief Number of scalars in the vector (its dimension).
		 * @return The size n of K^n.
		 */
		size_t size() const { return m_scalars.size();}

		/** @brief Print the vector to standard output followed by a newline. */
		void print() const { std::cout << *this << std::endl; }


	private:
		std::vector<K> m_scalars; ///< Scalars of the vector, in order.
	};

}

#endif
