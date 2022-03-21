//
// Complex number class declaration
//

#pragma once
#include <iostream>
using namespace std;


template<typename T=double>
class Complex
{
public:
    explicit Complex(double real_p = 0, double imag_p = 0);

    Complex(const Complex& obj);

    template<typename T_1>
    Complex(const Complex<T_1>& obj);

    T real() const noexcept;

    template<typename T1>
    void real(T1&& obj);

    T imag() const noexcept;

    template<typename T1>
    void imag(T1&& obj);

    template<typename T1> friend class Complex;

    template<typename T1>
    Complex& operator=(const Complex<T1>& obj);

    template<typename T1>
    Complex& operator=(const T1& obj);

    template<typename T1> 
    Complex& operator *= (const T1& obj);

    template<typename T1>
    Complex& operator += (const T1& obj);

    template<typename T1>
    Complex& operator -= (const T1& obj);

    template<typename T1, typename T2> 
    friend Complex<decltype(T1() + T2())> operator *(const T1 & left, const Complex<T2> & right);

    template<typename T1, typename T2> 
    friend Complex<decltype(T1() + T2())> operator *(const Complex<T1> & left, const T2 & right);

    template<typename T1, typename T2> 
    friend Complex<decltype(T1() + T2())> operator +(const Complex<T1> & left, const T2 & right);

    template<typename T1, typename T2>
    friend Complex<decltype(T1() + T2())> operator +(const T2 & left, const Complex<T1> & right);

    template<typename T1, typename T2> 
    friend Complex<decltype(T1() + T2())> operator -(const Complex<T1> & left, const T2 & right);

    template<typename T1, typename T2> 
    friend Complex<decltype(T1() + T2())> operator -(const T2 & left, const Complex<T1> & right);

    template<typename T2> 
    Complex& operator += (const Complex<T2> & obj);

    template<typename T2> 
    Complex& operator -= (const Complex<T2> & obj);

    template<typename T2> 
    Complex& operator *= (const Complex<T2> & obj);

    template<typename T1, typename T2> 
    friend Complex<decltype(T1() + T2())> operator +(const Complex<T1> & left, const Complex<T2> & right);

    template<typename T1, typename T2> //global Complex - Complex
    friend Complex<decltype(T1() + T2())> operator -(const Complex<T1> & left, const Complex<T2> & right);

    template<typename T1, typename T2> //global Complex * Complex
    friend Complex<decltype(T1() + T2())> operator *(const Complex<T1> & left, const Complex<T2> & right);

    template<typename T1> //Overloaded global operator << for print complex
    friend ostream & operator << (ostream &out, const Complex<T1> &obj);

    template<typename T1> //Overloaded global operator >> for print complex
    friend istream & operator >> (istream &in, Complex<T1> &obj);
private:
    T m_real;
    T m_imag;
};