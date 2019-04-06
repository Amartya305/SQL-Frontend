#include "DFA.h"

void DFA::buildTransitionTable(string regex){

	syntaxTree sTree(regex);
	startState = sTree.getfirstpos();
	set<char> symbols =sTree.Alphs();
	string alphSeq=sTree.AlphSeq();
	map<int, set<int> > followPos=sTree.getFollowPosTable();
	map<set<int>,bool>::iterator state;
	dStates[startState]=false;
	//cout<<alphSeq<<endl;
	//sTree.printFollowPosTable();
	while(true){
		
		for(state=dStates.begin();state!=dStates.end();state++){
			if(!state->second){
				state->second=true;
				for(auto symb:symbols){
					set<int> nextState;
					for(auto pos:state->first){
						if(alphSeq[pos-1]==symb){
							if(nextState.empty()) nextState=followPos[pos];
							else nextState=sTree.setUnion(nextState,followPos[pos]);
						}
					}
					if(!nextState.empty() && dStates.find(nextState)==dStates.end()){
							dStates[nextState]=false;
							if(nextState.find(alphSeq.length()) != nextState.end()) finalState=nextState;	
							}
					if(!nextState.empty()) {
						pair<set<int>,char> sspair;
						sspair.first = state->first;
						sspair.second=symb;
						dTrans[sspair]=nextState;
						}			
					}
					break;	
				}
			}
		if(state==dStates.end()) break;
		}
	//if(finalState.empty()) finalState=startState;	
//	printStates();	
	return;
	}
void DFA::printStates(){
	cout<<"------------------------"<<endl;
	cout<<" Start State :      ";
	for(auto pos:startState) cout<<pos<<"  ";
	cout<<endl<<" Final State :     ";
	for(auto pos:finalState) cout<<pos<<"  ";
	cout<<endl;
	cout<<"------------------------"<<endl;
	return;  
	}
void DFA::printTransitionTable(){
	cout<<"--------Transition Table------------"<<endl;
	cout<<"===================================="<<endl;
	cout<<"  STATE     SYMBOL        NEXTSTATE "<<endl;
	cout<<"===================================="<<endl;
	for(auto row:dTrans){
		pair< set<int> ,char> sspair=row.first;
		for(auto pos:sspair.first) cout<<pos<<" ";
		cout<<"\t";
		cout<<sspair.second<<"\t";
		for(auto pos:row.second){
			cout<<pos<<" ";
		}
		cout<<endl;
	}
}
DFA::DFA(){

}
DFA::DFA(string regex){
	buildTransitionTable(regex);
}
void DFA::create(string regex){
	buildTransitionTable(regex);
	return;
}
bool DFA::checkMembership(const string & exp){
	bool isMember=false;
	set<int> curState=startState;
	pair< set<int> , char > sspair;
	int i;
	for(i=0;i<exp.length();i++){
				sspair.first=curState;
				sspair.second=exp[i];
				if(dTrans.find(sspair)==dTrans.end()) break;
				else curState=dTrans[sspair];
		}
		if(i<exp.length()) return isMember;
		else if(curState!=finalState) return isMember;
		else{
			isMember=true;
			return isMember;
		}
}
/*int main(){
	string s;
	cout<<"Enter Regular Expression :";
	getline(cin,s);
	DFA d(s);
	d.printTransitionTable();
	cout<<endl<<endl<<"====================="<<endl;
	cout<<"	Simulations     "<<endl;
	cout<<"====================="<<endl;
	while(true){
		cout<<"ENter string:";
		getline(cin,s);
		cout<<endl;
		if(d.checkMembership(s)) cout<<"In the language";
		else cout<<"Not in the language";
		cout<<endl<<endl;
	}	
}*/