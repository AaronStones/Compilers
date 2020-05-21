//Name - Aaron Stones
//Module Code - CMP409
//Date - 12/04/2020
#include <sstream>
#include <algorithm>
#include "error/SyntaxError.hpp"
#include "parseCheck.hpp"


ParseAnalysis::ParseAnalysis(scanFile& scanner)
: getTokens(scanner)
, rec_(false)
, checkChar(err) {
    
}

ParseAnalysis::~ParseAnalysis() {}

bool ParseAnalysis::beginAnalysis() { //my own code
    start();
    sort(err.begin(), err.end(), [](const rec<reportErr>& a, const rec<reportErr>& b) { //sort all of the errors
        return *a < *b;
    });
    return false;
}

void ParseAnalysis::start() { //my own code
    getTokens.getNToken();
    beginProgram();
}

bool ParseAnalysis::check(const std::string& type) const { //my own code
    if(getTokens.getToken()){ //if we can get a new token return true
        return getTokens.getToken()->complete(type);
    }
    else{
        return false;
    }
}

void ParseAnalysis::assure(const std::string& type) { //partly my own code
    if(rec_ != false) { //while we are not in recovery mode
        while(getTokens.getToken()->complete(type) != true && getTokens.getToken()->complete(lexToke::eof) != true) {
            getTokens.getNToken();
        }
        if(getTokens.getToken()->complete(lexToke::eof) != true) { //turn recovery mode off and get the next token
            getTokens.getNToken();
            rec_ = false;
        }
        else{ //if we are at the end of the user file
            return;
        }
    }
    else { //we are in recovery mode
        if(getTokens.getToken()->complete(type) != true) {
            synError(type); //syntax error 
        }
        else {
            getTokens.getNToken(); //get a new token
        }
    }
}

void ParseAnalysis::synError(const std::string& expected) { //my own code
    if(rec_ != true) { //syntax error needing logged as the compiler is in recovery mode
        err.push_back(std::make_shared<SyntaxError>(getTokens.getToken(), expected));; 
    }
    else { //compiler not in recovery no need to log error 
        return;
    }
   
}


void ParseAnalysis::beginProgram() { //my own code
    gen.startProgram(); //start the code generation procedure
    for (std::uint64_t i = 0; i<5; i++){
        switch (i){
            case 0:
                assure("PROGRAM");
                assure(lexToke::ident);
                assure("WITH"); //complete assures
            break;

            case 1:
                variableDecl(); //declare variables
            break;

            case 2:
                assure("IN");
            break;

            case 3: 
                parseStatement();
                parseStatement2(); //parse statements
            break;

            case 4: 
                assure("END"); //assure there is an end at the end of the program
                assure(lexToke::eof);
            break;
        }
    }
    gen.endProgram(); //end the program generation
}

void ParseAnalysis::variableDecl() {
    while(check(lexToke::ident)) { //identifier detected log it and if it is not REAL or INT log an error
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
            if(check(lexToke::ident)) {
                getTokens.getNToken();
            }
        }
        
        for(auto id: idents) {
            checkChar.varDecl(id, type);
            gen.homeFunc(id->getDef());
            gen.Number(CODE_load_const, 0.0);
            gen.Variable(CODE_store_local, id->getDef());
        }
    }
}

vec<rec<lexToke>> ParseAnalysis::parseList() { //partly own code, upgraded to use labels as these are faster
    vec<rec<lexToke>> idents;
    idents.push_back(getTokens.getToken()); //add the token to the identifiers verctor
    assure(lexToke::ident);
    label:  
    if (check(",") == true){ //if the identifier contains further variables to initilaise
        assure(",");
        idents.push_back(getTokens.getToken());
        assure(lexToke::ident); //push them back 
        goto label; //repeat
    }
    return idents; //return the vector
}

void ParseAnalysis::parseStatement2() { //my own code
    label:
    bool ifS = check("IF");
    bool until = check("UNTIL");
    bool input = check("INPUT");
    bool output = check("OUTPUT");
    bool lex = check(lexToke::ident); //if a pertinant statement is made that requires parsing
    
    if (ifS == true || until == true 
        || input == true || output == true
        || lex == true){
        parseStatement(); //parse the statement 
        goto label;
    }
}

void ParseAnalysis::parseStatement() { //simple not much to be changed
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

    } 
    else {
        synError("statement");
    }
}

void ParseAnalysis::parseAssign() { //my own code
    
    auto var = getTokens.getToken();
    assure(lexToke::ident); //ensure the current token is an identifier
    
    auto op = getTokens.getToken();
    assure("=");

    auto rhs = parseExpr();

    if(check("=")) {
        checkChar.assiCheck(op, var, rhs); 
        gen.Variable(CODE_store_local, var->getDef()); //store the variables
    }
}

void ParseAnalysis::parseLoop() { //my own code
    for (std::uint64_t i = 0; i<6; i++){ //used to check the correctness of the UNTIL loop
        switch (i){
            case 0:
                assure("UNTIL");
            break;

            case 1:
                gen.startLoop();
                gen.labels(gen.loopLabel());
            break;

            case 2:
                parseBool();
            break;

            case 3: 
                gen.Leap(CODE_jump_if, gen.loopLabel());
            break;

            case 4: 
                assure("REPEAT");
                parseStatement2();
                assure("ENDLOOP");
            break;

            case 5:
                gen.Leap(CODE_rjump, gen.loopLabel());
                gen.closeLoop();
            break;
        }
    }
}

void ParseAnalysis::parseIf() { //my own code
    for (std::uint64_t i = 0; i<8; i++){ //if statement deteected must be parsed
        switch (i){
            case 0:
                assure("IF");
            break;

            case 1:
                gen.startIf();
            break;

            case 2:
                parseBool();
            break;

            case 3: 
                gen.Leap(CODE_jump_if, gen.ifLabel());
                gen.Leap(CODE_jump, gen.elseLabel());           
            break;

            case 4: 
                assure("THEN");
            break;

            case 5:
                gen.labels(gen.ifLabel());
            break;

            case 6:
                parseStatement2();
            break;

            case 7:
                gen.Leap(CODE_jump, gen.endifLabel());
                gen.labels(gen.elseLabel());
                if(check("ELSE")) { //check  and see if the if statement has an else clause
                    assure("ELSE");
                    parseStatement2();
                }            
                assure("ENDIF");
                gen.closeIf();
            break;
        }
    }
}

void ParseAnalysis::parseInOut() { //my own code
    bool inCheck = check("INPUT");
    bool outCheck = check("OUTPUT");
    if(inCheck == false && outCheck == false) { //check for input and output has failed log an error
        synError("IO");
    }
    else {
        if (inCheck == true){
            assure("INPUT"); //input detected parse this
            parseList();
        }    
        else {
            assure("OUTPUT");//output detected parse this
            parseExpr();
            gen.emitString(CODE_invoke_sym, "print(Num)");
            while(check(",")) {
                assure(",");
                parseExpr();
                gen.emitString(CODE_invoke_sym, "print(Num)");
            }
        }
    }
}

compType ParseAnalysis::parseExpr() { //utilises orbatism unsure of changing
    OrbitCode operation;
    
    auto type = parseT();
    while(check("+") || check("-")) { //check for a number functionality
        auto op = getTokens.getToken();
        if(check("+")) { //decide which number detector was used
            assure("+");
            operation = CODE_add; //add
        }
        else {
            assure("-");
            operation = CODE_sub; //subtract
        }
        auto rhs = parseT();
        type = checkChar.exprCheck(op, type, rhs);
        gen.emit(operation);
    }
    return type;
}

compType ParseAnalysis::parseT() { //utilises orbatism unsure of changing
    OrbitCode operation;
    
    auto type = parseF();
    while(check("*") || check("/")) {
        auto op = getTokens.getToken();
        if(check("*")) {
            assure("*");
            operation = CODE_mul;
        }
        else {
            assure("/");
            operation = CODE_div;
        }
        auto rhs = parseF();
        type = checkChar.exprCheck(op, type, rhs);
        gen.emit(operation);
    }
    return type;
}

compType ParseAnalysis::parseF() { //utilises orbatism unsure of changing
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
    if(negate) {
        gen.Number(CODE_load_const, -1.0);
        gen.emit(CODE_mul);
    }
}

compType ParseAnalysis::parseVal() { //utilises orbatism unsure of changing
    auto type = compType::inv;
    auto token = getTokens.getToken();
    
    if(check(lexToke::ident)) {
        assure(lexToke::ident);
        type = checkChar.varCheck(token);
        gen.Variable(CODE_load_local, token->getDef());
    }
    else if(check(lexToke::inte)) {
        assure(lexToke::inte);
        type = checkChar.valCheck(token);
        gen.Number(CODE_load_const, token->doubleValue());
    }
    else if(check(lexToke::real)) {
        assure(lexToke::real);
        type = checkChar.valCheck(token);
        gen.Number(CODE_load_const, token->doubleValue());
    }
    else {
        synError("value");
    }
    return type;
}

void ParseAnalysis::parseBool() { //utilises orbatism unsure of changing
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
    gen.emit(operation);
}
