#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <stdbool.h>


double A[5][5] = {
    { -1, 0.3, 0, 0, 0},
    { 0, 0.1, 0, 0, 0},
    { 0, 0, 0.4, 0, 0},
    { 0, 0, 0, 1, 0},
    { 0, 0, 0, 0.3, 0.01},
};

double I[5][5] = {
    {1, 0, 0, 0, 0},
    {0, 1, 0, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 0, 1, 0},
    {0, 0, 0, 0, 1},
};

double B[5][5] = {};
