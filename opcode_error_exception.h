/*  MONTANTA
    Michael Fahey, Miguel Liberato, Kyle Rodrigues, Evan Tam
    masc1202
    prog2
    opcode_error_exception.h
    Exception class for opcodetab
    CS530 Spring 2014
    
*/    

#ifndef OPCODE_ERROR_EXCEPTION_H
#define OPCODE_ERROR_EXCEPTION_H

#include <string>

class opcode_error_exception {

    public:
        opcode_error_exception(std::string s) {
            message = s;
        }

        opcode_error_exception() {
            message = "An opcode error has occurred, opcode not found in table.";
        }

        std::string getMessage() {
            return message;
        }

    private:
        std::string message;
};   
#endif
