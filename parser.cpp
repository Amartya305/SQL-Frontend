#include "global.h"


#define productions map< char,vector<string> >
#define parsingTbl  map< pair<char,char> , string >  

using namespace std;

set<char> non_terminals;
set<char> terminals;
map<char,set<char> > first,follow;
productions p;
parsingTbl pTbl;
char start;
ofstream fout("stats.txt"),gout("parser_action.txt");
void remove_left_recursion(){
	map<char,vector<string> >:: iterator itr;
	set<char> visited_nts;
	char new_nt='Z';
	for( itr= p.begin(); itr!=p.end() ;itr++){
			vector<string> & right = itr->second,new_right,new_right1;
			vector<string>:: iterator itr2;
			
			for(itr2 = right.begin();itr2!=right.end();itr2++){
					if( visited_nts.find((*itr2)[0]) != visited_nts.end()){
						
						string gamma="";
						
						if((*itr2).length()!=1) gamma.append((*itr2).substr(1,(*itr2).length()-1)); 
		       
						for( auto strings : p[(*itr2)[0]]){
								new_right.push_back(strings+gamma);
							}  	
						}
					else new_right.push_back(*itr2);  										        		
	               }
			itr->second= new_right;
			
		new_right.clear();
		bool has_left_recursion=false;
			for( auto strings : itr->second) {
				if(strings[0]== itr->first){
					has_left_recursion=true;
					string temp = strings.substr(1,strings.length()-1)+new_nt;
					new_right.push_back(temp);
					}
				
				}
			if(!new_right.empty()){
				
				for( auto strings : itr->second){
					if(strings[0] != itr->first){
					string temp="";
					if(strings!="#") temp=temp+strings+new_nt;
					else temp=temp+new_nt; 
					new_right1.push_back(temp);
					} 
				}
				new_right.push_back("#");
				p[new_nt] = new_right;
				itr->second =new_right1;
				new_nt--;
			}				
			visited_nts.insert(itr->first);		
		}
}

set<char> Union(set<char> s1, set<char> s2,bool &complete){
	if(s1.size()<s2.size()){
	for(auto elem: s1){
		if(s2.find(elem)==s2.end()){
			s2.insert(elem);
			complete=false;
		}		 
	}
	return s2;
	}
	else{
		for(auto elem: s2){
		if(s1.find(elem)==s1.end() ){
			s1.insert(elem);
			complete=false;
		}		 
		}
	return s1;
	}
}

bool is_terminal(char c){
	return non_terminals.find(c) == non_terminals.end();
}

set<char> frst(string alpha){
	set<char> first_local;
	bool dummy;
	for(int i=0;i<alpha.length();i++){
		if(is_terminal(alpha[i])){
			first_local.insert(alpha[i]);
			break;
		}

		else {

		    first_local=Union(first_local,first[alpha[i]],dummy);
			if(first_local.find('#')==first_local.end()) break;
		}
	}
	return first_local;	
}
set<char> calculate_first(char non_t,set<char> visited,bool &complete){
	visited.insert(non_t);
	for(auto prod: p[non_t]){
		for(int i=0;i<prod.length();i++){
			if(is_terminal(prod[i])){
				first[non_t]=Union(first[non_t],first[prod[i]],complete);
			}
			else{
				if(visited.find(prod[i])!=visited.end() && first[prod[i]].empty()) break;
				else if(visited.find(prod[i])!=visited.end() && !first[prod[i]].empty()){
					first[non_t]=Union(first[non_t],first[prod[i]],complete);
				}
				else first[non_t]=Union(first[non_t],calculate_first(prod[i],visited,complete),complete);				
			}
			if(first[prod[i]].find('#')==first[prod[i]].end()) break;
		
		}
	}
	return first[non_t];
	}

void calculate_follow(char nt,bool &complete){
	for(auto prod : p[nt]){
		for(int i=prod.length()-1;i>=0;i--){
			if(!is_terminal(prod[i])){
				if(i+1<prod.length()){
					follow[prod[i]]=Union(follow[prod[i]],first[prod[i+1]],complete);
					int j=i+1;
					while(j<prod.length() && first[prod[j]].find('#') != first[prod[j]].end()){	
						follow[prod[i]]=Union(follow[prod[i]],first[prod[j]],complete);
						j++;
					}
					if(j==prod.length()) follow[prod[i]]=Union(follow[prod[i]],follow[nt],complete);
				}	
			else{
				follow[prod[i]]=Union(follow[prod[i]],follow[nt],complete);
			}			
			}
		}	
	}
}

void build_predictive_parsing_table(){
	set<char> temp_first;
	for(auto rows: p){
		for(auto prod : rows.second){
			temp_first = frst(prod);
			if(temp_first.find('#') == temp_first.end()){
				for(auto ts:temp_first){
					pair<char,char> pr;
					pr.first = rows.first;
					pr.second = ts;
					pTbl[pr]= prod;
				}	
			}
			else {
				for(auto ts:follow[rows.first]){
					pair<char,char> pr;
					pr.first = rows.first;
					pr.second = ts;
					pTbl[pr]="#";										
				}	
			}	
		}
	}
	return;
}

void print_term_non_terms(){
	string decor1(""),decor2("");
	for(int i=0;i<45;i++) decor1=decor1+'=';
	for(int i=0;i<90;i++) decor2=decor2+'-';
	fout<<decor1<<decor1<<endl;
	fout<<"NON_TERMINALS:     ";
	for(auto chrs:non_terminals) fout<<chrs<<" ";
	fout<<endl<<"TERMINALS       ";
	for(auto chrs:terminals) fout<<chrs<<" ";
	fout<<endl<<decor2<<endl;
	return;  	
}	

void print_productions(){
		string decor1(""),decor2("");
		for(int i=0;i<40;i++) decor1=decor1+'=';
		for(int i=0;i<80;i++) decor2=decor2+'-';
		fout<<decor1<<"PRODUCTIONS"<<decor1<<endl; 
		for(auto rows:p){
		fout<<rows.first<<" -> ";
		for(auto st: rows.second){
			fout<<st<<" | ";
			}
		fout<<endl;
		}
		fout<<decor2<<endl;
		return ;
	}

void print_first(){
	string decor1(""),decor2("");
	for(int i=0;i<45;i++) decor1=decor1+'=';
	for(int i=0;i<90;i++) decor2=decor2+'-';
	fout<<decor1<<"FIRST"<<decor1<<endl; 	
	for(auto row:first){
		fout<<row.first<<" -> ";
		for(auto elem:row.second){
				fout<<elem<<" , ";
		}
		fout<<endl;		
	}
	fout<<decor2<<endl;
	return;

}

void print_follow(){
	string decor1(""),decor2("");
	for(int i=0;i<45;i++) decor1=decor1+'=';
	for(int i=0;i<90;i++) decor2=decor2+'-';
	fout<<decor1<<"FOLLOW"<<decor1<<endl; 	
	for(auto row:follow){
		fout<<row.first<<" -> ";
		for(auto elem:row.second){
				fout<<elem<<" , ";
		}
		fout<<endl;		
	}
	fout<<decor2<<endl;
	return;
}

void print_predictive_parsing_table(){
	string decor1(""),decor2("");
	for(int i=0;i<40;i++) decor1=decor1+'=';
	for(int i=0;i<80;i++) decor2=decor2+'-';
	fout<<decor1<<"PREDICTIVE PARSING TABLE"<<decor1<<endl; 
	
	for(auto rows:pTbl){
		fout<<"( "<<rows.first.first<<" ,";
		fout<<rows.first.second<<" ) -> ";
		fout<<rows.second;
		fout<<endl;
	}
	fout<<decor2<<endl;
	return; 
}

void print_stack(stack<char> st){
	string spc("");
	int count=0;
	for(int i=1;i<=16;i++) spc+=' ';
	while(!st.empty()){
			gout<<st.top();
			st.pop();
			count++;
		}
		spc=spc.substr(0,18-count);
		gout<<spc;
		return ;
}

void sql_parse(){
	stack<char> parser_stack;
	parser_stack.push('$');
	parser_stack.push(start);
	char X= parser_stack.top();
	char a =97+(char)sql_lex();
	string decor1(""),decor2("");
	for(int i=0;i<55;i++) decor1+='=';
		for(int i=0;i<55;i++) decor2+='-';
	gout<<decor1<<endl<<" 		STACK  		  INPUT   	      ACTION"<<endl<<decor1<<endl;		
	while(X != '$'){
		gout<<"\t\t";
		print_stack(parser_stack);
		gout<<a<<"\t\t\t";
		pair<char,char> pr;
		pr.first=X;
		pr.second=a;		
		if(X == a){
				gout<<"match "<<a<<endl;
				parser_stack.pop();
				token t=sql_lex();
				a=97+(char)t;
		}

		else if( is_terminal(X) ){
			sql_error("parsing error");
		}

		else if( pTbl.find(pr) == pTbl.end()){
			sql_error("parsing error");
		}
		else{
			
			gout<<X<<" -> "<<pTbl[pr]<<endl;
			parser_stack.pop();
			
			if(pTbl[pr]!="#"){
				for(int i=pTbl[pr].length() -1 ; i>=0 ; i--){
						parser_stack.push(pTbl[pr][i]);
				}
			}		
		}
		X=parser_stack.top();	
	}
	gout<<"\t\t";
	print_stack(parser_stack);
	gout<<'$'<<"\t\t\t"<<"accept"<<endl;
	gout<<decor2<<endl;
	cout<<"Parsing complete , no errors to show"<<endl;
	return;
}

void setup_parser(){
	vector<string> r_prods;
	string temp; 
	ifstream fin("CFG.txt");
		while(true){
		r_prods.clear();
		getline(fin,temp);
		if(temp == "end") break;
		int i=0,j;
		while(i<temp.length()){
			if(i==0){
				if(non_terminals.find(temp[0]) == non_terminals.end()){
					if(non_terminals.empty()) start=temp[0];
					non_terminals.insert(temp[0]);
					}
				i++;	
			}		 
			else if(temp[i] == '=') i++;
			else{
				j=i;
				while(j<temp.length() && temp[j]!='|') j++ ;
				r_prods.push_back(temp.substr(i,j-i));
				i=j+1;
			}
		}
		p[temp[0]]=r_prods;
	}

	remove_left_recursion();
	print_productions();
	
	for(auto rows: p) non_terminals.insert(rows.first);
	for(auto rows : p){
		for(auto prds : rows.second){
			for(int i=0;i<prds.length();i++){
				if(non_terminals.find(prds[i]) == non_terminals.end()){
					if(terminals.find(prds[i])==terminals.end()) terminals.insert(prds[i]);
				}
			}
		}
	}

	print_term_non_terms();

	for(auto ts:terminals){
				set<char> s;
				s.insert(ts);
				first[ts] =s;    
			}
	
	bool done=false;

	while(!done){
		done = true;

		for( auto row : p){
			set<char> visited; 
			calculate_first(row.first,visited,done);
		}
	}

	set<char> s;
	s.insert('$');
	follow[start]=s;
	done=false;
	
	while(!done){
		done=true;
		for(auto row: p){
		calculate_follow(row.first,done); 
		}
	}
	
	for(auto nt:non_terminals) follow[nt].erase('#'); 
	print_first();
	print_follow();

	build_predictive_parsing_table();
	print_predictive_parsing_table();
	
	return ;
}

