#include <iostream>
#include "../tokenHandler.hpp"
#include "../declVar.hpp"

#pragma once



class reportErr {

protected:
    rec<lexToke>    lexTokenV;
    std::string     msgV;
    
public:


    reportErr(rec<lexToke> token, const std::string& message);
    virtual ~reportErr();
    virtual void print(std::ostream& out) const;    

    const rec<lexToke> getlexToken() const { return lexTokenV; }
    const std::string& getMsg() const { return msgV; }


};

int compare(const reportErr& lhs, const reportErr& rhs);

inline bool operator<(const reportErr& lhs, const reportErr& rhs) {
    return compare(lhs, rhs) == -1;
}
inline bool operator>(const reportErr& lhs, const reportErr& rhs) {
    return compare(lhs, rhs) == 1;
}
inline std::ostream& operator<<(std::ostream& cmdOut, const reportErr& error) {
    error.print(cmdOut);
    return cmdOut;
}

