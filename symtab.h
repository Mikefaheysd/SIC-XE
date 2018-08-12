/* MONTANA
 Michael Fahey, Miguel Liberato, Evan Tam, Kyle Rodrigues
 masc1202
 prog5
 CS530, Spring 2014
 */

#ifndef SYMTAB_H
#define SYMTAB_H

#include <string>
#include <fstream>
#include <vector>
#include <map>

#include "file_parser.h"
#include "opcodetab.h"

class symtab {

    public:
        symtab(file_parser *fparser);

        ~symtab();

        // gets starting address of the program
        unsigned int get_start();

        //creates the file with proper .lis extension
        std::string create_file(std::string);

        //writes line number, address, label, opcode and operand to file
        void write_to_file(std::string, int, std::string);

        //generates all of the address for the input file
        void create_address();

        //create map with label as key and address as value
        void create_symtab();

        //converts int to hex
        std::string int_to_hex(int, int);

        //converts string to int
        int string_to_int(std::string);

        //converts string to uppercase
        std::string to_uppercase(std::string);

        //converts hex to int
        int hex_to_int(std::string s);

        //returns the current index address
        int current_address(std::string, int, int);

        //gets the symtab value provided a key
        int get_base(std::string, std::string, int);

        //checks if opcode is an assembler directive
        int check_assembler(std::string);

        //writes to obj file
        void write_to_obj(std::string, int, int);

        //creates the header section
        std::string create_header(std::string, int);

        //creates end section
        std::string create_end(int);

        //creates modified record
        std::string create_m(std::string, int, int);

        //creates text record
        std::string create_text(std::string, int, int, int);

    private:

        void checkForDuplicates();

        const unsigned int LABEL_COL = 0;
        const unsigned int OPCODE_COL = 1;
        const unsigned int OPERAND_COL = 2;
        const unsigned int MACHINE_CODE_COL = 3;
        unsigned int start;

        std::ofstream fout;
        std::vector <int> v;
        std::map <std::string, std::string> table;
        file_parser *fparser;
        opcodetab *opTab;
};

#endif
