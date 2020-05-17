#include "Error.hpp"

reportErr::reportErr(rec<lexToke> token, const std::string& message): lexTokenV(token), msgV(message) {}

reportErr::~reportErr() {}

int compare(const reportErr& rsg, const reportErr& lsg) {
    
    uint64_t lineLsg, lineRhs, columnLsg, columnRhs;

    columnLsg = rsg.getlexToken()->getColNum();
    columnRhs = lsg.getlexToken()->getColNum();

    lineLsg = rsg.getlexToken()->getLineNum();
    lineRhs = lsg.getlexToken()->getLineNum();
    
    if(lineLsg == lineRhs) {
        uint64_t val = columnLsg - columnRhs;
        return val;
    }
    else {
        uint64_t val = lineLsg - lineRhs;
        return val;
    }
}

void reportErr::print(std::ostream& cmdOut) const {    
    std::int64_t lineNumber = getlexToken()->getLineNum();
    std::int64_t columnNumber = getlexToken()->getColNum();
    std::string message = std::to_string(lineNumber) + ":" + std::to_string(columnNumber) + ": there is an issue" + getMsg();

    bool complete = getlexToken()->complete(lexToke::eof);

    if(complete == false) {
        message += "\n";
        cmdOut << message << *getlexToken();
    }
}
