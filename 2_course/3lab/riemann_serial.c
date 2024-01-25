#include <math.h>
#include <stdio.h>
#include "omp.h"
#include <sys/time.h>

// variant 5
double func(double x) {
    // return x / pow(sin(2*x),3);
    return x / (sin(2*x) * sin(2*x) * sin(2*x));
}

double wtime()
{
	struct timeval t;
	gettimeofday(&t, NULL);
	return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

int main() {
    double t = wtime();
    const double a = 0.1, b = 0.5, eps = 1E-5;
    const int n0 = 1000000;
    printf("Numerical integration: [%f, %f], n0 = %d, EPS = %f\n", a, b, n0, eps);
    double sq[2];
        int n = n0, k;
        double delta = 1;
        for (k = 0; delta > eps; n *= 2, k ^= 1) {
            double h = (b - a) / n;
            double s = 0.0;
            sq[k] = 0;
            for (int i = 0; i < n; i++) {
                s += func(a + h * (i + 0.5));
            }
            sq[k] += s * h;
            if (n > n0) {
                delta = fabs(sq[k] - sq[k ^ 1]) / 3.0;
            }
            #if 0
            printf("n=%d i=%d sq=%.12f delta=%.12f\n", n, k, sq[k], delta);
            #endif
        }
        printf("Result: %.12f; Runge rule: EPS = %.1e, n = %d\n", sq[k] * sq[k], eps, n / 2);
    t = wtime() - t;
    printf("Elapsed time (sec.): %.6f\n", t);
    return 0;
}

