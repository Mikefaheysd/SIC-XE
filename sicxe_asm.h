/* MONTANA
    Michael Fahey, Miguel Liberato, Evan Tam, Kyle Rodrigues
    masc1202
    prog4
    CS530, Spring 2014
*/

#ifndef SICXE_ASM_H
#define SICXE_ASM_H

#include <string>
#include <vector>
#include "symtab.h"
#include "file_parser.h"

class sicxe_asm{

	public:
		sicxe_asm();

		~sicxe_asm();

		/* runs the main processing of asm to lis */
		void appMain(std::string);

		//gets the machine code generate from
		//operand and opcode
		void get_machine_code(std::string,int);

		//returns the value of the n bit
		std::string set_n(std::string);

		//returns the value of the i bit
		std::string set_i(std::string);

		//returns value of x bit
		std::string set_x(std::string);

		//returns value of b bit
		std::string set_b(std::string,int);

		//returns value of p bit
		std::string set_p(std::string, std::string, int);

		//returns value of e bit
		std::string set_e(std::string);

		//returns the register number
		//provided an existing register
		std::string get_register(std::string);

		//returns the vector element of the machine code
		std::string return_machine_code(int);

		//converts hex to binary
		std::string hex_to_binary(std::string);

		//converts binary to hex
		std::string binary_to_hex(std::string);

		//sets width of string to 15
		std::string format_15(std::string);

	private:
		file_parser *fparser;
		int size;
		int start;
		std::string filename;
		std::vector<std::string> binary;
};
#endif
