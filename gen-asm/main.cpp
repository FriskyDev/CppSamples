#include <stdio.h>

float multiply(float a, float b)
{
    float c = a * b;
    return c;
}

int main()
{
    float a = 10;
    float b = 3;
    float c = a * b;

    c = multiply(a, b);


    int n = 0, m;
    scanf_s("%d", &m);
    for (int i = 0; i < m; ++i) {
        n += i;
    }
    for (int j = 0; j < m; ++j) {
        n += j;
    }
    printf("%d", n);
    return 0;
}