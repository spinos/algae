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
	syntax.addFlag("-ic", "-icon", MSyntax::kString);
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
			if(argData.isFlagSet("-ic")) appendToResult(ppiece->getIcon());
			if(argData.isFlagSet("-pl")) {
				char valbuf[32];
				ParamList params = ppiece->getAttrib();
				for(ParamList::iterator it= params.begin(); it != params.end(); ++it) {
//5 strings each - name type value (float with min max) detail access
					appendToResult((*it)->name.c_str());
					
					/*
					if((*it)->type == RSLFloat) {
						appendToResult("double");
						sprintf(valbuf, "%f %f %f", (*it)->r, (*it)->min, (*it)->max);
						appendToResult(valbuf);
					}
					else if((*it)->type == RSLColor) {
						appendToResult("double3");
						sprintf(valbuf, "%f %f %f", (*it)->r, (*it)->g, (*it)->b);
						appendToResult(valbuf);
					}
					else if((*it)->type == RSLString) {
						appendToResult("string");
						appendToResult((*it)->v.c_str());
					}
					else if((*it)->type == RSLVector) {
						appendToResult("double3");
						sprintf(valbuf, "%f %f %f", (*it)->r, (*it)->g, (*it)->b);
						appendToResult(valbuf);
					}
					else if((*it)->type == RSLPoint) {
						appendToResult("double3");
						sprintf(valbuf, "%f %f %f", (*it)->r, (*it)->g, (*it)->b);
						appendToResult(valbuf);
					}*/
					
					switch((*it)->type) {
						case RSLColor:
							appendToResult("double3");
							sprintf(valbuf, "%f %f %f", (*it)->r, (*it)->g, (*it)->b);
							appendToResult(valbuf);
							break;
						case RSLVector:
							appendToResult("double3");
							sprintf(valbuf, "%f %f %f", (*it)->r, (*it)->g, (*it)->b);
							appendToResult(valbuf);
							break;
						case RSLPoint:
							appendToResult("double3");
							sprintf(valbuf, "%f %f %f", (*it)->r, (*it)->g, (*it)->b);
							appendToResult(valbuf);
							break;
						case RSLString:
							appendToResult("string");
							appendToResult((*it)->v.c_str());
							break;
						default:
							appendToResult("double");
							sprintf(valbuf, "%f %f %f", (*it)->r, (*it)->min, (*it)->max);
							appendToResult(valbuf);
							break;
					}
					
					/*if((*it)->detail == Simple) appendToResult("simple");
					else if((*it)->detail == Slider) appendToResult("slider");
					else if((*it)->detail == Switch) appendToResult("switch");
					else if((*it)->detail == Connection) appendToResult("connection");*/
					
					switch((*it)->detail) {
						case Slider:
							appendToResult("slider");
							break;
						case Switch:
							appendToResult("switch");
							break;
						case Connection:
							appendToResult("connection");
							break;
						default:
							appendToResult("simple");
							break;
					}
					
					switch((*it)->access) {
						case Output:
							appendToResult("output");
							break;
						case Uniform:
							appendToResult("uniform");
							break;
						case Varying:
							appendToResult("varying");
							break;
						default:
							appendToResult("internal");
							break;
					}
				}
			}
			if(argData.isFlagSet("-pt")) {
				MString paramName;
				argData.getFlagArgument("-pt", 0, paramName);
				ParamList params = ppiece->getAttrib();
				for(ParamList::iterator it= params.begin(); it != params.end(); ++it) {
					if(strcmp((*it)->name.c_str(), paramName.asChar()) == 0) {
						if((*it)->type == RSLFloat) appendToResult("double");
						else if((*it)->type == RSLColor) appendToResult("double3");
						else if((*it)->type == RSLString) appendToResult("string");
						else if((*it)->type == RSLVector) appendToResult("double3");
						else if((*it)->type == RSLPoint) appendToResult("double3");
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