/* MONTANA
    Michael Fahey, Miguel Liberato, Evan Tam, Kyle Rodrigues
    masc1202
    prog4
    CS530, Spring 2014
*/

#ifndef SICXE_ASM_H
#define SICXE_ASM_H


#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include "symtab.h"
#include "opcodetab.h"
#include "file_parser.h"
#include "symtab_exception.h"
#include "file_parse_exception.h"
#include "opcode_error_exception.h"

class sicxe_asm{
	public:
	/*checks for errors*/
	void error_check(string);
	
	//gets the machine code generate from
	//operand and opcode
	void get_machine_code(string,int);
	
	//returns the value of the n bit
	string set_n(string);
	
	//returns the value of the i bit
	string set_i(string);
	
	//returns value of x bit
	string set_x(string);
	
	//returns value of b bit
	string set_b(string,int);
	
	//returns value of p bit
	string set_p(string, string, int);
	
	//returns value of e bit
	string set_e(string);
	
	//returns the register number
	//provided an existing register
	string get_register(string);
	
	//returns the vector element of the machine code
	string return_machine_code(int);
	
	//converts hex to binary
	string hex_to_binary(string);
	
	//converts binary to hex
	string binary_to_hex(string);
	
    //sets width of string to 15
	string format_15(string);
	
	private:
	int size;
	int start;
	string filename;
	vector<string> binary;
};
#endif
