/*  symtab_exception.h
    Exception class for symtab
    CS530 Spring 2014
   MONTANA
*/    

#ifndef SYMTAB_EXCEPTION_H
#define SYMTAB_EXCEPTION_H

#include <string>

class symtab_exception {

    public:
        symtab_exception(std::string s)
        {
            message = s;
        }

        symtab_exception()
        {
            message = "A symtab error has occurred";
        }

        std::string getMessage()
        {
            return message;
        }

    private:
        std::string message;
};    
#endif
