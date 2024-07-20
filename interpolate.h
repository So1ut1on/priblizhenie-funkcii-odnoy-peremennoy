/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   interpolate.h
 * Author: vlatse
 *
 * Created on December 1, 2019, 10:43 PM
 */

#ifndef INTERPOLATE_H
#define INTERPOLATE_H

#define EPS 1e-10

#include "array.h"
#include "matrix.h"
#include <fstream>
#include <string.h>

struct DataPoints {

    DataPoints(int size);
    DataPoints(double *xarr, double *yarr, int n);

    DataPoints(const DataPoints &vals);

    ~DataPoints();

    bool read(std::istream &input);

    void init(int size);

    std::ostream &operator>>(std::ostream &out);

    double *x;
    double *y;
    int N;
    bool fromExistingArrays;
};

bool calculateDerivApproxAkima(double *x, double *y, int N, double *d);
bool calculateInterpStateAkima(double *x, double *y, int N, double *state);

bool isInterpolationCorrect(const DataPoints &inputValues,
                            const DataPoints &interpValues, double eps = 1e-6);

bool fillCubicInterpolationMatrix(const DataPoints &values, const Array &deriv,
                                  Matrix &interp);
bool fillCubicInterpolationMatrix(double *x, double *y, int N, double *deriv,
                                  double *interp);

bool evaluateInterpolation(const Matrix &m, const double *x, int n,
                           DataPoints &result);
bool evaluateInterpolationAkima(const Matrix &m, const double *x, int n,
                                DataPoints &result);
bool evaluateInterpolationSplines(double *state, int n, DataPoints &result);
bool solveOpt(Matrix &M, Array &B, Array &X);

double evalAkima(double *state, const double *x, int n, double xi);
double evalSplines(double *state, int n, double xi);
bool interpolateAkima(const DataPoints &values, DataPoints &interpValues);
bool calculateStateSplines(double *state, double *x, double *y, int N);
bool interpolateSplines(const DataPoints &values, DataPoints &interpValues);
bool solve(Matrix &M, Array &B, Array &X);

#endif /* INTERPOLATE_H */
