#include <stdio.h>

int main()
{
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