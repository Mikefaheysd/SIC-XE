/* MONTANA
 Michael Fahey, Miguel Liberato, Evan Tam, Kyle Rodrigues
 masc1202
 prog5
 CS530, Spring 2014
 */

#ifndef SYMTAB_H
#define SYMTAB_H
#include "sicxe_asm.h"
#include "file_parser.h"
#include "opcodetab.h"
#include "symtab_exception.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <map>

using namespace std;

class symtab {
public:
    symtab(){
	}
	
	~symtab(){
	}
	
    // gets starting address of the program
    int get_start(string);
    
	//creates the file with proper .lis extention
    string create_file(string);
    
	//writes line number, address, label, opcode and operand to file
	void write_to_file(string,int,string);
	
	//generates all of the address for the input file
	//stores in a vector, takes in starting address
	void create_address(string,int);
    
    //create map with label as key and address as value
    void create_symtab(string,int);
	
	//converts int to hex
	string int_to_hex(int, int);
	
	//converts string to int
	int string_to_int(string);
	
	//converts string to uppercase
	string to_uppercase(string);
	
	//converts hex to int
	int hex_to_int(string s);
	
	//converts string to streamstring
	string string_to_sstring(string);
	
	//returns the current index address
	int current_address(string,int,int);
	
	//converts int to string
	string int_to_string(int);
	
	//gets the symtab value provided a key
	int get_base(string, string,int);
	
	//checks if opcode is an assembler directive
	int check_assembler(string);
	
	//writes to obj file
	void write_to_obj(string,int,int);
	
	//creates the header section
	string create_header(string,int);
	
	//creates end section
	string create_end(int);
	
	//creates modified record
	string create_m(string, int,int);
	
	//creates text record
	string create_text(string,int,int,int);
private:
    // your variables and private methods go here
	ofstream fout;
	vector <int> v;
	map <string,string> table;
};

#endif
