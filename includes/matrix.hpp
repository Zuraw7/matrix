#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "vector.hpp"
#include <cmath>
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
		 * @brief Matrix-vector product, M * v.
		 *
		 * Treats @p v as a column vector and returns @c M * v, whose i-th
		 * element is the dot product of row i with @p v. The vector's size
		 * must equal the number of columns; the result has one element per
		 * row. Accumulated with @c std::fma for accuracy.
		 *
		 * @param v Vector of size @c m_cols (column count).
		 * @return The product vector, of size @c m_rows (row count).
		 * @throws std::invalid_argument if @c v.size() != @c m_cols.
		 * @note Time complexity O(nm) for an n-by-m matrix.
		 */
		Vector<K> mulVec(const Vector<K> &v) const {
			if (m_cols != v.size())
				throw std::invalid_argument("mulVec: vector size must equal matrix column size");

			Vector<K> result(m_rows);
			for (size_t row = 0; row < m_rows; row++)
				for (size_t col = 0; col < m_cols; col++)
						result[row] = std::fma(at(row, col), v[col], result[row]);

			return result;
		}

		/**
		 * @brief Matrix-matrix product, this * mat.
		 *
		 * Returns the product @c A * B where @c A is @c *this. Element
		 * @c (i, j) is the dot product of row i of @c A with column j of
		 * @c mat. The inner dimensions must agree: this matrix's column
		 * count must equal @p mat's row count. The result is
		 * @c m_rows by @c mat.m_cols. Accumulated with @c std::fma.
		 *
		 * @param mat Right-hand matrix; its row count must equal @c m_cols.
		 * @return The product matrix, of shape @c m_rows by @c mat.m_cols.
		 * @throws std::invalid_argument if @c m_cols != @c mat.m_rows.
		 * @note Time complexity O(nmp) for shapes (n x m) * (m x p).
		 */
		Matrix<K> mulMat(const Matrix<K> &mat) const {
			if (m_cols != mat.m_rows)
				throw std::invalid_argument("mulMat: left matrix columns must equal right matrix rows");

			Matrix<K> result(m_rows, mat.m_cols);
			for (size_t row = 0; row < m_rows; row++) {
				for (size_t col = 0; col < mat.m_cols; col++) {
					for (size_t k = 0; k < m_cols; k++) {
						result.at(row, col) = std::fma(at(row, k), mat.at(k, col), result.at(row, col));
					}
				}
			}

			return result;
		}

		/**
		 * @brief Trace: sum of the main-diagonal elements.
		 *
		 * Returns @c at(0,0) + at(1,1) + ... for a square matrix. Defined
		 * only for square matrices.
		 *
		 * @return The trace as a scalar of type @p K.
		 * @throws std::invalid_argument if the matrix is not square.
		 * @note Time complexity O(n) for an n-by-n matrix.
		 */
		K trace() const {
			if (!isSquare())
				throw std::invalid_argument("trace: Matrix must be square");

			K result{0};
			for (size_t i = 0; i < m_rows; i++)
				result += at(i, i);

			return result;
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
		void print() const { std::cout << *this; }

	private:
		size_t m_rows;            ///< Number of rows.
		size_t m_cols;            ///< Number of columns.
		std::vector<K> m_scalars; ///< Scalars in column-major order (index col*rows+row).
	};

}

#endif
