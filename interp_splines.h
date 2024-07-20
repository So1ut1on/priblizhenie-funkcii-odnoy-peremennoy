#ifndef INTERP_SPLINES_H
#define INTERP_SPLINES_H

int method_init(int n, double a, double b, double *values, double *derivatives,
                double *state);
double method_compute(int n, double x, double *state);
#endif // INTERP_SPLINES_H
