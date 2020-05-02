PALC - PAL Compiler
===================

C++ compiler frontend for the PAL (Pretty Awful Language). `palc` implements
the lexical and semantic analysis stages of compilation, as well as basic
code generation for the OrbitVM stack-based machine. Code generation is
sources are restricted to `src/codegen/`. Code generation calls in
`src/PALParser.cpp` are marked with `// NOTE: Code Generation Code/`.

## Building

`palc` is written in C++11 and should compile with any compliant compiler. It
was tested with Clang (`Apple LLVM version 8.0.0 (clang-800.0.42.1)`) and GCC.
To compile, `cd` to the project directory and run `make`.




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
 
