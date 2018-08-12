/*  Michael Fahey, Miguel Liberato, Evan Tam, Kyle Rodrigues
    masc1202
    prog1
    CS530, Spring 2014
*/

#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include <string>
#include <vector>

class file_parser {

    public:
        /* ctor, filename is the parameter.  A driver program will read
         * the filename from the command line, and pass the filename to
         * the file_parser constructor.  Filenames must not be hard-coded.
         */
        file_parser(std::string filename);
    
        /* dtor */
        ~file_parser();
    
        /* reads the source file, storing the information is some
         * auxiliary data structure you define in the private section.
         * Throws a file_parse_exception if an error occurs.
         * if the source code file fails to conform to the above
         * specification, this is an error condition.
         */
        void read_file();
    
		/* returns the token found at (row, column).  Rows and columns
		 * are zero based.  Returns the empty string "" if there is no
		 * token at that location. column refers to the four fields
		 * identified above.
		 */
		std::string get_token(unsigned int, unsigned int);
    
        /* prints the source code file to stdout.  Should not repeat
         * the exact formatting of the original, but uses tabs to align
         * similar tokens in a column. The fields should match the
         * order of token fields given above (label/opcode/operands/comments)
         */
        void print_file();

        /* returns the number of lines in the source code file */
        int size();

    private:
        struct record {
            std::string label;
            std::string opcode;
            std::string operand;
            std::string comment;
        };

        std::vector<record> lineContent;
        std::string filename;

    std::vector <std::string> v;
    std::vector <std::string> v2;

    std::string int_to_string (int);
};

#endif

