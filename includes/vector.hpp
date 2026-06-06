#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <initializer_list>
#include <iostream>
#include <ostream>
#include <cstddef>
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
