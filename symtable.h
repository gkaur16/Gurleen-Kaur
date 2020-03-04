/*symtable.h*/

// 
// << Gurleen Kaur >>
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #03: symtable
// 02/12/2020
// 
// Symbol Table: a symbol table is a stack of scopes, typically used by a
// compiler to keep track of symbols in a program (functions, variables,
// types, etc.).  In most programming languages, you "enter scope" when you 
// see {, and "exit scope" when you see the corresponding }.  Example:
//
// int main()
// {              <-- enterScope()
//    int i;      <-- enter "i" into symbol table as type "int"
//    .
//    .
//    while (true)
//    {              <-- enterScope()
//       char i;     <-- enter "i" into symbol table as type "char"
//
// Notice there are two variables named "i", which is legal because
// they are in different scopes.
//

#pragma once

#include <iostream>
#include <deque>
#include <map>
#include <string>
#include <vector>

using namespace std;

template<typename KeyT, typename SymbolT>
class symtable
{
public:
  //
  // A symbol table is a stack of scopes.  Every scope has a name, and 
  // we use a map to hold the symbols in that scope.  You can *add* to
  // this class, but you must use the Name and Symbols as given to store
  // the scope name, and scope symbols, respectively.
  //
  class Scope
  {
  public:
    string              Name;
    map<KeyT, SymbolT>  Symbols;

    // constructors:
    Scope()
    {
      // default empty string and an empty map:
      Name = "x";
	  //Symbols.emplace(Name, "int");
    }

    Scope(string name)
    {
      this->Name = name;
      // empty map created by map's constructor:
      //this->Symbols = symbols.mplace(Name, "int");
    }
  };


private:
  //
  // TODO: implementation details
  //
  int currentSize = 0;      // to get the size of scopes inserted 
  deque<Scope> S;           // deque S that contains all the scopes
  int counterScope = 0;     // to get the number of scopes open


public:
  enum class ScopeOption
  {
    ALL, 
    CURRENT,
    GLOBAL
  };
  
  
  //
  // default constructor:
  //
  // Creates a new, empty symbol table.  No scope is open.
  //
  symtable()
  {
    //
    // TODO: note that member variables will have their default constructor 
    // called automatically, so there may be nothing to do here.
    //
  }//symtable()


  //
  // size
  //
  // Returns total # of symbols in the symbol table.
  //
  // Complexity: O(1)
  //
  int size() const
  {
	return currentSize;
  }//int size()


  //
  // numscopes
  //
  // Returns the # of open scopes.
  //
  // Complexity: O(1)
  //
  int numscopes() const
  {
    //
    // TODO:
    // 
    return counterScope;
  }//int numscopes()


  //
  // enterScope
  //
  // Enters a new, open scope in the symbol table, effectively "pushing" on
  // a new scope.  You must provide a name for the new scope, although
  // the name is currently used only for debugging purposes.
  //
  // NOTE: the first scope you enter is known as the GLOBAL scope, since this
  // is typically where GLOBAL symbols are stored.  
  //
  // Complexity: O(1)
  //
  void enterScope(string name)
  {
    // Push name to the front of the deque
	Scope insert(name);
	S.push_front(insert);
	counterScope++;
  }// void enterScope


  //
  // exitScope
  //
  // Exits the current open scope, discarding all symbols in this scope.
  // This effectively "pops" the symbol table so that it returns to the 
  // previously open scope.  A runtime_error is thrown if no scope is 
  // currently open.  
  //
  // Complexity: O(1)
  //
  void exitScope()
  {
    // if deque is not empty then get the currentSize by 
    // subtracting size of front scope by currentSize
    // and then pop from front of the deque and decrement counterScope
	if(!S.empty()){
		currentSize -= S.front().Symbols.size();
		S.pop_front();                             
		counterScope--;
	}
	else{
		throw runtime_error("symtable::No scope is open");
	}
  }//void exitScope()


  //
  // curScope
  //
  // Returns a copy of the current scope.  A runtime_error is thrown if
  // no scope is currently open.
  //
  // Complexity: O(N) where N is the # of symbols in the current scope
  //
  Scope curScope() const
  {
	// if deque S is not empty then return front of the deque
	if(!S.empty()){
		return S.front();
	}
	else{
		throw runtime_error("symtable::No scope is open");
	}
  }//Scope curScope()


  //
  // insert
  //
  // Inserts the (key, symbol) pair in the *current* scope.  If the key
  // already exists in the current scope, the associated symbol is replaced
  // by this new symbol.
  //
  // Complexity: O(lgN) where N is the # of symbols in current scope
  //
  void insert(KeyT key, SymbolT symbol)
  {
    // if the key is found in the deque S then make that key equal to new symbol
	if(S.front().Symbols.count(key)){
		S.front().Symbols.at(key) = symbol;
	}
	// else create a new map and input it inside the deque S and increment currentSize
	else{
		S.front().Symbols.emplace(key, symbol);
		currentSize++;
	}
  }//void insert


  //
  // lookup
  //
  // Searches the symbol table for the first (key, symbol) pair that 
  // matches the given key.  The search starts in the current scope, and 
  // proceeds "outward" to the GLOBAL scope.  If a matching (key, symbol)
  // pair is found, true is returned along with a copy of the symbol (via 
  // "symbol" reference parameter).  If not found, false is returned and
  // the "symbol" parameter is left unchanged.
  //
  // NOTE: the search can be controlled by the "option" parameter.  By 
  // default, the entire symbol table is searched as described above.
  // However, the search can also be limited to just the current scope, 
  // or just the GLOBAL scope, via the "option" parameter.
  //
  // Example:
  //   symtable<string,string>  table;
  //   string                   symbol;
  //   bool                     found;
  //   ...
  //   found = table.lookup("i", 
  //                        symbol, 
  //                        symtable<string,string>::ScopeOption::CURRENT);
  //
  // Complexity: O(SlgN) where S is the # of scopes and N is the largest #
  // of symbols in any one scope
  //
  bool lookup(KeyT key, 
              SymbolT& symbol, 
              ScopeOption option = ScopeOption::ALL) const
  {
    // if the option choose is CURRENT and key is found then store 
    // symbol of front key inside symbol and return true
	if(option == ScopeOption::CURRENT)
	{
		if(S.front().Symbols.count(key))
		{
			symbol = S.front().Symbols.at(key);
			return true;
		}//if(S.front()
    }//if(option 
	
	//if the option choose is ALL and using for loop go through all the scopes open
	//and if key is found then store symbol that index inside symbol and return true
	if(option == ScopeOption::ALL)
	{
		for(int i = 0; i <= counterScope; i++)
		{
			if(S[i].Symbols.count(key))
			{
				symbol = S[i].Symbols.at(key);
				return true;
			}//if(S[i]
		}//for(int i
	}//if(option
	
	// if the option choose is GLOBAL and key is found then store 
    // symbol of back key inside symbol and return true
	if(option == ScopeOption::GLOBAL)
	{
		if(S.back().Symbols.count(key))
		{
			symbol = S.back().Symbols.at(key);
			return true;
		}//if(S.back()
	}//if(option
    
    return false;
  }//bool lookup(


  //
  // dump
  // 
  // Dumps the contents of the symbol table to the output stream, 
  // starting with the current scope and working "outward" to the GLOBAL
  // scope.  You can dump the entire symbol table (the default), or dump
  // just the current scope or global scope; this is controlled by the 
  // "option" parameter.
  //
  // Example:
  //   symtable<string,string>  table;
  //   ...
  //   table.dump(std::cout, symtable<string,string>::ScopeOption::GLOBAL);
  //
  // Complexity: O(S*N) where S is the # of scopes and N is the largest #
  // of symbols in any one scope
  //
  void dump(ostream& output, ScopeOption option = ScopeOption::ALL) const
  {
    output << "**************************************************" << endl;
	
	// if option is ALL then display the numscopes and size in ALL
    if (option == ScopeOption::ALL){
      output << "*************** SYMBOL TABLE (ALL) ***************" << endl;
	  output << "** # of scopes: " << this->numscopes() << endl;
	  output << "** # of symbols: " << this->size() << endl;
	  
	  // for loop for going through the whole deque
	  for(int index = 0; index < S.size(); index++)
	  {
	      // output name of the scope at index and display all the contents in that scope
		  output << "** " << S.at(index).Name << " **" << endl;
		  for(pair<KeyT, SymbolT> display: S.at(index).Symbols)
		  {
			  output << display.first << ": " << display.second << endl;
		  }//for(pair<KeyT
	  }//for(int index
	}//if (option 
	
	// else if option is CURRENT then display numscopes and size in CURRENT and the name of current scope
    else if (option == ScopeOption::CURRENT){
      output << "*************** SYMBOL TABLE (CUR) ***************" << endl;
	  output << "** # of scopes: " << this->numscopes() << endl;
      output << "** # of symbols: " << this->size() << endl;
	  output << "** " << S.front().Name << " **" << endl;
	  
	  // display the content in front of the deque S
	  for(pair<KeyT, SymbolT> index: S.front().Symbols)
	  {
		  output << index.first << ": " << index.second << endl;
	  }//for(pair<KeyT
    }//else if (option
	
	// else the option is GLOBAL then display numscopes and size in GLOBAL and name of GLOBAL scope
	else{ // global:
      output << "*************** SYMBOL TABLE (GBL) ***************" << endl;
	  output << "** # of scopes: " << this->numscopes() << endl;
      output << "** # of symbols: " << this->size() << endl;
	  output << "** " << S.back().Name << " **" << endl;
	  
	  // display the content in back of the deque S
	  for(pair<KeyT, SymbolT> index: S.back().Symbols)
	  {
		  output << index.first << ": " << index.second << endl;
	  }//for(pair
	}//else{ 
	
    output << "**************************************************" << endl;
  }//void dump

};//class symtable
