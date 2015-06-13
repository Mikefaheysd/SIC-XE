/* MONTANA
    Michael Fahey, Miguel Liberato, Evan Tam, Kyle Rodrigues
    masc1202
    prog1
    CS530, Spring 2014
*/

#include "file_parser.h"


using namespace std;


//Takes in the filename and opens specified file, if file does not exist outputs error message.
file_parser::file_parser(string filename){
    	infile.open(filename.c_str()); 
	if(!infile.is_open())
		throw file_parse_exception(int_to_string(size()+2));
	while(!infile.eof()){
		getline(infile,line);
		v.push_back(line);
		}
	infile.close();		
}

file_parser::~file_parser(){
}

//Stores each line of text into the back of a vector until end of file. Closes file upon completetion
void file_parser::read_file(){
	for(unsigned int i=0; i < v.size(); i++) {									
		string line = v[i];
			while(1){
			if(line[0] == '.'){		//checks to see if full line comment
				label = " ";
				opcode = " ";		
				operand = " ";
				comment = line;
				break;
			}
			if(line.empty()){		//checks empty lines
				label = " ";
				opcode = " ";
				operand = " ";
				comment = " ";
				break;	
			}				/*start search for label*/
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
    infile.close();						//end reading file
}

//returns the elemnt in the vector by multiplying the row by 4 and adding
//the starting position column
string file_parser::get_token(unsigned int row, unsigned int column){
	int i= column + (4*row);
	return v2[i];
}

//iterates through each vector element and tabs to make space for next element
//when it has printed the fourth element in each row, creates new line
void file_parser::print_file(){
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
	return v2.size()/4 - 1;
}

string file_parser::int_to_string (int n){
	ostringstream s;
	s << "file parse error line number " << n;
	return s.str();
}
