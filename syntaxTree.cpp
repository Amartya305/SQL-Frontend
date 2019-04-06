#include "syntaxTree.h"

int syntaxTree::precedence(char c){
	if(c=='*') return 3;
	if(c=='.') return 2;
	if(c=='|') return 1;
	}

void syntaxTree::addConcat(string & exp){
	unordered_set<char> delim= {'*','|','.',')'};
	int i=1;

	while(i<exp.length()){
		if( (delim.find(exp[i]) ==delim.end() )  && (delim.find(exp[i-1]) == delim.end())){
			if(exp[i-1] != '(' ){
				exp.insert(i,".");
				i+=2;
				}
			else i++;
			}
		else if(exp[i-1]=='*'){
			exp.insert(i,".");
			i+=2;	
			}
		else i++;
		//cout<<i<<endl;
		}
	return;
	}     
string syntaxTree::toPostfix(const string & infix){
	string postfix("");
	stack<char> opStack;
	unordered_set<char> ops={'*','|','.'};
	for(int i=0;i<infix.length();i++){
		if(ops.find(infix[i])==ops.end()){
			if(infix[i]=='('){
				opStack. push(infix[i]);
				}
			else if(infix[i]==')'){
			  	while(!opStack.empty() &&  opStack.top()!='('){
					postfix.push_back(opStack.top());
					opStack.pop();
					}
				if(!opStack.empty()) opStack.pop();
				}
			else 	postfix.push_back(infix[i]);
			}
		else{
		   
                   if(opStack.empty()) opStack.push(infix[i]);
		   
                   else{
			if(opStack.top()=='(') opStack.push(infix[i]);
			
			else{
				if(precedence(opStack.top()) >=precedence(infix[i])){
					while(!opStack.empty() && opStack.top()!='(' && precedence(opStack.top())>=precedence(infix[i])){
						postfix.push_back(opStack.top());
						opStack.pop(); 
						}
					
					}
				opStack.push(infix[i]);
				}
			}			
		}

		}
	while(!opStack.empty()){
		postfix.push_back(opStack.top());
		opStack.pop();
		}
	postfix.append("#.");				
	//cout<<postfix<<endl;
	return postfix;		
	}

set<int> syntaxTree::setUnion(set<int> c1,set<int> c2){
			
			for(auto itr=c2.begin(); itr !=c2.end();itr++){
				 if(c1.find(*itr) == c1.end()) c1.insert(*itr);
		  	}
			return c1;
			}
void syntaxTree::printFollowPosTable(){
		cout<<endl<<"----followposTable-------"<<endl;
		cout<<"==========================="<<endl;
		cout<<"   POS n     FOLLOWPOS(n)  "<<endl;
		cout<<"==========================="<<endl;
		//std::sort(followposTable.begin(),followposTable.end());
		for(auto row:followposTable){
			cout<<"     "<<row.first<<"     \t";
			//std::sort(row.second.begin(),row.second.end());
			for(auto j:row.second){
				cout<<j<<" ";
			}
			cout<<endl;
		}
		cout<<"---------------------------"<<endl;
		return;	
	}
set<int> syntaxTree::buildTree(const string & postfix){
	//cout<<"---------syntax tree-----------"<<endl;
	//cout<<"======================"<<endl;
	//cout<<"  CHILDREN 	 PARENT "<<endl;
	//cout<<"======================"<<endl;
	stack<sTreeNode> tree;
	unordered_set<char> ops={'*','|','.'};
	int alphCount=1;
	alphSeq="";
	for(int i=0;i<postfix.length();i++){
		
		if(ops.find(postfix[i])==ops.end()){
			alphSeq.push_back(postfix[i]);
			if(alphs.find(postfix[i])==alphs.end()) alphs.insert(postfix[i]);
			sTreeNode leaf;
			leaf.alphabet=postfix[i];
			leaf.firstpos={alphCount};
			leaf.lastpos={alphCount};
			leaf.nullable=false; 
			tree.push(leaf);
			alphCount++;
			}
		
		else{
			if(postfix[i]=='|'){
				  sTreeNode orNode;
				  sTreeNode right = tree.top();
				  tree.pop();
				  sTreeNode left =tree.top();
				  tree.pop();
				  //cout<<"   ";
				  //cout<<left.alphabet;
				  if(ops.find(left.alphabet)==ops.end()){
				  	//cout<<"{";
				  	for(auto pos:left.firstpos){
				  	//	cout<<pos;
				  	}
				  	//cout<<"}";	
				  }
				  
				  //if(ops.find(right.alphabet)==ops.end() && ops.find(left.alphabet)==ops.end()) cout<<" ";
				  //else cout<<"    ";
				  //cout<<right.alphabet;
				  if(ops.find(right.alphabet)==ops.end()){
				  	//cout<<"{";
				  	for(auto pos:right.firstpos){
				  	//	cout<<pos;
				  	}
				  	//cout<<"}";	
				  }

				  //cout<<"\t";
				  
				  


				  orNode.alphabet=postfix[i];
				  orNode.nullable= left.nullable | right.nullable; ;
				  orNode.firstpos=setUnion(left.firstpos,right.firstpos);
				  orNode.lastpos=setUnion(left.lastpos,right.lastpos);
				  tree.push(orNode);
				 // cout<<orNode.alphabet<<endl;						  	
				}
			else if(postfix[i]=='.'){
					sTreeNode catNode;
					sTreeNode right = tree.top();
				  	tree.pop();
				  	sTreeNode left =tree.top();
				  	tree.pop();

				  	catNode.alphabet=postfix[i];
					catNode.nullable=left.nullable & right.nullable;
					if(left.nullable) catNode.firstpos=setUnion(left.firstpos,right.firstpos);
					else catNode.firstpos=left.firstpos;
					if(right.nullable) catNode.lastpos=setUnion(left.lastpos,right.lastpos);
					else catNode.lastpos=right.lastpos;
					tree.push(catNode);

				  //cout<<"   ";
				  //cout<<left.alphabet;
				  if(ops.find(left.alphabet)==ops.end()){
				  	//cout<<"{";
				  	for(auto pos:left.firstpos){
				  	//	cout<<pos;
				  	}
				  //	cout<<"}";	
				  }
				  //if(ops.find(left.alphabet)==ops.end() && ops.find(right.alphabet)==ops.end()) cout<<" ";
				  //else cout<<"    ";
				  //cout<<right.alphabet;
				  if(ops.find(right.alphabet)==ops.end()){
				  	//cout<<"{";
				  	for(auto pos:right.firstpos){
				  	//	cout<<pos;
				  	}
				  	//cout<<"}";
				  }
				  //cout<<"\t";
				  //cout<<catNode.alphabet<<endl;

					for(auto itr=left.lastpos.begin();itr != left.lastpos.end();++itr){
						 if(followposTable.find(*itr) == followposTable.end()){
						 		followposTable[*itr]=right.firstpos;
						 }
						 else{
						 	followposTable[*itr]=setUnion(followposTable[*itr],right.firstpos);
						 } 
						}
					}	
			else if(postfix[i]=='*'){
					//cout<<"   ";
					sTreeNode starNode;
					sTreeNode left = tree.top();
				  	tree.pop();
				  		  //cout<<left.alphabet;
				  if(ops.find(left.alphabet)==ops.end()){
				  	//cout<<"{";
				  	for(auto pos:left.firstpos){
				  	//	cout<<pos;
				  	}
				  	//cout<<"}";	
				  }
				  //if(ops.find(left.alphabet)==ops.end()) cout<<"\t        ";
				  //else cout<<"\t\t";
				  //cout<<"\t\t";
				  

				  	starNode.alphabet=postfix[i];
					starNode.nullable=true;
					starNode.firstpos=left.firstpos;
					starNode.lastpos=left.lastpos;
					tree.push(starNode);
					//cout<<starNode.alphabet<<endl;
					for(auto itr=starNode.lastpos.begin();itr != starNode.lastpos.end();++itr){
						 if(followposTable.find(*itr) == followposTable.end()){
						 		followposTable[*itr]=starNode.firstpos;
						 }
						 else{
						 	followposTable[*itr]=setUnion(followposTable[*itr],starNode.firstpos);
						 } 
						}
					}				
				}
		}
    //followposTable[alphCount-1]={0};		
	//cout<<"---------------------"<<endl;
	return tree.top().firstpos;      
}

syntaxTree::syntaxTree(string exp){
	//addConcat(exp);
	string pfix=toPostfix(exp);
	firstpos=buildTree(pfix);
	return;
}

map<int,set<int> > syntaxTree::getFollowPosTable(){
	return followposTable;
}
set<int> syntaxTree::getfirstpos(){
	return firstpos;	
}

set<char> syntaxTree::Alphs(){
	return alphs;
}
string syntaxTree::AlphSeq(){
	return alphSeq;
}