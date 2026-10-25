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
    TDynamicVector(T* arr, size_t s) : vec(s)
    {
        assert(arr != nullptr && "TDynamicVector ctor requires non-nullptr arg");
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
        return vec[ind];
    }
    const T& operator[](size_t ind) const
    {
        return vec[ind];
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
    TDynamicVector operator-(double val)
    {
        const size_t n = size();
        TDynamicVector result(n);
        for (size_t i = 0; i < n; i++)
            result[i] = vec[i] - val;
        return result;
    }
    TDynamicVector operator*(double val)
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
    using TDynamicVector<TDynamicVector<T>>::pMem;
    using TDynamicVector<TDynamicVector<T>>::sz;
public:
    TDynamicMatrix(size_t s = 1) : TDynamicVector<TDynamicVector<T>>(s)
    {
        for (size_t i = 0; i < sz; i++)
            pMem[i] = TDynamicVector<T>(sz);
    }

    using TDynamicVector<TDynamicVector<T>>::operator[];

    // сравнение
    bool operator==(const TDynamicMatrix& m) const noexcept
    {
        return false;
    }

    // матрично-скалярные операции
    TDynamicMatrix<T> operator*(const T& val)
    {
        return TDynamicMatrix<T>{};
    }

    // матрично-векторные операции
    TDynamicVector<T> operator*(const TDynamicVector<T>& v)
    {
        return TDynamicVector<T>{};
    }

    // матрично-матричные операции
    TDynamicMatrix operator+(const TDynamicMatrix& m)
    {
        // можно ли написать реализацию, используя operator+ вектора?
        return TDynamicMatrix<T>{};
    }
    TDynamicMatrix operator-(const TDynamicMatrix& m)
    {
        return TDynamicMatrix<T>{};
    }
    TDynamicMatrix operator*(const TDynamicMatrix& m)
    {
        return TDynamicMatrix<T>{};
    }

    // ввод/вывод
    friend std::istream& operator>>(std::istream& istr, TDynamicMatrix& v)
    {
        return istr;
    }
    friend std::ostream& operator<<(std::ostream& ostr, const TDynamicMatrix& v)
    {
        return ostr;
    }
};

#endif
