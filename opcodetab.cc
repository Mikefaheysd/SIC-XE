/* MONTANTA
    Michael Fahey, Miguel Liberato, Kyle Rodrigues, Evan Tam
    masc1202
    prog2
    CS530, Spring 2014
*/

#include <algorithm>

#include "opcodetab.h"
#include "opcode_error_exception.h"

using namespace std;

opcodetab::opcodetab()
{
    /* Add all opcode and machine code translations to map */
	for (unsigned int i = 0; i < NUM_OPCODES; i++)
	{
		opcodeToMachineLUT.insert(pair<string, string>(opcodes[i], machineCode[i]));
		opcodeToSizeLUT.insert(pair<string, int>(opcodes[i], opcodeLenth[i]));
	}
}

string opcodetab::get_machine_code(string opcode)
{
    // check to see if format four
	if (opcode.c_str()[0] == '+')
	{
        //remove "+" to get opcode
        opcode = opcode.substr(1);
    }

	// convert to uppercase for comparision
	transform(opcode.begin(), opcode.end(), opcode.begin(), ::toupper);

    // if opcode not found, error
	if(opcodeToMachineLUT.find(opcode) == opcodeToMachineLUT.end())
	    throw opcode_error_exception("Error opcode not found, opcode exception on token: " + opcode);

	return opcodeToMachineLUT.find(opcode)->second;
}

int opcodetab::get_instruction_size(string opcode)
{
    // convert to uppercase for opcode comparision
	transform(opcode.begin(), opcode.end(), opcode.begin(), ::toupper);

	// check to see if format four
	if(opcode.c_str()[0] == '+')
	{
		opcode = opcode.substr(1);
		// only opcodes with size of 3 can be extended to format 4
		if((opcodeToSizeLUT.find(opcode)->second) != 3)
			throw opcode_error_exception("Error only opcodes with size of 3 can be extended to format 4");

        // if format four, size is 4
		return 4;
	}

    // if opcode does not exist
    if(opcodeToSizeLUT.find(opcode) == opcodeToSizeLUT.end())
        throw opcode_error_exception("Error opcode not found, opcode exception on token: " + opcode);

	// opcode exists and is not format 4, return size
    return opcodeToSizeLUT.find(opcode)->second;
}
