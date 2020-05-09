//
//  PALScanner.cpp - PAL Compiler's scanner implementation
//  PAL Compiler
//
//  Created by Amy Parent on 2017-02-17.
//  Copyright © 2017 Amy Parent. All rights reserved.
//
#include <sstream>
#include <cctype>
#include <algorithm>
#include "scanFile.hpp"

const Vector<String> PALScanner::keywords = {
	"PROGRAM", "WITH", "IN", "END", "AS", "INTEGER", "REAL", "UNTIL","REPEAT",
    "ENDLOOP", "IF", "THEN", "ELSE", "ENDIF", "INPUT", "OUTPUT"
};

PALScanner::PALScanner(std::istream& input)
: line_(1)
, idx_(0)
, input_(input)
, currentToken_(nullptr)
, currentChar_(' ') {
    
}

PALScanner::~PALScanner() {
    
}

void PALScanner::nextChar() {
    if(currentChar_ == 25) { return; }
    
    idx_ += 1;
    // If we're not at the end of the line yet:
    if(idx_ < currentLine_.size()) {
        currentChar_ = currentLine_[idx_];
        return;
    }
    
    // If we're at the end, avoid getting a new line
    if(input_.eof()) {
        currentChar_ = 25;
        return;
    }
    
    // We need to get a new line from the source
    std::getline(input_, currentLine_);
    line_ += 1;
    idx_ = 0;
    currentLine_ += '\n';
    currentChar_ = currentLine_[0];
}

const RC<Token> PALScanner::nextToken() {
    RC<Token> token;
    
    std::stringstream buffer;
    UInt64 startLine = 0, startCol = 0;
    State lexState = State::Whitespace;
    
    while(!token) {
        switch(lexState) {
        case State::Whitespace:
            if(std::isspace(currentChar())) {
                lexState = State::Whitespace;
            } 
            else {
                startLine = line();
                startCol = column();
                
                if(std::isalpha(currentChar())) {
                    lexState = State::Identifier;
                }
                else if(std::isdigit(currentChar())) {
                    lexState = State::Integer;
                }
                else if(String("+-*/(),=<>").find(currentChar()) != String::npos) {
                    lexState = State::Punctuation;
                }
                else if(currentChar() == 25) {
                    lexState = State::EndOfFile;
                }
                else {
                    lexState = State::InvalidChar;
                }
            }
            break;
        
        case State::Identifier:
            if(std::isalnum(currentChar())) {
                lexState = State::Identifier;
            }
            else {
                String tok = buffer.str();
                if(std::find(keywords.begin(), keywords.end(), tok) != keywords.end()) {
                    token = std::make_shared<Token>(tok, startLine, startCol);
                }
                else {
                    token = std::make_shared<Token>(Token::Identifier, tok,
                                                       startLine,startCol);
                }
            }
            break;
            
        case State::Integer:
            if(std::isdigit(currentChar())) {
                lexState = State::Integer;
            }
            else if(currentChar() == '.') {
                lexState = State::Real;
            }
            else {
                token = std::make_shared<Token>(Token::Integer, buffer.str(),
                                                   startLine, startCol);
            }
            break;
        case State::Real:
            if(std::isdigit(currentChar())) {
                lexState = State::Real;
            }
            else {
                token = std::make_shared<Token>(Token::Real, buffer.str(),
                                                   startLine, startCol);
            }
            break;
            
        case State::Punctuation:
            token = std::make_shared<Token>(buffer.str(),
                                               startLine, startCol);
            break;
        case State::EndOfFile:
            token = std::make_shared<Token>(Token::EndOfFile,
                                               startLine, startCol);
            break;
        case State::InvalidChar:
            token = std::make_shared<Token>(Token::InvalidToken, buffer.str(),
                                               startLine, startCol);
            break;
        }
        if(!token && lexState != State::Whitespace) {
                buffer << currentChar();
            }
            
        if (!token){
            nextChar();
        }
    }
    token->sourceLine(currentLine_);
    return (currentToken_ = token);
}
