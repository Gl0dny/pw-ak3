#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <inttypes.h>
#include <omp.h>

#define MAX_TITLE_LENGTH 512

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
#endif


void matrix_print(int n, double matrix[n][n], const char *format, ...);
void matrix_copy(int n, double source[n][n], double destination[n][n]);
void matrix_multiply(int n, double A[n][n], double B[n][n], double C[n][n]);
void matrix_inversion_iteration(int n, double A[n][n], double B[n][n], double next[n][n]);
void matrix_zero(int n, double matrix[n][n]);

double matrix_norm(int n, double matrix[n][n]);

bool is_identity(int n, double matrix[n][n]);
bool matrix_has_invalid (int n, double matrix[n][n]);


int main(int argc, char* argv[])
{
    struct timeval start, end;
    int n = SIZE;
    double n_sqrt = sqrt(n);
    assert(n == 5 || n == 13 || n == 33 || n == 50 || n == 67 || n == 100);

    double next[n][n];
    double temp[n][n];

    bool found = false;
    bool invalid = false;

//    omp_set_num_threads(16);

#ifdef FLAT_PARALLEL
    printf("Wersja openmp-flat, SIZE: %d\n", SIZE);
#else
    printf("Wersja openmp, SIZE: %d\n", SIZE);
#endif


    matrix_copy(n, A, B);

    gettimeofday(&start, NULL);
    clock_t cpu0 = clock();

    int i;
    double diff, epsilon = 1e-6;
    for (i = 0; i < INT64_MAX; i++) {
        matrix_inversion_iteration(n, A, B, next);
        if (matrix_has_invalid(n, next)) {
            matrix_print(n,next, "NAN lub INF");
            invalid = true;
            break;
        }

        matrix_multiply(n, next, A, temp);
        diff = fabs(matrix_norm(n, temp) - n_sqrt);
        if (diff < epsilon) {
            found = true;
            break;
        }
        matrix_copy(n, next, B);
    }

    clock_t cpu1 = clock();
    gettimeofday(&end, NULL);

    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("Czas wykonania %.6f s\n", elapsed);
    double cpuTime = (double)(cpu1 - cpu0) / CLOCKS_PER_SEC;
    printf("Czas procesora %.6f s\n", cpuTime);

    if (!found || invalid) {
        printf("Niepowodzenie.\n");
        invalid && printf("INF lub NAN\n");
        !found && printf("Nie znaleziono w %d iteracjach.\n", i);
    } else {
        printf("OK. Odnaleziono w %d iteracjach\n", i);
    }

    return 0;
}

void matrix_print(int n, double matrix[n][n], const char *format, ...)
{
    assert(format != NULL);
    assert(matrix != NULL);
    assert(n > 0);

    char title[MAX_TITLE_LENGTH];
    va_list args;

    va_start(args, format);
    vsnprintf(title, MAX_TITLE_LENGTH, format, args);
    va_end(args);

    printf("\n%s: [\n", title);

    int rows = n;
    int cols = n;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%.2f\t", matrix[i][j]);
        }
        printf("\n");
    }
    printf("]\n");
}

void matrix_multiply(int n, double a[n][n], double b[n][n], double c[n][n])
{

//#define FLAT_PARALLEL
#ifdef FLAT_PARALLEL
    int n_squared = n*n;
    #pragma omp parallel for
    for (int x = 0; x < n_squared; x++) {
        int i = x / n;
        int j = x % n;

        c[i][j] = 0;
        for (int k = 0; k < n; k++) {
            c[i][j] += a[i][k] * b[k][j];
        }
    }
#else
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            c[i][j] = 0;

            for (int k = 0; k < n; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
#endif

    /*
    // wersja sekwencyjna
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            c[i][j] = 0;

            for (int k = 0; k < n; k++) {
                #pragma omp atomic
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    */


}

void matrix_zero(int n, double matrix[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = 0;
        }
    }
}

void matrix_copy(int n, double source[n][n], double destination[n][n])
{

//#define FLAT_PARALLEL
#ifdef FLAT_PARALLEL
    int n_squared = n*n;
    #pragma omp parallel for
    for (int x = 0; x < n_squared; x++) {
        int i = x / n;
        int j = x % n;
        destination[i][j] = source[i][j];
    }
#else
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            destination[i][j] = source[i][j];
        }
    }
#endif

    /*
    // wersja sekwencyjna
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            destination[i][j] = source[i][j];
        }
    }
    */
}

void matrix_inversion_iteration(int n, double a[n][n], double b[n][n], double next[n][n])
{
    double r[n][n], temp[n][n];

//#define FLAT_PARALLEL
#ifdef FLAT_PARALLEL
    int n_squared = n*n;

    matrix_multiply(n, b, a, temp);
    #pragma omp parallel for
    for (int x = 0; x < n_squared; x++) {
        int i = x / n;
        int j = x % n;
        r[i][j] = (i == j) - temp[i][j];
    }

    matrix_multiply(n, r, b, temp);
    #pragma omp parallel for
    for (int x = 0; x < n_squared; x++) {
        int i = x / n;
        int j = x % n;
        next[i][j] = temp[i][j] + b[i][j];
    }
#else
    matrix_multiply(n, b, a, temp);
    #pragma omp parallel for
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            r[i][j] = (i == j) - temp[i][j];
        }
    }

    matrix_multiply(n, r, b, temp);
    #pragma omp parallel for
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            next[i][j] = temp[i][j] + b[i][j];
        }
    }
#endif

    /*
    // wersja sekwencyjna
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
    */
}

bool is_identity(int n, double matrix[n][n])
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if ((i == j && matrix[i][j] != 1.0) || (i != j && matrix[i][j] != 0.0)) {
                return false;
            }
        }
    }
    return true;
}

double matrix_norm(int n, double matrix[n][n]) {
    double sum = 0.0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            sum += matrix[i][j] * matrix[i][j];
        }
    }

    return sqrt(sum);
}

bool matrix_has_invalid (int n, double matrix[n][n]) {
//    int invalid = 0;

//    #pragma omp parallel for reduction(+:invalid)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (isnan(matrix[i][j]) || isinf(matrix[i][j])) {
               return true;
//                invalid++;
            }
        }
    }

//    return invalid < 1;
    return false;
}
