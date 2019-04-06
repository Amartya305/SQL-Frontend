#include "global.h"
using namespace std;

int sql_pos=0,sql_len,line_no,col_no,line_start;
string source;
string file_nm;
int main(int argc,char ** argv){
	file_nm=argv[1];
	ifstream input_file(argv[1]);
	stringstream contents;
	contents<<input_file.rdbuf();
	source=contents.str();
	sql_len=source.length();	
	setup_lexer();
	setup_parser();
	sql_parse();
	
	return 0;	
}