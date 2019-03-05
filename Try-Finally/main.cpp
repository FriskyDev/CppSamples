#include "FinalAction.h"

#include <Windows.h>

#include <exception>
#include <iostream>
#include <functional>


using namespace std;


int main(int argc, char* argv[])
{
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    int* a = new int;
    try {
        //int* a = nullptr;
        auto delete_a = finally([&a]() {
            delete a;
            a = nullptr;
            cout << "leaving block, deleting a" << endl;
        });
        cout << "doing something" << endl;
        throw exception();
    }
    catch (...) {
        cerr << "Exception thrown" << endl;
    }

    exit(EXIT_SUCCESS);
}

