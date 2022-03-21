#pragma once

#include <cmath>
#include <limits>
#include <iomanip> // input output manipulations
#include <sstream> // чтобы работать со строками через интерфейс потоков
#include "Matrix_utilites.h"

namespace linalg {
    const int g_interval = 2; // кол-во пробелов между элементами

// смотрит из каких символов состоит каждый элемент первого столбца матрицы при выводе в поток и возвращает наибольшее кол-во символов.
    template <typename T, typename Alloc>
    size_t el_size_first(const Matrix<T, Alloc>& object) {
        std::stringstream ioss; // поток для одновременной работы с вводом и выводом
        size_t size = 0; // сюда будем класть размер строки (т.е. кол-во символов)
        for (size_t row = 0; row < object.get_rows(); ++row) {
            ioss << object(row, 0); // отправил элемент в поток
            size = std::max(size, ioss.str().size()); // считал в строку всё, что есть буфере потока (получилась строка) и взял её размер (size())
            ioss.str(""); // перезаписали в буфер потока пустую строку
        }
        return size;
    }

    // смотрит из каких символов состоит каждый элемент второго+ столбцов матрицы при выводе в поток и возвращает наибольшее кол-во символов.
    template <typename T, typename Alloc>
    size_t el_size_else(const Matrix<T, Alloc>& object) {
        std::stringstream ioss; // поток для одновременной работы с вводом и выводом
        size_t size = 0; // сюда будем класть размер строки (т.е. кол-во символов)
        for (size_t row = 0; row < object.get_rows(); ++row) {
            for (size_t column = 1; column < object.get_columns(); ++column) {
                ioss << object(row, column); // отправил элемент в поток
                size = std::max(size, ioss.str().size()); // считал в строку всё, что есть буфере потока (получилась строка) и взял её размер (size())
                ioss.str(""); // перезаписали в буфер потока пустую строку
            }
        }
        return size;
    }

    //----------------------------------------------------------------------------------
    // Операторы:

    // Оператор вывода в поток:
    template <typename T, typename Alloc>
    std::ostream& operator << (std::ostream& out, const Matrix<T, Alloc>& object) {
        if (object.get_rows() == 0) return out << "|empty|\n";
        size_t size_first = el_size_first(object); // количество символов для заполнения одного элемента в первом столбце
        size_t size_else = el_size_else(object) + g_interval; // количество символов для заполнения одного элемента в остальных столбцах
        for (size_t row = 0; row < object.get_rows(); ++row) {
            out << '|';
            for (size_t column = 0; column < object.get_columns(); ++column) { // остальные с отступом
                out << std::right;
                if (column == 0)	out << std::setw(size_first);
                else				out << std::setw(size_else);
                out << object(row, column);
            }
            out << "|\n";
            //if (row != object.get_rows() - 1) out << '\n'; // если строка не последняя, сделай отступ
        }
        out << "\n";
        return out;
    }

    // Оператор суммирования двух матриц (разных типов)
    template<typename T1, typename A1, typename T2, typename A2>
    auto operator+(const Matrix<T1, A1>& obj1, const Matrix<T2, A2>& obj2)->Matrix<decltype(T1() + T2())>
    {
        if (obj1.get_rows() != obj2.get_rows() || obj1.get_columns() != obj2.get_columns())
            throw std::runtime_error("Error: different shapes for operator +!");
        Matrix<decltype(T1() + T2())> obj_new(obj1);
        obj_new += obj2;
        return obj_new;
    }


    // Оператор разности двух матриц (разных типов)
    template<typename T1, typename A1, typename T2, typename A2>
    auto operator-(const Matrix<T1, A1>& obj1, const Matrix<T2, A2>& obj2) -> Matrix<decltype(T1() - T2())> {
        if (obj1.get_rows() != obj2.get_rows() || obj1.get_columns() != obj2.get_columns())
            throw std::runtime_error("Error: different shapes for operator -!");
        Matrix<decltype(T1() - T2())> obj_new(obj1);
        obj_new -= obj2;
        return obj_new;
    }


    // Оператор перемножения двух матриц (разных типов)
    template<typename T1, typename A1, typename T2, typename A2>
    auto operator*(const Matrix<T1, A1>& obj1, const Matrix<T2, A2>& obj2) -> Matrix<decltype(T1()* T2() + T1() * T2())> {
        if (obj1.get_columns() != obj2.get_rows())
            throw std::runtime_error("Error: bad shapes for operator *!");
        Matrix<decltype(T1()* T2() + T1() * T2())> obj_new(obj1.get_rows(), obj2.get_columns()); // пустая матрица, т.е. элементы которой построены с помощью дефолтных конструкторов
        for (size_t col = 0; col < obj2.get_columns(); ++col) {
            for (size_t row = 0; row < obj1.get_rows(); ++row) {
                for (size_t term = 0; term < obj1.get_columns(); ++term) {
                    obj_new(row, col) += obj1(row, term) * obj2(term, col);
                }
            }
        }
        // Matrix<decltype(T1()* T2() + T1() * T2())> obj_new(obj1);	// так тоже сработает, но зачем лишний раз создавать дубликат левой матрицы
        // obj_new *= obj2;												// если *= всё равно перевыделит память => дубликат сотрётся
        return obj_new;
    }


    // Оператор перемножения числа на матрицу (разных типов)
    template<typename T, typename Alloc, typename Other>
    inline auto operator*(const Other& value, const Matrix<T, Alloc>& obj)->Matrix<decltype(T()* Other())> {
        Matrix<decltype(T()* Other())> obj_new(obj);
        return obj_new *= value;
    }


    // Оператор перемножения матрицы на число (разных типов)
    template<typename T, typename Alloc, typename Other>
    inline auto operator*(const Matrix<T, Alloc>& obj, const Other& value)->Matrix<decltype(T()* Other())> {
        Matrix<decltype(T()* Other())> obj_new(obj);
        return obj_new *= value;
    }
    //детерминант
    template<typename T, typename Alloc>
    T Matrix<T, Alloc>::det() const {
        if (m_rows == 1 && m_columns == 1) { return (*this)(0, 0); }
        if (m_rows != m_columns) throw LinAlgError("Is not a square matrix, cannot find Det");
        if ((m_columns == 2) && (m_rows == 2)) {
            return (*this)(0, 0) * (*this)(1, 1) - (*this)(1, 0) * (*this)(0, 1);
        }

        T determinant = 0;

        for (int i = 0; i < m_rows; i++) {
            determinant += std::pow(-1, i) * (*this)(i, 0) * (*this).minor(i, 0).det();
        }

        return determinant;
    }
    //минор
    template<typename T, typename Alloc>
    Matrix<T, Alloc> Matrix<T, Alloc>::minor(const int row, const int column) const {

        if (m_rows <= row) throw  LinAlgError("Wrong row of element for taking minor ");
        if (m_columns <= column) throw  LinAlgError("Wrong column of element for taking minor ");

        Matrix<T> result(m_columns - 1, m_rows - 1);

        int index_i = 0, index_k = 0;

        for (int i = 0; i < result.m_rows; i++) {
            for (int k = 0; k < result.m_columns; k++) {

                if (i >= row) index_i = 1;
                else index_i = 0;

                if (k >= column) index_k = 1;
                else index_k = 0;


                (*this)(index_i, index_k);
                result(i, k);
                result(i, k) = (*this)(i + index_i, k + index_k);

                ++index_i;
                ++index_k;
            }
        }


        return result;
    }
    //ранг матрицы
    template<typename T, typename Alloc>
    int Matrix<T, Alloc>::rank() const {
        Matrix<decltype(T() + double())> temp((*this));
        T result = 0;

        for (int i = 0; i < temp.m_rows; i++) {


            for (int k = 0; k < temp.m_columns; k++) {
                try {
                    if ((*this)(i, i) == 0) throw std::runtime_error("O");
                    temp(i, k) = temp(i, k) / (*this)(i, i);
                }
                catch (std::runtime_error err) {
                    for (int h = i; h < temp.m_columns; h++) {
                        if (temp(i, h) != 0) {
                            for (int m = i; m < temp.m_rows; m++) {
                                T cell_temp;
                                cell_temp = temp(m, h);
                                temp(m, h) = temp(m, i);
                                temp(m, i) = cell_temp;
                            }
                        } else { continue; }
                    }
                }
            }


            for (int k = i; k < temp.m_rows; k++) {
                for (int j = i; j < temp.m_rows; j++) {
                    if ((i != k) && (i != 100)) temp(k, j) = temp(k, j) - temp(i, j) * temp(k, i);
                }
            }
        }

        for (int i = 0; i < temp.m_rows; i++) {
            if (temp(i, i) != 0) ++result;
        }

        return result;
    }
    //норма
    template<typename T, typename Alloc>
    double Matrix<T, Alloc>::norm() const {
        double result = 0;

        for (int i = 0; i < m_rows; i++) {
            for (int k = 0; k < m_columns; k++) {
                result += (*this)(i, k) * (*this)(i, k);
            }
        }


        return std::pow(result, 0.5);
    }
    //след
    template<typename T1, typename Alloc>
    double Matrix<T1, Alloc>::trace() const {
        if (m_rows != m_columns) throw  LinAlgError("Is not a square matrix, cannot find Trace");

        double result = 0;
        for (int i = 0; i < (*this).m_rows; i++) {
            result += (*this)(i, i);
        }
        return result;
    }
    //транспонирование
    template<typename T1, typename Alloc>
    Matrix<T1, Alloc> transpose(const Matrix<T1, Alloc> &obj) {
        Matrix<T1> result(obj.m_columns, obj.m_rows);

        for (int i = 0; i < obj.m_rows; i++) {
            for (int k = 0; k < obj.m_columns; k++) {
                result(i, k) += obj(k, i);
            }
        }

        return result;
    }
    //обратная
    template<typename T1, typename Alloc_1>
    auto inv(const Matrix<T1, Alloc_1> &obj) {

        if (obj.m_rows != obj.m_columns) throw  LinAlgError("Is not a square matrix, cannot find inv matrix");
        double determ = obj.det();
        if (determ == 0) throw  LinAlgError("Det = 0, There is no inv matrix");

        Matrix<decltype(double() + T1())> result(obj);

        for (int i = 0; i < obj.m_rows; ++i) {
            for (int k = 0; k < obj.m_columns; ++k) {
                result(i, k) = std:: pow(-1, i + k)* linalg::transpose(obj).minor(i, k).det() / determ;
            }
        }

        return result;
    }
    //возведение в степень
    template<typename T, typename Alloc>
    Matrix<T, Alloc> pow(const Matrix<T, Alloc> &obj, size_t number) {
        Matrix<T> result(obj);

        for (size_t i = 0; i < number - 1; i++) {
            result *= obj;
        }
        return result;
    }
    //решение слау
    template<typename T1, typename Alloc_1, typename T2, typename Alloc_2>
    auto solve(const Matrix<T1, Alloc_1> &mat, const Matrix<T2, Alloc_2> &vec) {

        if (vec.get_columns() != 1) throw  LinAlgError("It's not a vector, cannot resolve equalization");
        if (vec.m_rows != mat.m_rows) throw  LinAlgError("Different row amounts, cannot resolve equalization");
        if (mat.m_rows != mat.m_columns)
            throw  LinAlgError("Not square matrix provides not normal system, cannot resolve equalization");
        double determ = mat.det();
        if (determ == 0) throw  LinAlgError("Det = 0, cannot resolve equalization");


        Matrix<> temp(mat);
        Matrix<> result(mat.m_columns, 1);

        for (int i = 0; i < mat.m_columns; ++i) {
            for (int j = 0; j < mat.m_rows; ++j) 
            {
                temp(i, j) = vec(i , j); 
            }
            result(i, 1) = temp.det() / determ;
        }
        return result;
    }

    //оператор ==
    template<typename T1, typename Alloc_1, typename T2, typename Alloc_2>
    bool operator==(const Matrix<T1, Alloc_1>& right, const Matrix<T2, Alloc_2>& left) {

        typedef decltype(T1() + T2()) T3;
        if ((right.m_rows != left.m_rows) || (right.m_columns != left.m_columns)) {
            throw  LinAlgError("Not equal sizes, operation == can not be done");
        }

        for (int i = 0; i < right.m_rows; i++) {
            for (int k = 0; k < right.m_columns; k++) {
                if (std::fabs(T3(right(i, k)) - T3(left(i, k))) >= std::numeric_limits<double>::epsilon()) {
                    return 0;
                }
            }
        }
        return 1;
    }

//----------------------------------------|векторы|-----------------------------------------------



    template<typename T1, typename A1, typename T2, typename A2>
    double multy_scalar(const Matrix<T1, A1> &obj1, const Matrix<T2, A2> &obj2){
        if (obj1.get_columns() != 1) throw  LinAlgError("obj1 is not a vector, scalar multiplication is impossible");
        if (obj2.get_columns() != 1) throw  LinAlgError("obj2 is not a vector, scalar multiplication is impossible");
        if (obj1.get_rows() != obj2.get_rows())
            throw  LinAlgError("Number of rows is not equal, scalar multiplication is impossible");

        double scal = 0;
        for (int i = 0; i < obj1.get_rows(); ++i) {
            scal += obj1(i, 0) * obj2(i, 0);
        }
        return scal;
    }

    template<typename T1, typename A1, typename T2, typename A2>
    double angle(const Matrix<T1, A1> &obj1, const Matrix<T2, A2> &obj2) {
        return acos(multy_scalar(obj1, obj2) / ((obj1.norm() * obj2.norm())));
    }

    template<typename T1, typename A1, typename T2, typename A2>
    auto multy_vector(const Matrix<T1, A1> &obj1, const Matrix<T2, A2> &obj2) -> Matrix<decltype(T1() * T2())> {
        Matrix<decltype(T1() * T2())> temp(3, 1);
        if (obj1.get_columns() != 1) throw  LinAlgError("obj1 is not a vector, vector multiplication is impossible");
        if (obj2.get_columns() != 1) throw  LinAlgError("obj2 is not a vector, vector multiplication is impossible");
        if (obj1.get_rows() != 3)
            throw  LinAlgError("Dimension vector obj1 is not 3, vector multiplication is impossible");
        if (obj2.get_rows() != 3)
            throw  LinAlgError("Dimension vector obj3 is not 3, vector multiplication is impossible");

        for (int i = 0; i < obj1.get_rows(); ++i) {
            temp(i, 0) = obj1((1 + i) % 3, 0) * obj2((2 + i) % 3, 0) - obj1((2 + i) % 3, 0) * obj2((1 + i) % 3, 0);
        }
        return temp;
    }

    template<typename T, typename Alloc>
    auto unit(const Matrix<T, Alloc> &obj) -> Matrix<decltype(T() * double())> {
        if (int a = obj.get_columns() != 1)
            throw  LinAlgError("Is not a vector, cannot find unit vector");
        double koef = (1.0 / obj.norm());
        Matrix<decltype(T() * double())> temp = obj * koef;
        return temp;
    }
}
