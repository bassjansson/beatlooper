#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

#define MAX_FRACTION_PRODUCT 32

const int integers[] = {
    1,
    2, 3,
    4, 6,  5,
    8, 12, 10, 7,
    16,24, 20, 14, 9,
    32
};

const int NUM_OF_INTEGERS  = sizeof(integers) / sizeof(int);
const int NUM_OF_FRACTIONS = NUM_OF_INTEGERS * NUM_OF_INTEGERS;

struct Fraction
{
    int   x;
    int   y;
    float f;
};

Fraction fractions[NUM_OF_FRACTIONS];

bool compareFractions(Fraction a, Fraction b)
{
    if (a.f == b.f)
        return a.x < b.x;

    return a.f < b.f;
}

int main()
{
    for (int i = 0; i < NUM_OF_FRACTIONS; ++i)
    {
        fractions[i].x = integers[i % NUM_OF_INTEGERS];
        fractions[i].y = integers[i / NUM_OF_INTEGERS];
        fractions[i].f = (float) fractions[i].x / (float) fractions[i].y;
    }

    vector<Fraction> fracVec(fractions, fractions + NUM_OF_FRACTIONS);

    sort(fracVec.begin(), fracVec.end(), compareFractions);

    cout
        <<
        "\
#ifndef __FRACTIONS_H__\n\
#define __FRACTIONS_H__\n\
\n\
struct Fraction\n\
{\n\
    int   x;\n\
    int   y;\n\
    float f;\n\
};\n\
\n\
const Fraction FRACTIONS_ARRAY[] = {\n";

    for (int i = 0; i < NUM_OF_FRACTIONS; ++i)
    {
        if (i > 0)
        {
            if (fracVec[i].f == fracVec[i - 1].f)
                continue;
        }

        if (fracVec[i].x * fracVec[i].y > MAX_FRACTION_PRODUCT)
            continue;

        cout << "    { " << fracVec[i].x;
        cout << ", " << fracVec[i].y;
        cout << ", " << fracVec[i].f << " }";
        cout << (i < NUM_OF_FRACTIONS - 1 ? "," : "");
        cout << endl;
    }

    cout
        <<
        "\
};\n\
\n\
const int FRACTIONS_ARRAY_LENGTH = sizeof(FRACTIONS_ARRAY) / sizeof(Fraction);\n\
\n\
#endif // __FRACTIONS_H__\n";

    return 0;
} // main
