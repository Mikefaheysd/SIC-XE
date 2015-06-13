/* MONTANTA
    Michael Fahey, Miguel Liberato, Kyle Rodrigues, Evan Tam
    masc1202
    prog2
    CS530, Spring 2014
*/

#include "opcodetab.h"


using namespace std;

opcodetab::opcodetab(){
/*Add all opcode and machine code translations to map*/
	for (int i =0; i <59; i++){
		tab.insert(pair<string,string>( codes[i],values[i])) ;
		size.insert (pair<string,int>(codes[i],length[i]));
	}
	
}

string opcodetab::get_machine_code(string command){
	string plus = command.substr(0,1);
	if (plus == "+")	//remove "+" to get opcode
		command = command.substr(1);	
	transform(command.begin(),command.end(),command.begin(),::toupper);
	if(tab.find(command) == tab.end())
	throw opcode_error_exception(string_to_sstring(command)); //if opcode not found, error
	return tab.find(command)->second;
}

int opcodetab::get_instruction_size(string opcode){
	transform(opcode.begin(),opcode.end(),opcode.begin(),::toupper);
	string plus = opcode.substr(0,1);
	if(plus == "+")	{		//if format four, size is 4
		opcode = opcode.substr(1);
		if((size.find(opcode)->second)!= 3)
			throw opcode_error_exception(string_to_sstring(opcode));
							
		
		return 4;
	}
	if(size.find(opcode) == size.end())
		throw opcode_error_exception(string_to_sstring(opcode)); //if opcode does not
		 						
						//exist throw error
	return size.find(opcode)->second;
}

const  string opcodetab::codes[] = {"ADD",
"ADDF","ADDR","AND","CLEAR","COMP","COMPF","COMPR","DIV","DIVF","DIVR","FIX","FLOAT","HIO","J","JEQ","JGT","JLT","JSUB","LDA","LDB",
"LDCH","LDF","LDL","LDS","LDT","LDX","LPS","MUL","MULF","MULR","NORM","OR","RD","RMO","RSUB","SHIFTL","SHIFTR","SIO","SSK","STA","STB",
"STCH","STF","STI","STL","STS","STSW","STT","STX","SUB","SUBF","SUBR","SVC","TD","TIO","TIX","TIXR","WD"};

const string opcodetab ::values[] = {"18", "58", "90", "40", "B4", "28", "88", "A0", "24", "64", "9C", "C4", "C0", "F4", "3C", "30",
"34", "38", "48", "00", "68", "50", "70", "08", "6C", "74", "04", "D0", "20", "60", "98",
"C8", "44", "D8", "AC", "4C", "A4", "A8", "F0", "EC", "0C", "78", "54", "80", "D4", "14", "7C", "E8", "84", "10", "1C", "5C", "94", "B0",
"E0", "F8", "2C", "B8", "DC"};

const int opcodetab::length[]={3,3,2,3,2,3,3,2,3,3,2,1,1,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,3,3,2,3,2,2,1,3,3,3,3,3,3,3,3,3,3,3,3,3,2,2,3,1,3,2,3};

//converts string to sstream
string opcodetab::string_to_sstring (string st){
	ostringstream s;
	s<<"opcode exception on token "<<st;
	return s.str();
}
