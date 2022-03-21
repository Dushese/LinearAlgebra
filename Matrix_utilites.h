
#pragma once

#include "Matrix.hpp"


namespace linalg {
    template<typename T, typename Alloc>
    // Оператор вывода в поток
    std::ostream& operator<<(std::ostream& out, const Matrix<T, Alloc>& object);

    template<typename T1, typename A1, typename T2, typename A2>
    // Оператор суммирования двух матриц (разных типов)
    auto operator+(const Matrix<T1, A1>& obj1, const Matrix<T2, A2>& obj2)->Matrix<decltype(T1() + T2())>;

    template<typename T1, typename A1, typename T2, typename A2>
    // Оператор разности двух матриц (разных типов)
    auto operator-(const Matrix<T1, A1>& obj1, const Matrix<T2, A2>& obj2)->Matrix<decltype(T1() - T2())>;

    template<typename T1, typename A1, typename T2, typename A2>
    // Оператор перемножения двух матриц (разных типов)
    auto
        operator*(const Matrix<T1, A1>& obj1, const Matrix<T2, A2>& obj2)->Matrix<decltype(T1()* T2() + T1() * T2())>;

    template<typename T, typename Alloc, typename Other>
    // Оператор перемножения числа на матрицу (разных типов)
    auto operator*(const Other& value, const Matrix<T, Alloc>& obj)->Matrix<decltype(T()* Other())>;

    template<typename T, typename Alloc, typename Other>
    // Оператор перемножения матрицы на число (разных типов)
    auto operator*(const Matrix<T, Alloc>& obj, const Other& value)->Matrix<decltype(T()* Other())>;

// создаёт транспанированную матрицу
template <typename T1, typename Alloc>
Matrix<T1, Alloc> transpose(const Matrix<T1, Alloc> &obj);

// создаёт обратную матрицу
// если Det != 0
template <typename T1, typename Alloc_1>
auto inv(const Matrix<T1, Alloc_1> &obj);

// возводит в степень
template <typename T, typename Alloc>
Matrix<T, Alloc>  pow (const Matrix<T, Alloc> &obj,  size_t number);

template <typename T1, typename Alloc_1, typename T2, typename Alloc_2>
auto  solve (const Matrix<T1, Alloc_1> &mat,  const Matrix<T2, Alloc_2> &vec);

template <typename T1, typename Alloc_1, typename T2, typename Alloc_2>
bool operator==(const Matrix<T1, Alloc_1> &right,  const Matrix<T2, Alloc_2> &left);




//Скалярное произведение
template<typename T1, typename A1, typename T2,  typename A2>
double multy_scalar(const Matrix<T1, A1>& obj1, const Matrix<T2, A2>& obj2);

//Нахождение угла между векторами
template<typename T1, typename A1, typename T2, typename A2>
double angle(const Matrix<T1, A1>& obj1, const Matrix<T2, A2>& obj2);

//Векторное произведение
template<typename T1, typename A1, typename T2, typename A2>
auto multy_vector(const Matrix<T1, A1>& obj1, const Matrix<T2, A2>& obj2)->Matrix<decltype(T1()* T2())>;

//Вектор единичный
template<typename T, typename Alloc>
auto unit(const Matrix<T, Alloc>& obj)->Matrix<decltype(T() * double())>;
}