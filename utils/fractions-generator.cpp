#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

const int NUM_OF_INTEGERS  = 23;
const int NUM_OF_FRACTIONS = NUM_OF_INTEGERS * NUM_OF_INTEGERS;

const int integers[NUM_OF_INTEGERS] = {
    1,  2,  3,  4,  5,  6, 7, 8, 9, // 9
    10, 12, 14, 15, 16, 18,         // 6
    20, 21, 24, 25, 27, 28,         // 6
    30, 32                          // 2
};

struct Fraction
{
    int   x;
    int   y;
    float f;
};

Fraction fractions[NUM_OF_FRACTIONS];

bool compareFractions(Fraction a, Fraction b)
{
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

    cout << "const Fraction constantFractions[] = {" << endl;

    for (int i = 0; i < NUM_OF_FRACTIONS; ++i)
    {
        cout << "\t{ " << fracVec[i].x;
        cout << ", " << fracVec[i].y;
        cout << ", " << fracVec[i].f << "f }";
        if (i < NUM_OF_FRACTIONS - 1) cout << ",";
        cout << endl;
    }

    cout << "};" << endl;

    return 0;
}
