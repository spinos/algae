/*
 *  ParseACacheCmd.h
 *  vertexCache
 *
 *  Created by zhang on 08-10-9.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <maya/MGlobal.h>
#include <maya/MPxCommand.h>
#include <maya/MPxDeformerNode.h> 
#include <maya/MPointArray.h>
#include <maya/MFloatArray.h>
#include <maya/MVectorArray.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MDagPath.h>
#include <maya/MDagPathArray.h>
#include "SLDoc.h"

class ParseACache : public MPxCommand
 {
 public:
 				ParseACache();
	virtual		~ParseACache();
 	MStatus doIt( const MArgList& args );
 	static void* creator();
	static MSyntax newSyntax();

private:
	MStatus parseArgs ( const MArgList& args );
	MObject getDirectEnsembleNode(MPlug& plg, MString& objname, MString& passname);
	void injectSurfaceStatement(MObject& node, MString& objname, MString& passname);
	MString funcOrVarNode(MObject& node, MString& objname, MString& passname, VariableList& dwnextvars);
	MString valueVarNode(MObject& node);
	int getMatchedCondition(MObject& node, char byobj, MString& name);
	
	SLDoc* _sl;
 };

