/*
 *  SLDoc.h
 *  acache
 *
 *  Created by jian zhang on 2/3/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#include <string>
#include <fstream>
#include <vector>
using namespace std;

struct SLVariable
{
	string name;
	string value;
	string type;
};

typedef vector<SLVariable*>VariableList;

struct SLBlock
{
	SLBlock() {no_var=0;}
	string type;
	string name;
	string body;
	VariableList _vars;
	VariableList _extns;
	char no_var;
	
	char checkExistingExternal(string& name)
	{
		for(VariableList::iterator varit= _extns.begin(); varit != _extns.end(); ++varit) {
				if((*varit)->name == name) return 1;
		}
		return 0;
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

	void addVariable(const char* type, const char* name, const char* value);
	void addOutput(const char* type, const char* name);
	void addBlock(SLBlock* blk);
	void save();
	
	VariableList _extns;
	
	char checkExistingExternal(string& name);
	char checkExistingBlock(string& name);
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
	VariableList _outputs;
	BlockList _blocks;
};