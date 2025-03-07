#ifndef ALGEBRAEXCEPTS_H
#define ALGEBRAEXCEPTS_H

#include <stdexcept>

namespace Algebra {
    class MatrixIndexOutOfBound : public std::exception{
    public:
        const char* what() const noexcept override {
            return "Index out of bound";
        }
    };
    class MatrixNotSquare : public std::exception{
    public:
        const char* what() const noexcept override {
            return "Matrix must be square";
        }
    };
    class NullDeterminant : public std::exception{
    public:
        const char* what() const noexcept override {
            return "Determinant is 0";
        }
    };
    class NullMatrix : public std::exception{
    public:
        const char* what() const noexcept override {
            return "Matrix is empty";
        }
    };
    class NonCompatibleMatrixes : public std::exception{
    public:
        const char* what() const noexcept override {
            return "Cannot operate on matrixes with different rows and/or index";
        }
    };
}

#endif