#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <stdbool.h>


double A[5][5] = {
    { 0,0,0, .1, 1},
    { 0,0.0005,0, 0.0029, -0.3},
    { -.1,0.05,0, 0, 0.006},
    { -0.01,0,0.02, -1, 0.08},
    { 0,0, 0,0.01, -0.0003},
};

double I[5][5] = {
    {1, 0, 0, 0, 0},
    {0, 1, 0, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 0, 1, 0},
    {0, 0, 0, 0, 1},
};

double B[5][5] = {
    { 0,0,0, .1, 1},
    { 0,0.0005,0, 0.0029, -0.3},
    { -.1,0.05,0, 0, 0.006},
    { -0.01,0,0.02, -1, 0.08},
    { 0,0, 0,0.01, -0.0003},
};
