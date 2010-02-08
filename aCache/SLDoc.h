/*
 *  SLDoc.h
 *  acache
 *
 *  Created by jian zhang on 2/3/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _SL_DOC_H
#define _SL_DOC_H
#include <string>
#include <fstream>
#include <vector>
using namespace std;

struct SLVariable
{
	string name;
	string value;
	string type;
	string access;
	
	void setDefault() 
	{
		if(type == "float") value = "0.0";
		else if(type == "color") value = "color( 0.0, 0.0, 0.0 )";
		else if(type == "string") value = "nil";
		else if(type == "vector") value = "vector( 0.0, 0.0, 0.0 )";
		else if(type == "point") value = "point( 0.0, 0.0, 0.0 )";
	}
};

typedef vector<SLVariable*>VariableList;

struct SLBlock
{
	SLBlock() {no_var=0;}
	string type;
	string name;
	string body;
	VariableList _vars;
	VariableList _args;
	char no_var;
	
	char checkExistingArgs(string& name)
	{
		for(VariableList::iterator it= _args.begin(); it != _args.end(); ++it) {
				if((*it)->name == name) return 1;
		}
		return 0;
	}
	
	void addInternal(SLVariable *var)
	{
		_vars.push_back(var);
	}
	
	void addArg(SLVariable *var)
	{
		if(!checkExistingArgs(var->name)) _args.push_back(var);
	}
};

typedef vector<SLBlock*>BlockList;

class SLDoc
{
public:
	SLDoc::SLDoc() {}
	SLDoc::~SLDoc() {}
	
	void setPath(const char* name) {_path = name;}
	void setName(const char* name) {_name = name;}
	void setType(const char* name) {_type = name;}
	void setMain(const char* name) {_main = name;}

	void addVariable(SLVariable* var);
	void addBlock(SLBlock* blk);
	void save();
	
	void addArg(SLVariable* var);

	char checkExistingArgs(string& name);
	char checkExistingBlock(string& name);
	
	VariableList _args;
private:
	string _path;
	string _name;
	string _type;
	string _main;
	ofstream _file;
	void emitSpace();
	void emitEquals();
	void emitEOL();
	void parenthesisBegin();
	void parenthesisEnd();
	void braceBegin();
	void braceEnd();
	void separate();
	
	VariableList _vars;
	
	BlockList _blocks;
};
#endif