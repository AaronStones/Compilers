
#include <sstream>
#include <algorithm>
#include "error/SyntaxError.hpp"
#include "parseCheck.hpp"


ParseAnalysis::ParseAnalysis(scanFile& scanner)
: getTokens(scanner)
, rec_(false)
, checkChar(err) {
    
}

ParseAnalysis::~ParseAnalysis() {
    
}

bool ParseAnalysis::beginAnalysis() {
    start();
    sort(err.begin(), err.end(), [](const rec<reportErr>& a, const rec<reportErr>& b) {
        return *a < *b;
    });
    return err.size() == 0;
}

void ParseAnalysis::start() {
    getTokens.getNToken();
    beginProgram();
}

// MARK: - Recursive Descent utilities

bool ParseAnalysis::check(const std::string& type) const {
    if(!getTokens.getToken()){ 
        return false;
    }
    else{
        return getTokens.getToken()->complete(type);
    }
}

void ParseAnalysis::assure(const std::string& type) {
    if(rec_) {
        // If we're in recovery, we keep skipping until we find the token type
        // we wanted, without printing more errors.
        while(!getTokens.getToken()->complete(type)
           && !getTokens.getToken()->complete(lexToke::eof)) {
            
            getTokens.getNToken();
        }
        if(getTokens.getToken()->complete(lexToke::eof)) {
            return;
        }
        getTokens.getNToken();
        rec_ = false;
    }
    else {
        if(getTokens.getToken()->complete(type)) {
            getTokens.getNToken();
        }
        else {
            synError(type);
        }
    }
}

void ParseAnalysis::synError(const std::string& expected) {
    if(rec_) { 
        return; 
    }
    rec_ = true;
    err.push_back(std::make_shared<SyntaxError>(getTokens.getToken(), expected));
}


void ParseAnalysis::beginProgram() {

    gen.beginAnalysis();

    assure("PROGRAM");
    assure(lexToke::ident);
    assure("WITH");
    variableDecl();
    assure("IN");
    parseStatement();
    parseStatement2();
    assure("END");
    assure(lexToke::eof);

    // NOTE: Code Generation code
    gen.finishAnalysis();
}

// <VarDecls> ::= (<IdentList> AS <Type>)* ;
void ParseAnalysis::variableDecl() {
    while(check(lexToke::ident)) {
        auto idents = parseList();
        assure("AS");
        compType type;
        if(check("INTEGER")) {
            assure("INTEGER");
            type = compType::inte;
        }
        else if(check("REAL")) {
            assure("REAL");
            type = compType::real;
        } 
        else {
            synError("type name");
            // if the token is an identifier, we need to drop it. Otherwise,
            // there's the while(have(Identifier)) will catch it, and we'll
            // parse it improperly.
            if(check(lexToke::ident)) {
                getTokens.getNToken();
            }
        }
        
        for(auto id: idents) {
            checkChar.varDecl(id, type);
            
            // NOTE: Code Generation code
            gen.divisional(id->getDef());
            gen.NumberConst(CODE_load_const, 0.0);
            gen.VarConst(CODE_store_local, id->getDef());
        }
    }
}

// <IdentList> ::= Identifier ( , Identifier)* ;
vec<rec<lexToke>> ParseAnalysis::parseList() {
    vec<rec<lexToke>> idents;
    idents.push_back(getTokens.getToken());
    assure(lexToke::ident);
    
    while(check(",")) {
        assure(",");
        idents.push_back(getTokens.getToken());
        assure(lexToke::ident);
    }
    return idents;
}

void ParseAnalysis::parseStatement2() {
    while(check("IF")
       || check("UNTIL")
       || check("INPUT")
       || check("OUTPUT")
       || check(lexToke::ident)) {
        parseStatement();
    }
}

// <Statement> ::= <Assignment> | <Loop> | <Conditional> | <I-o> ;
void ParseAnalysis::parseStatement() {
    if(check("IF")) {
        parseIf();
    }
    else if(check("UNTIL")) {
        parseLoop();
    }
    else if(check("INPUT") || check("OUTPUT")) {
        parseInOut();
    }
    else if(check(lexToke::ident)) {
        parseAssign();
    } else {
        synError("statement");
    }
}

// <Assignment> ::= Identifier = <Expression> ;
void ParseAnalysis::parseAssign() {
    
    auto var = getTokens.getToken();
    assure(lexToke::ident);
    
    auto op = getTokens.getToken();
    bool shouldCheck = check("=");
    assure("=");
    
    auto rhs = parseExpr();
    
    // Only check if we do have the equals token. Otherwise we're checking
    // something that is not a valid assignmnet
    if(shouldCheck) {
        checkChar.assiCheck(op, var, rhs);
        
        // NOTE: Code Generation code
        gen.VarConst(CODE_store_local, var->getDef());
    }
}

// <Loop> ::= UNTIL <BooleanExpr> REPEAT (<Statement>)* ENDLOOP ;
void ParseAnalysis::parseLoop() {
    assure("UNTIL");
    
    // NOTE: Code Generation code/ We start the loop (so that we get unique
    //       label IDs) and emit the start label for the future rjump.
    gen.loopBegin();
    gen.LabConstr(gen.markerloop());
    
    parseBool();
    
    // NOTE: Code Generation code/ conditional loop exit
    gen.LabArg(CODE_jump_if, gen.markerEndLoop());
    
    assure("REPEAT");
    parseStatement2();
    
    assure("ENDLOOP");
    
    // NOTE: Code Generation code/ jump back to the start of the loop
    gen.LabArg(CODE_rjump, gen.markerloop());
    gen.loopFinish();
}

// <Conditional> ::= IF <BooleanExpr> THEN (<Statement>)*
//                       ( ELSE (<Statement>)* )?
//                       ENDIF ;
void ParseAnalysis::parseIf() {
    assure("IF");
    
    // NOTE: Code Generation code/ Start codegen if so we get the proper labels
    gen.ifFinish();
    
    parseBool();
    
    // NOTE: Code Generation code/ Emit conditional jump to if label, and
    //       otherwise we jump to the else block (if there's none, it'll just
    //       go past the label).
    gen.LabArg(CODE_jump_if, gen.markerIf());
    gen.LabArg(CODE_jump, gen.markerElse());
    
    assure("THEN");
    
    // NOTE: Code Generation code/ Emit start of the if block label.
    gen.LabConstr(gen.markerIf());
    
    parseStatement2();
    
    // NOTE: Code Generation code/ Emit start of the else block label.
    //       we also need to jump to the endif label, otherwise we're going to
    //       run the else block too.
    gen.LabArg(CODE_jump, gen.markerEndIf());
    gen.LabConstr(gen.markerElse());
    if(check("ELSE")) {
        assure("ELSE");
        parseStatement2();
    }
    assure("ENDIF");
    
    // NOTE: Code Generation code/ Finish if/else block
    gen.ifBegin();
}

// <I-o> ::= INPUT <IdentList> | 
//           OUTPUT <Expression> ( , <Expression>)* ;
void ParseAnalysis::parseInOut() {
    if(check("INPUT")) {
        assure("INPUT");
        parseList();
        // TODO: call the right input function in orbit
    }
    else if(check("OUTPUT")) {
        // OUTPUT <Expression>
        assure("OUTPUT");
        parseExpr();
        
        // NOTE: Code Generation code/ call to stdlib print()
        gen.StrConst(CODE_invoke_sym, "print(Num)");
        
        // ( , <Expression>)* ;
        while(check(",")) {
            assure(",");
            parseExpr();
            
            // NOTE: Code Generation code/ call to stdlib print()
            gen.StrConst(CODE_invoke_sym, "print(Num)");
        }
    } else {
        synError("IO");
    }
}

// <Expression> ::= <Term> ( (+|-) <Term>)* ;
compType ParseAnalysis::parseExpr() {
    // NOTE: Code Generation code/ Since we have a stack-based vm, we need to
    //       keep the operation and only emit it at the end of the expr.
    OrbitCode operation;
    
    auto type = parseT();
    // ( (+|-) <Term>)* ;
    while(check("+") || check("-")) {
        auto op = getTokens.getToken();

        // (+|-)
        if(check("+")) {
            assure("+");
            operation = CODE_add;
        }
        else {
            assure("-");
            operation = CODE_sub;
        }
        //  <Term> ;
        auto rhs = parseT();
        type = checkChar.exprCheck(op, type, rhs);
        
        // NOTE: Code Generation code/ finally emit the operation.
        gen.withOArg(operation);
    }
    return type;
}

// <Term> ::= <Factor> ( (*|/) <Factor>)* ;
compType ParseAnalysis::parseT() {
    // NOTE: Code Generation code/ Since we have a stack-based vm, we need to
    //       keep the operation and only emit it at the end of the expr.
    OrbitCode operation;
    
    auto type = parseF();
    while(check("*") || check("/")) {
        auto op = getTokens.getToken();
        
        // ( (*|/) <Factor>)* ;
        if(check("*")) {
            assure("*");
            operation = CODE_mul;
        }
        else {
            assure("/");
            operation = CODE_div;
        }
        // <Factor>
        auto rhs = parseF();
        type = checkChar.exprCheck(op, type, rhs);
        
        // NOTE: Code Generation code/ finally emit the operation.
        gen.withOArg(operation);
    }
    return type;
}

// <Factor> ::= (+|-)? ( <Value> | "(" <Expression> ")" ) ;
compType ParseAnalysis::parseF() {
    bool negate = false;
    
    if(check("+")) {
        assure("+");
    }
    else if(check("-")) {
        assure("-");
        negate = true;
    }
    
    if(check("(")) {
        assure("(");
        auto type = parseExpr();
        assure(")");
        return type;
    }
    else {
        return parseVal();
    }
    
    // NOTE: Code Generation code/ Factors emit themselves with the different 
    //       recognisers, but if we have a leading '-' then we need to negate 
    //       the resulting top of the stack.
    if(negate) {
        gen.NumberConst(CODE_load_const, -1.0);
        gen.withOArg(CODE_mul);
    }
}

// <Value> ::= Identifier | IntegerValue | RealValue ;
compType ParseAnalysis::parseVal() {
    auto type = compType::inv;
    auto token = getTokens.getToken();
    
    if(check(lexToke::ident)) {
        assure(lexToke::ident);
        type = checkChar.varCheck(token);
        
        // NOTE: Code Generation code/ load the variable on the stack
        gen.VarConst(CODE_load_local, token->getDef());
    }
    else if(check(lexToke::inte)) {
        assure(lexToke::inte);
        type = checkChar.valCheck(token);
        
        // NOTE: Code Generation code/ load the const number on the stack
        gen.NumberConst(CODE_load_const, token->doubleValue());
    }
    else if(check(lexToke::real)) {
        assure(lexToke::real);
        type = checkChar.valCheck(token);
        
        // NOTE: Code Generation code/ load the const number on the stack
        gen.NumberConst(CODE_load_const, token->doubleValue());
    }
    else {
        synError("value");
    }
    return type;
}

// <BooleanExpr> ::= <Expression> ("<" | "=" | ">") <Expression> ;
void ParseAnalysis::parseBool() {
    // NOTE: Code Generation code/ Since we have a stack-based vm, we need to
    //       keep the operation and only emit it at the end of the expr.
    OrbitCode operation;
    
    auto lhs = parseExpr();
    auto op = getTokens.getToken();
    
    if(check("<")) {
        assure("<");
        operation = CODE_test_lt;
    }
    else if(check(">")) {
        assure(">");
        operation = CODE_test_gt;
    }
    else if(check("=")) {
        assure("=");
        operation = CODE_test_eq;
    }
    else {
        synError("boolean expression");
        return;
    }
    
    auto rhs = parseExpr();
    checkChar.boolCheck(op, lhs, rhs);
    
    // NOTE: Code Generation code/ finally emit the operation.
    gen.withOArg(operation);
}
