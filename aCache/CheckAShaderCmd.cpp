/*
 *  CheckAShaderCmd.cpp
 *  vertexCache
 *
 *  Created by zhang on 08-10-9.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "CheckAShaderCmd.h"

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

  
CheckAShader::CheckAShader() {}

CheckAShader::~CheckAShader() {}

MSyntax CheckAShader::newSyntax() 
{
	MSyntax syntax;

	syntax.addFlag("-n", "-node", MSyntax::kString);
	syntax.addFlag("-nm", "-name", MSyntax::kString);
	syntax.addFlag("-t", "-type", MSyntax::kString);
	syntax.addFlag("-pl", "-paramList", MSyntax::kString);
	syntax.addFlag("-pt", "-paramType", MSyntax::kString);
	syntax.addFlag("-ej", "-externaljob", MSyntax::kString);
	syntax.addFlag("-fs", "-framestart", MSyntax::kLong );
	syntax.addFlag("-fe", "-frameend", MSyntax::kLong );
	
	syntax.enableQuery(false);
	syntax.enableEdit(false);

	return syntax;
}

MStatus CheckAShader::doIt( const MArgList& args ) 
{

	MStatus status = parseArgs( args );
	
	if( status != MS::kSuccess ) return status;
	
	MArgDatabase argData(syntax(), args);
	
	MString node_name;
	if (argData.isFlagSet("-n")) argData.getFlagArgument("-n", 0, node_name);
// get obj
	MObject opiece;
	AHelper::getNamedObject(node_name, opiece);
	
	if(opiece == MObject::kNullObj) {
		MGlobal::displayInfo( MString("cannot find ") + node_name);
		return MS::kSuccess;
	}
	
	MFnDependencyNode fnNode(opiece);
	
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
	}
	
	
	//if (argData.isFlagSet("-bc")) argData.getFlagArgument("-bc", 0, sscat);
	

	//if (argData.isFlagSet("-ec")) argData.getFlagArgument("-ec", 0, seye);
	
	//if(argData.isFlagSet("-fs")) argData.getFlagArgument("-fs", 0, istart);
	//if(argData.isFlagSet("-fe")) argData.getFlagArgument("-fe", 0, iend);
		
	//
 return MS::kSuccess;
 }

void* CheckAShader::creator() {
 return new CheckAShader;
 }
 
MStatus CheckAShader::parseArgs( const MArgList& args )
{
	// Parse the arguments.
	MStatus stat = MS::kSuccess;
	return MS::kSuccess;
}
//:~