/* MONTANA
    Michael Fahey, Miguel Liberato, Evan Tam, Kyle Rodrigues
    masc1202
    prog4
    CS530, Spring 2014
*/
#include "sicxe_asm.h"

using namespace std;

int main(int argc, char *argv[]) {
    if(argc != 2) {
        cout << "Error, you must supply the name of the file " <<
        "to process at the command line." << endl;
        exit(1);
    }
   string filename = argv[1];
   sicxe_asm obj;
   obj.error_check(filename);
}
   
//checks for errors and if no errors writes to file.   
void sicxe_asm::error_check(string filename){
   try{
   	file_parser parser(filename);
   	parser.read_file();
  	int size = parser.size();
  	sicxe_asm sic;
   	symtab obj;
   	int start = obj.get_start(filename);
   	string newfile =obj.create_file(filename); //now has the filename.lis
   	obj.create_address(filename,start);
   	obj.write_to_file(newfile,size,filename);
   	obj.create_symtab(filename,start);
    }
    catch (symtab_exception excpt) {
        cout << "**Sorry, error " << excpt.getMessage() << endl;
    }
    catch (opcode_error_exception excpt) {
        cout << "**Sorry, error " << excpt.getMessage() << endl;
    }
    catch (file_parse_exception excpt) {
        cout << "**Sorry, error " << excpt.getMessage() << endl;
    }
}

//stores the string value of the machine code
void sicxe_asm::get_machine_code(string filename, int start){
	int base = 0;
	int source=0;
	int destination=0;
	int current=0;
	string opcode;
	string operand;
	string machine_code;
	string bits;
	symtab sym;
	sicxe_asm obj;
	opcodetab tab;
	file_parser parser(filename);
	parser.read_file();
	for(int i=0; i<parser.size(); i++){
		opcode = parser.get_token(i,1);
		opcode = sym.to_uppercase(opcode);
		if(opcode == " "){
			binary.push_back(opcode);
			continue;
		}
		else if(opcode=="START"){
			binary.push_back(" ");
			continue;
		}
		else if(opcode=="END"){
			binary.push_back(" ");
			break;
		}
		else if(opcode == "WORD"){
			//convert decimal to hex
			//fill all 6 hex digits
			//push back as machine code
			int word = sym.string_to_int(parser.get_token(i,2));
			opcode = sym.int_to_hex(word,6);
			binary.push_back(opcode);
			continue;
		}
		else if(opcode == "BYTE"){
			//if X, store what is inside ' '
			//if C, convert characters into hex values
			//push back as machine code
			string machine="";
			operand = parser.get_token(i,2);
			if(operand[0]=='X'||operand[0]=='x'){
				int position = operand.find("'");
				operand = operand.substr(position+1, operand.find_last_of("'")-2);
				binary.push_back(operand);
			}
			if(operand[0]=='C'||operand[0]=='c'){
				int position = operand.find("'");
				operand = operand.substr(position+1, operand.find_last_of("'")-2);
				for(unsigned int i=0; i<operand.length(); i++){
					char x = operand.at(i);
					int ascii = int(x);
					string letter=sym.int_to_hex(ascii,0);
					machine.append(letter);
				}
					binary.push_back(machine);
			}
			continue;
		}
		else if(opcode=="RESB"){
			binary.push_back(" ");
			continue;
		}
		else if (opcode=="RESW"){
			binary.push_back(" ");
			continue;
		}
		else if(opcode == "EQU"){
			binary.push_back(" ");
			continue;
		}
		else if(opcode == "BASE"){
			base++;
			source=sym.get_base(parser.get_token(i,2),filename,start);
			binary.push_back(" ");
			continue;
		}
		else if(opcode=="NOBASE"){
			base--;
		}
		else if(opcode=="RSUB"){
			binary.push_back("4F0000");
			continue;
		}	
		else if(opcode=="+RSUB"){
			binary.push_back("4F000000");
			continue;
		}
		if(tab.get_instruction_size(opcode)==1){
			binary.push_back(tab.get_machine_code(opcode));
			continue;
		}
		if(tab.get_instruction_size(opcode)==2){
			machine_code = tab.get_machine_code(opcode);
			operand = parser.get_token(i,2);
			int position = operand.find(",");
			//check for shiftl or shiftr
			if(opcode=="SHIFTL"||opcode=="SHIFTR"){
				machine_code.append(get_register(operand.substr(0,position)));
				int n=(sym.string_to_int(operand.substr(position+1)));
				n=n-1;
				if(n<0){
					throw symtab_exception(sym.string_to_sstring(sym.int_to_string(i+1)));
				}
				machine_code.append(sym.int_to_string(n));
			}else if(position == -1){
				if(obj.get_register(operand)=="ERROR"){
					int hex = sym.string_to_int(operand);
					operand= sym.int_to_hex(hex,1);
					machine_code.append(operand);
				}else{
					machine_code.append(obj.get_register(operand));
				}
				machine_code.append("0");
			}else{
				machine_code.append(get_register(operand.substr(0,position)));
				machine_code.append(get_register(operand.substr(position+1)));
			}
			binary.push_back(machine_code);
			machine_code="";
			continue;
		}else{
			operand = parser.get_token(i,2);
			machine_code = tab.get_machine_code(opcode);
			machine_code = obj.hex_to_binary(machine_code);
			machine_code = machine_code.substr(0,6);
			machine_code.append(obj.set_n(operand));
			machine_code.append(obj.set_i(operand));
			machine_code.append(obj.set_x(operand));
	
			//*******check for # and @ *******///
			if(operand.substr(0,1)=="@"){
				operand=operand.substr(1);
				if(tab.get_instruction_size(opcode)==4){
					destination = sym.get_base(operand,filename,start);
					string based = sym.int_to_hex(destination,5);
					machine_code.append("001");
					machine_code=binary_to_hex(machine_code);
					if(machine_code.length()<3)
						machine_code.insert(0,"0");
					machine_code.append(based);
					binary.push_back(machine_code);
					machine_code="";
					continue;
				}else{
					string pced;
					current = sym.current_address(filename,start,i);
					current=current+3;
					destination=sym.get_base(operand,filename,start);
					int pc=destination-current;
					if(pc<0){
						pced=sym.int_to_hex(pc,3);
						pced = pced.substr(pced.length()-3);
					}else{
						if(pc<-2048||pc>2047){
							if(base!=1)
								throw symtab_exception(sym.string_to_sstring(sym.int_to_string(i+1)));
						else{
							destination=sym.get_base(operand,filename,start);
							int diff =(destination-source);
							if(diff <0)
								throw symtab_exception(sym.string_to_sstring(sym.int_to_string(i+1)));
							string based=sym.int_to_hex(diff,3);
							machine_code.append("100");
							machine_code=binary_to_hex(machine_code);
							if(machine_code.length()<3)
								machine_code.insert(0,"0");
							machine_code.append(based);
							binary.push_back(machine_code);
							machine_code="";
							continue;
						}
					pced=sym.int_to_hex(pc,3);
					machine_code.append("010");
					machine_code=binary_to_hex(machine_code);
					if(machine_code.length()<3)
						machine_code.insert(0,"0");
					machine_code.append(pced);
					binary.push_back(machine_code);
					machine_code="";
					continue;
						}
					}
				}
			}
			
			if(operand.substr(0,1)=="#"){
				int hashtag=sym.get_base(operand.substr(1),filename,start);
				operand=operand.substr(1);
				if(hashtag==-1){
					hashtag=sym.string_to_int(operand);
					if(tab.get_instruction_size(opcode)==4){
						operand=sym.int_to_hex(hashtag,5);
						machine_code.append("001");
					}else{
						operand=sym.int_to_hex(hashtag,3);
						machine_code.append("000");
					}
					machine_code=binary_to_hex(machine_code);
					if(machine_code.length()<3)
						machine_code.insert(0,"0");
					machine_code.append(operand);
					binary.push_back(machine_code);
					machine_code="";
					continue;
				}else if(tab.get_instruction_size(opcode)==4){
					destination = sym.get_base(operand,filename,start);
					string based = sym.int_to_hex(destination,5);
					machine_code.append("001");
					machine_code=binary_to_hex(machine_code);
					machine_code.append(based);
					binary.push_back(machine_code);
					machine_code="";
					continue;
				}else{
					string pced;
					current = sym.current_address(filename,start,i);
					current=current+3;
					destination=sym.get_base(operand,filename,start);
					int pc=destination-current;
					if(pc<0){
						pced=sym.int_to_hex(pc,3);
						pced = pced.substr(pced.length()-3);
					}else if(pc<-2048||pc>2047){
							if(base!=1)
								throw symtab_exception(sym.string_to_sstring(sym.int_to_string(i+1)));
						else{
							destination=sym.get_base(operand,filename,start);
							int diff =(destination-source);
							if(diff <0)
								throw symtab_exception(sym.string_to_sstring(sym.int_to_string(i+1)));
							string based=sym.int_to_hex(diff,3);
							machine_code.append("100");
							machine_code=binary_to_hex(machine_code);
							if(machine_code.length()<3)
								machine_code.insert(0,"0");
							machine_code.append(based);
							binary.push_back(machine_code);
							machine_code="";
							continue;
						}	
					}else{
					pced=sym.int_to_hex(pc,3);
					machine_code.append("010");
					machine_code=binary_to_hex(machine_code);
					if(machine_code.length()<3)
						machine_code.insert(0,"0");
					machine_code.append(pced);
					binary.push_back(machine_code);
					machine_code="";
					continue;
					}
				}
			}
					
			else{
				if(tab.get_instruction_size(opcode)==4){
					destination = sym.get_base(operand,filename,start);
					string based = sym.int_to_hex(destination,5);
					machine_code.append("001");
					machine_code=binary_to_hex(machine_code);
					if(machine_code.length()<3)
						machine_code.insert(0,"0");
					machine_code.append(based);
					binary.push_back(machine_code);
					machine_code="";
					continue;
				}
				string pced;
				current = sym.current_address(filename,start,i);
				current=current+3;
				destination=sym.get_base(parser.get_token(i,2),filename,start);
				int pc=destination-current;
				if(pc<0 && pc>-2048){
					pced=sym.int_to_hex(pc,3);
					pced = pced.substr(pced.length()-3);
					machine_code.append("010");
					machine_code=binary_to_hex(machine_code);
					if(machine_code.length()<3)
						machine_code.insert(0,"0");
					machine_code.append(pced);
				}else if(pc>2047){
						if(base!=1)
							throw symtab_exception(sym.string_to_sstring(sym.int_to_string(i+1)));
						else{
							destination=sym.get_base(operand,filename,start);
							int diff =(destination-source);
							if(diff <0)
								throw symtab_exception(sym.string_to_sstring(sym.int_to_string(i+1)));
							string based=sym.int_to_hex(diff,3);
							machine_code.append("100");
							machine_code=binary_to_hex(machine_code);
							if(machine_code.length()<3)
								machine_code.insert(0,"0");
							machine_code.append(based);
							binary.push_back(machine_code);
							machine_code="";
							continue;
						}
				}
				else if(pc>0 && pc<2047){
					pced=sym.int_to_hex(pc,3);
					machine_code.append("010");
					machine_code=binary_to_hex(machine_code);
					if(machine_code.length()<3)
						machine_code.insert(0,"0");
					machine_code.append(pced);
					binary.push_back(machine_code);
					machine_code="";
					continue;
				}
				else{
					throw symtab_exception(sym.string_to_sstring(sym.int_to_string(i+1)));
				}
			}		
			binary.push_back(machine_code);
			machine_code="";
		}
	}
}

//returns the value of the n bit
string sicxe_asm::set_n(string operand){
	if(operand.substr(0,1)== "#")
		return "0";
	return "1";
}
	
//returns the value of the i bit
string sicxe_asm::set_i(string operand){
	if(operand.substr(0,1)=="@")
		return "0";
	return "1";
}
	
//returns value of x bit
string sicxe_asm::set_x(string operand){
	symtab sym;
	if(operand.find(",")>=0){
		operand = operand.substr(operand.find(",")+1);
		operand =sym.to_uppercase(operand);
		if(operand=="X")
			return "1";
	}	
	return "0";
}
	
//returns value of b bit
string sicxe_asm::set_b(string opcode, int bit){
	opcodetab tab;
	if(tab.get_instruction_size(opcode)==4)
		bit=0;
	stringstream ss;
	ss<<bit;
	string base = ss.str();
	return base;
}
	
//returns value of p bit
string sicxe_asm::set_p(string opcode, string operand, int base){
	opcodetab tab;
	sicxe_asm obj;
	if(tab.get_instruction_size(opcode)==4 || base!=0 || obj.set_n(operand)=="0")
		return "0";
	return "1";
}
	
//returns value of e bit
string sicxe_asm::set_e(string opcode){
	opcodetab tab;
	int size=tab.get_instruction_size(opcode);
	if(size==4)
		return "1";
	return "0";
}

string sicxe_asm::return_machine_code(int index){
	string machine = binary[index];
	//machine = format_15(machine);
	return machine;
	
}
//reurns corresponding register value
string sicxe_asm::get_register(string reg){
	symtab sym;
	reg = sym.to_uppercase(reg);
	if(reg == "A")
		return "0";
	if(reg == "X")
		return "1";
	if(reg == "L")
		return "2";
	if(reg == "B")
		return "3";
	if(reg == "S")
		return "4";
	if(reg == "T")
		return "5";
	if(reg == "PC")
		return "8";
	if(reg == "SW")
		return "9";
	return "ERROR";
}
//converts hex to binary
string sicxe_asm::hex_to_binary(string s){
	string binary="";
	string byte;
	for(unsigned int i=0; i<s.length(); i++){
		byte=s.substr(i,i+1);
		if(byte=="0")
			binary.append("0000");
		if(byte=="1")
			binary.append("0001");
		if(byte=="2")
			binary.append("0010");
		if(byte=="3")
			binary.append("0011");
		if(byte=="4")
			binary.append("0100");
		if(byte=="5")
			binary.append("0101");
		if(byte=="6")
			binary.append("0110");
		if(byte=="7")
			binary.append("0111");
		if(byte=="8")
			binary.append("1000");
		if(byte=="9")
			binary.append("1001");
		if(byte=="A")
			binary.append("1010");
		if(byte=="B")
			binary.append("1011");
		if(byte=="C")
			binary.append("1100");
		if(byte=="D")
			binary.append("1101");
		if(byte=="E")
			binary.append("1110");
		if(byte=="F")
			binary.append("1111");
	}
	return binary;
}
//converts binary to hex
string sicxe_asm::binary_to_hex(string binary){
    stringstream ss;
    bitset<12> set(binary);
    ss << hex << uppercase << set.to_ulong();
    return ss.str();
}

//Function to convert string to upper case 
string sicxe_asm::format_15(string xx) {
 	//string xx = x.str();
 	stringstream tmmp;
 	tmmp << setw(15) << xx;
 	return tmmp.str();
 }
