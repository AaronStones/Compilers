//
//  PALParser.cpp - PAL Compiler's parser implementation
//  PAL Compiler
//
//  Created by Amy Parent on 2017-02-17.
//  Copyright © 2017 Amy Parent. All rights reserved.
//
#include <sstream>
#include <algorithm>
#include "error/SyntaxError.hpp"
#include "PALParser.hpp"

PALParser::PALParser(PALScanner& scanner)
: scanner_(scanner)
, recovering_(false)
, semantics_(errors_) {
    
}

PALParser::~PALParser() {
    
}

bool PALParser::invoke() {
    scanner_.nextToken();
    recProgram();
    
    std::sort(errors_.begin(), errors_.end(), [](const RC<Error>& a, const RC<Error>& b) {
        return *a < *b;
    });
    return errors_.size() == 0;
}

// MARK: - Recursive Descent utilities

bool PALParser::have(const String& type) const {
    if(!scanner_.currentToken()) return false;
    return scanner_.currentToken()->is(type);
}

void PALParser::mustBe(const String& type) {
    if(recovering_) {
        // If we're in recovery, we keep skipping until we find the token typ
        // we wanted, without printing more errors.
        while(!scanner_.currentToken()->is(type)
           && !scanner_.currentToken()->is(Token::EndOfFile)) {
            
            scanner_.nextToken();
        }
        if(scanner_.currentToken()->is(Token::EndOfFile)) {
            return;
        }
        scanner_.nextToken();
        recovering_ = false;
    }
    else {
        if(scanner_.currentToken()->is(type)) {
            scanner_.nextToken();
        }
        else {
            syntaxError(type);
        }
    }
}

void PALParser::syntaxError(const String& expected) {
    if(recovering_) { return; }
    recovering_ = true;
    errors_.push_back(std::make_shared<SyntaxError>(scanner_.currentToken(),
                                                       expected));
}

// MARK: - Grammar implementation

// <Program> ::= PROGRAM Identifier
//               WITH <VarDecls>
//               IN (<Statement>)+
//               END ;
void PALParser::recProgram() {
    // NOTE: Code Generation code
    codegen_.startProgram();
    
    mustBe("PROGRAM");
    mustBe(Token::Identifier);
    mustBe("WITH");
    recVarDecls();
    mustBe("IN");
    recStatement();
    recStatementBlock();
    mustBe("END");
    mustBe(Token::EndOfFile);
    
    // NOTE: Code Generation code
    codegen_.endProgram();
}

// <VarDecls> ::= (<IdentList> AS <Type>)* ;
void PALParser::recVarDecls() {
    while(have(Token::Identifier)) {
        auto idents = recIdentList();
        mustBe("AS");
        PALType type;
        if(have("INTEGER")) {
            mustBe("INTEGER");
            type = PALType::Integer;
        }
        else if(have("REAL")) {
            mustBe("REAL");
            type = PALType::Real;
        } else {
            syntaxError("type name");
            // if the token is an identifier, we need to drop it. Otherwise,
            // there's the while(have(Identifier)) will catch it, and we'll
            // parse it improperly.
            if(have(Token::Identifier)) {
                scanner_.nextToken();
            }
        }
        
        for(auto id: idents) {
            semantics_.declareVariable(id, type);
            
            // NOTE: Code Generation code
            codegen_.local(id->value());
            codegen_.emitNum(CODE_load_const, 0.0);
            codegen_.emitVar(CODE_store_local, id->value());
        }
    }
}

// <IdentList> ::= Identifier ( , Identifier)* ;
Vector<RC<Token>> PALParser::recIdentList() {
    Vector<RC<Token>> idents;
    idents.push_back(scanner_.currentToken());
    mustBe(Token::Identifier);
    
    while(have(",")) {
        mustBe(",");
        idents.push_back(scanner_.currentToken());
        mustBe(Token::Identifier);
    }
    return idents;
}

void PALParser::recStatementBlock() {
    while(have("IF")
       || have("UNTIL")
       || have("INPUT")
       || have("OUTPUT")
       || have(Token::Identifier)) {
        recStatement();
    }
}

// <Statement> ::= <Assignment> | <Loop> | <Conditional> | <I-o> ;
void PALParser::recStatement() {
    if(have("IF")) {
        recConditional();
    }
    else if(have("UNTIL")) {
        recLoop();
    }
    else if(have("INPUT") || have("OUTPUT")) {
        recIO();
    }
    else if(have(Token::Identifier)) {
        recAssignment();
    } else {
        syntaxError("statement");
    }
}

// <Assignment> ::= Identifier = <Expression> ;
void PALParser::recAssignment() {
    
    auto var = scanner_.currentToken();
    mustBe(Token::Identifier);
    
    auto op = scanner_.currentToken();
    bool shouldCheck = have("=");
    mustBe("=");
    
    auto rhs = recExpression();
    
    // Only check if we do have the equals token. Otherwise we're checking
    // something that is not a valid assignmnet
    if(shouldCheck) {
        semantics_.checkAssignment(op, var, rhs);
        
        // NOTE: Code Generation code
        codegen_.emitVar(CODE_store_local, var->value());
    }
}

// <Loop> ::= UNTIL <BooleanExpr> REPEAT (<Statement>)* ENDLOOP ;
void PALParser::recLoop() {
    mustBe("UNTIL");
    
    // NOTE: Code Generation code/ We start the loop (so that we get unique
    //       label IDs) and emit the start label for the future rjump.
    codegen_.startLoop();
    codegen_.emitLabel(codegen_.loopLabel());
    
    recBooleanExpr();
    
    // NOTE: Code Generation code/ conditional loop exit
    codegen_.emitJump(CODE_jump_if, codegen_.endLoopLabel());
    
    mustBe("REPEAT");
    recStatementBlock();
    
    mustBe("ENDLOOP");
    
    // NOTE: Code Generation code/ jump back to the start of the loop
    codegen_.emitJump(CODE_rjump, codegen_.loopLabel());
    codegen_.closeLoop();
}

// <Conditional> ::= IF <BooleanExpr> THEN (<Statement>)*
//                       ( ELSE (<Statement>)* )?
//                       ENDIF ;
void PALParser::recConditional() {
    mustBe("IF");
    
    // NOTE: Code Generation code/ Start codegen if so we get the proper labels
    codegen_.startIf();
    
    recBooleanExpr();
    
    // NOTE: Code Generation code/ Emit conditional jump to if label, and
    //       otherwise we jump to the else block (if there's none, it'll just
    //       go past the label).
    codegen_.emitJump(CODE_jump_if, codegen_.ifLabel());
    codegen_.emitJump(CODE_jump, codegen_.elseLabel());
    
    mustBe("THEN");
    
    // NOTE: Code Generation code/ Emit start of the if block label.
    codegen_.emitLabel(codegen_.ifLabel());
    
    recStatementBlock();
    
    // NOTE: Code Generation code/ Emit start of the else block label.
    //       we also need to jump to the endif label, otherwise we're going to
    //       run the else block too.
    codegen_.emitJump(CODE_jump, codegen_.endifLabel());
    codegen_.emitLabel(codegen_.elseLabel());
    if(have("ELSE")) {
        mustBe("ELSE");
        recStatementBlock();
    }
    mustBe("ENDIF");
    
    // NOTE: Code Generation code/ Finish if/else block
    codegen_.closeIf();
}

// <I-o> ::= INPUT <IdentList> | 
//           OUTPUT <Expression> ( , <Expression>)* ;
void PALParser::recIO() {
    if(have("INPUT")) {
        mustBe("INPUT");
        recIdentList();
        // TODO: call the right input function in orbit
    }
    else if(have("OUTPUT")) {
        // OUTPUT <Expression>
        mustBe("OUTPUT");
        recExpression();
        
        // NOTE: Code Generation code/ call to stdlib print()
        codegen_.emitString(CODE_invoke_sym, "print(Num)");
        
        // ( , <Expression>)* ;
        while(have(",")) {
            mustBe(",");
            recExpression();
            
            // NOTE: Code Generation code/ call to stdlib print()
            codegen_.emitString(CODE_invoke_sym, "print(Num)");
        }
    } else {
        syntaxError("IO");
    }
}

// <Expression> ::= <Term> ( (+|-) <Term>)* ;
PALType PALParser::recExpression() {
    // NOTE: Code Generation code/ Since we have a stack-based vm, we need to
    //       keep the operation and only emit it at the end of the expr.
    OrbitCode operation;
    
    auto type = recTerm();
    // ( (+|-) <Term>)* ;
    while(have("+") || have("-")) {
        auto op = scanner_.currentToken();

        // (+|-)
        if(have("+")) {
            mustBe("+");
            operation = CODE_add;
        }
        else {
            mustBe("-");
            operation = CODE_sub;
        }
        //  <Term> ;
        auto rhs = recTerm();
        type = semantics_.checkExpression(op, type, rhs);
        
        // NOTE: Code Generation code/ finally emit the operation.
        codegen_.emit(operation);
    }
    return type;
}

// <Term> ::= <Factor> ( (*|/) <Factor>)* ;
PALType PALParser::recTerm() {
    // NOTE: Code Generation code/ Since we have a stack-based vm, we need to
    //       keep the operation and only emit it at the end of the expr.
    OrbitCode operation;
    
    auto type = recFactor();
    while(have("*") || have("/")) {
        auto op = scanner_.currentToken();
        
        // ( (*|/) <Factor>)* ;
        if(have("*")) {
            mustBe("*");
            operation = CODE_mul;
        }
        else {
            mustBe("/");
            operation = CODE_div;
        }
        // <Factor>
        auto rhs = recFactor();
        type = semantics_.checkExpression(op, type, rhs);
        
        // NOTE: Code Generation code/ finally emit the operation.
        codegen_.emit(operation);
    }
    return type;
}

// <Factor> ::= (+|-)? ( <Value> | "(" <Expression> ")" ) ;
PALType PALParser::recFactor() {
    bool negate = false;
    
    if(have("+")) {
        mustBe("+");
    }
    else if(have("-")) {
        mustBe("-");
        negate = true;
    }
    
    if(have("(")) {
        mustBe("(");
        auto type = recExpression();
        mustBe(")");
        return type;
    }
    else {
        return recValue();
    }
    
    // NOTE: Code Generation code/ Factors emit themselves with the different 
    //       recognisers, but if we have a leading '-' then we need to negate 
    //       the resulting top of the stack.
    if(negate) {
        codegen_.emitNum(CODE_load_const, -1.0);
        codegen_.emit(CODE_mul);
    }
}

// <Value> ::= Identifier | IntegerValue | RealValue ;
PALType PALParser::recValue() {
    auto type = PALType::Invalid;
    auto token = scanner_.currentToken();
    
    if(have(Token::Identifier)) {
        mustBe(Token::Identifier);
        type = semantics_.checkVariable(token);
        
        // NOTE: Code Generation code/ load the variable on the stack
        codegen_.emitVar(CODE_load_local, token->value());
    }
    else if(have(Token::Integer)) {
        mustBe(Token::Integer);
        type = semantics_.checkValue(token);
        
        // NOTE: Code Generation code/ load the const number on the stack
        codegen_.emitNum(CODE_load_const, token->doubleValue());
    }
    else if(have(Token::Real)) {
        mustBe(Token::Real);
        type = semantics_.checkValue(token);
        
        // NOTE: Code Generation code/ load the const number on the stack
        codegen_.emitNum(CODE_load_const, token->doubleValue());
    }
    else {
        syntaxError("value");
    }
    return type;
}

// <BooleanExpr> ::= <Expression> ("<" | "=" | ">") <Expression> ;
void PALParser::recBooleanExpr() {
    // NOTE: Code Generation code/ Since we have a stack-based vm, we need to
    //       keep the operation and only emit it at the end of the expr.
    OrbitCode operation;
    
    auto lhs = recExpression();
    auto op = scanner_.currentToken();
    
    if(have("<")) {
        mustBe("<");
        operation = CODE_test_lt;
    }
    else if(have(">")) {
        mustBe(">");
        operation = CODE_test_gt;
    }
    else if(have("=")) {
        mustBe("=");
        operation = CODE_test_eq;
    }
    else {
        syntaxError("boolean expression");
        return;
    }
    
    auto rhs = recExpression();
    semantics_.checkBoolean(op, lhs, rhs);
    
    // NOTE: Code Generation code/ finally emit the operation.
    codegen_.emit(operation);
}
