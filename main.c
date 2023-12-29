#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>

#define MAX_TITLE_LENGTH 512

#define MATRIX_SIZE 2
#define MAX_ITERATIONS 25

#if MATRIX_SIZE == 2
#include "vars_2.h"
#elif MATRIX_SIZE == 3
#include "vars_3.h"
#elif MATRIX_SIZE == 4
#include "vars_4.h"
#elif MATRIX_SIZE == 5
#include "vars_5.h"
#elif MATRIX_SIZE == 10
#include "vars_10.h"
#else
#include "vars_2.h"
#endif



bool is_identity(int n, double matrix[n][n]);
void matrix_print(int n, double matrix[n][n], const char *format, ...);

void matrix_copy(int n, double source[n][n], double destination[n][n]);
void matrix_multiply(int n, double A[n][n], double B[n][n], double C[n][n]);
void matrix_subtract(int n, double A[n][n], double B[n][n], double C[n][n]);
void matrix_inversion_iteration(int n, double A[n][n], double B[n][n], double B_next[n][n]);

double determinant(int n, double matrix[n][n]);


double result[MATRIX_SIZE][MATRIX_SIZE];

int main()
{

    if (MATRIX_SIZE == 2 &&
            MATRIX_SIZE == 3 &&
            MATRIX_SIZE == 4 &&
            MATRIX_SIZE == 5) {
        printf("Invalid MATRIX_SIZE");
        exit(1);
    }

    printf("MATRIX_SIZE: %d\n", MATRIX_SIZE);
    if (determinant(MATRIX_SIZE, A) == 0) {
        printf("MACIERZ JEST OSOBLIWA.\n");
        exit(0);
    }

    matrix_print(MATRIX_SIZE, A, "A");
    matrix_print(MATRIX_SIZE, I, "I");

    matrix_multiply(MATRIX_SIZE, A, I, result);
    matrix_print(MATRIX_SIZE, result, "A * I");

    matrix_print(MATRIX_SIZE, B, "B");
    double B_next[MATRIX_SIZE][MATRIX_SIZE];
    double B_temp[MATRIX_SIZE][MATRIX_SIZE];

    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        matrix_inversion_iteration(MATRIX_SIZE, A, B, B_next);
        matrix_print(MATRIX_SIZE, B_next, "iter:%d, Next iteration of B", iter);
        matrix_multiply(MATRIX_SIZE, B_next, A, B_temp);

        if (is_identity(MATRIX_SIZE, B_temp)) {
            printf("FOUND!\n");
            break;
        } else {
            matrix_copy(MATRIX_SIZE, B_next, B);
        }


    }




    matrix_print(MATRIX_SIZE, B, "Final B");
    matrix_multiply(MATRIX_SIZE, B, A, result);

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

    printf("%s: [\n", title);

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
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            c[i][j] = 0;

            for(int k = 0; k < n; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void matrix_subtract(int n, double a[n][n], double b[n][n], double c[n][n])
{
    // Odejmuje macierz B od A, wynik w C
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

void matrix_inversion_iteration(int n, double a[n][n], double b[n][n], double b_next[n][n])
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
            b_next[i][j] = temp[i][j] + b[i][j];
        }
    }
}

bool is_identity(int n, double matrix[n][n])
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if ((i == j && matrix[i][j] != 1) || (i != j && matrix[i][j] != 0)) {
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
