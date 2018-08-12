/* MONTANA
    Michael Fahey, Miguel Liberato, Evan Tam, Kyle Rodrigues
    masc1202
    prog4
    CS530, Spring 2014
*/

#include <iomanip>
#include <sstream>
#include <algorithm>
#include <iostream>

#include "symtab.h"
#include "sicxe_asm.h"
#include "symtab_exception.h"

using namespace std;

symtab::symtab(file_parser *fparser) : start(0)
{
	this->fparser = fparser;
	this->opTab = new opcodetab();
}

symtab::~symtab()
{
    if (NULL != fparser)
    {
        delete fparser;
    }

    if (NULL != opTab)
    {
        delete opTab;
    }
}

// gets the starting address of the program in decimal
// if no start address is provided, default to address 0
unsigned int symtab::get_start()
{
	unsigned int address = 0;
	string token;

	// look through all tokens for START opcode
	for(int i = 0; i < fparser->size(); i++)
	{
	    token = fparser->get_token(i, OPCODE_COL);
        transform(token.begin(), token.end(), token.begin(), ::toupper);

        if("START" == token)
        {
            // operand contains the address to start from.
            // The starting address can be HEX ($) or decimal.
            token = fparser->get_token(i, OPERAND_COL);

            //test if operand is decimal or hex
            int pos = token.find("$");
            if (-1 == pos) // decimal
            {
                address = string_to_int(token);
            }
            else // HEX
            {
                // strip out '$' from start address
                token = token.substr(pos + 1);
                address = hex_to_int(token);
            }
        }
	}

	return address;
}

// appends the .lis extension to the existing filename
string symtab::create_file(string filename)
{
    // search for extension on filename
	unsigned long extPos = filename.find(".");
	filename = filename.substr(0, extPos);

    // attach .lis extension to filename
    filename.append(".lis");
	return filename;
}

//write the line number, address, label, opcode and operand to file
void symtab::write_to_file(string s, int size, string filename){
	int line = 1;
	int space=25;
	string token;
	sicxe_asm sic;
	int start=get_start();
	sic.get_machine_code(filename,start);
	fout.open(s.c_str());		//open previously created file from create_file
	fout<<"Line#"<<"\t"<<"Address"<<"\t"<<"Label"<<"\t"<<"Opcode"<<"\t"<<"Operand"<<setw(25)<<"Machine code"<<endl;
	fout<<"====="<<"\t"<<"======="<<"\t"<<"====="<<"\t"<<"======"<<"\t"<<"======="<<setw(25)<<"=========== "<<endl;
	while (line<size+1){
		for(int i = 0; i<size; i++){
		fout<< line << "\t" <<int_to_hex(v[i],5);
			for(int j = 0; j < 3; j++){		//iterate through label, opcode, operand
				token = fparser->get_token(i,j);  //and write to file
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

// generates all of the address for the input file
// handles all cases of assembler directives
void symtab::create_address()
{
    const unsigned int WORD_SIZE = 3;
	int address = 0;
	string opcode = "";

	for(unsigned int i = 0; i < fparser->size(); i++)
	{
		opcode = fparser->get_token(i, OPCODE_COL);
		transform(opcode.begin(), opcode.end(), opcode.begin(), ::toupper);

        /* begin check for assembler directives */
		if("" == opcode)
		{
            v.push_back(address);
        }
		else if("START" == opcode)
		{
			v.push_back(address);
			address = start;
		}
		else if("END" == opcode)
		{
			v.push_back(address);
			// reached end of program, no further instructions are valid. exit
			break;
		}
		else if("WORD" == opcode)
        {
		    v.push_back(address);
			address += WORD_SIZE;
		}
		else if("BYTE" == opcode)
		{
            v.push_back(address);
			string byte = fparser->get_token(i, OPERAND_COL);

			if(('X' == byte[0]) || ('x' == byte[0]))
			{
				if(0 == (byte.length() % 2))
					throw symtab_exception("Symtab exception on line: " + byte);
				else
					address += ((byte.length() - 3) / 2);
			}
			else if(('C' == byte[0]) || ('c' == byte[0]))
			{
                address += (byte.length() - 3);
            }
			else // invalid syntax
			{
                throw symtab_exception("Symtab exception on line: " + byte);
            }
		}
		else if("RESB" == opcode)
		{
			string rbyte = fparser->get_token(i, OPERAND_COL);
			v.push_back(address);
			address += string_to_int(rbyte);
		}
		else if ("RESW" == opcode)
		{
			string rword = fparser->get_token(i, OPERAND_COL);
			v.push_back(address);
			address += (string_to_int(rword) * WORD_SIZE);
		}
		else if("EQU" == opcode)
		{
            // does not cause a change in address, use previous version of address
			v.push_back(v[i-1]);
		}
		else if("BASE" == opcode)
		{
			if(0 == (check_assembler(fparser->get_token(i-1, OPCODE_COL))))
			{
                // does not cause a change in address, use previous version of address
                v.push_back(v[i-1]);
			}
			else
			{
				v.push_back(address);
			}
		}
		else if("NOBASE" == opcode)
		{
		    // does not cause a change in address, use previous version of address
			v.push_back(v[i-1]);
		}
		/* end check for assembler directive */

        else // default
        {
			v.push_back(address);
			address += opTab->get_instruction_size(opcode);
		}
	}
}

// creates symbol table for file using map
// stores into map with label as key and address/constant as value
void symtab::create_symtab()
{
    // validate labels before proceeding
    checkForDuplicates();

	start = get_start();
	create_address();

}

void symtab::checkForDuplicates()
{
    string token = "";

    // iterates through labels checks for duplicates.
    // all labels must be unique
    for(int i = 0; i < fparser->size(); i++)
    {
        if(!(fparser->get_token(i, LABEL_COL).empty()))
        {
            token = fparser->get_token(i, OPCODE_COL);
            token = to_uppercase(token);

            // handles EQU cases
            if("EQU" == token)
            {
                token = fparser->get_token(i, OPERAND_COL);
                int n = string_to_int(token);

                if(0 != n)
                {
                    token = int_to_hex(n, 4);

                    // checks to see if token already defined in symtable
                    if(table.end() != table.find(fparser->get_token(i, LABEL_COL)))
                        throw symtab_exception("Duplicate label symtab exception on token: " + (fparser->get_token(i, LABEL_COL)));
                    table[fparser->get_token(i, LABEL_COL)] = token;	//puts symbol into table
                }
            }
            else
            {
                // checks to see if token already defined in symtable
                if(table.end() != table.find(fparser->get_token(i, LABEL_COL)))
                    throw symtab_exception("Duplicate label symtab exception on token: " + fparser->get_token(i, LABEL_COL));
                table[fparser->get_token(i, LABEL_COL)] = int_to_hex(v[i], 5);
            }
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

int symtab::current_address(string filename, int start, int index){
	create_address(filename, start);
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

int symtab::check_assembler(string opcode)
{
    int retVal = 1;
    opcode = to_uppercase(opcode);

    /* begin check for assembler directives */
    if("" == opcode)
    {
        retVal = 1;
    }
    else if("START" == opcode)
    {
        retVal = 0;
    }
    else if("END" == opcode)
    {
        retVal = 0;
    }
    else if("WORD" == opcode)
    {
        retVal = 0;
    }
    else if("BYTE" == opcode)
    {
        retVal = 0;
    }
    else if("RESB" == opcode)
    {
        retVal = 0;
    }
    else if ("RESW" == opcode)
    {
        retVal = 0;
    }
    else if("EQU" == opcode)
    {
        retVal = 0;
    }
    else if("BASE" == opcode)
    {
        retVal = 0;
    }
    else if("NOBASE" == opcode)
    {
        retVal = 0;
    }
    else // default
    {
        retVal = 1;
    }
    /* end check for assembler directive */

	return retVal;
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

// creates the end record
string symtab::create_end(int start)
{
	string end = "";
	end = int_to_hex(start, 6); //gets starting address
	end.insert(0, "E");
	return end;
}

//creates modified record
string symtab::create_m(string filename, int start, int position)
{
	string modified="";
    string opcode = fparser->get_token(position, OPCODE_COL);
	string operand = fparser->get_token(position, OPERAND_COL);

	opcode = to_uppercase(opcode);

	if(check_assembler(opcode)==1 && opcode!=" "){ 
		if(opTab->get_instruction_size(fparser->get_token(position, OPCODE_COL))==4){
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
