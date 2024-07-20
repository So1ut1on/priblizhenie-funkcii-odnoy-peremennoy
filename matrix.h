/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   matrix.h
 * Author: vlatse
 *
 * Created on November 30, 2019, 9:39 PM
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>

struct Matrix {
    Matrix(const Matrix &) = delete;
    Matrix &operator=(const Matrix &) = delete;

    Matrix(int sizeX, int sizeY);

    ~Matrix();

    double &at(int x, int y);

    double at(int x, int y) const;

    void read(std::istream &input);

    void print(std::ostream &output);

    double *buffer;
    int size;
    int nx;
    int ny;
};

std::ostream &operator<<(std::ostream &out, const Matrix &m);

#endif /* MATRIX_H */
