#ifndef MATRIX_HPP
#define MATRIX_HPP
 #include <iostream> 

class Matrix {
private:
    int rows;
    int cols;
    double** data;

     void allocate(int r, int c);
    void deallocate();
    void copy_from(const Matrix& other);
    void move_from(Matrix& other) noexcept;

public:
    explicit Matrix(int n);
    Matrix(int m, int n, double fill_value = 0.0);
    Matrix(const Matrix& other);
    Matrix(Matrix&& other) noexcept;
    ~Matrix();
 
    double get(int i, int j) const;
    void   set(int i, int j, double value);
    int    get_height() const;
    int    get_width()  const;
    Matrix& operator=(const Matrix& other);
    Matrix& operator=(Matrix&& other) noexcept;

    Matrix& operator+=(const Matrix& other);
    Matrix& operator-=(const Matrix& other);
    Matrix& operator*=(double k);
    Matrix& operator/=(double k);
 
    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;
    Matrix operator*(double k)            const;
    Matrix operator/(double k)            const;

    void print();
    void print(std::ostream& os) const;
};



Matrix operator-(const Matrix& m);
Matrix operator*(double k, const Matrix& m);
std::ostream& operator<<(std::ostream& os, const Matrix& m);
#endif
