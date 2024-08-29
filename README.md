# Arpl - A Real Programming Language, Our Pal

## History
Arpl started out many years ago 1990s ish as a C compiler. It has since gained a number of features of its own.
Coding work started on Arpl at home and *before* the internet became popular. Some of the original code is not the best.

# Arpl Compiler
Arpl is an optimizing compiler. It does many optimizations, but it is not GCC or LLVM.
It does common subexpression elimination and many peephole optimizations.
For instance, it looks for def's without uses's and removed code where possible.

## Language Features
Arpl can compile most 'C' programs. So if you know C you know a lot of arpl.
There are additional convenience keywords like 'until' documented in the arpl guide.
There are additional operators not found in 'C'.
There is a multiplex operator ?? that works in a fashion similar to ?
Using features specific to arpl will will result in a program that is not 'C' compatible.
The default calling convention is Pascal calling convention where arguments are popped off the stack by the called function.
Arpl supports nested functions like Pascal. It also support co-routines and the 'yield' keyword.

# Test Suite
There is a port of a 'C' language test suite originally licensed by Andrew Chambers to arpl code which is a work-in-progress itself.
There are a handful of new test routines specific to arpl.

# Ports
The current compiler is somewhat specific to the Qupls architecture. The compiler is quite large and complex.
It was not designed to be a portable compiler.
It was used in the past to compile simple programs for the Thor project.
There is a trial port of the compiler for Riscv code, but is untested and not known to be working.
A port to i386 code is also in progress. Resulting code does not work yet for the i386.

# Status
Still very much a work-in-progress. The code is not stable.
The compiler generates code that looks reasonable in most cases.

