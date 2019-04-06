#include<bits/stdc++.h>
#include "global.h"
#include "DFA.h"
using namespace std;

/* regex and DFAs for accepting valid identifiers , numbers and literals */
string id_regex,no_regex,literal_regex;  
DFA id_DFA,no_DFA,lit_DFA;





/*delimiters punctuations keywords and operators*/
set<char> delims={',',';','(',' ',')','\n'};

set<char> puncs={'(',')',';',','};

set<string> keywords={
					"INT","VARCHAR","DATE","TIME","FLOAT",
					"WHERE","FROM","AND","SELECT","VALUES"
					};

set<string> ops={
				"<",">","=",
				"LIKE","BETWEEN","IN"
				};


/*string input type to token type mapping*/ 
map< string , token > op_token_bindings,kword_token_bindings;

map< char, token > punc_token_bindings;

void sql_error(string s){
	cout<<file_nm<<":"<<line_no<<":"<<col_no<<": error :"<<s<<endl;
	exit(0);
	return ;
}
/*function to expand a regex , eg:- f(a,d) = a|b|c|d*/
string expand(const char &frm,const char & to){
	string exp="";
	for(int i=frm;i<to;i++){
		exp.push_back((char)i);
		exp.push_back('|');
	}
	exp.push_back((char)to);
	return exp;
}

/*lexer function , returns with a token type , when called , returns with 'EOI' when end of input encountered */
token sql_lex(){

	bool match=false;
	int cur=sql_pos;
	token t=EOI;
	while(!match && cur< sql_len){

	if(source[cur] == ' ' or source[cur] == '\n' or source[cur] == '\t'){
		if(source[cur] == '\n' ){
			line_no++;
			line_start=sql_pos;
			col_no=1;
		}
		cur++;
		}
		/*return token type if  a punctuation is encountered*/
		else if(puncs.find(source[cur]) != puncs.end()){
			t=punc_token_bindings[source[cur]];
			break; 
		}	

		else{
			
			int j=cur;
			while( j< source.length() && delims.find(source[j]) == delims.end() ) {j++;}
			if(source.substr(cur,j-cur) == "CREATE"){
				int k=j+1;
				while( k < source.length() && delims.find(source[k]) == delims.end()) k++;
				if(source.substr(j,k-j) == " TABLE"){
					t=CREAT;
					cur=k;
					break;
				}
				else{
					sql_error("CREATE must be followed by TABLE");
				}
			}

			else if(source.substr(cur,j-cur) == "INSERT"){
				int k=j+1;
				while( k < source.length() && delims.find(source[k]) == delims.end()) k++;
				if(source.substr(j,k-j) == " INTO"){
					//cout<<"Insert into table"<<endl;
					t=INSRT;
					cur=k;
					break;
				}
				else{
					sql_error("INSERT must be followed by INTO");
				}
			}

			/*returns PK for "PRIMARY KEY"*/
			else if(source.substr(cur,j-cur) == "PRIMARY"){
				int k=j+1;
				while( k < source.length() && delims.find(source[k]) == delims.end()) k++;
				if(source.substr(j,k-j) == " KEY"){
					t=PK;
					cur=k;
					break;
				}
				else{
					sql_error("PRIMARY should be followed by KEY");
				}
			}

			/*returns respective token for keyword*/
			else if(keywords.find(source.substr(cur,j-cur))!=keywords.end()){
				t=kword_token_bindings[source.substr(cur,j-cur)];
				cur=j;
				break;
			}
			/*returns respective token for operator*/
			else if(ops.find(source.substr(cur,j-cur))!=ops.end()){
				t=op_token_bindings[source.substr(cur,j-cur)];
				cur=j;
				break;
			}
			
			/* identifier and constant block*/
			else{

				/*check if the returned word is a number and return token 'NUM' */
				if(no_DFA.checkMembership(source.substr(cur,j-cur)) == true ) t=NUM;
				
				/*check if returned block is a literal and return token 'LIT'*/
				else if(lit_DFA.checkMembership(source.substr(cur,j-cur)) == true ) t=LIT;
				
				/*check if word is valid identifier and return token 'ID'*/	
				else if(id_DFA.checkMembership(source.substr(cur,j-cur)) == true ) t=ID;
				
				/*invalid identifier*/
				else{
					sql_error("invalid identifier");
					}
				
				cur=j;
				
				break;
			}

		}
	}
 if(t == COMMA || t == SEMI || t == OPN_BKT || t == CLS_BKT) sql_pos=cur+1;
 else sql_pos = cur;
 col_no=cur-line_start;
 return t;		

}
void setup_lexer(){
		id_regex = "(" + 
				expand('a','z') + 
				"|_|" + 
				expand('A','Z') + 
				").(" +
				expand('a','z') + 
				"|" + 
				expand('A','Z') + 
				"|_|" + 
				expand('0','9') + 
				")*" 
				;
	
	no_regex = "(" + 
				expand('0','9') + 
				").(" + 
				expand('0','9') + 
				")*"
				;

	literal_regex=  "'.(" + 
					expand('a','z') + 
					"|" + 
					expand('0','9') + 
					"|" + 
					expand('A','Z') + 
					").(" + 
					expand('a','z') + 
					"|" + 
					expand('A','Z') + 
					"|" +
					expand('0','9') + 
					")*.'" 
					;

	id_DFA.create(id_regex);
	no_DFA.create(no_regex);
	lit_DFA.create(literal_regex);

	line_no=col_no=line_start=1;

	op_token_bindings.insert({"<",LSR});
	op_token_bindings.insert({">",GRTR});
	op_token_bindings.insert({"=",EQ});
	op_token_bindings.insert({"BETWEEN",BTWN});
	op_token_bindings.insert({"IN",IN});
	op_token_bindings.insert({"LIKE",LIKE});

	kword_token_bindings.insert({"INT",TYP});
	kword_token_bindings.insert({"FLOAT",TYP});
	kword_token_bindings.insert({"VARCHAR",TYP});
	kword_token_bindings.insert({"DATE",TYP});
	kword_token_bindings.insert({"TIME",TYP});
	kword_token_bindings.insert({"WHERE",WHR});
	kword_token_bindings.insert({"FROM",FRM});
	kword_token_bindings.insert({"AND",AND});
	kword_token_bindings.insert({"SELECT",SEL});
	kword_token_bindings.insert({"VALUES",VAL});

	punc_token_bindings.insert({'(',OPN_BKT});
	punc_token_bindings.insert({')',CLS_BKT});
	punc_token_bindings.insert({',',COMMA});
	punc_token_bindings.insert({';',SEMI});
	return;
}


