#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>

#include "vector.h"

namespace matrix_space {

template <typename T>
class matrix final {
private:
    using row_t = typename my_containers::vector<T>;
    struct proxy_row_t final {
        T &operator[](size_t num_col);

        row_t &row;
    };

    struct const_proxy_row_t final {
        const T &operator[](size_t num_col) const;

        const row_t &row;
    };

public:
    matrix(const size_t num_rows = 0, const size_t num_cols = 0, const T &value = {});
    static matrix eye(const size_t num_rows, const size_t num_cols);
    static matrix square(const size_t num_rows, const T &value = {});

    proxy_row_t operator[](size_t num_row) &;
    const const_proxy_row_t operator[](size_t num_row) const &;

    matrix operator+(const matrix &other) const;
    matrix operator-(const matrix &other) const;

    matrix operator*(const matrix &other) const;

    matrix &transpose();

    T determinant() const;

    size_t get_num_rows() const;
    size_t get_num_cols() const;

    bool is_square() const;

    void dump() const;

private:
    my_containers::vector<row_t> data_;

    size_t num_rows_;
    size_t num_cols_;
};

template <typename T>
matrix<T>::matrix(const size_t num_rows, const size_t num_cols, const T &value)
    : data_ {num_rows, row_t {num_cols, value}}, num_rows_ {num_rows}, num_cols_ {num_cols}
{
}

template <typename T>
matrix<T> matrix<T>::eye(const size_t num_rows, const size_t num_cols)
{
    matrix eye {num_rows, num_cols};
    int min = std::min(num_rows, num_cols);

    for (int index = 0; index != min; ++index) {
        eye[index][index] = 1;
    }

    return eye;
}

template <typename T>
matrix<T> matrix<T>::square(const size_t num_rows, const T &value)
{
    return {num_rows, num_rows, value};
}

template <typename T>
T &matrix<T>::proxy_row_t::operator[](size_t num_col)
{
    return row[num_col];
}

template <typename T>
const T &matrix<T>::const_proxy_row_t::operator[](size_t num_col) const
{
    return row[num_col];
}

template <typename T>
typename matrix<T>::proxy_row_t matrix<T>::operator[](size_t num_row) &
{
    row_t &row = data_[num_row];
    proxy_row_t proxy_row {row};
    return proxy_row;
}

template <typename T>
const typename matrix<T>::const_proxy_row_t matrix<T>::operator[](size_t num_row) const &
{
    const row_t &row = data_[num_row];
    const_proxy_row_t proxy_row {row};
    return proxy_row;
}

template <typename T>
matrix<T> matrix<T>::operator+(const matrix &other) const
{
    if (num_rows_ != other.get_num_rows() || num_cols_ != other.get_num_cols()) {
        std::cout << "ahaha 123" << std::endl;
        return {};
    }

    matrix<T> sum {*this};

    for (int index_row = 0; index_row != num_rows_; ++index_row) {
        for (int index_col = 0; index_col != num_cols_; ++index_col) {
            sum[index_row][index_col] += other[index_row][index_col];
        }
    }

    return sum;
}

template <typename T>
matrix<T> matrix<T>::operator-(const matrix &other) const
{
    if (num_rows_ != other.get_num_rows() || num_cols_ != other.get_num_cols()) {
        std::cout << "ahaha 123" << std::endl;
        return {};
    }

    matrix<T> sub {*this};

    for (int index_row = 0; index_row != num_rows_; ++index_row) {
        for (int index_col = 0; index_col != num_cols_; ++index_col) {
            sub[index_row][index_col] -= other[index_row][index_col];
        }
    }

    return sub;
}

template <typename T>
matrix<T> matrix<T>::operator*(const matrix &other) const
{
    if (num_rows_ != other.get_num_cols() || num_cols_ != other.get_num_rows()) {
        std::cout << "ahaha 123" << std::endl;
        return {};
    }

    matrix<T> muled {num_rows_, num_rows_};
    matrix<T> other_transposed = other;
    other_transposed.transpose();

    for (int index = 0; index != num_rows_; ++index) {
        for (int index_row = 0; index_row != num_rows_; ++index_row) {
            T val = 0;
            for (int index_col = 0; index_col != num_cols_; ++index_col) {
                val += data_[index][index_col] * other_transposed[index_row][index_col];
            }

            muled[index][index_row] = val;
        }
    }

    return muled;
}

template <typename T>
T matrix<T>::determinant() const
{
    if (!is_square()) {
        std::cout << "ahaha 123" << std::endl;
        return {};
    }

    matrix lower = eye(num_rows_, num_cols_);
    matrix upper = square(num_rows_);

    T determinant = 1;
    for (int index_row = 0; index_row != num_rows_; ++index_row) {
        for (int index_col = 0; index_col != num_rows_; ++index_col) {
            T temp {};
            if (index_col >= index_row) {
                for (int index = 0; index != index_row; ++index) {
                    temp += lower[index_row][index] * upper[index][index_col];
                }

                T elem = data_[index_row][index_col] - temp;
                upper[index_row][index_col] = elem;
                if (index_row == index_col) {
                    determinant *= elem;
                }

            } else {
                for (int index = 0; index != index_col; ++index) {
                    temp += lower[index_row][index] * upper[index][index_col];
                }

                lower[index_row][index_col] = (data_[index_row][index_col] - temp) / upper[index_col][index_col];
            }
        }
    }

    return determinant;
}

template <typename T>
matrix<T> &matrix<T>::transpose()
{
    matrix<T> transposed_matrix {num_cols_, num_rows_};

    for (int index_row = 0; index_row != num_cols_; ++index_row) {
        for (int index_col = 0; index_col != num_rows_; ++index_col) {
            transposed_matrix[index_row][index_col] = data_[index_col][index_row];
        }
    }

    std::swap(*this, transposed_matrix);

    return *this;
}

template <typename T>
void matrix<T>::dump() const
{
    for (int index_row = 0; index_row != num_rows_; ++index_row) {
        for (int index_col = 0; index_col != num_cols_; ++index_col) {
            std::cout << data_[index_row][index_col] << ' ';
        }

        std::cout << std::endl;
    }
}

template <typename T>
size_t matrix<T>::get_num_rows() const
{
    return num_rows_;
}

template <typename T>
size_t matrix<T>::get_num_cols() const
{
    return num_cols_;
}

template <typename T>
bool matrix<T>::is_square() const
{
    return num_cols_ == num_rows_;
}

}  // namespace matrix_space

#endif
