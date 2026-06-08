#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <utility>
#include <vector>

namespace mx {

	/**
	 * @brief Matrix over a field K (the scalar type).
	 *
	 * Stores rows x cols scalars of type @p K in a single contiguous buffer in
	 * @b column-major order: the element at logical position (row, col) lives at
	 * index @c col*rows+row. Operations (add, subtract, scale) mutate the
	 * matrix in place.
	 *
	 * @tparam K Scalar (field) type, e.g. @c float, @c double, @c int.
	 */
	template<typename K>
	class Matrix {
	public:
		/**
		 * @brief Construct a zero-filled matrix of a given shape.
		 * @param rows Number of rows.
		 * @param cols Number of columns.
		 * @note Every element is value-initialized (K{}).
		 */
		Matrix(size_t rows, size_t cols) : m_rows(rows), m_cols(cols), m_scalars((m_cols * m_rows), K{}) { }

		/**
		 * @brief Construct a matrix from a list of rows.
		 * @param list Rows of equal length; each inner list is one row.
		 *
		 * Shape is inferred: @c m_rows is the number of inner lists, @c m_cols is
		 * the length of a row. Values are stored column-major.
		 *
		 * @code
		 * mx::Matrix<float> m{{1.0f, 2.0f}, {3.0f, 4.0f}}; // [1.0 2.0 / 3.0 4.0]
		 * @endcode
		 *
		 * @throws std::invalid_argument if the rows are not all the same length.
		 */
		Matrix(std::initializer_list<std::initializer_list<K>> list) : m_rows(list.size()), m_cols(list.begin()->size()) {
			m_scalars.resize(m_rows * m_cols);

			for (const auto &line: list) {
				if (line.size() != m_cols)
					throw std::invalid_argument("Matrix: all rows must have equal length");
			}

			size_t row = 0;
			for (const auto &line: list) {
				size_t col = 0;
				for (const K& val: line) {
					m_scalars[col * m_rows + row] = val;
					col++;
				}
				row++;
			}
		}

		/**
		 * @brief Element-wise sum, as a new matrix.
		 * @param other Matrix of the same shape.
		 * @return A new matrix equal to @c *this + @p other; operands unchanged.
		 */
		Matrix<K> operator+(const Matrix<K> &other) const {
			Matrix<K> retMat = *this;
			retMat.add(other);
			return retMat;
		}

		/**
		 * @brief Element-wise difference, as a new matrix.
		 * @param other Matrix of the same shape.
		 * @return A new matrix equal to @c *this - @p other; operands unchanged.
		 */
		Matrix<K> operator-(const Matrix<K> &other) const {
			Matrix<K> retMat = *this;
			retMat.subtract(other);
			return retMat;
		}

		/**
		 * @brief Scalar multiple, as a new matrix.
		 * @param scalar Factor applied to every element.
		 * @return A new matrix equal to @c *this * @p scalar; @c *this unchanged.
		 * @note Member form, so the matrix must be the left operand
		 *       (@c mat * s, not @c s * mat).
		 */
		Matrix<K> operator*(const K &scalar) const {
			Matrix<K> retMat = *this;
			retMat.scale(scalar);
			return retMat;
		}

		/**
		 * @brief Stream the matrix row by row as @c [ a, b ] per line.
		 * @param out Output stream.
		 * @param mat Matrix to print.
		 * @return The same stream, for chaining.
		 */
		friend std::ostream& operator<<(std::ostream &out, const Matrix<K>& mat) {
			for (size_t row = 0; row < mat.m_rows; row++) {
				out << "[ ";
				for (size_t col = 0; col < mat.m_cols; col++) {
					out << mat.m_scalars[col * mat.m_rows + row];
					if (col + 1 < mat.m_cols)
						out << ", ";
				}
				out << " ]\n";
			}
			return out;
		}

		/**
		 * @brief Access the element at (row, col), mutable.
		 * @param row Zero-based row index, must be < rows.
		 * @param col Zero-based column index, must be < cols.
		 * @return Reference to the element, allowing read and write.
		 */
		K& at(size_t row, size_t col) { return m_scalars[col * m_rows + row]; }

		/**
		 * @brief Access the element at (row, col), read-only.
		 * @param row Zero-based row index, must be < rows.
		 * @param col Zero-based column index, must be < cols.
		 * @return Const reference to the element.
		 */
		const K& at(size_t row, size_t col) const { return m_scalars[col * m_rows + row]; }

		/**
		 * @brief Add another matrix element-wise, in place.
		 * @param other Matrix of the same shape; rows and cols must match.
		 * @throws std::invalid_argument if the shapes differ.
		 */
		void add(const Matrix<K>& other) {
			if (m_rows != other.m_rows || m_cols != other.m_cols)
				throw std::invalid_argument("Matrix: add requires equal shapes");
			for (size_t i = 0; i < m_scalars.size(); i++)
				m_scalars[i] += other.m_scalars[i];
		}

		/**
		 * @brief Subtract another matrix element-wise, in place.
		 * @param other Matrix of the same shape; rows and cols must match.
		 * @throws std::invalid_argument if the shapes differ.
		 */
		void subtract(const Matrix<K>& other) {
			if (m_rows != other.m_rows || m_cols != other.m_cols)
				throw std::invalid_argument("Matrix: subtract requires equal shapes");
			for (size_t i = 0; i < m_scalars.size(); i++)
				m_scalars[i] -= other.m_scalars[i];
		}

		/**
		 * @brief Scale every element by a scalar, in place.
		 * @param scalar Factor multiplying each element.
		 */
		void scale(const K& scalar) {
			for (K& s: m_scalars)
				s *= scalar;
		}

		/**
		 * @brief Shape of the matrix.
		 * @return A pair {rows, cols}.
		 */
		std::pair<size_t, size_t> shape() const { return {m_rows, m_cols}; }

		/**
		 * @brief Whether the matrix is square.
		 * @return @c true if rows == cols.
		 */
		bool isSquare() const { return m_rows == m_cols; }

		/** @brief Print the matrix to standard output followed by a newline. */
		void print() const { std::cout << *this << std::endl; }

	private:
		size_t m_rows;            ///< Number of rows.
		size_t m_cols;            ///< Number of columns.
		std::vector<K> m_scalars; ///< Scalars in column-major order (index col*rows+row).
	};

}

#endif
