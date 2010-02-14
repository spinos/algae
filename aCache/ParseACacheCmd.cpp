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

	//syntax.setObjectType(MSyntax::kStringObjects, 0, 1);
syntax.setObjectType(MSyntax::kSelectionList, 0, 1); 
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
			
// store frame
		m_iFrame = (int)fframe;
			
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
		
		int bSubd = 1;
// find render attrib node
		MPlug plgatt = fviz.findPlug("achMsg", &status);
		if(status) {
			MObject oatt;
			AHelper::getConnectedNode(oatt, plgatt);
			if(oatt != MObject::kNullObj) {
				MFnDependencyNode fatt(oatt);
				if(!fatt.findPlug("asSubdiv").asBool()) bSubd = 0;
			}
		}
		
// find ensemble attached
		MPlug plgmsg = fviz.findPlug("aensembleMsg", &status);
		if(status) {
			MString ssname = fviz.name();
			MObject oensemble = getDirectEnsembleNode(plgmsg, ssname, spass);
			if(oensemble != MObject::kNullObj) {
// default rib
				defRIBStat();
				MFnDependencyNode fens(oensemble);
// rib box first
				MPlug rbxplg = fens.findPlug("rbx", &status);
				MObject orbx;
				AHelper::getConnectedNode(orbx, rbxplg);
				injectRIBStatement(orbx);
// find surface
				MPlug surfplg = fens.findPlug("surfaceShader", &status);
				MObject osurf;
				AHelper::getConnectedNode(osurf, surfplg);
				injectShaderStatement(osurf, ssname, spass, 0);
				
// find displacement
				MPlug dispplg = fens.findPlug("displacementShader", &status);
				MObject odisp;
				AHelper::getConnectedNode(odisp, dispplg);
				injectShaderStatement(odisp, ssname, spass, 1);
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
		sprintf(argbuf, "%s %s %d %f %f %d",
						scache.asChar(), smesh.asChar(), 
						(int)fframe, fshutteropen, fshutterclose,
						bSubd);
						
		char bboxbuf[128];
		sprintf(bboxbuf, "[%f %f %f %f %f %f]", fbbox[0], fbbox[1], fbbox[2], fbbox[3], fbbox[4], fbbox[5]);
		MString sproc = MString("Procedural \\\"DynamicLoad\\\" [\\\"aCacheMeshProcedural.so\\\" \\\"") +argbuf+"\\\"] "+bboxbuf;
		MString scmd = MString("RiArchiveRecord -m \"verbatim\" -t \"") + sproc + "\\n\"";
		MGlobal::executeCommand(scmd);

	}
	/*else if(argData.isFlagSet("-ts")) {
		argData.getFlagArgument("-ts", 0, semit);
		MObject onode;
		AHelper::getNamedObject(semit, onode);
		MString ssname("nil");
		MString spass("nil");
		injectShaderStatement(onode, ssname, spass, 0);
	}
	/*else if (argData.isFlagSet("-l")) {
		
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
	}*/

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

void ParseACache::defRIBStat()
{
//Attribute "visibility" "integer transmission" [ 1 ] 
//        Attribute "shade" "string transmissionhitmode" [ "opaque" ] 
	MString slog = MString("RiArchiveRecord -m \"verbatim\" -t \"Attribute \\\"visibility\\\" \\\"integer transmission\\\" [ 1 ]\\n\"");
	MGlobal::executeCommand(slog);
	
//Attribute "shade" "string transmissionhitmode" [ "opaque" ] 
	slog = MString("RiArchiveRecord -m \"verbatim\" -t \"Attribute \\\"shade\\\" \\\"string transmissionhitmode\\\" [ \\\"opaque\\\" ]\\n\"");
	MGlobal::executeCommand(slog);
}

void ParseACache::injectRIBStatement(MObject& node)
{
	if(node == MObject::kNullObj) return;
	MStatus status;
	MFnDependencyNode fnode(node);
	if(fnode.typeName() == "aShaderVariable") {
// is a variable
		string srib = fnode.findPlug("value").asString().asChar();
		
		int start =0;
		string sline;
/*could be \r\n on windows*/
		int end = SHelper::findPartBeforeChar(srib, sline, start, '\r');
		while(end > 0) {
		
			SHelper::protectCommaFree(sline);
			MString slog = MString("RiArchiveRecord -m \"verbatim\" -t \"") + sline.c_str() + "\\n\"";
			MGlobal::executeCommand(slog);

			start = end+1;
			end = SHelper::findPartBeforeChar(srib, sline, start, '\r');
		}
	}
}

void ParseACache::injectShaderStatement(MObject& node, MString& objname, MString& passname, int type)
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
	MGlobal::executeCommand(pathcmd);
	
	MString ptc_path = proj+"/rmantmp/"+sscn.c_str()+"/";
	pathcmd = MString("system(\"mkdir ")+ ptc_path+"\")";
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
			if(type == 0) _sl->setType("surface");
			else _sl->setType("displacement");
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
					if(oconn == MObject::kNullObj) valueFromPieceNode((*it)->type, (*it)->name.c_str(), pgattr, oconn, var);
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
// rib statement
			MString slog;
		// Surface "/Users/jianzhang/Documents/maya/projects/default/3delight/untitled/shaders/OBJ/lambert1"			
			if(type == 0) slog = MString("RiArchiveRecord -m \"verbatim\" -t \"Surface \\\"") + shader_path + fnode.name() + "\\\"";
			else slog = MString("RiArchiveRecord -m \"verbatim\" -t \"Displacement \\\"") + shader_path + fnode.name() + "\\\"";
// parameter list
			MString paramlist = " ";
			for(VariableList::iterator varit= _sl->_args.begin(); varit != _sl->_args.end(); ++varit) {
				if((*varit)->access == "uniform") {
				
					paramlist = paramlist + "\\\" ";
					paramlist = paramlist + (*varit)->type.c_str();
					paramlist = paramlist + " ";
					paramlist = paramlist + (*varit)->name.c_str();
					paramlist = paramlist + "\\\" ";
					
					string sval = (*varit)->value;

					if( (*varit)->type == "color" || (*varit)->type == "vector" || (*varit)->type == "point" ) SHelper::ribthree(sval);
					else if( (*varit)->type == "string" ) {
						SHelper::protectComma(sval);
// filename expression 
						pathExpression(sval, objname);
					}
						
					paramlist = paramlist + "[" + sval.c_str() + "]";
					
					paramlist = paramlist + " ";
				}
			}
			//MGlobal::displayInfo(paramlist);
			slog = slog + paramlist + "\\n\"";
			MGlobal::executeCommand(slog);

// compile sl
			slog = MString("system(\"shaderdl -o "+ shader_path + "/" + fnode.name() + ".sdl "+shader_path + "/" + fnode.name() +".sl\")");
			MGlobal::displayInfo(MString("compile ")+fnode.name());
			MGlobal::executeCommand(slog);
		}
	}
}

MString ParseACache::funcOrVarNode(MObject& node, MString& objname, MString& passname, VariableList& downstreamargs)
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
					if(oconn == MObject::kNullObj) valueFromPieceNode((int)(*it)->type, (*it)->name.c_str(), pgattr, oconn, var);
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
			var->access = "uniform";
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

int ParseACache::getMatchedCondition(MObject& node, MString& name)
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

void ParseACache::valueFromPieceNode(int type, const char* nmattr, MPlug& pgattr, MObject& node, SLVariable* var)
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

void ParseACache::valueFromVarNode(MObject& node, SLVariable* var, int& access)
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

void ParseACache::convertType(int type, SLVariable* var)
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

void ParseACache::pathExpression(string& res, MString& objname)
{
	MString proj;
	MGlobal::executeCommand( MString ("string $p = `workspace -q -fn`"), proj );
	MString scn;
	MGlobal::executeCommand( MString ("string $p = `file -q -sceneName`"), scn );
	string sscn = scn.asChar();
	SHelper::filenameWithoutPath(sscn);
	
	MString glb_path = proj+"/rmantmp/"+sscn.c_str()+"/global";
	
	char buf[8];
	sprintf(buf, "%d", m_iFrame);
	MString sframe = buf;
	
	MString obj_path = proj+"/rmantmp/"+sscn.c_str()+"/"+objname;
	
	int ls = res.find("$S", 0);
	if(ls > 0) {
		res.erase(ls, 2);
		res.insert(ls, glb_path.asChar(), glb_path.length());
	}
	
	int lf = res.find("$F", 0);
	if(lf > 0) {
		res.erase(lf, 2);
		res.insert(lf, sframe.asChar(), sframe.length());
	}
	
	int lo = res.find("$O", 0);
	if(lo > 0) {
		res.erase(lo, 2);
		res.insert(lo, obj_path.asChar(), obj_path.length());
	}
}
//:~