//
//  PALSemantics.hpp - PAL Compiler's token interface
//  PAL Compiler
//
//  Created by Amy Parent on 2017-02-25.
//  Copyright © 2017 Amy Parent. All rights reserved.
//
#pragma once
#include "declVar.hpp"
#include "tokenHandler.hpp"
#include "error/Error.hpp"

/// Represents the type of a variable, value or expression.
enum class PALType {
    Integer,
    Real,
    Boolean,
    Invalid,
};

/// Semantics analysis class
class PALSemantics {
    
    /// In the variable table, we keep track of the type, but also where the
    /// variables are declared. This allows better error printing
    struct Declaration {
        PALType         type;
        rec<Token>    where;
    };
    
    /// Map of all declared variables and their types.
    mp<str, Declaration> variables_;
    
    /// A reference to the compiler's list of errors/
    vec<rec<Error>>&   errors_;
    
    /// Returns the type name for a PALType enum value.
    str typeName(PALType type);
    
    /// Logs a generic semantics analysis error.
    void semanticsError(rec<Token> op, const str& message);
    
    // Logs a bad variable assignment error.
    void assignmentError(rec<Token> op, rec<Token> decl,
                         PALType lhs, PALType rhs);
    
    /// Logs a type conflict error.
    void expressionError(rec<Token> op, const str& type,
                         PALType lhs, PALType rhs);
    
    void variableError(rec<Token> op, rec<Token> decl,
                       const str& message);

public:
    
    /// Creates a semantics analyser.
    PALSemantics(vec<rec<Error>>& errors)
        : errors_(errors) {}
    
    /// PALSemantics destructor.
    ~PALSemantics() {}
    
    /// Declares a the variable [var], with [type]. If a variable is already
    /// declared with that name, logs an error.
    void declareVariable(rec<Token> var, PALType type);
    
    /// Returns whether [var] has been declared.
    bool variableExists(rec<Token> var);
    
    /// Returns the token where the variable referenced by [var] is declared.
    rec<Token> variableDecl(rec<Token> var);
    
    /// Returns the type of [var], or [PALType::Invalid] if it hasn't been
    /// declared previously.
    PALType variableType(rec<Token> var);
    
    /// Checks that a [var] is declared, and returns its type. This is
    /// essentially the same as [variableType()], except it logs the semantic
    /// error.
    PALType checkVariable(rec<Token> var);
    
    /// Returns the type of a the language value pointed to by [value].
    PALType checkValue(rec<Token> value);
    
    /// Returns the type of an the expression [lhs] [op] [rhs]. If the
    /// expression is invlaid, logs a type mismatch error.
    PALType checkExpression(rec<Token> op, PALType lhs, PALType rhs);
    
    /// Checks that the boolean expression [lhs] [op] [rhs] is valid, or
    /// logs a type mismatch error.
    void checkBoolean(rec<Token> op, PALType lhs, PALType rhs);
    
    /// Checks that [var] = [rhs] is a valid assignment. If the [var] is not
    /// declared or [var] and [rhs] are of different types, an error is logged.
    void checkAssignment(rec<Token> op, rec<Token> var, PALType rhs);

};
