/*
 *  TranslateAShaderCmd.cpp
 *  vertexCache
 *
 *  Created by zhang on 08-10-9.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "TranslateAShaderCmd.h"

#include <maya/MAnimControl.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MItSurfaceCV.h>
#include <maya/MItGeometry.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>
#include <maya/MItMeshEdge.h>
#include <maya/MFnMesh.h>
#include <maya/MFnMeshData.h>
#include <maya/MFnTransform.h>
#include <maya/MFnCamera.h>
#include <maya/MItSelectionList.h>

#include "../shared/AHelper.h"
#include "../shared/SHelper.h"
#include "../shared/TVectors.h"
#include "PieceASHader.h"

  
TranslateAShader::TranslateAShader() {}

TranslateAShader::~TranslateAShader() {}

MSyntax TranslateAShader::newSyntax() 
{
	MSyntax syntax;

	syntax.addFlag("-e", "-emit", MSyntax::kString);

	//syntax.setObjectType(MSyntax::kStringObjects, 0, 1);
syntax.setObjectType(MSyntax::kSelectionList, 0, 1); 
	return syntax;
}

MStatus TranslateAShader::doIt( const MArgList& args ) 
{

	MStatus status = parseArgs( args );
	
	if( status != MS::kSuccess ) return status;
	
	MArgDatabase argData(syntax(), args);
	
	MString semit;
	if (argData.isFlagSet("-e")) {
		argData.getFlagArgument("-e", 0, semit);
			
	// get obj
		MObject oviz;
		AHelper::getNamedObject(semit, oviz);
		
		if(oviz == MObject::kNullObj) {
			MGlobal::displayInfo( MString("cannot find ") + semit);
			return MS::kSuccess;
		}
		
		MString ssname("nil");
		MString spass("nil");
// find ensemble attached
		
		injectShaderStatement(oviz, ssname, spass, 0);
	}

 return MS::kSuccess;
 }

void* TranslateAShader::creator() {
 return new TranslateAShader;
 }
 
MStatus TranslateAShader::parseArgs( const MArgList& args )
{
	// Parse the arguments.
	MStatus stat = MS::kSuccess;
	return MS::kSuccess;
}

MObject TranslateAShader::getDirectEnsembleNode(MPlug& plg, MString& objname, MString& passname)
{
	MStatus status;
		MObject oconn;
		AHelper::getConnectedNode(oconn, plg);
		if(oconn == MObject::kNullObj) return oconn;
		
		MFnDependencyNode fnode(oconn);
		
		if(fnode.typeName() == "aShaderEnsemble") return oconn;
		else if(fnode.typeName() == "aShaderAdaptEnsemble") {

			char byobj = 0;
			if(fnode.findPlug("handle").asString() != "context") byobj = 1; 
			
			int match_sit = 0;
			
			if(byobj) match_sit = getMatchedCondition(oconn, objname);
			else match_sit = getMatchedCondition(oconn, passname);

// get matched plug
			MPlug plgens = fnode.findPlug("ensemble").elementByLogicalIndex( match_sit, &status);
			if(status) {
				return getDirectEnsembleNode(plgens, objname, passname);
			}
			else {
				MGlobal::displayInfo("error on get ensemble");
				return MObject::kNullObj;
			}
		}
		else return MObject::kNullObj;
}

void TranslateAShader::injectShaderStatement(MObject& node, MString& objname, MString& passname, int type)
{
	if(node == MObject::kNullObj) return;
// shader and scene path
	MString proj;
	MGlobal::executeCommand( MString ("string $p = `workspace -q -fn`"), proj );
	MString scn;
	MGlobal::executeCommand( MString ("string $p = `file -q -sceneName`"), scn );
	string sscn = scn.asChar();
	SHelper::filenameWithoutPath(sscn);
	MString shader_path = proj+"/rmanshaders/"+sscn.c_str()+"/";
	
	MString pathcmd = MString("system(\"mkdir ")+shader_path+"\")";
//MGlobal::displayInfo(pathcmd);
	MGlobal::executeCommand(pathcmd);
	
	MFnDependencyNode fnode(node);
	if(fnode.typeName() == "aShaderPiece") {
		PieceAShaderNode* pnode = (PieceAShaderNode*) fnode.userNode();
		XRSLPiece *ppiece = pnode->getPiece();
		if(ppiece) {
// write sl
			_sl = new SLDoc();
			_sl->setPath(shader_path.asChar());
			_sl->setName(fnode.name().asChar());
			_sl->setType("surface");
			_sl->setMain(ppiece->getBody());
			
// variables, if no connections, use values from node
			ParamList params = ppiece->getAttrib();
			for(ParamList::iterator it= params.begin(); it != params.end(); ++it) {
				
				SLVariable* var = new SLVariable();
				var->name = (*it)->name;
				convertType((*it)->type, var);
				var->setDefault();
				
				if((*it)->access != Output) {

					MPlug pgattr = fnode.findPlug((*it)->name.c_str());
					
					MObject oconn;
					AHelper::getConnectedNode(oconn, pgattr);
					if(oconn == MObject::kNullObj) valueFromPieceNode((*it)->type, (*it)->name.c_str(), pgattr, node, var);
					else {
// set by a func or var, collect uniform varying output
						VariableList uniformvar;
						MString funcname = funcOrVarNode(oconn, objname, passname, uniformvar);
						
						var->value = funcname.asChar();

						for(VariableList::iterator varit= uniformvar.begin(); varit != uniformvar.end(); ++varit)
							_sl->addArg(*varit);
					}
				}
// add var to main
				switch((*it)->access) {
					case Uniform:
						var->access = "uniform";
						_sl->addArg(var);
						break;
					case Varying:
						var->access = "varying";
						_sl->addArg(var);
						break;
					case Output:
						var->access = "output varying";
						_sl->addArg(var);
						break;
					default:
						var->access = "internal";
						_sl->addVariable(var);
						break;
				}
			}
			
			_sl->save();

			MString slog;

// compile sl
			slog = MString("system(\"shaderdl -o "+ shader_path + "/" + fnode.name() + ".sdl "+shader_path + "/" + fnode.name() +".sl\")");
			MGlobal::displayInfo(MString("compile ")+fnode.name());
			MGlobal::executeCommand(slog);
		}
	}
}

MString TranslateAShader::funcOrVarNode(MObject& node, MString& objname, MString& passname, VariableList& downstreamargs)
{
	MStatus status;
	MString res;
	MFnDependencyNode fnode(node);
	//char valbuf[256];
	if(fnode.typeName() == "aShaderPiece") {
		PieceAShaderNode* pnode = (PieceAShaderNode*) fnode.userNode();
		XRSLPiece *ppiece = pnode->getPiece();
		if(ppiece) {
			
			SLBlock *block = new SLBlock();
			block->name = fnode.name().asChar();
			block->type = ppiece->getType();
			block->body = ppiece->getBody();
// block variables
			// variables, if no connections, use values from node
			
			ParamList params = ppiece->getAttrib();
			for(ParamList::iterator it= params.begin(); it != params.end(); ++it) {
				SLVariable * var = new SLVariable();
				var->name = (*it)->name.c_str();
				
				convertType((*it)->type, var);
				var->setDefault();
				
				if((*it)->access != Output) {
// standard
					MPlug pgattr = fnode.findPlug((*it)->name.c_str());
					MObject oconn;
					AHelper::getConnectedNode(oconn, pgattr);
					if(oconn == MObject::kNullObj) valueFromPieceNode((int)(*it)->type, (*it)->name.c_str(), pgattr, node, var);
					else {
// set by a func or var
						VariableList uniformvar;
						MString funcname = funcOrVarNode(oconn, objname, passname, uniformvar);
						
						var->value = funcname.asChar();
	
						for(VariableList::iterator varit= uniformvar.begin(); varit != uniformvar.end(); ++varit) {
							block->addArg(*varit);
						}
					}
				}
				
// add var to sl block
				switch((*it)->access) {
					case Uniform:
						var->access = "uniform";
						block->addArg(var);
						break;
					case Varying:
						var->access = "varying";
						block->addArg(var);
						break;
					case Output:
						var->access = "output varying";
						block->addArg(var);
						break;
					default:
						var->access = "internal";
						block->addInternal(var);
						break;
				}
			}
			if(!_sl->checkExistingBlock(block->name)) _sl->addBlock(block);
			res = fnode.name() + " ( ";
			int i = 0;
			for(VariableList::iterator varit= block->_args.begin(); varit != block->_args.end(); ++varit) {
				res = res + (*varit)->name.c_str();
				i++;
				if(i < block->_args.size()) res = res + ", ";
			}
			res = res + " )";
			
			// update external vars
			downstreamargs = block->_args;
		}
	}
	else if(fnode.typeName() == "aShaderVariable") {
// is a variable
		SLVariable *var = new SLVariable();

		int access = 0;
		valueFromVarNode(node, var, access);
		if(access==0) var->access = "uniform";
		else var->access = "varying";
		
		res = fnode.name();
		
// update external vars		
				downstreamargs.push_back(var);
	}
	else if(fnode.typeName() == "aShaderAdaptVariable") {
// is a adapt variable
		string tadp = fnode.findPlug("opt").asString().asChar();
		string tvar = fnode.findPlug("handle").asString().asChar();
		if(tadp == "combine") {
// combine vars need a block

			SLBlock *block = new SLBlock();
			block->no_var = 1;
			block->name = fnode.name().asChar();
			block->type = tvar;
			MString sattr("input");
			if(tvar == "color") sattr = "inputC";
			int nsit = fnode.findPlug("numCondition").asInt();
			for(int i=0; i<nsit; i++) {
// add var				
				MPlug plgvar = fnode.findPlug(sattr).elementByLogicalIndex( i, &status);
				if(status) {
					MObject oconn;
					AHelper::getConnectedNode(oconn, plgvar);
					if(oconn != MObject::kNullObj) { // only when it is connected
						VariableList uniformvar;
						MString funcname = funcOrVarNode(oconn, objname, passname, uniformvar);
						
						SLVariable * var = new SLVariable();
						var->type = block->type;
						var->value = funcname.asChar();
						var->name = MFnDependencyNode(oconn).name().asChar();
						//block->_extns.push_back(var);
// only have external vars	
						for(VariableList::iterator varit= uniformvar.begin(); varit != uniformvar.end(); ++varit) {
							block->addArg(*varit);
						}
						
// add var to sl block
						block->addInternal(var);

					}
				}
			}
// t res = 0
// res += var
// ...
// return res
			string sbody = block->type;
// only color and float can be combined 
			sbody += " res = ";
			if(tvar == "color") sbody += "color( 0.0, 0.0, 0.0 ); \n";
			else sbody += "0.0; \n";
			
			for(VariableList::iterator varit= block->_vars.begin(); varit != block->_vars.end(); ++varit) {
				sbody += "res += ";
				sbody += (*varit)->value;
				sbody += " ;\n";
			}
			
			sbody += "return res;\n";
			block->body = sbody;
			if(!_sl->checkExistingBlock(block->name)) _sl->addBlock(block);
			
// output func
			res = fnode.name() + " ( ";
			int i = 0;
			for(VariableList::iterator varit= block->_args.begin(); varit != block->_args.end(); ++varit) {
				res = res + (*varit)->name.c_str();
				i++;
				if(i < block->_args.size()) res = res + ", ";
			}
			res = res + " )";
			
// update external vars
			downstreamargs = block->_args;
		}
		else {
// context or objname var select as single external var
			
			MString sattr("input");
			if(tvar == "color") sattr = "inputC";
			else if(tvar == "string") sattr = "inputS";
			
			char byobj = 0; 
			if( tadp == "objname" ) byobj = 1; 
			
			int match_sit = 0;
			
			if( tadp != "objname" ) match_sit = getMatchedCondition(node, passname);
			else match_sit = getMatchedCondition(node, objname);
			
			SLVariable *var = new SLVariable();
			var->name = fnode.name().asChar();
			var->type = tvar;
			var->setDefault();
			
			res = fnode.name();

			MPlug plgvar = fnode.findPlug(sattr).elementByLogicalIndex( match_sit, &status);
			if(status) {
				MObject oconn;
				AHelper::getConnectedNode(oconn, plgvar);
				if(oconn != MObject::kNullObj) { // only when it is connected
					var->value = valueVarNode(oconn).asChar();
				}
			}
// update external vars, only uniform ones can be adaptive
			downstreamargs.push_back(var);
		}
	}
	
	return res;
}

int TranslateAShader::getMatchedCondition(MObject& node, MString& name)
{
	MFnDependencyNode fnode(node);
	int nsit = fnode.findPlug("numCondition").asInt();

	int default_sit = 0;
	int match_sit = 0;
	
	MStringArray ctxlist;
	MStatus status;
	for(int i=0; i<nsit; i++) {
		
		MPlug plgcondition = fnode.findPlug("condition").elementByLogicalIndex( i, &status);
		if(status) {
			MString scondition = plgcondition.asString();
			ctxlist.append(scondition);
// empty as default situation
			if(scondition == "") default_sit = i;
			
				if(SHelper::isInArrayDividedBySpace(name.asChar(), scondition.asChar())) {
				
					match_sit = i;
					return match_sit;
				}
		}
		else {
			MGlobal::displayInfo("error on get condition");
			return 0;
		}
	}
// use default if no match is found				
	return default_sit;
}

MString TranslateAShader::valueVarNode(MObject& node)
{
	MFnDependencyNode fnode(node);
	char valbuf[256];
	MString handle = fnode.findPlug("handle").asString();

	if(handle == "float") {
		float x =  fnode.findPlug("value").asFloat();
		sprintf(valbuf, "%f", x);
	}
	else if(handle == "color") {
		float x = fnode.findPlug("valueX").asFloat();
		float y = fnode.findPlug("valueY").asFloat();
		float z = fnode.findPlug("valueZ").asFloat();
		sprintf(valbuf, "color( %f, %f, %f )", x, y, z);				
	}
	else if(handle == "vector") {
		float x = fnode.findPlug("valueX").asFloat();
		float y = fnode.findPlug("valueY").asFloat();
		float z = fnode.findPlug("valueZ").asFloat();
		sprintf(valbuf, "vector( %f, %f, %f )", x, y, z);				
	}
	else if(handle == "point") {
		float x = fnode.findPlug("valueX").asFloat();
		float y = fnode.findPlug("valueY").asFloat();
		float z = fnode.findPlug("valueZ").asFloat();
		sprintf(valbuf, "point( %f, %f, %f )", x, y, z);				
	}
	else if(handle == "string"){
		sprintf(valbuf, "\"%s\"", fnode.findPlug("value").asString().asChar());
	}
	
	return MString(valbuf);
}

void TranslateAShader::valueFromPieceNode(int type, const char* nmattr, MPlug& pgattr, MObject& node, SLVariable* var)
{
	MFnDependencyNode fnode(node);
	float x, y, z;
	char valbuf[256];
// value from node 
	switch(type) {
		case RSLFloat:
			var->type = "float";
			x = pgattr.asFloat();
			sprintf(valbuf, "%f", x);
			break;
		case RSLColor:
			var->type = "color";
			x = fnode.findPlug(MString(nmattr)+"X").asFloat();
			y = fnode.findPlug(MString(nmattr)+"Y").asFloat();
			z = fnode.findPlug(MString(nmattr)+"Z").asFloat();
			sprintf(valbuf, "color( %f, %f, %f )", x, y, z);
			break;
		case RSLVector:
			var->type = "vector";
			x = fnode.findPlug(MString(nmattr)+"X").asFloat();
			y = fnode.findPlug(MString(nmattr)+"Y").asFloat();
			z = fnode.findPlug(MString(nmattr)+"Z").asFloat();
			sprintf(valbuf, "vector( %f, %f, %f )", x, y, z);
			break;
		case RSLPoint:
			var->type = "point";
			x = fnode.findPlug(MString(nmattr)+"X").asFloat();
			y = fnode.findPlug(MString(nmattr)+"Y").asFloat();
			z = fnode.findPlug(MString(nmattr)+"Z").asFloat();
			sprintf(valbuf, "point( %f, %f, %f )", x, y, z);
			break;
		default:
			var->type = "string";
			MString sv = pgattr.asString();
			sprintf(valbuf, "\"%s\"", sv.asChar());
			break;
	}
	var->value = valbuf;
}

void TranslateAShader::valueFromVarNode(MObject& node, SLVariable* var, int& access)
{
	MFnDependencyNode fnode(node);
	var->name = fnode.name().asChar();
	var->type = fnode.findPlug("handle").asString().asChar();
	MString saccess = fnode.findPlug("access").asString();
// no output here
	if(saccess == "uniform") access = 0;
	else access = 1;
	
	char valbuf[256];
	if(var->type == "float") {
		float x =  fnode.findPlug("value").asFloat();
		sprintf(valbuf, "%f", x);
	}
	else if(var->type == "color") {
		float x = fnode.findPlug("valueX").asFloat();
		float y = fnode.findPlug("valueY").asFloat();
		float z = fnode.findPlug("valueZ").asFloat();
		sprintf(valbuf, "color( %f, %f, %f )", x, y, z);				
	}
	else if(var->type == "vector") {
		float x = fnode.findPlug("valueX").asFloat();
		float y = fnode.findPlug("valueY").asFloat();
		float z = fnode.findPlug("valueZ").asFloat();
		sprintf(valbuf, "vector( %f, %f, %f )", x, y, z);				
	}
	else if(var->type == "point") {
		float x = fnode.findPlug("valueX").asFloat();
		float y = fnode.findPlug("valueY").asFloat();
		float z = fnode.findPlug("valueZ").asFloat();
		sprintf(valbuf, "point( %f, %f, %f )", x, y, z);				
	}
	else if(var->type == "string"){
		sprintf(valbuf, "\"%s\"", fnode.findPlug("value").asString().asChar());
	}
	var->value = valbuf;
}

void TranslateAShader::convertType(int type, SLVariable* var)
{
	switch(type) {
		case RSLFloat:
			var->type = "float";
			break;
		case RSLColor:
			var->type = "color";
			break;
		case RSLVector:
			var->type = "vector";
			break;
		case RSLPoint:
			var->type = "point";
			break;
		default:
			var->type = "string";
			break;
	}
}
//:~