#include<bits/stdc++.h>
#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H
using namespace std; 
struct sTreeNode{										/*syntax tree node with 5 fields*/
	char alphabet;
	set<int> firstpos,lastpos,followpos;
	bool nullable;
	};
class syntaxTree{
		set<char> alphs;  								/*set of input symbols */
		string alphSeq;	   								/*string of terminal symbols in order as found in the reg ex*/	
		set<int> firstpos; 								/*firstpos of the root node , the start state of the DFA */
		map<int, set<int> > followposTable;        
		string toPostfix(const string & );  			/*function to convert the input regex to postfix notation*/
		set<int> buildTree(const string &); 			/*builds syntax tree and returns start state(firstpos of root node)*/ 
		int precedence(char );    						/*returns precedence of operators in regex*/
		void addConcat(string & );						/*adds concatenation operator where required in regex*/

	public:
		set<int> setUnion(set<int> ,set<int> );			/*	returns union of two sets*/		
		syntaxTree(string );							/*constructor*/
		void printFollowPosTable();						/*prints the followpos table*/
		set<int> getfirstpos();							/*getter function returning start state*/
		set<char> Alphs();								/*getter function returning set of input symbols*/
		string AlphSeq();								/*getter function returning symbols of the regex*/
		map<int,set<int> > getFollowPosTable();			/*getter function returning followpos table*/
	};
#endif
