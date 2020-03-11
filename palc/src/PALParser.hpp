//
//  PALParser.hpp - PAL Compiler's parser interface
//  PAL Compiler
//
//  Created by Amy Parent on 2017-02-17.
//  Copyright © 2017 Amy Parent. All rights reserved.
//
#pragma once
#include <iostream>
#include "common.hpp"
#include "codegen/CodeGen.hpp"
#include "PALScanner.hpp"
#include "error/Error.hpp"
#include "PALSemantics.hpp"

class PALParser {
    
    /// Scanner used to get tokens from the source file.
    PALScanner&         scanner_;
    
    /// Whether the parser is in recovery mode or not.
    bool                recovering_;
    
    /// The errors generated during compilation.
    Vector<RC<Error>> errors_;
    
    /// Semantics checker.
    PALSemantics        semantics_;
    
    /// Code Generator
    CodeGen             codegen_;
    
    /// Returns true if the current token being parsed is of [type].
    bool have(const String& type) const;
    
    /// Checks if the current token is of [type] and consumes it, and otherwise
    /// sends the parser into recovery mode. If the parser is recovering,
    /// mustBe() will consume tokens until it reaches one of [type].
    void mustBe(const String& type);
    
    /// Generates a syntax error at the scanner's current token, and puts the
    /// parser in recovery mode
    void syntaxError(const String& expected);
    
    // <Program> ::= ...
    void recProgram();
    
    // <VarDecls> ::= ...
    void recVarDecls();
    
    // <IdentList> ::= Identifier ( , Identifier)* ;
    Vector<RC<Token>> recIdentList();
    
    void recStatementBlock();
    
    // <Statement> ::= ...
    void recStatement();
    
    // <Assignment> ::= Identifier = <Expression> ;
    void recAssignment();
    
    // <Loop> ::= UNTIL <BooleanExpr> REPEAT (<Statement>)* ENDLOOP ;
    void recLoop();
    
    // <Conditional> ::= IF <BooleanExpr> THEN (<Statement>)*
    //                       ( ELSE (<Statement>)* )?
    //                       ENDIF ;
    void recConditional();
    
    // <I-o> ::= INPUT <IdentList> | 
    //           OUTPUT <Expression> ( , <Expression>)* ;
    void recIO();
    
    // <Expression> ::= <Term> ( (+|-) <Term>)* ;
    PALType recExpression();
    
    // <Term> ::= <Factor> ( (*|/) <Factor>)* ;
    PALType recTerm();
    
    // <Factor> ::= (+|-)? ( <Value> | "(" <Expression> ")" ) ;
    PALType recFactor();
    
    // <Value> ::= Identifier | IntegerValue | RealValue ;
    PALType recValue();
    
    // <BooleanExpr> ::= <Expression> ("<" | "=" | ">") <Expression> ;
    void recBooleanExpr();
    
public:
    
    /// Creates a parser that uses [scanner] to fetch tokens.
    PALParser(PALScanner& scanner);
    
    /// Parser destructor.
    ~PALParser();
    
    /// Start compilation. Returns true if compilation was successful, or false
    /// if errors where generated.
    bool invoke();
    
    // MARK: - straight getters
    
    CodeGen generator() const { return codegen_; }
    bool isRecovering() const { return recovering_; }
    const Vector<RC<Error>>& errors() const { return errors_; }
};
