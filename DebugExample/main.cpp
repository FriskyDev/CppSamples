#include <iostream>

void DisplayGreeting(const char* const value)
{
    const char* const prefix = "Hello";
    printf("%s, %s\n", prefix, value);
}

int main()
{
    DisplayGreeting("debugger");
}

