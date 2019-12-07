#ifndef __FRACTIONS_H__
#define __FRACTIONS_H__

struct Fraction
{
    int   x;
    int   y;
    float f;
};

const Fraction FRACTIONS_ARRAY[] = {
    { 1, 32, 0.03125 },
    { 1, 30, 0.0333333 },
    { 1, 28, 0.0357143 },
    { 1, 27, 0.037037 },
    { 1, 25, 0.04 },
    { 1, 24, 0.0416667 },
    { 1, 21, 0.047619 },
    { 1, 20, 0.05 },
    { 1, 18, 0.0555556 },
    { 1, 16, 0.0625 },
    { 1, 15, 0.0666667 },
    { 1, 14, 0.0714286 },
    { 2, 27, 0.0740741 },
    { 2, 25, 0.08 },
    { 1, 12, 0.0833333 },
    { 3, 32, 0.09375 },
    { 2, 21, 0.0952381 },
    { 1, 10, 0.1 },
    { 3, 28, 0.107143 },
    { 1, 9, 0.111111 },
    { 3, 25, 0.12 },
    { 1, 8, 0.125 },
    { 2, 15, 0.133333 },
    { 1, 7, 0.142857 },
    { 4, 27, 0.148148 },
    { 3, 20, 0.15 },
    { 5, 32, 0.15625 },
    { 4, 25, 0.16 },
    { 1, 6, 0.166667 },
    { 5, 28, 0.178571 },
    { 5, 27, 0.185185 },
    { 3, 16, 0.1875 },
    { 4, 21, 0.190476 },
    { 1, 5, 0.2 },
    { 5, 24, 0.208333 },
    { 3, 14, 0.214286 },
    { 7, 32, 0.21875 },
    { 2, 9, 0.222222 },
    { 7, 30, 0.233333 },
    { 5, 21, 0.238095 },
    { 6, 25, 0.24 },
    { 1, 4, 0.25 },
    { 7, 27, 0.259259 },
    { 4, 15, 0.266667 },
    { 5, 18, 0.277778 },
    { 7, 25, 0.28 },
    { 9, 32, 0.28125 },
    { 2, 7, 0.285714 },
    { 7, 24, 0.291667 },
    { 8, 27, 0.296296 },
    { 3, 10, 0.3 },
    { 5, 16, 0.3125 },
    { 8, 25, 0.32 },
    { 9, 28, 0.321429 },
    { 1, 3, 0.333333 },
    { 7, 20, 0.35 },
    { 5, 14, 0.357143 },
    { 9, 25, 0.36 },
    { 10, 27, 0.37037 },
    { 3, 8, 0.375 },
    { 8, 21, 0.380952 },
    { 7, 18, 0.388889 },
    { 2, 5, 0.4 },
    { 5, 12, 0.416667 },
    { 3, 7, 0.428571 },
    { 7, 16, 0.4375 },
    { 4, 9, 0.444444 },
    { 9, 20, 0.45 },
    { 7, 15, 0.466667 },
    { 15, 32, 0.46875 },
    { 10, 21, 0.47619 },
    { 12, 25, 0.48 },
    { 1, 2, 0.5 },
    { 14, 27, 0.518519 },
    { 8, 15, 0.533333 },
    { 15, 28, 0.535714 },
    { 5, 9, 0.555556 },
    { 14, 25, 0.56 },
    { 9, 16, 0.5625 },
    { 4, 7, 0.571429 },
    { 7, 12, 0.583333 },
    { 16, 27, 0.592593 },
    { 3, 5, 0.6 },
    { 5, 8, 0.625 },
    { 16, 25, 0.64 },
    { 9, 14, 0.642857 },
    { 21, 32, 0.65625 },
    { 2, 3, 0.666667 },
    { 7, 10, 0.7 },
    { 5, 7, 0.714286 },
    { 18, 25, 0.72 },
    { 20, 27, 0.740741 },
    { 3, 4, 0.75 },
    { 16, 21, 0.761905 },
    { 7, 9, 0.777778 },
    { 25, 32, 0.78125 },
    { 4, 5, 0.8 },
    { 5, 6, 0.833333 },
    { 21, 25, 0.84 },
    { 27, 32, 0.84375 },
    { 6, 7, 0.857143 },
    { 7, 8, 0.875 },
    { 8, 9, 0.888889 },
    { 25, 28, 0.892857 },
    { 9, 10, 0.9 },
    { 25, 27, 0.925926 },
    { 14, 15, 0.933333 },
    { 15, 16, 0.9375 },
    { 20, 21, 0.952381 },
    { 24, 25, 0.96 },
    { 27, 28, 0.964286 },
    { 1, 1, 1 },
    { 28, 27, 1.03704 },
    { 25, 24, 1.04167 },
    { 21, 20, 1.05 },
    { 16, 15, 1.06667 },
    { 15, 14, 1.07143 },
    { 27, 25, 1.08 },
    { 10, 9, 1.11111 },
    { 28, 25, 1.12 },
    { 9, 8, 1.125 },
    { 8, 7, 1.14286 },
    { 7, 6, 1.16667 },
    { 32, 27, 1.18519 },
    { 25, 21, 1.19048 },
    { 6, 5, 1.2 },
    { 5, 4, 1.25 },
    { 32, 25, 1.28 },
    { 9, 7, 1.28571 },
    { 21, 16, 1.3125 },
    { 4, 3, 1.33333 },
    { 27, 20, 1.35 },
    { 25, 18, 1.38889 },
    { 7, 5, 1.4 },
    { 10, 7, 1.42857 },
    { 3, 2, 1.5 },
    { 32, 21, 1.52381 },
    { 14, 9, 1.55556 },
    { 25, 16, 1.5625 },
    { 8, 5, 1.6 },
    { 5, 3, 1.66667 },
    { 27, 16, 1.6875 },
    { 12, 7, 1.71429 },
    { 7, 4, 1.75 },
    { 16, 9, 1.77778 },
    { 25, 14, 1.78571 },
    { 9, 5, 1.8 },
    { 28, 15, 1.86667 },
    { 15, 8, 1.875 },
    { 27, 14, 1.92857 },
    { 2, 1, 2 },
    { 25, 12, 2.08333 },
    { 21, 10, 2.1 },
    { 32, 15, 2.13333 },
    { 15, 7, 2.14286 },
    { 20, 9, 2.22222 },
    { 9, 4, 2.25 },
    { 16, 7, 2.28571 },
    { 7, 3, 2.33333 },
    { 12, 5, 2.4 },
    { 5, 2, 2.5 },
    { 18, 7, 2.57143 },
    { 21, 8, 2.625 },
    { 8, 3, 2.66667 },
    { 27, 10, 2.7 },
    { 25, 9, 2.77778 },
    { 14, 5, 2.8 },
    { 20, 7, 2.85714 },
    { 3, 1, 3 },
    { 28, 9, 3.11111 },
    { 25, 8, 3.125 },
    { 16, 5, 3.2 },
    { 10, 3, 3.33333 },
    { 27, 8, 3.375 },
    { 24, 7, 3.42857 },
    { 7, 2, 3.5 },
    { 32, 9, 3.55556 },
    { 25, 7, 3.57143 },
    { 18, 5, 3.6 },
    { 15, 4, 3.75 },
    { 27, 7, 3.85714 },
    { 4, 1, 4 },
    { 25, 6, 4.16667 },
    { 21, 5, 4.2 },
    { 30, 7, 4.28571 },
    { 9, 2, 4.5 },
    { 32, 7, 4.57143 },
    { 14, 3, 4.66667 },
    { 24, 5, 4.8 },
    { 5, 1, 5 },
    { 21, 4, 5.25 },
    { 16, 3, 5.33333 },
    { 27, 5, 5.4 },
    { 28, 5, 5.6 },
    { 6, 1, 6 },
    { 25, 4, 6.25 },
    { 32, 5, 6.4 },
    { 20, 3, 6.66667 },
    { 27, 4, 6.75 },
    { 7, 1, 7 },
    { 15, 2, 7.5 },
    { 8, 1, 8 },
    { 25, 3, 8.33333 },
    { 9, 1, 9 },
    { 28, 3, 9.33333 },
    { 10, 1, 10 },
    { 21, 2, 10.5 },
    { 32, 3, 10.6667 },
    { 12, 1, 12 },
    { 25, 2, 12.5 },
    { 27, 2, 13.5 },
    { 14, 1, 14 },
    { 15, 1, 15 },
    { 16, 1, 16 },
    { 18, 1, 18 },
    { 20, 1, 20 },
    { 21, 1, 21 },
    { 24, 1, 24 },
    { 25, 1, 25 },
    { 27, 1, 27 },
    { 28, 1, 28 },
    { 30, 1, 30 },
    { 32, 1, 32 }
};

const int FRACTIONS_ARRAY_LENGTH = sizeof(FRACTIONS_ARRAY) / sizeof(Fraction);

#endif // __FRACTIONS_H__
