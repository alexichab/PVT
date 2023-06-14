#include <math.h>
#include <stdio.h>
#include "omp.h"

// variant 5
double func(double x) {
    // return x / pow(sin(2*x),3);
    return x / (sin(2*x) * sin(2*x) * sin(2*x));
}

int main() {
    double t = omp_get_wtime();
    const double a = 0.1, b = 0.5, eps = 1E-5;
    const int n0 = 1000000;
    printf("Numerical integration: [%f, %f], n0 = %d, EPS = %f\n", a, b, n0, eps);
    double sq[2];
    #pragma omp parallel num_threads(8) // 1 - serial, 2 ... multithreaded
    {
        int n = n0, k;
        double delta = 1;
        for (k = 0; delta > eps; n *= 2, k ^= 1) {
            double h = (b - a) / n;
            double s = 0.0;
            sq[k] = 0;
            // Ждем пока все потоки закончат обнуление sq[k], s #pragma omp barrier
            #pragma omp barrier // барьерная синхронизация, ожидание выполнения всех потоков.
            #pragma omp for nowait//чтобы основной поток не ждал завершения остальных потоков
            for (int i = 0; i < n; i++) {
                s += func(a + h * (i + 0.5));
            }
            #pragma omp atomic // используется для блокировки ячейки памяти одним потоком, 
                               // для предотвращения data race
            sq[k] += s * h;
            // Ждем пока все потоки обновят sq[k]
            #pragma omp barrier
            if (n > n0) {
                delta = fabs(sq[k] - sq[k ^ 1]) / 3.0;
            }
            #if 0
            printf("n=%d i=%d sq=%.12f delta=%.12f\n", n, k, sq[k], delta);
            #endif
        }
        #pragma omp master
        printf("Result: %.12f; Runge rule: EPS = %.1e, n = %d\n", sq[k] * sq[k], eps, n / 2);
    }
    t = omp_get_wtime() - t;
    printf("Elapsed time (sec.): %.6f\n", t);
    return 0;
}
