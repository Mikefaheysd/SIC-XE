/* MONTANA
    Michael Fahey, Miguel Liberato, Evan Tam, Kyle Rodrigues
    masc1202
    prog1
    CS530, Spring 2014
*/

#include <iostream>
#include <fstream>
#include <sstream>

#include "file_parser.h"
#include "file_parse_exception.h"

using namespace std;

// Takes in the filename and stores for later
file_parser::file_parser(string filename)
{
    this->filename = filename;
}

file_parser::~file_parser()
{
}

// Stores each line of text into the back of a vector until end of file. Closes file upon completion
void file_parser::read_file()
{
    ifstream infile;

    // open file with read-only privileges
	infile.open(filename.c_str(), ios::in);

	// check to see if file opened successfully
	if(!infile.is_open())
		throw file_parse_exception("Error: Failed to open file: " + filename);

    struct record rec;
    string line;

    // read the content of the file line by line
	while(!infile.eof())
	{
	    // read the current line
		getline(infile, line);

		// reset data for line
        rec.label =    "";
        rec.comment =  "";
        rec.opcode =   "";
        rec.operand =  "";

        // checks for empty line
        if(line.empty())
        {
            // add empty line to data
            lineContent.push_back(rec);
            break;
        }

        // find the first character of the line
        unsigned long index = line.find_first_not_of(" \t\r\n");

        // check for no characters in the line
        if(-1 == index)
        {
            // line only has whitespace, content is empty
            lineContent.push_back(rec);
            break;
        }

        // checks for full line comment
        if('.' == line[index])
        {
            // update comment, leave everything else empty
            rec.comment = line.substr(index);
            lineContent.push_back(rec);
            break;
        }

        /* line is not empty, has non-white space characters and is not a full line comment
         * must have content to parse start search for label (optional), opcode, operand and comment (optional)
         */

        // check to see if label is on the line, labels start at position 0 on the line
        // label can only be 8 characters and must begin with alphanumeric
        if(0 == index)
        {
            // update label with line content
            rec.label = "";
        }
    }



	for(unsigned int i=0; i < v.size(); i++) {									
		string line = v[i];
        string label = " ";
        string opcode = " ";					//clear each string for next use
        string operand = " ";
        string comment = " ";
			while(1){

			/*start search for label*/
			int is_empty = line.find(" ");  	//validates there is a label, if not return empty string
			if(is_empty == -1||is_empty>7)
				is_empty = line.find ("\t");    //if line contains no spaces, check for tabs
			if(is_empty>7||is_empty==-1)
				is_empty=7;
				
			label = line.substr(0,is_empty);	//takes substring from beginning of
								//line to first whitespace, store in label
			char * str =new char [label.length()+1];
			strcpy(str,label.c_str());
			for(unsigned int i=0; i<label.length();i++){	//validates label is alphanumerical
				if(isalnum(str[i])==0)
					throw file_parse_exception(int_to_string(size()+2)); 
						
			}					
								/*end search for label*/
		
			line = line.substr(is_empty);		//take out label, create new substring of opcode/operand
			int j = line.find_first_not_of(' ');
			if (j == -1){ 
				opcode = " ";
				operand = " ";
				comment = " ";
				break;
			}
			if (j==0)
				j= line.find_first_not_of('\t');	//remove any additional white space
			line = line.substr(j);
			if(line.substr(0,1)=="."){		//checks that token is not comment
				opcode = " ";
				operand = " ";
				comment = line;
				break;
			}
			int a = line.find(" ");
			int b = line.find("\t");		//tests if space or tab comes first
			if(a == -1 && b == -1){
				opcode = line;
				operand = " ";
				comment = " ";
				break;
			}
			if(a==-1)
				a= 100;	
			if(b==-1)
				b = 100;
			
			if (b < a)
				is_empty = b;
	
			else
				is_empty = a;	
			
			opcode = line.substr(0,is_empty);	//takes token from beginning to first whitespace
			
			line = line.substr(is_empty);		//take out opcode, create new substring of operand/comment
			j = line.find_first_not_of(' '); 
			if (j==0)
				j= line.find_first_not_of('\t');	//remove any additional white space
			if(j==-1){
				operand = " ";
				comment = " ";
				break;
			}
			line = line.substr(j);
			
			if(line.substr(0,1)=="."){		//checks to see if token is comment
				comment = line;
				operand = " ";
				break;
			}
			
			else{
			int i = line.find("'");			//checks to see if there is a quoted token
			
			if (i != -1) {
           			int position = line.find_last_of("'");
				if(i==position)
					throw file_parse_exception(int_to_string(size()+2));
           		 	operand = line.substr(0,position+1);
		
				line = line.substr(position+1);
       			}
			else{
				
				int position = line.find_first_of(" ");
				
				if(position == -1){
					operand = line;
					comment = " ";
					break;
				}
				else{
					operand = line.substr(0,position);
					line = line.substr(position);
				}				//end search for operand
			}
       			j = line.find_first_not_of(' '); 
			
			if(j==-1)
				line = " ";
			else if (j==0){
				j= line.find_first_not_of('\t');	//remove any additional white space
				line=line.substr(j);
				if(j==0)
					line = " ";
			}
			else
				line = line.substr(j);
			
			if(line.substr(0,1)!="." && line.substr(0,1)!=" ") //throws error if next token is not comment
				throw file_parse_exception(int_to_string(size()+2));
			else		
        			comment = line;				//end search for comment
			break;
   			}
			}
		v2.push_back(label);
		v2.push_back(opcode);				//pushes each element to the back of the vector
		v2.push_back(operand);	
		v2.push_back(comment);

		label = " ";
		opcode = " ";					//clear each string for next use
		operand = " ";
		comment = " ";
	}

	// done reading file, clean up
    infile.close();
}

//returns the element in the vector by multiplying the row by 4 and adding
//the starting position column
string file_parser::get_token(unsigned int row, unsigned int column)
{
    string token = "";
    if(lineContent.size() < row)
    {
        cout << "Error: invalid token row request" << endl;
        return token;
    }

    switch (column)
    {
        case 0:
            token = lineContent[row].label;
            break;
        case 1:
            token = lineContent[row].opcode;
            break;
        case 2:
            token = lineContent[row].operand;
            break;
        case 3:
            token = lineContent[row].comment;
            break;
        default:
            cout << "Error: invalid token column request" << endl;
    }
    //return token;
	int i= column + (4*row);
	return v2[i];
}

//iterates through each vector element and tabs to make space for next element
//when it has printed the fourth element in each row, creates new line
void file_parser::print_file()
{
//    for(unsigned int i = 0; i < lineContent.size(); i++)
//    {
//        cout << lineContent[i].label   << "\t"
//             << lineContent[i].opcode  << "\t"
//             << lineContent[i].operand << "\t"
//             << lineContent[i].comment << "\n";
//    }

	for(unsigned int i = 0; i<v2.size();i++){
		if(i % 4 == 0 && i!=0)
			cout<<endl;
		cout<<v2[i]<<"\t"<<"\t";		
	}
	cout<<endl;
}

//returns the size of text file by dividing the total number of 
//vector elements by four and subtracting 1 
int file_parser::size(){
    return lineContent.size();
	//return v2.size()/4 - 1;
}

string file_parser::int_to_string (int n){
	ostringstream s;
	s << "file parse error line number " << n;
	return s.str();
}
