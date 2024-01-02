#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <omp.h>

#define MAX_TITLE_LENGTH 512

#define MATRIX_SIZE 5
#define MAX_ITERATIONS 25

#if MATRIX_SIZE == 2
#include "vars_2.h"
#elif MATRIX_SIZE == 3
#include "vars_3.h"
#elif MATRIX_SIZE == 4
#include "vars_4.h"
#elif MATRIX_SIZE == 5
#include "vars_5.h"
#else
#include "vars_2.h"
#endif

void matrix_print(int n, double matrix[n][n], const char *format, ...);
void matrix_copy(int n, double source[n][n], double destination[n][n]);
void matrix_multiply(int n, double A[n][n], double B[n][n], double C[n][n]);
void matrix_inversion_iteration(int n, double A[n][n], double B[n][n], double next[n][n]);

void matrix_zero(int n, double matrix[n][n]);

double matrix_norm(int n, double matrix[n][n]);
double determinant(int n, double matrix[n][n]);

bool is_identity(int n, double matrix[n][n]);

bool matrix_has_invalid (int n, double matrix[n][n]);


int main(int argc, char* argv[])
{
    struct timeval start, end;
    int n = MATRIX_SIZE;
    assert(n > 1 && n < 6);

    double next[n][n];
    double temp[n][n];

    bool found = false;
    bool invalid = false;

    omp_set_num_threads(n*n);

    printf("MATRIX_SIZE: %d\n", MATRIX_SIZE);
    if (determinant(MATRIX_SIZE, A) == 0) {
        printf("MACIERZ JEST OSOBLIWA.\n");
        exit(0);
    }

    matrix_print(n, A, "macierz");
    matrix_print(n, I, "macierz jednostkowa");
    matrix_print(n, B, "inicjalna macierz odwrócona");

    gettimeofday(&start, NULL);

    clock_t cpu0 = clock();

    int i;
    for (i = 0; i < MAX_ITERATIONS; i++) {
        matrix_inversion_iteration(n, A, B, next);

        matrix_multiply(n, next, A, temp);
        if (is_identity(n, temp)) {
            matrix_print(n,next, "Macierz odnaleziona");
            found = true;
            break;
        }

        if (matrix_has_invalid(n, next)) {
            matrix_print(n,next, "NAN lub INF");
            invalid = true;
            break;
        }
        matrix_copy(n, next, B);
    }

    clock_t cpu1 = clock();

    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    printf("\nCzas wykonania %.6f s\n", elapsed);
    double cpuTime = (double)(cpu1 - cpu0) / CLOCKS_PER_SEC;
    printf("\nCzas procesora %.6f s\n", cpuTime);

    if (!found || invalid) {
        printf("\nNiepowodzenie");
        invalid && printf("\nINF lub NAN\n");
        !found && printf("\nNie znaleziono w %d iteracjach.\n", i);
    } else {
        printf("\nOK\n");
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

///*
#pragma omp parallel
{
    int thread_id = omp_get_thread_num();
    int row = thread_id / n;
    int col = thread_id % n;

    c[row][col] = 0;
    for (int k = 0; k < n; k++) {
        c[row][col] += a[row][k] * b[k][col];
    }

#pragma omp barrier
}
//*/

    /*
    // wersja sekwencyjna
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            c[i][j] = 0;

            for (int k = 0; k < n; k++) {
                // tu dodać printy
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

/*
#pragma omp parallel
{
    int thread_id = omp_get_thread_num();
    int row = thread_id / n;
    int col = thread_id % n;

    destination[row][col] = source[row][col];
#pragma omp barrier
}
*/

    ///*
    // wersja sekwencyjna
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            destination[i][j] = source[i][j];
        }
    }
    //*/
}

void matrix_inversion_iteration(int n, double a[n][n], double b[n][n], double next[n][n])
{
    double r[n][n], temp[n][n];

    /*
    matrix_multiply(n, b, a, temp);
#pragma omp parallel
{
    int thread_id = omp_get_thread_num();
    int row = thread_id / n;
    int col = thread_id % n;

    r[row][col] = (row == col) - temp[row][col];
#pragma omp barrier
}

    matrix_multiply(n, r, b, temp);
#pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int row = thread_id / n;
        int col = thread_id % n;

        r[row][col] = (row == col) - temp[row][col];
#pragma omp barrier
    }
    */


    ///*
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
    //*/
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

double determinant(int n, double matrix[n][n])
{
    int i, j, k;
    double det = 1;
    double ratio;

    for (i = 0; i < n; i++) {
        if (matrix[i][i] == 0.0) {
            for (j = i + 1; j < n; j++) {
                if (matrix[j][i] != 0.0) {
                    for (k = 0; k < n; k++) {
                        double temp = matrix[i][k];
                        matrix[i][k] = matrix[j][k];
                        matrix[j][k] = temp;
                    }
                    det *= -1;
                    break;
                }
            }
        }

        if (matrix[i][i] == 0.0)
            return 0;

        for (j = i + 1; j < n; j++) {
            ratio = matrix[j][i] / matrix[i][i];
            for (k = i; k < n; k++)
                matrix[j][k] -= ratio * matrix[i][k];
        }
    }

    for (i = 0; i < n; i++) {
        det *= matrix[i][i];
    }

    return det;
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
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (isnan(matrix[i][j]) || isinf(matrix[i][j])) {
               return true;
            }
        }
    }

    return false;
}
