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
			if(oensemble != MObject::kNullObj) {//MGlobal::displayInfo("attached");
			
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
/*	
	if(fnNode.typeName() == "aShaderPiece") {
	
		PieceAShaderNode* pnode = (PieceAShaderNode*) fnNode.userNode();
		XRSLPiece *ppiece = pnode->getPiece();
		if(ppiece) {
			if(argData.isFlagSet("-nm")) appendToResult(ppiece->getName());
			if(argData.isFlagSet("-t")) appendToResult(ppiece->getType());
			if(argData.isFlagSet("-pl")) {
				char valbuf[32];
				ParamList params = ppiece->getAttrib();
				for(ParamList::iterator it= params.begin(); it != params.end(); ++it) {
//4 strings - name type value (float with min max) detail
					appendToResult((*it)->name.c_str());
					if((*it)->type == Float) {
						appendToResult("double");
						sprintf(valbuf, "%f %f %f", (*it)->r, (*it)->min, (*it)->max);
						appendToResult(valbuf);
					}
					else if((*it)->type == Color) {
						appendToResult("double3");
						sprintf(valbuf, "%f %f %f", (*it)->r, (*it)->g, (*it)->b);
						appendToResult(valbuf);
					}
					else {
						appendToResult("string");
						appendToResult((*it)->v.c_str());
					}
					if((*it)->detail == Simple) appendToResult("simple");
					else if((*it)->detail == Slider) appendToResult("slider");
					else if((*it)->detail == Switch) appendToResult("switch");
					else if((*it)->detail == Connection) appendToResult("connection");
					else appendToResult("output");
				}
			}
			if(argData.isFlagSet("-pt")) {
				MString paramName;
				argData.getFlagArgument("-pt", 0, paramName);
				ParamList params = ppiece->getAttrib();
				for(ParamList::iterator it= params.begin(); it != params.end(); ++it) {
					if(strcmp((*it)->name.c_str(), paramName.asChar()) == 0) {
						if((*it)->type == Float) appendToResult("double");
						else if((*it)->type == Color) appendToResult("double3");
						else appendToResult("string");
					}
				}
			}
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
		MObject oconn;
		AHelper::getConnectedNode(oconn, plg);
		if(oconn == MObject::kNullObj) return oconn;
		
		MFnDependencyNode fnode(oconn);
		
		if(fnode.typeName() == "aShaderEnsemble") return oconn;
		else if(fnode.typeName() == "aShaderAdaptEnsemble") {

			char byobj = 0;
			if(fnode.findPlug("handle").asString() != "context") byobj = 1; 
			int nsit = fnode.findPlug("numCondition").asInt();
			char found_sit = 0;
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
						if(scondition == passname) {
							found_sit = 1;
							match_sit = i;
						}
					}
					else { // adapt by objname
						if(SHelper::isInArrayDividedBySpace(objname.asChar(), scondition.asChar())) {
						
							found_sit = 1;
							match_sit = i;
						}
					}
				}
				else {
					MGlobal::displayInfo("error on get condition");
					return MObject::kNullObj;
				}
			}
// use default if no match is found				
			if(!found_sit) match_sit = default_sit;
			
// log context 	
			MString slog;
			if(!byobj) {
				slog = MString("RiArchiveRecord -m \"verbatim\" -t \"# use context ") + ctxlist[match_sit] + "\\n\"";
				MGlobal::executeCommand(slog);
			}
			else {
				if(found_sit) slog = MString("RiArchiveRecord -m \"verbatim\" -t \"# found ") + objname + "\\n\"";
				else slog = MString("RiArchiveRecord -m \"verbatim\" -t \"# not found ") + objname + ", use default\\n\"";
				MGlobal::executeCommand(slog);
			}

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
//:~