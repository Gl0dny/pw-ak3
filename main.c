#include <stdio.h>
#include <assert.h>


//void matrix_print(const char *title, int rows, int cols, double matrix[rows][cols]) {
void matrix_print(const char *title, int n, double matrix[n][n]) {
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

void matrix_multiply(int n, double A[n][n], double B[n][n], double C[n][n]) {
    // Mnoży macierze A i B, wynik w C
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            C[i][j] = 0;

            for(int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void matrix_subtract(int n, double A[n][n], double B[n][n], double C[n][n]) {
    // Odejmuje macierz B od A, wynik w C
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
}

void matrix_inversion_iteration(int n, double A[n][n], double B[n][n], double B_next[n][n]) {
    // Oblicza k-tą iterację odwracania macierzy
    double R[n][n], temp[n][n];

    // R(B) = I - BA
    matrix_multiply(n, B, A, temp);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            R[i][j] = (i == j) - temp[i][j];
        }
    }

    // B_{k+1} = (I + R(B_k))B_k
    matrix_multiply(n, R, B, temp);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            B_next[i][j] = temp[i][j] + B[i][j];
        }
    }
}



int main() {

    // Definicja macierzy A i macierzy jednostkowej I
    double A[2][2] = {{2, 3}, {1, 4}};
    double I[2][2] = {{1, 0}, {0, 1}};
    double result[2][2];

    matrix_print("A", 2, A);
//    matrix_print("A", 2, 2, A);
    matrix_print("I", 2, I);
//    matrix_print("I", 2, 2, I);

    // Mnożenie A przez I
    matrix_multiply(2, A, I, result);
//    matrix_print("A * I", 2, 2, result);
    matrix_print("A * I", 2, result);

    // Przykład obliczania iteracji odwracania macierzy
    double B[2][2] = {{0.5, 0}, {0, 0.25}}; // Początkowa aproksymacja A^-1
//    matrix_print("B", 2, 2, B);
    matrix_print("B", 2, B);
    double B_next[2][2];

    for (int iter = 0; iter < 4; iter++) {
        matrix_inversion_iteration(2, A, B, B_next);
//        matrix_print("Next iteration of B", 2, 2, B_next);
        matrix_print("Next iteration of B", 2, B_next);
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                B[i][j] = B_next[i][j];
            }
        }
    }

//    matrix_print("Final B", 2, 2, B);
    matrix_print("Final B", 2, B);

    matrix_multiply(2, B, A, result);
//    matrix_print("The result is", 2, 2, result);
    matrix_print("The result is", 2,result);

    return 0;
}
