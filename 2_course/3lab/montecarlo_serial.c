#define _POSIX_C_SOURCE 1
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

const double PI = 3.14159265358979323846;
const int n = 100000000; // 10 mil

// variant 2: y = exp((x+y)*(x+y))  x = [0, 1]; y = [0, 1-x];
double func(double x, double y) {
    return exp((x+y)*(x+y));
}

double getrand() {
    return (double)rand() / RAND_MAX;
}

double wtime()
{
	struct timeval t;
	gettimeofday(&t, NULL);
	return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

int main () {
    printf("Numerical integration using Monte-Carlo method\nn = %d\n", n);
    int in = 0;
    double s = 0.0;
    double t = wtime();
        double s_loc = 0;
        int in_loc = 0;
        for (int i = 0; i < n; i++) {
            double x = getrand();         // x in [0, 1]
            double y = getrand() - x;     // y in [0, 1-x]
            if (y <= func(x, y)) {
                in_loc++;
                s_loc += func(x, y);
            }
        }
        s += s_loc;
        in += in_loc;
    double v = 3; // b - a
    double res = v * s / in;
    t = wtime() - t;
    printf("Result: %.12f\nElapsed time: %.12f\n", res, t);
    return 0;
}

