/*
 *  SLDoc.cpp
 *  acache
 *
 *  Created by jian zhang on 2/3/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SLDoc.h"

char SLDoc::checkExistingExternal(string& name)
{
	for(VariableList::iterator varit= _extns.begin(); varit != _extns.end(); ++varit) {
			if((*varit)->name == name) return 1;
	}
	return 0;
}

char SLDoc::checkExistingBlock(string& name)
{
	for(BlockList::iterator it= _blocks.begin(); it != _blocks.end(); ++it) {
			if((*it)->name == name) return 1;
	}
	return 0;
}

void SLDoc::addVariable(SLVariable* var)
{
	_vars.push_back(var);
}

void SLDoc::addVariable(const char* type, const char* name, const char* value)
{
	SLVariable* var = new SLVariable();
	var->type = type;
	var->name = name;
	var->value = value;
	_vars.push_back(var);
}

void SLDoc::addBlock(SLBlock* blk)
{
	_blocks.push_back(blk);
}

void SLDoc::save()
{
	string slpath = _path+_name+".sl";
	_file.open(slpath.c_str(), ios::out);
	if(!_file) return;
	
// declare blocks
	for(BlockList::iterator it= _blocks.begin(); it != _blocks.end(); ++it) {
		_file.write((*it)->type.c_str(), (*it)->type.length());
		emitSpace();
		_file.write((*it)->name.c_str(), (*it)->name.length());
		parenthesisBegin();
// declare external vars
		for(VariableList::iterator varit= (*it)->_extns.begin(); varit != (*it)->_extns.end(); ++varit) {
			_file.write((*varit)->type.c_str(), (*varit)->type.length());
			emitSpace();
			_file.write((*varit)->name.c_str(), (*varit)->name.length());
			emitEOL();
		}
		parenthesisEnd();
		braceBegin();
		if(!(*it)->no_var) {
			for(VariableList::iterator varit= (*it)->_vars.begin(); varit != (*it)->_vars.end(); ++varit) {
				_file.write((*varit)->type.c_str(), (*varit)->type.length());
				emitSpace();
				_file.write((*varit)->name.c_str(), (*varit)->name.length());
				emitEquals();
				_file.write((*varit)->value.c_str(), (*varit)->value.length());
				emitEOL();
			}
		}
		_file.write((*it)->body.c_str(), (*it)->body.length());
		braceEnd();
		separate();
	}

	_file.write(_type.c_str(), _type.length());
	emitSpace();
	_file.write(_name.c_str(), _name.length());
	parenthesisBegin();
// declare external vars
	for(VariableList::iterator it= _extns.begin(); it != _extns.end(); ++it) {
		_file.write((*it)->type.c_str(), (*it)->type.length());
		emitSpace();
		_file.write((*it)->name.c_str(), (*it)->name.length());
		emitEquals();
		_file.write((*it)->value.c_str(), (*it)->value.length());
		emitEOL();
	}
// declare output
	for(VariableList::iterator it= _outputs.begin(); it != _outputs.end(); ++it) {
		_file.write("output ", 7);
		_file.write((*it)->type.c_str(), (*it)->type.length());
		emitSpace();
		_file.write((*it)->name.c_str(), (*it)->name.length());
		emitEquals();
		_file.write((*it)->value.c_str(), (*it)->value.length());
		emitEOL();
	}
	parenthesisEnd();
	braceBegin();
// declare internal vars
	for(VariableList::iterator it= _vars.begin(); it != _vars.end(); ++it) {
		_file.write((*it)->type.c_str(), (*it)->type.length());
		emitSpace();
		_file.write((*it)->name.c_str(), (*it)->name.length());
		emitEquals();
		_file.write((*it)->value.c_str(), (*it)->value.length());
		emitEOL();
	}
	
// body
	_file.write(_main.c_str(), _main.length());
	braceEnd();
	_file.close();
}

void SLDoc::emitSpace()
{
	_file.write(" ", 1);
}

void SLDoc::parenthesisBegin()
{
	_file.write(" (\n", 3);
}

void SLDoc::parenthesisEnd()
{
	_file.write(")\n", 2);
}

void SLDoc::braceBegin()
{
	_file.write("{\n", 2);
}

void SLDoc::braceEnd()
{
	_file.write("}\n", 2);
}

void SLDoc::emitEquals()
{
	_file.write(" = ", 3);
}

void SLDoc::emitEOL()
{
	_file.write(";\n", 2);
}

void SLDoc::separate()
{
	_file.write("\n\n", 2);
}