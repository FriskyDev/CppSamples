#include <stdio.h>

float multiply(float a, float b)
{
    float c = a * b;
    return c;
}

void DisplayGreeting(const char const* value)
{
    const char const* prefix = "Hello";
    printf("%s, %s\n", prefix, value);
    return;
}

int main()
{
    DisplayGreeting("world");

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