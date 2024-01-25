#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define IND(i, j) ((i) *nx + (j))

double wtime(void) {
    struct timeval t;
    gettimeofday(&t, NULL);

    return (double) t.tv_sec + (double) t.tv_usec * 1E-6;
}

int main(int argc, char** argv) {
    double ttotal = -wtime();

    const int n = (argc > 1) ? (int) strtol(argv[1], NULL, 10) : 1000;
    const int ny = n;   // rows
    const int nx = ny;  // cols

    double* local_grid = (double*) calloc(ny * nx, sizeof(*local_grid));
    double* local_newgrid = (double*) calloc(ny * nx, sizeof(*local_newgrid));

    const double dx = 1.0 / (nx - 1.0);

    // Initialize top border: u(x, 0) = sin(pi * x)
    for (int j = 0; j < nx; j++) {
        const int ind = IND(0, j);
        local_newgrid[ind] = local_grid[ind] = sin(M_PI * dx * j);
    }

    // Initialize bottom border: u(x, 1) = sin(pi * x) * exp(-pi)
    for (int j = 0; j < nx; j++) {
        const int ind = IND(ny - 1, j);
        local_newgrid[ind] = local_grid[ind] = sin(M_PI * dx * j) * exp(-M_PI);
    }

    const double EPS = 0.001;
    int niters = 0;

    while (1) {
        niters++;

        for (int i = 1; i < ny - 1; i++) {
            // Update interior points
            for (int j = 1; j < nx - 1; j++) {
                local_newgrid[IND(i, j)] = (local_grid[IND(i - 1, j)] + local_grid[IND(i + 1, j)] +
                                            local_grid[IND(i, j - 1)] + local_grid[IND(i, j + 1)]) *
                                           0.25;
            }
        }

        // Check termination condition
        double maxdiff = 0.0;

        for (int i = 1; i < ny - 1; i++) {
            for (int j = 1; j < nx - 1; j++) {
                const int ind = IND(i, j);
                maxdiff = fmax(maxdiff, fabs(local_grid[ind] - local_newgrid[ind]));
            }
        }

        // Swap grids (after termination local_grid will contain result)
        double* tmp = local_grid;
        local_grid = local_newgrid;
        local_newgrid = tmp;

        if (maxdiff < EPS) {
            break;
        }
    }

    ttotal += wtime();

    printf("Heat 2D (serial) grid: rows %d, cols %d, niters %d, total time %.6f\n", ny, nx, niters,
           ttotal);

    // Save grid
    if (argc > 2) {
        const char* filename = argv[2];

        FILE* fout = fopen(filename, "w");
        if (!fout) {
            perror("Can't open file");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < ny; i++) {
            for (int j = 0; j < nx; j++) {
                fprintf(fout, "%.4f ", local_grid[IND(i, j)]);
            }
            fprintf(fout, "\n");
        }

        fclose(fout);
    }

    return 0;
}
