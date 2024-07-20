#include "matrix.h"
#include <assert.h>

std::ostream &operator<<(std::ostream &out, const Matrix &m)
{

    for (int i = 0; i < m.nx; i++) {
        for (int j = 0; j < m.ny; j++) {
            out << m.at(i, j) << ' ';
        }
        out << '\n';
    }
    return out;
}

Matrix::Matrix(int sizeX, int sizeY)
{
    assert(sizeX > 0);
    assert(sizeY > 0);
    size = sizeX * sizeY;
    buffer = new double[size];
    for (int i = 0; i < size; i++) {
        buffer[i] = 0.;
    }
    nx = sizeX;
    ny = sizeY;
}

Matrix::~Matrix()
{
    delete[] buffer;
}

double &Matrix::at(int x, int y)
{
    int shift = nx * y + x;
    return buffer[shift];
}

double Matrix::at(int x, int y) const
{
    return buffer[nx * y + x];
}

void Matrix::read(std::istream &input)
{
    for (int j = 0; j < nx; j++) {
        for (int i = 0; i < nx; i++) {
            input >> at(i, j);
        }
    }
}

void Matrix::print(std::ostream &output)
{
    for (int j = 0; j < nx; j++) {
        for (int i = 0; i < nx; i++) {
            output << at(i, j);
        }
    }
}
