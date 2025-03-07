#ifndef MATRIX_H
#define MATRIX_H

#include <initializer_list>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cfloat>
#include "AlgebraExcepts.h"

namespace Algebra {
    class Matrix {
    private:
        int rows, cols;
        double **data;
        void GaussStairForm();
        Matrix subMatrix(int rCut, int cCut) const;
        void lineSwitch(int row1, int row2);
        void lineSub(int target, int sub, double scalar);

    public:
        Matrix (int rows = 0, int cols = 0);
        Matrix (int rows, int cols, std::initializer_list<double> list);

        Matrix (const Matrix& other);
        Matrix (Matrix&& other);
        Matrix& operator= (const Matrix& other) noexcept;
        Matrix& operator= (Matrix&& other) noexcept;
        ~Matrix();

        int getCols() const {return cols;}
        int getRows() const {return rows;}

        void set(int r, int c, double val);
        double& get(int r, int c) const;

        double determinant() const;
        Matrix invert() const;
        Matrix transpose() const;
        int rank() const noexcept;

        Matrix operator+(const Matrix& other);
        Matrix operator-(const Matrix& other);
        Matrix operator*(const Matrix& other);
        Matrix operator*(double scalar) noexcept;
        bool operator==(const Matrix& other) const;

        friend std::ostream& operator<<(std::ostream& os, const Matrix& M);
    };
}

#endif