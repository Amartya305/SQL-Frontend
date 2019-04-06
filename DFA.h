#include<bits/stdc++.h>
#include "syntaxTree.h"
#ifndef DFA_H
#define DFA_H
using namespace std;
class DFA{
		
		set<int> alph;
		map< set<int> , bool > dStates;
		set<int> startState;
		set<int> finalState;
		map< pair< set<int> , char > , set<int> > dTrans;
		void buildTransitionTable(string);	
		void printStates();
	public:	
		DFA();	
		DFA(string regex);
		void create(string regex);
		void printTransitionTable();
		bool checkMembership(const string &);
};
#endif