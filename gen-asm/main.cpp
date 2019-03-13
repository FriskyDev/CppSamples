#include <cstdio>
#include <cstdlib>
#include <cstdint>

void foo()
{
    printf("foo\n");
}

int bar()
{
    printf("bar\n");
    return 42;
}

int dee(int x)
{
    printf("dee\n");
    return x * 2;
}

int doo(int* x)
{
    printf("doo\n");
    if (x) {
        return *x * 4;
    }
    return 0;
}

int main(int argc, char* argv[], int envc, char* envv[])
{
    foo();
    int x = bar();
    x = dee(x);
    int y = doo(&x);
    printf("y=%d\n", y);

    return EXIT_SUCCESS;
}