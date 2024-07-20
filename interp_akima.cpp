#include "interp_akima.h"
#include "interpolate.h"

int fillDataPoints(int n, double a, double b, double *values,
                   DataPoints &dataPoints)
{
    if (n < 2) {
        return -1;
    }
    if (b - a < n * EPS) {
        return -1;
    }

    double dx = (b - a) / (n - 1);
    for (int i = 0; i < n; i++) {
        dataPoints.x[i] = a + dx * i;
        dataPoints.y[i] = values[i];
    }
}

int method_init(int n, double a, double b, double *values, double *derivatives,
                double *state)
{
    DataPoints dataPoints(n);
    int ret = fillDataPoints(n, a, b, values, dataPoints);
    if (ret != 0) {
        return ret;
    }
    Array d(n);
    if (!calculateDerivApproxAkima(dataPoints, d)) {
        return -1;
    }
    Matrix resAkima(4, n);
    if (!fillCubicInterpolationMatrix(dataPoints, d, resAkima)) {
        return -1;
    }
    for (int i = 0; i < n; i++) {
        state[i * resAkima.nx] = resAkima.at(0, i);
        state[i * resAkima.nx + 1] = resAkima.at(1, i);
        state[i * resAkima.nx + 2] = resAkima.at(2, i);
        state[i * resAkima.nx + 3] = resAkima.at(3, i);
    }
}

double method_compute(int n, double x, double *state)
{
    evaluateInterpolation()
}
