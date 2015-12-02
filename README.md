# C++ Sample Code

## TreeTest
Simple project that creates a binary tree, adds some nodes to the tree, and traverses the tree without recusion.

## Try-Finally
C++ does not include a try/finally because it supports RAII (Resource Acquisition is Initialization). In simple terms, an object
initializes itself in the constructor and cleans up after itself in the destructor. Using this pattern, you don't need finally.
This sample shows a very simple mechanism using lambda's to execute the cleanup at the end of a try block regardless of whether
the function throws.
