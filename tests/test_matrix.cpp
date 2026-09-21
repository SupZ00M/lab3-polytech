#include "Matrix.hpp"
#include <iostream>
#include <stdexcept>

static int failures = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::cerr << "FAIL: " #cond " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    ++failures; } } while (0)

static bool eq(const Matrix& m, double v) {
    for (int i = 0; i < m.get_height(); ++i)
        for (int j = 0; j < m.get_width(); ++j)
            if (m.get(i, j) != v) return false;
    return true;
}

int main() {
    // 1.1 Присваивание
    {
        Matrix m1(10), m2(10, 10, 2);   // оба 10×10
        m1 = m2;
        m2 = Matrix(3, 4, 1.0);
        m1 = m1;
        CHECK(m1.get_height() == 10 && m1.get_width() == 10);
        CHECK(m2.get_height() == 3  && m2.get_width()  == 4);
        CHECK(eq(m2, 1.0));
    }

    // 1.2.1 Составное
    {
        Matrix m1(4), m2(4, 4, 1);
        m2 += m1;
        m2 -= m1 += m1;
        m1 *= 2;
        m2 /= 3;
        // m1 после "+= m1" и "*= 2" — диагональ 4.0
        CHECK(m1.get(0, 0) == 4.0);
        CHECK(m1.get(0, 1) == 0.0);
    }

    // 1.2.2 Бинарные
    {
        Matrix m1(4), m2(4, 4, 1);
        Matrix m3 = (m1 + m2) * m1 * 4 / 2 - m2;
        CHECK(m3.get_height() == 4 && m3.get_width() == 4);
    }

    // 1.2.3 Глобальные
    {
        Matrix m(3, 3, 2.0);
        Matrix a = 2.0 * m;
        Matrix b = -m;
        CHECK(a.get(0, 0) ==  4.0);
        CHECK(b.get(0, 0) == -2.0);
    }

    // 1.3 Move
    {
        Matrix m1(4), m2 = m1 + m1;
        m1 = m2 * m1;
        m2 = std::move(m1);
        CHECK(m2.get_height() == 4);
    }

    // Исключения
    {
        Matrix a(3, 3), b(4, 4);
        bool threw = false;
        try { a += b; } catch (const std::exception&) { threw = true; }
        CHECK(threw);
    }

    if (failures) { std::cerr << failures << " check(s) failed\n"; return 1; }
    std::cout << "test_matrix: OK\n";
    return 0;
}