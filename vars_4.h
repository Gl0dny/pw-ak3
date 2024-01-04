#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <stdbool.h>


double A[4][4] = {
{0.2, 0.1, 0, 0},
{0, 0.04, -0.2, 0.1},
{0, 0.02, 1, 0},
{0.1, 0.1, 0, 0},
};

double I[4][4] = {
{1, 0, 0, 0},
{0, 1, 0, 0},
{0, 0, 1, 0},
{0, 0, 0, 1},
};

double B[4][4] = {};

