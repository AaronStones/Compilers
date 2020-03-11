PALC - PAL Compiler
===================

C++ compiler frontend for the PAL (Pretty Awful Language). `palc` implements
the lexical and semantic analysis stages of compilation, as well as basic
code generation for the OrbitVM stack-based machine. Code generation is
sources are restricted to `src/codegen/`. Code generation calls in
`src/PALParser.cpp` are marked with `// NOTE: Code Generation Code/`.

## Usage

`palc` is invoked by calling `palc <input file> <output file>`. If no output
file is specified, `palc` will output an Orbit Module File to `a.omf`.

## Building

`palc` is written in C++11 and should compile with any compliant compiler. It
was tested with Clang (`Apple LLVM version 8.0.0 (clang-800.0.42.1)`) and GCC.
To compile, `cd` to the project directory and run `make`.

The resulting binary is located at `./palc`. The tests PAL programs are located
in `tests`, and can be tested by running `make tests`.

## Error Reporting

Defining `COLORFUL=1` at compile time (`make palc COLORFUL=1`) enables coloured
error printing. This allows nicer error messages on terminals that support
ANSI colours.

Error messages are meant to carry as much meaning as possible for a project of
that size. Whenever possible, the source line that caused the error is printed,
and the troublesome token highlighted.

    $ palc test.pal.txt 
    5:15: error: redefinition of 'i'
        someReal, i AS REAL
                  ^
    4:5: note: variable 'i' was declared here
        i AS INTEGER
        ^
    7:13: error: invalid operands to boolean expression ('Integer' and 'Real')
        UNTIL i = 100.0 REPEAT
                ^
    12:14: error: expected 'boolean expression'
        IF i + 2 THEN
                 ^~~~
    9:9: error: use of undeclared variable 'testVar'
            testVar = 100.2
            ^~~~~~~
    13:18: error: assigning 'Real' from incompatible type 'Integer'
            someReal = 2
                     ^
    5:5: note: variable 'someReal' was declared here
        someReal, i AS REAL
        ^~~~~~~~
    5 errors generated

## Structure

 * `src/common.hpp`: typedefs and declarations used across `palc`.
 
 * `src/Compiler.cpp`: program's entry point, drives the parser.
 
 * `src/PALError.*`: error class for `palc`, defines how errors are stored and printed.
 
 * `src/PALToken.*`: token representation used by `palc`. Stores the line &
   column numbers, the type and text value of the token, as well as a copy of
   the source line's text where the token was encountered.
 
 * `src/PALScanner.*`: lexer for the PAL language, derived from Allan Milne's
   C# implementation PALScanner.cs.
 
 * `src/PALParser.*`: Recursive-Descent parser for the PAL language, derived
   from the EBNF grammar.
 
 * `src/PALSemantics.*`: semantic analyser for the PAL language. Provides
   variable declaration and type checking, as well as an expression type checker.
 
