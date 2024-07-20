#include "interpolate.h"

bool evaluateInterpolationAkima(const Matrix &m, const double *x, int n,
                                DataPoints &result)
{

    // Check if all data points to compute interpolation are inside valid range
    if (result.x[0] < x[0] || result.x[result.N - 1] > x[n - 1]) {
        return false;
    }

    for (int i = 0; i < result.N; i++) {
        result.y[i] = evalAkima(m.buffer, x, n, result.x[i]);
    }
    return true;
}

bool interpolateAkima(const DataPoints &values, DataPoints &interpValues)
{

    int N = values.N;
    Matrix resAkima(4, N);

    Array derivApprox(N);
    calculateInterpStateAkima(values.x, values.y, values.N, resAkima.buffer);

    evaluateInterpolationAkima(resAkima, values.x, N, interpValues);
    return true;
}

bool solve(Matrix &M, Array &B, Array &X)
{
    int n = M.nx;
    for (int r = 0; r < n - 1; r++) {
        int k = r;
        for (; k < n; k++) {
            if (std::abs(M.at(r, k)) > EPS) {
                break;
            }
        }
        if (k == n)
            continue;
        if (k != r) {
            for (int p = r; p < n; p++) {
                double v = M.at(r, p);
                M.at(r, p) = M.at(k, p);
                M.at(k, p) = v;
                v = B[r];
                B[r] = B[k];
                B[k] = v;
            }
        }

        double den = M.at(r, r);
        if (std::abs(den) < EPS) {
            return false;
        }

        double m = M.at(r + 1, r) / den;
        for (int c = r; c < n; c++) {
            M.at(r + 1, c) = M.at(r + 1, c) - m * M.at(r, c);
        }

        B[r + 1] -= B[r] * m;
    }
    for (int i = n - 1; i >= 0; i--) {
        if (std::abs(M.at(i, i)) < EPS) {
            return false;
        }
        X[i] = B[i];
        for (int j = n - 1; j > i; j--) {
            X[i] -= M.at(i, j) * X[j];
        }
        X[i] /= M.at(i, i);
    }
    return true;
}

bool evaluateInterpolationSplines(double *state, int n, DataPoints &result)
{
    for (int i = 0; i < result.N; i++) {
        double xi = result.x[i];
        result.y[i] = evalSplines(state, n, xi);
    }
    return true;
}

bool interpolateSplines(const DataPoints &values, DataPoints &interpValues)
{
    int n = values.N - 2;
    //    Matrix C(4, n);
    Array state(4 * n + 1);
    if (!calculateStateSplines(state.data, values.x, values.y, values.N)) {
        return false;
    }
    evaluateInterpolationSplines(state.data, n, interpValues);
    return true;
}

bool isInterpolationCorrect(const DataPoints &inputValues,
                            const DataPoints &interpValues, double eps)
{
    assert(inputValues.N == interpValues.N);

    for (int i = 0; i < inputValues.N; i++) {
        if (std::abs(inputValues.y[i] - interpValues.y[i]) >= eps) {
            return false;
        }
    }
    return true;
}

DataPoints::DataPoints(int size)
{
    init(size);
}

DataPoints::DataPoints(const DataPoints &vals)
{
    init(vals.N);
    mempcpy((void *)x, (void *)vals.x, sizeof(x[0]) * vals.N);
    mempcpy((void *)y, (void *)vals.y, sizeof(y[0]) * vals.N);
}

DataPoints::~DataPoints()
{
    if (fromExistingArrays) {
        return;
    }
    delete[] x;
    delete[] y;
}

bool DataPoints::read(std::istream &input)
{
    for (int i = 0; i < N; i++) {
        input >> x[i];
        if (!input.good()) {
            return false;
        }
        input >> y[i];
    }
    return true;
}

void DataPoints::init(int size)
{
    assert(size > 0);
    N = size;
    x = new double[size];
    y = new double[size];
    fromExistingArrays = false;
}

std::ostream &DataPoints::operator>>(std::ostream &out)
{
    for (int i = 0; i < N; i++) {
        out << x[i] << ' ' << y[i] << '\n';
    }
    return out;
}
