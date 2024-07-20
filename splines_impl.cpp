#include "interpolate.h"

bool solveOpt(Matrix &M, Array &B, Array &X)
{
    int n = M.nx;

    for (int r = 1; r < n - 1; r++) {
        if (M.at(r - 1, 1) < EPS) {
            return false;
        }
        double k = M.at(r, 0) / M.at(r - 1, 1);
        M.at(r, 0) = 0;
        M.at(r, 1) -= M.at(r - 1, 2) * k;
        B[r] -= B[r - 1] * k;
    }

    X[n - 1] = B[n - 1] / M.at(n - 1, 1);

    for (int i = n - 2; i >= 0; i--) {
        if (std::abs(M.at(i, 1)) < EPS) {
            return false;
        }
        X[i] = (B[i] - M.at(i, 2) * X[i + 1]) / M.at(i, 1);
    }
    return true;
}

bool calculateStateSplines(double *state, double *x, double *y, int N)
{

    int n = N - 2;
    Array ksi(n + 1);

    const double *xvals = x + 1;
    for (int i = 1; i < n; i++) {
        ksi[i] = (xvals[i] + xvals[i - 1]) / 2;
    }
    ksi[0] = x[0];
    ksi[n] = x[N - 1];

    Matrix M(n + 1, 3);
    Array B(n + 1);

    const double *yvals = y + 1;

    for (int i = 1; i < n; i++) {
        double b1 = 1 / (xvals[i - 1] - ksi[i - 1]) - 1 / (ksi[i] - ksi[i - 1]);
        double b2 = 1 / (ksi[i] - xvals[i - 1]) + 1 / (ksi[i] - ksi[i - 1]) +
                    1 / (xvals[i] - ksi[i]) + 1 / (ksi[i + 1] - ksi[i]);
        double b3 = 1 / (ksi[i + 1] - xvals[i]) - 1 / (ksi[i + 1] - ksi[i]);
        double b4 =
            1 / (xvals[i - 1] - ksi[i - 1]) + 1 / (ksi[i] - xvals[i - 1]);
        double b5 = 1 / (xvals[i] - ksi[i]) + 1 / (ksi[i + 1] - xvals[i]);
        M.at(i, 0) = b1;
        M.at(i, 1) = b2;
        M.at(i, 2) = b3;
        B[i] = b4 * yvals[i - 1] + b5 * yvals[i];
    }
    M.at(0, 1) = 1;
    M.at(n, 1) = 1;
    B[0] = y[0];
    B[n] = y[N - 1];
    Array V(n + 1);
    bool ret = solveOpt(M, B, V);
    if (!ret) {
        return false;
    }

    for (int i = 0; i < n; i++) {
        double vi = V[i];

        //        C.at(0, i) = vi;
        state[i] = vi;

        double fi = yvals[i];
        double ksi_i = ksi[i];
        double a2 = (fi - vi) / (xvals[i] - ksi_i);
        double ksi_ip1 = ksi[i + 1];
        double vip1 = V[i + 1];
        double xi = xvals[i];
        double a3 = 1 / (ksi_ip1 - ksi_i) *
                    ((vip1 - fi) / (ksi_ip1 - xi) - (fi - vi) / (xi - ksi_i));
        //        C.at(1, i) = a2 - (xi - ksi[i]) * a3;
        state[n + i] = a2 - (xi - ksi[i]) * a3;
        //        C.at(2, i) = a3;
        state[2 * n + i] = a3;
        //        C.at(3, i) = ksi[i];
        state[3 * n + i] = ksi[i];
    }
    state[4 * n] = ksi[n];
    return true;
}

double evalSplines(double *state, int n, double xi)
{
    int g = 0;
    double *x = state + 3 * n;
    if (xi < x[0]) {
        xi = x[0];
    }
    if (xi > x[n]) {
        xi = x[n] - EPS;
    }
    // Find interpolation range corresponding to result.x[i]
    while ((g <= n) && (x[g + 1] < xi)) {
        g += 1;
    }
    if (g > n)
        return false;

    // Polynome evaluation
    double xval = xi - x[g];
    double xPowN = 1; // x in the power of n. Current value is x^0
    double yInterp = 0;
    int nx = 3;
    for (int d = 0; d < nx; d++) {
        double aij = state[d * n + g];
        yInterp += aij * xPowN;
        xPowN *= xval;
    }

    return yInterp;
}
