/*
 *  ParseACacheCmd.cpp
 *  vertexCache
 *
 *  Created by zhang on 08-10-9.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParseACacheCmd.h"

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

  
ParseACache::ParseACache() {}

ParseACache::~ParseACache() {}

MSyntax ParseACache::newSyntax() 
{
	MSyntax syntax;

	syntax.addFlag("-st", "-sampleTime", MSyntax::kDouble);
	syntax.addFlag("-a", "-addstep");
	syntax.addFlag("-r", "-remove");
	syntax.addFlag("-e", "-emit", MSyntax::kString);
	syntax.addFlag("-f", "-flush");
	syntax.addFlag("-c", "-contains");
	syntax.addFlag("-l", "-list" );

	syntax.setObjectType(MSyntax::kStringObjects, 0, 1);

	return syntax;
}

MStatus ParseACache::doIt( const MArgList& args ) 
{

	MStatus status = parseArgs( args );
	
	if( status != MS::kSuccess ) return status;
	
	MArgDatabase argData(syntax(), args);
	
	MString semit;
	if (argData.isFlagSet("-e")) {
		argData.getFlagArgument("-e", 0, semit);
		
	//|pSphereShape7|aCacheMesh7 delightRenderPass1 20 0 0 0 24 perspShape	

		char nodename[512];
		char passname[512];
		char cameraname[512];
		float fframe, fshutteropen, fshutterclose, ffps;
		int bshadow;
		sscanf(semit.asChar(), "%s %s %f %f %f %d %f %s", 
			nodename, passname, &fframe, &fshutteropen, &fshutterclose, &bshadow, &ffps, cameraname);
			
	// get obj
		MObject oviz;
		MString snode(nodename);
		AHelper::getNamedObject(snode, oviz);
		
		if(oviz == MObject::kNullObj) {
			//MGlobal::displayInfo( MString("cannot find ") + nodename);
			return MS::kSuccess;
		}
		
		MString spass(passname);
		
		MFnDependencyNode fviz(oviz);
		
// find ensemble attached
		MPlug plgmsg = fviz.findPlug("aensembleMsg", &status);
		if(status) {
			MString ssname = fviz.name();
			MObject oensemble = getDirectEnsembleNode(plgmsg, ssname, spass);
			if(oensemble != MObject::kNullObj) {
// find surface
				MFnDependencyNode fens(oensemble);
				MPlug surfplg = fens.findPlug("surfaceShader", &status);
				MObject osurf;
				AHelper::getConnectedNode(osurf, surfplg);
				injectSurfaceStatement(osurf, ssname, spass);
				
// find displacement
				MPlug dispplg = fens.findPlug("displacementShader", &status);
				MObject odisp;
				AHelper::getConnectedNode(odisp, dispplg);
				if(odisp != MObject::kNullObj) {
				
				}
			}
		}

	// get bbox
		float fbbox[6];
		fbbox[0] = fviz.findPlug("boundingBoxMinX").asFloat();
		fbbox[1] = fviz.findPlug("boundingBoxMaxX").asFloat();
		fbbox[2] = fviz.findPlug("boundingBoxMinY").asFloat();
		fbbox[3] = fviz.findPlug("boundingBoxMaxY").asFloat();
		fbbox[4] = fviz.findPlug("boundingBoxMinZ").asFloat();
		fbbox[5] = fviz.findPlug("boundingBoxMaxZ").asFloat();

	// get cache file	
		MString scache =  fviz.findPlug("cachePath").asString();

	// get mesh name
		MString smesh =  fviz.findPlug("meshName").asString();
		
	// get camera
		MDagPath pcam;
		MString scam(cameraname);
		AHelper::getTypedPathByName(MFn::kCamera, scam, pcam);
		MFnCamera fcam(pcam);
		
		MVector viewDir = fcam.viewDirection( MSpace::kWorld );
		MVector eyePos = fcam.eyePoint( MSpace::kWorld );
		MVector rightDir = fcam.rightDirection( MSpace::kWorld);
		MVector upDir = fcam.upDirection( MSpace::kWorld );
		
		int bpersp = 1;
		float ffov = fcam.horizontalFieldOfView();
		ffov = ffov/3.1415927f*180.f;
		if (fcam.isOrtho()) {
			bpersp = 0;
			ffov = fcam.orthoWidth();
		}
		
		char argbuf[1024];
		sprintf(argbuf, "%s %s %f %f %f",
						scache.asChar(), smesh.asChar(), 
						fframe, fshutteropen, fshutterclose);
						
		char bboxbuf[128];
		sprintf(bboxbuf, "[%f %f %f %f %f %f]", fbbox[0], fbbox[1], fbbox[2], fbbox[3], fbbox[4], fbbox[5]);
		MString sproc = MString("Procedural \\\"DynamicLoad\\\" [\\\"aCacheMeshProcedural.so\\\" \\\"") +argbuf+"\\\"] "+bboxbuf;
		MString scmd = MString("RiArchiveRecord -m \"verbatim\" -t \"") + sproc + "\\n\"";
		MGlobal::executeCommand(scmd);
	}
	if (argData.isFlagSet("-f")) {
		//MString scmd = MString("RiArchiveRecord -m \"verbatim\" -t \"# flush here \\n\"");
		//MGlobal::executeCommand(scmd);
		
		MString proj;
		MGlobal::executeCommand( MString ("string $p = `workspace -q -fn`"), proj );
		MString scn;
		MGlobal::executeCommand( MString ("string $p = `file -q -sceneName`"), scn );
		string sscn = scn.asChar();
		SHelper::filenameWithoutPath(sscn);
		MString shader_path = proj+"/rmanshaders/"+sscn.c_str();
		
		MString scmd = MString("getFileList -folder \"") + shader_path + "\" -filespec \"*.sl\"";
		string srcname;
		MStringArray srcs;
		MGlobal::executeCommand (scmd, srcs);
		for(int i=0; i<srcs.length(); i++) {
			srcname = srcs[i].asChar();
			SHelper::cutByLastDot(srcname);
// compile sl
			scmd = MString("system(\"shaderdl -o "+ shader_path + "/" + srcname.c_str() + ".sdl "+shader_path + "/" + srcname.c_str() +".sl\")");
			MGlobal::displayInfo(MString("compile ")+srcs[i]);
			MGlobal::executeCommand(scmd);
		}
	}

 return MS::kSuccess;
 }

void* ParseACache::creator() {
 return new ParseACache;
 }
 
MStatus ParseACache::parseArgs( const MArgList& args )
{
	// Parse the arguments.
	MStatus stat = MS::kSuccess;
	return MS::kSuccess;
}

MObject ParseACache::getDirectEnsembleNode(MPlug& plg, MString& objname, MString& passname)
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
			
			if(byobj) match_sit = getMatchedCondition(oconn, byobj, objname);
			else match_sit = getMatchedCondition(oconn, byobj, passname);

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

void ParseACache::injectSurfaceStatement(MObject& node, MString& objname, MString& passname)
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
			char valbuf[256];
			ParamList params = ppiece->getAttrib();
			for(ParamList::iterator it= params.begin(); it != params.end(); ++it) {
				if((*it)->detail == Output) {
// output
					if((*it)->type == Float) {
						_sl->addOutput("float", (*it)->name.c_str());
					}
					else if((*it)->type == Color) {
						_sl->addOutput("color", (*it)->name.c_str());
					}
				}
// todo varying
				else { 
// standard
					MPlug pgattr = fnode.findPlug((*it)->name.c_str());
					MObject oconn;
					AHelper::getConnectedNode(oconn, pgattr);
					if(oconn == MObject::kNullObj) { 
// no connection
						if((*it)->type == Float) {
							float x = pgattr.asFloat();
							sprintf(valbuf, "%f", x);
							_sl->addVariable("float", (*it)->name.c_str(), valbuf);
						}
						else if((*it)->type == Color) {
							float x = fnode.findPlug(MString((*it)->name.c_str())+"X").asFloat();
							float y = fnode.findPlug(MString((*it)->name.c_str())+"Y").asFloat();
							float z = fnode.findPlug(MString((*it)->name.c_str())+"Z").asFloat();
							sprintf(valbuf, "color( %f, %f, %f )", x, y, z);
							_sl->addVariable("color", (*it)->name.c_str(), valbuf);
						}
						else {
							MString sv = pgattr.asString();
							sprintf(valbuf, "\"%s\"", sv.asChar());
							_sl->addVariable("string", (*it)->name.c_str(), valbuf);
						}
					}
					else {
// set by a func or var
						VariableList extvar;
						MString funcname = funcOrVarNode(oconn, objname, passname, extvar);
						if((*it)->type == Float) {
							_sl->addVariable("float", (*it)->name.c_str(), funcname.asChar());
						}
						else if((*it)->type == Color) {
							_sl->addVariable("color", (*it)->name.c_str(), funcname.asChar());
						}
						else {
							_sl->addVariable("string", (*it)->name.c_str(), funcname.asChar());
						}
						
						for(VariableList::iterator varit= extvar.begin(); varit != extvar.end(); ++varit) {
							if(!_sl->checkExistingExternal((*varit)->name)) _sl->_extns.push_back(*varit);
						}
					}
				}
			}
			
			_sl->save();
// rib statement
			MString slog;
		// Surface "/Users/jianzhang/Documents/maya/projects/default/3delight/untitled/shaders/OBJ/lambert1"			
			slog = MString("RiArchiveRecord -m \"verbatim\" -t \"Surface \\\"") + shader_path + fnode.name() + "\\\" ";
			//MGlobal::executeCommand(slog);
// parameter list
			MString paramlist = " ";
			for(VariableList::iterator varit= _sl->_extns.begin(); varit != _sl->_extns.end(); ++varit) {
					paramlist = paramlist + "\\\" ";
					paramlist = paramlist + (*varit)->type.c_str();
					paramlist = paramlist + " ";
					paramlist = paramlist + (*varit)->name.c_str();
					paramlist = paramlist + "\\\" ";
					
					string sval = (*varit)->value;

					if( (*varit)->type == "color") SHelper::ribthree(sval);
					else if( (*varit)->type == "string" ) SHelper::protectComma(sval);
						
					paramlist = paramlist + "[" + sval.c_str() + "]";
					
					paramlist = paramlist + " ";
			}
			//MGlobal::displayInfo(paramlist);
			slog = slog + paramlist + "\\n\"";
			MGlobal::executeCommand(slog);

		}
	}
}

MString ParseACache::funcOrVarNode(MObject& node, MString& objname, MString& passname, VariableList& dwnextvars)
{
	MStatus status;
	MString res;
	MFnDependencyNode fnode(node);
	char valbuf[256];
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
// standard
					MPlug pgattr = fnode.findPlug((*it)->name.c_str());
					MObject oconn;
					AHelper::getConnectedNode(oconn, pgattr);
					if(oconn == MObject::kNullObj) { 
// no connection
						if((*it)->type == Float) {
							float x = pgattr.asFloat();
							sprintf(valbuf, "%f", x);
							
							var->type = "float";
							var->value = valbuf;
						}
						else if((*it)->type == Color) {
							float x = fnode.findPlug(MString((*it)->name.c_str())+"X").asFloat();
							float y = fnode.findPlug(MString((*it)->name.c_str())+"Y").asFloat();
							float z = fnode.findPlug(MString((*it)->name.c_str())+"Z").asFloat();
							sprintf(valbuf, "color( %f, %f, %f )", x, y, z);
							
							var->type = "color";
							var->value = valbuf;
						}
						else {
							MString sv = pgattr.asString();
							sprintf(valbuf, "\"%s\"", sv.asChar());
							
							var->type = "string";
							var->value = valbuf;
						}
						var->name = (*it)->name.c_str();
						block->_vars.push_back(var);
					}
					else {
// set by a func or var
						VariableList extvars;
						MString funcname = funcOrVarNode(oconn, objname, passname, extvars);
						
						
							if((*it)->type == Float) var->type = "float";
							else if((*it)->type == Color) var->type = "color";
							else var->type = "string";
							
							var->value = funcname.asChar();
							var->name = (*it)->name.c_str();
							block->_vars.push_back(var);
							
						for(VariableList::iterator varit= extvars.begin(); varit != extvars.end(); ++varit) {
							if(!block->checkExistingExternal((*varit)->name)) block->_extns.push_back(*varit);
						}
						
					}
			}
			if(!_sl->checkExistingBlock(block->name)) _sl->addBlock(block);
			res = fnode.name() + " ( ";
			int i = 0;
			for(VariableList::iterator varit= block->_extns.begin(); varit != block->_extns.end(); ++varit) {
				res = res + (*varit)->name.c_str();
				i++;
				if(i < block->_extns.size()) res = res + ", ";
			}
			res = res + " )";
			
			// update external vars
			dwnextvars = block->_extns;
		}
	}
	else if(fnode.typeName() == "aShaderVariable") {
// is a variable
		SLVariable *var = new SLVariable();
		var->name = fnode.name().asChar();
		var->type = fnode.findPlug("handle").asString().asChar();
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
		else {
			sprintf(valbuf, "\"%s\"", fnode.findPlug("value").asString().asChar());
		}
		var->value = valbuf;
		
		res = fnode.name();
// update external vars
		dwnextvars.push_back(var);
	}
	else if(fnode.typeName() == "aShaderAdaptVariable") {
// is a adapt variable
		string tadp = fnode.findPlug("opt").asString().asChar();
		string tvar = fnode.findPlug("handle").asString().asChar();
		if(tadp == "combine") {
// combine vars need a block

			SLBlock *block = new SLBlock();
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
						VariableList extvars;
						MString funcname = funcOrVarNode(oconn, objname, passname, extvars);
						
						SLVariable * var = new SLVariable();
						var->type = block->type;
						var->value = funcname.asChar();
						var->name = MFnDependencyNode(oconn).name().asChar();
						//block->_extns.push_back(var);
// only have external vars	
						for(VariableList::iterator varit= extvars.begin(); varit != extvars.end(); ++varit) {
							if(!block->checkExistingExternal((*varit)->name)) block->_extns.push_back(*varit);
						}
					}
				}
			}
// t res = 0
// res += var
// ...
// return res
			string sbody = block->type;
			sbody += " res = ";
			if(tvar == "color") sbody += "color( 0.0, 0.0, 0.0 ); \n";
			else sbody += "0.0; \n";
			
			for(VariableList::iterator varit= block->_extns.begin(); varit != block->_extns.end(); ++varit) {
				sbody += "res += ";
				sbody += (*varit)->name;
				sbody += " ;\n";
			}
			
			sbody += "return res;\n";
			block->body = sbody;
			if(!_sl->checkExistingBlock(block->name)) _sl->addBlock(block);
			
// output func
			res = fnode.name() + " ( ";
			int i = 0;
			for(VariableList::iterator varit= block->_extns.begin(); varit != block->_extns.end(); ++varit) {
				res = res + (*varit)->name.c_str();
				i++;
				if(i < block->_extns.size()) res = res + ", ";
			}
			res = res + " )";
			
// update external vars
			dwnextvars = block->_extns;
		}
		/*else if(tadp == "objname") {
// adapt by objname should parse all vars with switch 
			SLBlock *block = new SLBlock();
			block->name = fnode.name().asChar();
			block->type = tvar;
			
			MString sattr("input");
			if(tvar == "color") sattr = "inputC";
			else if(tvar == "string") sattr = "inputS";
			
			char byobj = 1;
			int match_sit = 0;
			match_sit = getMatchedCondition(node, byobj, objname);
			
			SLVariable * mcvar = new SLVariable();
			mcvar->type = "float";
			sprintf(valbuf, "%d", match_sit);
			mcvar->value = valbuf;
			sprintf(valbuf, "%s_use", fnode.name().asChar());
			mcvar->name = valbuf;
// external switch handle
			block->_extns.push_back(mcvar);
// for all inputs
			
			int nsit = fnode.findPlug("numCondition").asInt();
			int i;
			for(i=0; i<nsit; i++) {
// add var				
				MPlug plgvar = fnode.findPlug(sattr).elementByLogicalIndex( i, &status);
				if(status) {
					MObject oconn;
					AHelper::getConnectedNode(oconn, plgvar);
					if(oconn != MObject::kNullObj) { // only when it is connected
						VariableList extvars;
						MString funcname = funcOrVarNode(oconn, objname, passname, extvars);
						
						SLVariable * var = new SLVariable();
						var->type = block->type;
						var->value = funcname.asChar();
						var->name = MFnDependencyNode(oconn).name().asChar();
						
						block->_vars.push_back(var);
	
						for(VariableList::iterator varit= extvars.begin(); varit != extvars.end(); ++varit) {
							if(!block->checkExistingExternal((*varit)->name)) block->_extns.push_back(*varit);
						}
					}
				}
			}
// t res
// if res = 
// else if res =
// return res			
			string sbody = block->type;
			sbody += " res; \n";
			i = 0;
			for(VariableList::iterator varit= block->_vars.begin(); varit != block->_vars.end(); ++varit) {
				if(i==0) sprintf(valbuf, "if( %s_use == %d ) ", fnode.name().asChar(), i);
				else sprintf(valbuf, "else if( %s_use == %d ) ", fnode.name().asChar(), i);
				sbody += valbuf;
				sbody += "res = ";
				sbody += (*varit)->value;
				sbody += " ;\n";
				i++;
			}
			
			sbody += "return res;\n";
			block->body = sbody;
			block->no_var = 1;
			if(!_sl->checkExistingBlock(block->name)) _sl->addBlock(block);
			
// output func
			res = fnode.name() + " ( ";
			i = 0;
			for(VariableList::iterator varit= block->_extns.begin(); varit != block->_extns.end(); ++varit) {
				res = res + (*varit)->name.c_str();
				i++;
				if(i < block->_extns.size()) res = res + ", ";
			}
			res = res + " )";
			
// update external vars
			dwnextvars = block->_extns;
		}*/
		else {
// context or objname var select as single external var
			
			MString sattr("input");
			if(tvar == "color") sattr = "inputC";
			else if(tvar == "string") sattr = "inputS";
			
			char byobj = 0; 
			if( tadp == "objname" ) byobj = 1; 
			
			int match_sit = 0;
			
			if( tadp != "objname" ) match_sit = getMatchedCondition(node, byobj, passname);
			else match_sit = getMatchedCondition(node, byobj, objname);
			
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
// update external vars
			dwnextvars.push_back(var);
		}
	}
	
	return res;
}

int ParseACache::getMatchedCondition(MObject& node, char byobj, MString& name)
{
	MFnDependencyNode fnode(node);
	int nsit = fnode.findPlug("numCondition").asInt();
			
	//char found_sit = 0;
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
			
			if(!byobj) { // adapt by context
				if(scondition == name) {
					
					match_sit = i;
					return match_sit;
				}
			}
			else { // adapt by objname
				if(SHelper::isInArrayDividedBySpace(name.asChar(), scondition.asChar())) {
				
					match_sit = i;
					return match_sit;
				}
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

MString ParseACache::valueVarNode(MObject& node)
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
	else {
		sprintf(valbuf, "\"%s\"", fnode.findPlug("value").asString().asChar());
	}
	return MString(valbuf);
}
//:~