#include "Error.hpp"


reportErr::reportErr(rec<lexToke> token, const std::string& message): lexTokenV(token), msgV(message) {}

reportErr::~reportErr() {}

int compare(const reportErr& rsg, const reportErr& lsg) { //my own code
    
    uint64_t lineLsg, lineRhs, columnLsg, columnRhs; //compare the types of errors expected 

    columnLsg = rsg.getlexToken()->getColNum();
    columnRhs = lsg.getlexToken()->getColNum();

    lineLsg = rsg.getlexToken()->getLineNum();
    lineRhs = lsg.getlexToken()->getLineNum();
    
    if(lineLsg == lineRhs) {
        uint64_t val = columnLsg - columnRhs; //return said comparison
        return val;
    }
    else {
        uint64_t val = lineLsg - lineRhs;
        return val;
    }
}

void reportErr::print(std::ostream& cmdOut) const {    //my own code
    std::int64_t lineNumber = getlexToken()->getLineNum();
    std::int64_t columnNumber = getlexToken()->getColNum();
    std::string message = std::to_string(lineNumber) + ":" + std::to_string(columnNumber) + ": there is an issue " + getMsg(); //log that there's an issue found

    bool complete = getlexToken()->complete(lexToke::eof); //find the eof tag

    if(complete == false) {
        message += "\n";
        cmdOut << message << *getlexToken(); //print the error message
    }
}
