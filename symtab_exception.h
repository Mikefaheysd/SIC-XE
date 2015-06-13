/*  symtab_exception.h
    Exception class for symtab
    CS530 Spring 2014
   MONTANA
*/    

#ifndef SYMTAB_EXCEPTION_H
#define SYMTAB_EXCEPTION_H
#include <string>

using namespace std;

class symtab_exception {

public:
    symtab_exception(string s) {
        message = s;        
        }
        
    symtab_exception() {
        message = "A symtab error has occurred";
        }
        
    string getMessage() {
        return message;
    }
    
private:
    string message;
};    
#endif
