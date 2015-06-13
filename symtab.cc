/* MONTANA
    Michael Fahey, Miguel Liberato, Evan Tam, Kyle Rodrigues
    masc1202
    prog4
    CS530, Spring 2014
*/

#include "symtab.h"


//gets the starting address of the program in decimal
int symtab::get_start(string s){
	int address = 0;
	string token;
	file_parser parser(s);
	parser.read_file();	//read in file, check for errors
	for(int i = 0; i<parser.size(); i++){
		for(int j = 0; j < 4; j++){	//iterate through all opcodes
		token = parser.get_token(i,j);	
		transform(token.begin(),token.end(),token.begin(),::toupper);
			if(token == "START"){	
				token = parser.get_token(i,j+1);	//look for start opcode
				int pos = token.find("$");
					if(pos==-1){		//test if operand is hex or decimal
						address = string_to_int(token); //convert string to int
						
					}
					else{
						token = token.substr(pos+1);
						address = hex_to_int(token);	//convert hex to int
					}
			}	
		}
	}	
	return address;
}

//creates the .lis file to write to 
string symtab::create_file(string s){
	int i = s.find(".");	//search for extention
	if (i==-1)
		i = s.length();
	s=s.substr(0,i);
	s.append(".lis");	//attach .lis extention to file
	return s;
}

//write the line number, address, label, opcode and operand to file
void symtab::write_to_file(string s, int size, string filename){
	int line = 1;
	int space=25;
	string token;
	sicxe_asm sic;
	int start=get_start(filename);
	sic.get_machine_code(filename,start);
	file_parser parser(filename);
	parser.read_file();
	fout.open(s.c_str());		//open previously created file from create_file
	fout<<"Line#"<<"\t"<<"Address"<<"\t"<<"Label"<<"\t"<<"Opcode"<<"\t"<<"Operand"<<setw(25)<<"Machine code"<<endl;
	fout<<"====="<<"\t"<<"======="<<"\t"<<"====="<<"\t"<<"======"<<"\t"<<"======="<<setw(25)<<"=========== "<<endl;
	while (line<size+1){
		for(int i = 0; i<size; i++){
		fout<< line << "\t" <<int_to_hex(v[i],5);
			for(int j = 0; j < 3; j++){		//iterate through label, opcode, operand
				token = parser.get_token(i,j);  //and write to file
				fout<<"\t"<<token;	
		 	}
		space=space-token.length();
		if(space<0)
			space=5;
		for(int k=0;k<space;k++){	
			fout<<" ";
		}
		fout<<sic.return_machine_code(i);
		fout<<endl;
		space=20;
		line++;
		}
	}
	fout.close();	//end writing to file
}

//creates and writes to .obj file
void symtab::write_to_obj(string filename,int start,int size){
	int flag=0;
	int count=60;
	int text_length;
	int current;
	int length;
	sicxe_asm sic;
	string obj;
	string tmp;
	string text;
	string text2="";
	string prev="";
	string header=create_header(filename,start);
	obj=create_file(filename);
	obj=obj.substr(0,obj.length()-3);
	obj.append("obj");
	fout.open(obj.c_str());
	fout<<header<<endl;
	text = create_text(filename,start,0,1);
	for(int i=1; i<size;i++){
		flag=0;
		prev = create_text(filename,start,i-1,flag);
		tmp = create_text(filename,start,i,flag);
		length=tmp.length();
		count = count-tmp.length();
		if(tmp!="" && prev==""&& count!=60-length){ 
			current = current_address(filename,start,i);
			flag=1;
			text_length=text2.length()/2;
			text.append(int_to_hex(text_length,2));
			text.append(text2);
			fout<<text<<endl;
			text = create_text(filename,current,i,flag);
			text2=tmp;
			count=60-tmp.length();
		}
		else if(count<=0){ //create new line for full text record
			current = current_address(filename,start,i);
			flag=1;
			text_length=text2.length()/2;
			text.append(int_to_hex(text_length,2));
			text.append(text2);
			fout<<text<<endl;
			text = create_text(filename,current,i,flag);
			text2=tmp;
			count=60-tmp.length();
		}else{
			text2.append(tmp);
		}

	}
	if(count>0){ //while text file is not full, continue appending machine code
		text_length=text2.length()/2;
		text.append(int_to_hex(text_length,2));
		text.append(text2);
		fout<<text<<endl;
	}
	if(start==0){ //only modifed if starting address is 0
		for(int i=0;i<size;i++){
			if(create_m(filename,start,i)!= "")
				fout<<create_m(filename,start,i)<<endl;
		}
	}
	fout<<create_end(start);
	fout.close();
}

//generates all of the address for the input file
//stores in a vector, takes in starting address
//handles all cases of assembler directives
void symtab::create_address(string s,int start){
	int j = 1;
	int address = 0;
	string opcode;
	opcodetab obj;
	file_parser parser(s);
	parser.read_file();	//reads in file
	for(int i=0; i <parser.size();i++){
		opcode = parser.get_token(i,j);
		transform(opcode.begin(),opcode.end(),opcode.begin(),::toupper);
		if(opcode==" ")					/*begin check for assembler directives*/
			v.push_back(address);
		else if(opcode=="START"){
			v.push_back(address);
			address = start;
		}
		else if(opcode=="END"){
			v.push_back(address);
			break;
		}
		else if(opcode == "WORD"){
		        v.push_back(address);
			address= address + 3;
			
		}
		else if(opcode == "BYTE"){
			string byte = parser.get_token(i,j+1);
			v.push_back(address);
			if(byte.substr(0,1)== "X"||byte.substr(0,1)=="x"){
				if(byte.length()%2==0)
					throw symtab_exception(string_to_sstring(byte));
				else
					address = address + (((byte.length())-3)/2);
			}
			else if(byte.substr(0,1)== "C"||byte.substr(0,1)=="c")
				address = address + (byte.length()-3);
			else
				throw symtab_exception(string_to_sstring(byte));
		}
		else if(opcode=="RESB"){
			string rbyte = parser.get_token(i,j+1);
				v.push_back(address);
			address = address + string_to_int(rbyte);
		}
		else if (opcode=="RESW"){
			string rword = parser.get_token(i,j+1);
				v.push_back(address);
			address = address + string_to_int(rword) *3;	
		}
		else if(opcode == "EQU"){
			v.push_back(v[i-1]);
		}
		else if(opcode == "BASE"){
			if(check_assembler(parser.get_token(i-1,j))==0){
				v.push_back(v[i-1]);
			}else{
				//address= address + obj.get_instruction_size(parser.get_token(i-1,j));
				v.push_back(address);
			}
		}
		else if(opcode=="NOBASE"){
			v.push_back(v[i-1]);			/*end check for assembler directive*/
		}
		else {
			v.push_back(address);			//push decimal address to back of vector
			address = address + obj.get_instruction_size(opcode); //update address 
		}
	}
}

//creates symbol table for file using map
//stores label as key and address/constant as value
void symtab::create_symtab(string s,int start){
	string check;
	file_parser parser(s);
	parser.read_file();	//reads in file
	create_address(s,start);
	for(int i = 0; i<parser.size()-1; i++){
	if(parser.get_token(i,0).length()>0 && parser.get_token(i,0)!=" "){
		check =(parser.get_token(i,1));		//iterates through labels
		check = to_uppercase(check);
		if(check == "EQU"){			//handles EQU cases
			check = parser.get_token(i,2);
			int n= string_to_int(check);
			if(n==0){
				continue;
			}
			check=int_to_hex(n,4);
			table [parser.get_token(i,0)] = check;	//puts symbol into table
			continue;	
		}
		if(table.find(parser.get_token(i,0))!=table.end())	//checks to see if token already defined in symtable
			throw symtab_exception(string_to_sstring(parser.get_token(i,0)));
		table [parser.get_token(i,0)] = int_to_hex(v[i],5);
	}
	
	}
}

//Function to convert string to int
int symtab::string_to_int(string s){
 	istringstream instr(s);
 	int n=0;
 	instr >> n;
 	return n; 
}

//converts int to hex
string symtab::int_to_hex(int num, int width){ 
	stringstream out;
	out << setw(width) << setfill('0') << hex << num; 
	return to_uppercase(out.str());
}

//Function to convert string to upper case
string symtab::to_uppercase(string s){ 
	transform(s.begin(),s.end(),s.begin(),::toupper);
	return s; 
}

//Function to convert string hexadecimal to a decimal integer
int symtab::hex_to_int(string s){
	int value;
	sscanf(s.c_str(),"%x",&value);
	return value;
}

//converts string to sstream
string symtab::string_to_sstring (string st){
	ostringstream s;
	s<<"symtab exception on line "<<st;
	return s.str();
}

//converts int to string
string symtab::int_to_string(int a){
	stringstream ss;
	ss << a;
	string str = ss.str();
	return str;

}
int symtab::current_address(string filename, int start, int index){
	create_address(filename,start);
	int add=v[index];
	return add;
}

int symtab::get_base(string operand, string filename,int start){
	string address;
	create_symtab(filename,start);
	int position = operand.find(",");
	int tag = operand.find("#");
	int at=operand.find("@");
	if(position!=-1){
		operand = operand.substr(0,position);
	}
	else if(tag!=-1){
		operand=operand.substr(1);
	}
	else if(at!=-1){
		operand=operand.substr(1);
	}
	else{
		operand=operand;
	}
	
	typedef map<string,string>::iterator it_type;
	it_type iterator = table.begin();
	iterator=table.find(operand);
	if(iterator==table.end()){
		table.clear();
	 	return -1;
	}
	 address=iterator->second;
	 int add=hex_to_int(address);
	 table.clear();
	 return add;
}

int symtab::check_assembler(string opcode){
		opcode = to_uppercase(opcode);
		if(opcode==" ")					/*begin check for assembler directives*/
			return 1;
		else if(opcode=="START"){
			return 0;
		}
		else if(opcode=="END"){
			return 0;
		}
		else if(opcode == "WORD"){
		      return 0;
			
		}
		else if(opcode == "BYTE"){
			return 0;
		}
		else if(opcode=="RESB"){
			return 0;
		}
		else if (opcode=="RESW"){
			return 0;
		}
		else if(opcode == "EQU"){
			return 0;
		}
		else if(opcode == "BASE"){
			return 0;
		}
		else if(opcode=="NOBASE"){
			return 0;			/*end check for assembler directive*/
		}
	return 1;
}

//creates the header record
string symtab::create_header(string filename,int start){
	create_address(filename,start);
	int length=6;
	string header="H";
	filename = create_file(filename);
	filename= filename.substr(0,filename.length()-4);
	if(filename.length()>6)
		filename=filename.substr(0,6);
	else if(filename.length()<6){
		length=length-filename.length();
		for(int i=0; i<length;i++)
			filename.append(" ");
	}else
		filename=filename;
	
	header.append(filename);	
	header.append(int_to_hex(start,6)); //gets start address
	header.append(int_to_hex((v.back()-start),6));//calulates size of file
	return header;

}

//creates the text record
string symtab::create_text(string filename, int start,int position, int flag){
	sicxe_asm sic;
	sic.get_machine_code(filename,start);
	string text="";
	if(flag==1){
		text.append ("T");
		text.append(int_to_hex(start,6));
		return text;
	}
	if(sic.return_machine_code(position)==" ")
		text="";
	else
		text.append(sic.return_machine_code(position));
	return text;
}

//creates the end record
string symtab::create_end(int start){
	string end="";
	end=int_to_hex(start,6); //gets starting address
	end.insert(0,"E");
	return end;
}

//creates modified record
string symtab::create_m(string filename,int start,int position){
	file_parser parser(filename);
	parser.read_file();
	opcodetab tab;
	string modified="";
	string operand=parser.get_token(position,2);
	string opcode = parser.get_token(position,1);
	opcode = to_uppercase(opcode);
	if(check_assembler(opcode)==1 && opcode!=" "){ 
		if(tab.get_instruction_size(parser.get_token(position,1))==4){
			if(get_base(operand,filename,start)!=-1){
				modified="M";
				modified.append(int_to_hex(v[position]+1,6));
				modified.append("05");
				return modified;
			}
		}
	}else if(check_assembler(opcode)==0 && opcode!=" "){
		if(opcode=="WORD" || opcode=="BYTE"){
			if(get_base(operand,filename,start )!=-1){
				modified="M";
				modified.append(int_to_hex(v[position],6));
				modified.append("06");
				return modified;
			}
		}
	}else
		return modified;
	return modified;
}
