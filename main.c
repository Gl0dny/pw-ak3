#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <stdbool.h>

#define MAX_TITLE_LENGTH 256



bool is_identity(int n, double matrix[n][n]);
void matrix_print(int n, double matrix[n][n], const char *title);
void matrix_print2(int n, double matrix[n][n], const char *format, ...);

void matrix_multiply(int n, double A[n][n], double B[n][n], double C[n][n]);
void matrix_subtract(int n, double A[n][n], double B[n][n], double C[n][n]);
void matrix_inversion_iteration(int n, double A[n][n], double B[n][n], double B_next[n][n]);


double A[5][5] = {
    {0.25, 0.33, -0.75, 1, 1},
    {0, -1, 0, 0.75, 0.5},
    {0, 0, 0, -1, 0},
    {0, 0, 0, 0.75, -1},
    {0, -0.5, 0, 0.33, 0.25}
};

// Definicja macierzy jednostkowej I
double I[5][5] = {
    {1, 0, 0, 0, 0},
    {0, 1, 0, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 0, 1, 0},
    {0, 0, 0, 0, 1},
};

// Początkowa aproksymacja A^-1
double B[5][5] = {
    {1,0,0, 0, 0},
    {0,1,0, 0, 0},
    {0,0,1, 0, 0},
    {0,0,0, 1, 0},
    {0,0,0, 0, 1}
};


double result[5][5];

int main() {
    /*
    // Definicja macierzy A
    double A[5][5] = {
        {2, 3, -2, 1, 1},
        {1, -1, 0, 2, 2},
        {1, 0, 0, 1, 0},
        {1, 0, 0, 2, 1},
        {1, -0.5, 0, 2, -0.25}
    };
    // Definicja macierzy jednostkowej I
    double I[5][5] = {
        {1, 0, 0, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 0, 0, 1},
    };

    // Początkowa aproksymacja A^-1
    double B[5][5] = {
        {0.5, 0.25, 0, -0.25, -5},
        {0, 0.25,0, 1, 2},
        {1, -0.25,0, 1, 2},
        {0, 0.25,0, 0, -0.25},
        {1, 0,0, 1, -1}
    };
     */


    matrix_print(5, A, "A");
    matrix_print(5, I, "I");

    // Mnożenie A przez I
    matrix_multiply(5, A, I, result);
    matrix_print(5, result, "A * I");

    // Przykład obliczania iteracji odwracania macierzy
    matrix_print(5, B, "B");
    double B_next[5][5];

    for (int iter = 0; iter < 100; iter++) {
        matrix_inversion_iteration(5, A, B, B_next);
        matrix_print2(5, B_next, "iter:%d, Next iteration of B", iter);
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                B[i][j] = B_next[i][j];
            }
        }
    }

    matrix_print(5, B, "Final B");

    matrix_multiply(5, B, A, result);
    matrix_print(5,result, "The result is");

    return 0;
}

void matrix_print2(int n, double matrix[n][n], const char *format, ...) {
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

void matrix_print(int n, double matrix[n][n], const char *title) {
    assert(title != NULL);
    printf("%s: [\n", title);

    assert(matrix != NULL);
    assert(n > 0);

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

void matrix_multiply(int n, double a[n][n], double b[n][n], double c[n][n]) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            c[i][j] = 0;

            for(int k = 0; k < n; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void matrix_subtract(int n, double a[n][n], double b[n][n], double c[n][n]) {
    // Odejmuje macierz B od A, wynik w C
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            c[i][j] = a[i][j] - b[i][j];
        }
    }
}

void matrix_inversion_iteration(int n, double a[n][n], double b[n][n], double b_next[n][n]) {
    // Oblicza k-tą iterację odwracania macierzy
    double r[n][n], temp[n][n];

    // r(B) = I - BA
    matrix_multiply(n, b, a, temp);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            r[i][j] = (i == j) - temp[i][j];
        }
    }

    // B_{k+1} = (I + R(B_k))B_k
    matrix_multiply(n, r, b, temp);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            b_next[i][j] = temp[i][j] + b[i][j];
        }
    }
}

bool is_identity(int n, double matrix[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if ((i == j && matrix[i][j] != 1) || (i != j && matrix[i][j] != 0)) {
                return false;
            }
        }
    }
    return true;
}
