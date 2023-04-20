#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;

float f(float x) {
    return x - 10 * sin(x) + abs(pow(x, 2) - x * exp(5));
}

int main() {
    float y, x;
    int N = 19;
    int k = N / 2;
    int h = N / k;

    int i = 1;
    while (i <= N + 1) {
        y = f(x);
        i += h;
        cout << "x = " << x << "; y = " << y << endl;
    }

    return 0;
}
