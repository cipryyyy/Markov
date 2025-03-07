#include "Algebra.h"

//! HELPER
int pivotForGauss(const Algebra::Matrix& m, int col) {
    if (col < 0 || col >= m.getCols()) throw Algebra::MatrixIndexOutOfBound();
    int pos = -1;  // Inizializziamo pos a -1 (non trovato)
    double bestVal = DBL_MAX;
    for (int r = 0; r < m.getRows(); r++) {
        double cell = m.get(r, col);
        if (cell != 0 && std::abs(cell) < std::abs(bestVal)) {
            bestVal = cell;
            pos = r;
        }
    }
    return pos;
}
template <typename T>
T* ptrCl(T* src, int len) {
    T* copy = new T[len];
    for (int i = 0; i < len; i++) {
        copy[i] = src[i];
    }
    return std::move(copy);
}

//! PRIVATE
void Algebra::Matrix::GaussStairForm() {
    for (int col = 0; col < cols; col++) {
        int pivot = pivotForGauss(*this, col);
        if (pivot == -1) continue;
        for (int row = 0; row < rows; row++) {
            if (row == pivot) continue;
            double ratio = data[row][col] / data[pivot][col];
            lineSub(row, pivot, ratio);
        }
    }
}
Algebra::Matrix Algebra::Matrix::subMatrix(int rCut, int cCut) const {
    if (rCut < 0 || rCut >= rows) throw MatrixIndexOutOfBound();
    if (cCut < 0 || cCut >= cols) throw MatrixIndexOutOfBound();
    Matrix sub(rows-1, cols-1);
    for (int r = 0; r < rows-1; r++) {
        for (int c = 0; c < cols-1; c++) {
            if (r < rCut) {
                if (c < cCut) {
                    sub.set(r,c, get(r,c));
                } else {
                    sub.set(r,c, get(r,c+1));
                }
            } else {
                if (c < cCut) {
                    sub.set(r,c, get(r+1,c));
                } else {
                    sub.set(r,c, get(r+1,c+1));
                }
            }
        }
    }
    return sub;
}
void Algebra::Matrix::lineSwitch(int row1, int row2) {
    if ((row1 < 0 || row1 >= rows) || (row2 < 0 || row2 >= rows)) throw MatrixIndexOutOfBound();
    if (row1 == row2) return;
    double *temp = data[row1];
    data[row1] = data[row2];
    data[row2] = temp;
    temp = nullptr;
}
void Algebra::Matrix::lineSub(int target, int sub, double scalar) {
    if ((target < 0 || target >= rows) || (sub < 0 || sub >= rows)) throw MatrixIndexOutOfBound();
    for (int c = 0; c < cols; c++) {
        data[target][c] -= data[sub][c] * scalar;
    }
}

//! CONSTRUCTORS, MOVE, COPY AND DESTROYER
Algebra::Matrix::Matrix(int rows, int cols) {
    this->rows = rows;
    this->cols = cols;
    this -> data = new double*[rows];
    for (int i = 0; i < rows; i++) {
        this -> data[i] = new double [cols];
    }
}
Algebra::Matrix::Matrix(int rows, int cols, std::initializer_list<double> list) {
    this->rows = rows;
    this->cols = cols;
    this->data = new double*[rows];
    for (int i = 0; i < rows; i++) {
        this->data[i] = new double[cols];
        for (int j = 0; j < cols; j++) {
            this->data[i][j] = *(list.begin() + i * cols + j);
        }
    }
}
Algebra::Matrix::Matrix(const Matrix& other) {
    rows = other.rows;
    cols = other.cols;
    data = new double*[rows];
    for (int i = 0; i < rows; ++i) {
        data[i] = new double[cols];
        for (int j = 0; j < cols; ++j) {
            data[i][j] = other.data[i][j];
        }
    }
}

Algebra::Matrix::Matrix(Matrix&& other) {
    rows = other.rows;
    cols = other.cols;
    for (int i = 0; i < rows; i++) {
        data[i] = new double[cols];
        for (int j = 0; j < cols; j++) {
            data[i][j] = other.data[i][j];
        }
    }

    other.rows = 0;
    other.cols = 0;
    for (int i = 0; i < rows; i++) {
        delete[] other.data[i];
        other.data[i] = nullptr;
    }
    delete[] other.data;
    other.data = nullptr;
}
Algebra::Matrix& Algebra::Matrix::operator= (const Matrix& other) noexcept {
    if (this == &other) {
        return *this;
    }

    for (int i = 0; i < rows; ++i) {
        delete[] data[i];
    }
    delete[] data;

    rows = other.rows;
    cols = other.cols;
    data = new double*[rows];
    for (int i = 0; i < rows; ++i) {
        data[i] = new double[cols];
        for (int j = 0; j < cols; ++j) {
            data[i][j] = other.data[i][j];
        }
    }

    return *this;
}
Algebra::Matrix& Algebra::Matrix::operator= (Matrix&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    for (int i = 0; i < rows; ++i) {
        delete[] data[i];
    }
    delete[] data;

    rows = other.rows;
    cols = other.cols;
    data = other.data;

    other.data = nullptr;
    other.rows = 0;
    other.cols = 0;

    return *this;
}
Algebra::Matrix::~Matrix() {
    for (int i = 0; i < rows; i++) {
        delete[] data[i];
        data[i] = nullptr;
    }
    delete[] data;
    data = nullptr;
}

//! SETTER AND GETTER
void Algebra::Matrix::set(int r, int c, double val) {
    if (r < 0 || r >= rows) throw MatrixIndexOutOfBound();
    if (c < 0 || c >= cols) throw MatrixIndexOutOfBound();
    data[r][c] = val;
}
double& Algebra::Matrix::get(int r, int c) const {
    if (r < 0 || r >= rows) throw MatrixIndexOutOfBound();
    if (c < 0 || c >= cols) throw MatrixIndexOutOfBound();
    return data[r][c];
}

//! OPERATIONS
double Algebra::Matrix::determinant() const {
    if (rows != cols) throw MatrixNotSquare();
    if (rows == 0) throw NullMatrix();
    if (rows == 2) {
        return (data[0][0] * data[1][1] - data[1][0] * data[0][1]);
    }
    double val = 0;
    for (int i = 0; i < cols; i++) {
        val += (std::pow(-1, i) * data[0][i] * subMatrix(0, i).determinant());
    }
    return val;
}
Algebra::Matrix Algebra::Matrix::invert() const {
    if (rows != cols) throw MatrixNotSquare();
    if (rows == 0) throw NullMatrix();
    double det = determinant();
    if (det == 0) throw NullDeterminant();

    Matrix I(rows, cols);
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            I.set(r,c, (std::pow(-1, r+c) * subMatrix(r,c).determinant())/det);
        }
    }
    I = I.transpose();
    return I;
}
Algebra::Matrix Algebra::Matrix::transpose() const {
    Matrix T(rows, cols);
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            T.set(r,c, data[c][r]);
        }
    }
    return T;
}
int Algebra::Matrix::rank() const noexcept {
    int rankCounter = 0;
    Algebra::Matrix RankMatrix = *this;
    RankMatrix.GaussStairForm();
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++){
            if (RankMatrix.get(r,c) != 0) {
                rankCounter++;
                break;
            }
        }
    }
    return rankCounter;
}

//! OVERRIDES
Algebra::Matrix Algebra::Matrix::operator+(const Matrix& other) {
    if ((rows != other.rows) || (cols != other.cols)) throw NonCompatibleMatrixes();
    Matrix add(rows, cols);
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            add.set(r, c, data[r][c] + other.data[r][c]);
        }
    }
    return add;
}
Algebra::Matrix Algebra::Matrix::operator-(const Matrix& other) {
    if ((rows != other.rows) || (cols != other.cols)) throw NonCompatibleMatrixes();
    Matrix sub(rows, cols);
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            sub.set(r, c, data[r][c] - other.data[r][c]);
        }
    }
    return sub;
}
Algebra::Matrix Algebra::Matrix::operator*(const Matrix& other) {
    if (cols != other.rows) throw NonCompatibleMatrixes();
    Matrix Product(rows, other.cols);
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < other.cols; c++) {
            double val = 0;
            for (int t = 0; t < cols; t++) {
                val += (data[r][t] * other.data[t][c]);
            }
            Product.set(r,c,val);
        }
    }
    return Product;
}
Algebra::Matrix Algebra::Matrix::operator*(double scalar) noexcept {
    Matrix Scalar(rows, cols);
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Scalar.set(r, c, data[r][c] * scalar);
        }
    }
    return Scalar;
}

bool Algebra::Matrix::operator==(const Matrix& other) const {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (data[r][c] != other.data[r][c]) return false;
        }
    }
    return true;
}

//! FRIENDS
namespace Algebra {
    std::ostream& operator<< (std::ostream& os, const Algebra::Matrix& M) {
        for (int i = 0; i < M.rows; i++) {
            for (int j = 0; j < M.cols; j++) {
                os << M.data[i][j] << " ";
            }
            os << std::endl;
        }
        return os;
    }
}