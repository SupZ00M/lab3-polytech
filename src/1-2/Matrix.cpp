#include "Matrix.hpp"
#include <stdexcept>
#include <iostream>
#include <cstdlib> 
using namespace std;
// Конструкторы
Matrix::Matrix(int n) : rows(0), cols(0), data(nullptr) {
    allocate(n, n);
    for (int i = 0; i < n; ++i) data[i][i] = 1.0;
}

Matrix::Matrix(int m, int n, double fill_value) : rows(0), cols(0), data(nullptr) {
    allocate(m, n);
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            data[i][j] = fill_value;
}

Matrix::Matrix(const Matrix& other) : rows(0), cols(0), data(nullptr) {
    copy_from(other);
}

Matrix::Matrix(Matrix&& other) noexcept : rows(0), cols(0), data(nullptr) {
    move_from(other);
}

Matrix::~Matrix() { deallocate(); }

//Данные
double Matrix::get(int i, int j) const {
    if (i < 0 || i >= rows || j < 0 || j >= cols)
        throw std::runtime_error("get: Index out of range");
    return data[i][j];
}

void Matrix::set(int i, int j, double value) {
    if (i < 0 || i >= rows || j < 0 || j >= cols)
        throw std::runtime_error("set: Index out of range");
    data[i][j] = value;
}

int Matrix::get_height() const {
    return rows;
}

int Matrix::get_width() const {
    return cols;
}


// перегрузки 
Matrix& Matrix::operator+=(const Matrix& other) {
    if (rows != other.rows || cols != other.cols)
        throw std::runtime_error("+=: size mismatch");
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            data[i][j] += other.data[i][j];
    return *this;
}
Matrix Matrix::operator+(const Matrix& other) const {
    Matrix result(*this);   // копия
    result += other;
    return result;
}

Matrix Matrix::operator-(const Matrix& other) const {
    Matrix result(*this);
    result -= other;
    return result;
}

Matrix Matrix::operator*(const Matrix& other) const {
    if (cols != other.rows)
        throw std::runtime_error("*: cols != other.rows");
    Matrix result(rows, other.cols, 0.0);
    for (int i = 0; i < rows; ++i)
        for (int k = 0; k < cols; ++k) {
            double r = data[i][k];
            for (int j = 0; j < other.cols; ++j)
                result.data[i][j] += r * other.data[k][j];
        }
    return result;
}

Matrix Matrix::operator*(double k) const {
    Matrix result(*this);
    result *= k;
    return result;
}

Matrix Matrix::operator/(double k) const {
    Matrix result(*this);
    result /= k;
    return result;
}

Matrix& Matrix::operator-=(const Matrix& other) {
    if (rows != other.rows || cols != other.cols)
        throw std::runtime_error("add_in_place: rows != other.rows || cols != other.cols");
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            data[i][j] -= other.data[i][j];
    return *this;
}

Matrix& Matrix::operator*=(double k) {
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            data[i][j] *= k;
    return *this;
}

Matrix& Matrix::operator/=(double k) {
    if (k == 0.0)
        throw std::runtime_error("/=: division by zero");
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            data[i][j] /= k;
    return *this;
}




Matrix& Matrix::operator=(const Matrix& other) {
    if (this == &other) return *this;
    deallocate();
    copy_from(other);
    return *this;
}
Matrix& Matrix::operator=(Matrix&& other) noexcept {
    if (this == &other) return *this;
    deallocate();
    move_from(other);
    return *this;
}

//манипуляции с памятью 
void Matrix::allocate(int r, int c) {
    rows = r; cols = c;
    data = new double*[rows];
    for (int i = 0; i < rows; ++i)
        data[i] = new double[cols]();
}

void Matrix::deallocate() {
    if (!data) return;
    for (int i = 0; i < rows; ++i) delete[] data[i];
    delete[] data;
    data = nullptr;
    rows = cols = 0;
}

void Matrix::copy_from(const Matrix& other) {
    allocate(other.rows, other.cols);
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            data[i][j] = other.data[i][j];
}

void Matrix::move_from(Matrix& other) noexcept {
    rows = other.rows;
    cols = other.cols;
    data = other.data;
    other.data = nullptr;
    other.rows = other.cols = 0;
}














// принт 
void Matrix::print() {
    for (int i = 0; i < rows; ++i){
        cout << "|";
        for (int j = 0; j < cols; ++j)
            cout << data[i][j] << "|";
    cout << endl;
    }
}


Matrix operator-(const Matrix& m) {
    Matrix result(m.get_height(), m.get_width(), 0.0);
    for (int i = 0; i < m.get_height(); ++i)
        for (int j = 0; j < m.get_width(); ++j)
            result.set(i, j, -m.get(i, j));
    return result;
}

Matrix operator*(double k, const Matrix& m) {
    return m * k;   // переиспользуем метод
}

std::ostream& operator<<(std::ostream& os, const Matrix& m) {
    for (int i = 0; i < m.get_height(); ++i) {
        os << "|";
        for (int j = 0; j < m.get_width(); ++j)
            os << m.get(i, j) << "|";
        os << "\n";
    }
    return os;
}
void Matrix::print(std::ostream& os) const { os << *this; }