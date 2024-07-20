#include "interpolate.h"

bool fillCubicInterpolationMatrix(double *x, double *y, int N, double *deriv,
                                  double *interp)
{
    int nx = 4;
    for (int i = 0; i < N - 1; i++) {
        double dx = x[i + 1] - x[i];
        if (dx < EPS) {
            return false;
        }
        double yi = y[i];
        double yip1 = y[i + 1];
        double dydx = (yip1 - yi) / dx;

        interp[nx * i] = y[i];
        interp[nx * i + 1] = deriv[i];
        interp[nx * i + 2] = (3 * dydx - 2 * deriv[i] - deriv[i + 1]) / (dx);
        interp[nx * i + 3] = (deriv[i] + deriv[i + 1] - 2 * dydx) / (dx * dx);
    }
    return true;
}

bool calculateInterpStateAkima(double *x, double *y, int N, double *state)
{

    Array derivApprox(N);
    if (!calculateDerivApproxAkima(x, y, N, derivApprox.data)) {
        return false;
    }
    if (!fillCubicInterpolationMatrix(x, y, N, derivApprox.data, state)) {
        return false;
    }
    return true;
}

bool calculateDerivApproxAkima(double *x, double *y, int n, double *d)
{

    // w[0] and w[N-1] are unknown assume they are the same as their neighbours

    Array m(n + 3); // N + 3 == (N - 1) (number of ranges) + 4 (extra ranges)
    for (int j = 0; j < n - 1; j++) {
        double yi = y[j];
        double yip1 = y[j + 1];
        double dx = x[j + 1] - x[j];
        if (dx < EPS) {
            return false;
        }
        m[j + 2] = (yip1 - yi) / dx;
    }
    m[1] = 2 * m[2] - m[3];
    m[0] = 2 * m[1] - m[2];

    m[m.size - 2] = 2 * m[m.size - 3] - m[m.size - 4];
    m[m.size - 1] = 2 * m[m.size - 2] - m[m.size - 3];

    Array w(m.size);

    for (int i = 0; i < m.size - 1; i++) {
        w[i] = std::abs(m[i + 1] - m[i]);
    }

    for (int i = 0; i < n; i++) {

        int im = i + 2;
        double w1 = w[im];
        double w2 = w[im - 2];
        if (w2 < EPS && w1 < EPS) {
            w2 = 0.5;
            w1 = 0.5;
        }

        double m1 = m[im - 1];
        double m2 = m[im];
        d[i] = (w1 * m1 + w2 * m2) / (w1 + w2);
    }
    return true;
}

double evalAkima(double *state, const double *x, int n, double xi)
{
    int g = 0;
    if (xi < x[0]) {
        xi = x[0];
    }
    if (xi > x[n - 1]) {
        xi = x[n - 1] - EPS;
    }
    // Find interpolation range corresponding to result.x[i]
    while ((g < n - 1) && (x[g + 1] < xi)) {
        g += 1;
    }
    if (g == n - 1)
        return false;

    // Polynome evaluation
    double xval = xi - x[g];
    double xPowN = 1; // x in the power of n. Current value is x^0
    double yInterp = 0;
    int nx = 4;
    for (int d = 0; d < nx; d++) {
        double aij = state[nx * g + d];
        //    return buffer[nx * y + x];
        yInterp += aij * xPowN;
        xPowN *= xval;
    }
    return yInterp;
}
