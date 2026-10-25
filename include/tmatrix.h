// ННГУ, ИИТММ, Курс "Алгоритмы и структуры данных"
//
// Copyright (c) Сысоев А.В.
//
//

#ifndef __TDynamicMatrix_H__
#define __TDynamicMatrix_H__

#include <iostream>
#include <cassert>
#include "VectorN.h"
#include <stdexcept>

const int MAX_VECTOR_SIZE = 100000000;
const int MAX_MATRIX_SIZE = 10000;

// Динамический вектор -
// шаблонный вектор на динамической памяти

template<typename T>
class TDynamicVector
{
private:
    static size_t check_vec_size(size_t s) {
        if (s == 0) throw std::out_of_range("Vector size should be greater than zero");
        if (s > MAX_VECTOR_SIZE) throw std::length_error("Vector size is too large");
        return s;
    }
protected:
    std_plus::VectorN<T> vec;
public:
    TDynamicVector(size_t size = 1) : vec(check_vec_size(size)) {}
    TDynamicVector(T* arr, size_t s) : vec(check_vec_size(s))
    {
        if (arr == nullptr)
            throw std::invalid_argument("TDynamicVector ctor requires non-nullptr arg");

        for (size_t i = 0; i < s; i++)
            vec[i] = arr[i];
    }
    TDynamicVector(const TDynamicVector& v) : vec(v.vec)
    {
    }
    TDynamicVector(TDynamicVector&& v) noexcept : vec(std::move(v.vec))
    {
    }
    ~TDynamicVector()
    {
    }
    TDynamicVector& operator=(const TDynamicVector& v)
    {
        vec = v.vec;
        return *this;
    }
    TDynamicVector& operator=(TDynamicVector&& v) noexcept
    {
        vec = std::move(v.vec);
        return *this;
    }

    size_t size() const noexcept { return vec.size(); }

    // индексация
    T& operator[](size_t ind)
    {
        return vec.data()[ind];
    }
    const T& operator[](size_t ind) const
    {
        return vec.data()[ind];
    }
    // индексация с контролем
    T& at(size_t ind)
    {
        return vec[ind];
    }
    const T& at(size_t ind) const
    {
        return vec[ind];
    }

    // сравнение
    bool operator==(const TDynamicVector& v) const noexcept
    {
        return vec == v.vec;
    }
    bool operator!=(const TDynamicVector& v) const noexcept
    {
        return vec != v.vec;
    }

    // скалярные операции
    TDynamicVector operator+(T val)
    {
        const size_t n = size();
        TDynamicVector result(n);
        for (size_t i = 0; i < n; i++)
            result[i] = vec[i] + val;
        return result;
    }
    TDynamicVector operator-(T val)
    {
        const size_t n = size();
        TDynamicVector result(n);
        for (size_t i = 0; i < n; i++)
            result[i] = vec[i] - val;
        return result;
    }
    TDynamicVector operator*(T val)
    {
        const size_t n = size();
        TDynamicVector result(n);
        for (size_t i = 0; i < n; i++)
            result[i] = vec[i] * val;
        return result;
    }

    // векторные операции
    TDynamicVector operator+(const TDynamicVector& v)
    {
        TDynamicVector result;
        result.vec = vec + v.vec;
        return result;
    }
    TDynamicVector operator-(const TDynamicVector& v)
    {
        TDynamicVector result;
        result.vec = vec - v.vec;
        return result;
    }
    T operator*(const TDynamicVector& v) //noexcept(noexcept(T()))
    {
        return vec * v.vec;
    }

    friend void swap(TDynamicVector& lhs, TDynamicVector& rhs) noexcept
    {
        lhs.vec.swap(rhs.vec);
    }

    // ввод/вывод
    friend std::istream& operator>>(std::istream& istr, TDynamicVector& v)
    {
        for (size_t i = 0; i < v.size(); i++)
            istr >> v.vec[i]; // требуется оператор>> для типа T
        return istr;
    }
    friend std::ostream& operator<<(std::ostream& ostr, const TDynamicVector& v)
    {
        for (size_t i = 0; i < v.size(); i++)
            ostr << v.vec[i] << ' '; // требуется оператор<< для типа T
        return ostr;
    }
};

// Динамическая матрица -
// шаблонная матрица на динамической памяти
template<typename T>
class TDynamicMatrix : private TDynamicVector<TDynamicVector<T>>
{
private:
    static size_t checked_size(size_t s)
    {
        if (s == 0)
            throw std::out_of_range("Matrix size should be greater than zero");
        if (s > static_cast<size_t>(MAX_MATRIX_SIZE))
            throw std::length_error("Matrix size is too large");
        return s;
    }
public:
    using TDynamicVector<TDynamicVector<T>>::size;
    using TDynamicVector<TDynamicVector<T>>::operator[];
    TDynamicMatrix(size_t s = 1)
        : TDynamicVector<TDynamicVector<T>>(checked_size(s))
    {
        const size_t n = this->size();
        for (size_t i = 0; i < n; i++)
            (*this)[i] = TDynamicVector<T>(n);
    }


    // сравнение
    bool operator==(const TDynamicMatrix& m) const noexcept
    {
        return TDynamicVector<TDynamicVector<T>>::operator==(m);
    }
    bool operator!=(const TDynamicMatrix& m) const noexcept
    {
        return TDynamicVector<TDynamicVector<T>>::operator!=(m);
    }
    // матрично-скалярные операции
    TDynamicMatrix<T> operator*(const T& val)
    {
        const size_t n = this->size();
        TDynamicMatrix<T> result(n);
        for (size_t i = 0; i < n; i++)
            result[i] = (*this)[i] * val; 
        return result;
    }

    // матрично-векторные операции
    TDynamicVector<T> operator*(const TDynamicVector<T>& v)
    {
        if (v.size() != this->size())
            throw std::length_error("TDynamicMatrix: incompatible sizes");
        const size_t n = this->size();
        TDynamicVector<T> result(n);
        for (size_t i = 0; i < n; i++)
            result[i] = (*this)[i] * v; // скалярное произведение строки на вектор
        return result;
    }

    // матрично-матричные операции
    TDynamicMatrix operator+(const TDynamicMatrix& m)
    {
        if (m.size() != this->size())
            throw std::length_error("TDynamicMatrix: incompatible sizes");
        const size_t n = this->size();
        TDynamicMatrix<T> result(n);
        for (size_t i = 0; i < n; i++)
            result[i] = (*this)[i] + m[i]; // operator+ векторов
        return result;
    }
    TDynamicMatrix operator-(const TDynamicMatrix& m)
    {
        if (m.size() != this->size())
            throw std::length_error("TDynamicMatrix: incompatible sizes");
        const size_t n = this->size();
        TDynamicMatrix<T> result(n);
        for (size_t i = 0; i < n; i++)
            result[i] = (*this)[i] - m[i];
        return result;
    }
    TDynamicMatrix operator*(const TDynamicMatrix& m)
    {
        if (m.size() != this->size())
            throw std::length_error("TDynamicMatrix: incompatible sizes");
        const size_t n = this->size();
        TDynamicMatrix<T> result(n);
        for (size_t i = 0; i < n; i++)
            for (size_t j = 0; j < n; j++)
            {
                T sum = T();
                for (size_t k = 0; k < n; k++)
                    sum += (*this)[i][k] * m[k][j];
                result[i][j] = sum;
            }
        return result;
    }

    // ввод/вывод
    friend std::istream& operator>>(std::istream& istr, TDynamicMatrix& v)
    {
        for (size_t i = 0; i < v.size(); i++)
            istr >> v[i];
        return istr;
    }
    friend std::ostream& operator<<(std::ostream& ostr, const TDynamicMatrix& v)
    {
        for (size_t i = 0; i < v.size(); i++)
            ostr << v[i] << '\n';
        return ostr;
    }
};

#endif