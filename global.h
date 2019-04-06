#include<bits/stdc++.h>
/*tokens to be returned by the lexical analyser to the parser */  
typedef enum  {
				
				EOI,
				OPN_BKT,CLS_BKT,SEMI,COMMA,
				CREAT,INSRT,SEL,
				ID,TYP,
				PK,VAL,
				WHR,FRM,AND,
				EQ,GRTR,LSR,
				BTWN,LIKE,IN,
				NUM,LIT

			} token;

/* current location of lexer and length of total source code */
extern int sql_len,sql_pos,line_no,col_no,line_start;
extern std::string source;
extern std::string file_nm;
/*function to return token from source code */
token sql_lex();
/*parser*/
void sql_parse();
void sql_error(std::string s);
void setup_lexer();
void setup_parser();
