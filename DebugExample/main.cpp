#include <iostream>

char crash_me(const char* const value)
{
    char x = value[0];
    return x;
}

void DisplayGreeting(const char* const value)
{
    const char* const prefix = "Hello";
    printf("%s, %s\n", prefix, value);

    auto a = crash_me(nullptr);
    printf("%c\n", a);
}

int main()
{
    DisplayGreeting("debugger");
}
