/*  file_parse_exception.h
    Exception class for file_parser
    CS530 Spring 2014
    Alan Riggins
*/    

#ifndef FILE_PARSE_EXCEPTION_H
#define FILE_PARSE_EXCEPTION_H

#include <string>

class file_parse_exception {

    public:
        file_parse_exception(std::string s)
        {
            message = s;
        }

        file_parse_exception()
        {
            message = "An error has occurred";
        }

        std::string getMessage()
        {
            return message;
        }

    private:
        std::string message;
    
};
#endif
