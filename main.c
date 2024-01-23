#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>

#ifndef MAX_ITERATIONS
#define MAX_ITERATIONS 10000
#endif

#if SIZE == 5
#include "vars_5.h"
#elif SIZE == 13
#include "vars_13.h"
#elif SIZE == 33
#include "vars_33.h"
#elif SIZE == 50
#include "vars_50.h"
#elif SIZE == 67
#include "vars_67.h"
#elif SIZE == 100
#include "vars_100.h"
#elif SIZE == 167
#include "vars_167.h"
#elif SIZE == 333
#include "vars_333.h"
#elif SIZE == 500
#include "vars_500.h"
#endif

void matrix_copy(int n, double source[n][n], double destination[n][n]);
void matrix_multiply(int n, double A[n][n], double B[n][n], double C[n][n]);
void matrix_inversion_iteration(int n, double A[n][n], double B[n][n], double next[n][n]);
void matrix_subtract(int n, double a[n][n], double b[n][n], double c[n][n]);
double matrix_norm(int n, double matrix[n][n]);
bool matrix_has_invalid(int n, double matrix[n][n]);

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    struct timeval start, end;
    int n = SIZE;
    double n_sqrt = sqrt(n);
    assert(n == 5 || n == 13 || n == 33 || n == 50 || n == 67 || n == 100 || n == 167 || n == 333 || n == 500);

    double next[n][n];
    double temp[n][n];

    bool found = false;
    bool invalid = false;

    double total_elapsed = 0.0;
    double total_cpu_time = 0.0;

    if (rank == 0)
    {
        printf("MPI Version, SIZE: %d, Processes: %d\n", SIZE, size);
    }

    matrix_copy(n, A, B);

    gettimeofday(&start, NULL);
    clock_t cpu0 = clock();

    int i;
    double epsilon = 1e-6;
    for (i = 0; i < MAX_ITERATIONS; i++)
    {
        matrix_inversion_iteration(n, A, B, next);
        int send_invalid = invalid;
        MPI_Allreduce(&send_invalid, &invalid, 1, MPI_C_BOOL, MPI_LOR, MPI_COMM_WORLD);


        if (invalid)
        {
            break;
        }

        matrix_multiply(n, next, A, temp);
        if (fabs(matrix_norm(n, temp) - sqrt(n)) < epsilon)
        {
            found = true;
            break;
        }
        matrix_copy(n, next, B);
    }

    clock_t cpu1 = clock();
    gettimeofday(&end, NULL);

    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    double cpuTime = (double)(cpu1 - cpu0) / CLOCKS_PER_SEC;

    MPI_Reduce(&elapsed, &total_elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&cpuTime, &total_cpu_time, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Total Elapsed Time: %.6f s\n", total_elapsed);
        printf("Total CPU Time across all processes: %.6f s\n", total_cpu_time);

        if (!found || invalid)
        {
            printf("Failure.\n");
            invalid && printf("INF or NAN\n");
            !found && printf("Not found in %d iterations.\n", i);
        }
        else
        {
            matrix_multiply(n, next, A, temp);
            matrix_subtract(n, temp, I, temp);
            printf("OK. Found in %d iterations. Error: %.10f\n", i, matrix_norm(n, temp));
        }
    }

    MPI_Finalize();

    return 0;
}



void matrix_multiply(int n, double a[n][n], double b[n][n], double c[n][n])
{
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            c[i][j] = 0;

            for(int k = 0; k < n; k++) {
                #pragma omp atomic
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void matrix_subtract(int n, double a[n][n], double b[n][n], double c[n][n])
{
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            c[i][j] = a[i][j] - b[i][j];
        }
    }
}

void matrix_copy(int n, double source[n][n], double destination[n][n])
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            destination[i][j] = source[i][j];
        }
    }
}

void matrix_inversion_iteration(int n, double a[n][n], double b[n][n], double next[n][n])
{
    double r[n][n], temp[n][n];
    matrix_multiply(n, b, a, temp);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            r[i][j] = (i == j) - temp[i][j];
        }
    }

    matrix_multiply(n, r, b, temp);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            next[i][j] = temp[i][j] + b[i][j];
        }
    }
}

double matrix_norm(int n, double matrix[n][n])
{
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            sum += matrix[i][j] * matrix[i][j];
        }
    }
    return sqrt(sum);
}

bool matrix_has_invalid (int n, double matrix[n][n])
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (isnan(matrix[i][j]) || isinf(matrix[i][j])) {
               return true;
            }
        }
    }
    return false;
}
