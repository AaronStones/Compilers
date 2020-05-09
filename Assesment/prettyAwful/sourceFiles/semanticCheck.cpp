//
//  PALSemantics.cpp - PAL Compiler's token implementation.
//  PAL Compiler
//
//  Created by Amy Parent on 2017-02-25.
//  Copyright © 2017 Amy Parent. All rights reserved.
//
#include <sstream>
#include "semanticCheck.hpp"
#include "error/VariableError.hpp"
#include "error/ExprError.hpp"

String PALSemantics::typeName(PALType type) {
    switch(type) {
        case PALType::Integer:
            return "Integer";
            break;
        case PALType::Real:
            return "Real";
            break;
        case PALType::Boolean:
            return "Boolean";
            break;
        default:
            break;
    }
    return "Invalid";
}

void PALSemantics::declareVariable(RC<Token> var, PALType type) {
    if(variableExists(var)) {
        variableError(var, variableDecl(var), "redefinition of '" + var->value() + "'");
    } else {
        variables_[var->value()] = {type, var};
    }
}

bool PALSemantics::variableExists(RC<Token> var) {
    return variables_.find(var->value()) != variables_.end();
}

PALType PALSemantics::variableType(RC<Token> var) {
    return variableExists(var) ? variables_.at(var->value()).type : PALType::Invalid;
}

RC<Token> PALSemantics::variableDecl(RC<Token> var) {
    if(!variableExists(var)) { return nullptr; }
    return variables_.at(var->value()).where;
}

PALType PALSemantics::checkVariable(RC<Token> var) {
    if(!variableExists(var)) {
        semanticsError(var, "use of undeclared variable '" + var->value() + "'");
        return PALType::Invalid;
    }
    return variables_.at(var->value()).type;
}

PALType PALSemantics::checkValue(RC<Token> value) {
    if(value->type() == Token::Integer) {
        return PALType::Integer;
    }
    if(value->type() == Token::Real) {
        return PALType::Real;
    }
    return PALType::Invalid;
}

PALType PALSemantics::checkExpression(RC<Token> op, PALType lhs, PALType rhs) {
    // If both members are of invlid type, there is no point reporting any more
    // errors, they are likely to be bogus anyway.
    if(lhs == PALType::Invalid && rhs == PALType::Invalid) { return PALType::Invalid; }
    
    // If only one member is invalid, try to coerce. This way, we can get past
    // one error and check if the rest is valid at least.
    if(lhs == PALType::Invalid) { lhs = rhs; }
    if(rhs == PALType::Invalid) { rhs = lhs; }
    
    // Now that we have valid types on both sides, we can check:
    if(lhs != rhs) {
        expressionError(op, "binary", lhs, rhs);
        return PALType::Invalid;
    }
    
    return lhs;
}

void PALSemantics::checkBoolean(RC<Token> op, PALType lhs, PALType rhs) {
    // If both members are of invlid type, there is no point reporting any more
    // errors, they are likely to be bogus anyway.
    if(lhs == PALType::Invalid && rhs == PALType::Invalid) { return; }
    
    // If only one member is invalid, try to coerce. This way, we can get past
    // one error and check if the rest is valid at least.
    if(lhs == PALType::Invalid) { lhs = rhs; }
    if(rhs == PALType::Invalid) { rhs = lhs; }
    
    // Now that we have valid types on both sides, we can check:
    if(lhs != rhs) {
        expressionError(op, "boolean", lhs, rhs);
    }
}

void PALSemantics::checkAssignment(RC<Token> op, RC<Token> var, PALType rhs) {
    // check that the variable exists first.
    if(!variableExists(var)) {
        semanticsError(var, "use of undeclared variable '" + var->value() + "'");
        return;
    }
    
    // now that we're sure, check we have a good assignment.
    auto lhs = variableType(var);
    auto decl = variableDecl(var);
    
    // If only one member is invalid, try to coerce. This way, we can get past
    // one error and check if the rest is valid at least.
    if(lhs == PALType::Invalid) { lhs = rhs; }
    if(rhs == PALType::Invalid) { rhs = lhs; }
    
    if(lhs == rhs) { return; }
    assignmentError(op, decl, lhs, rhs);
}

void PALSemantics::semanticsError(RC<Token> opToken, const String& message) {
    errors_.push_back(std::make_shared<Error>(opToken, message));
}


// Logs a bad variable assignment error.
void PALSemantics::assignmentError(RC<Token> op, RC<Token> decl,
                                   PALType lhs, PALType rhs) {
    std::stringstream buffer;
    buffer << "assigning '" << typeName(lhs);
    buffer <<  "' from incompatible type '" << typeName(rhs) << "'";
    variableError(op, decl, buffer.str());
}

void PALSemantics::expressionError(RC<Token> op, const String& type,
                                   PALType lhs, PALType rhs) {
    errors_.push_back(std::make_shared<ExprError>(op, type,
                                                  typeName(lhs),
                                                  typeName(rhs)));
}

void PALSemantics::variableError(RC<Token> op, RC<Token> decl,
                                 const String& message) {
    errors_.push_back(std::make_shared<VariableError>(op, decl, message));
}
