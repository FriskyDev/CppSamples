# C++ Sample Code

## DebugExample

Code that can be used as an example in WindBg for debugging type scenarios.

## TreeTest
Simple project that creates a binary tree, adds some nodes to the tree, and traverses the tree without recusion.

## Try-Finally
C++ does not include a try/finally because it supports RAII (Resource Acquisition is Initialization). In simple terms, an object
initializes itself in the constructor and cleans up after itself in the destructor. Using this pattern, you don't need finally.
This sample shows a very simple mechanism using lambda's to execute the cleanup at the end of a try block regardless of whether
the function throws.

## DC-Tree

Direct Composition example code.

## gen-asm

Stands for generate assembly language. This project allows you to put code in a file, compile it, and look at
the assembly/machine code output from the compile. The output file containing assembly code is `main.cod` and the map
file is `main.map`.