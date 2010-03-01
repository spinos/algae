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
	void defRIBStat();
	void defBakeStat();
	void defTraceBias(float val);
	void defDisplaceBound(float val);
	void injectRIBStatement(MObject& node);
	void injectShaderStatement(MObject& node, MString& objname, MString& passname, int type);
	MString funcOrVarNode(MObject& node, MString& objname, MString& passname, VariableList& downstreamargs);
	MString valueVarNode(MObject& node);
	int getMatchedCondition(MObject& node, MString& name);
	void valueFromPieceNode(int type, const char* nmattr, MPlug& pgattr, MObject& node, SLVariable* var);
	void valueFromVarNode(MObject& node, SLVariable* var, int& access);
	void convertType(int type, SLVariable* var);
	void pathExpression(string& res, MString& objname);
	void getTmpPath();

	SLDoc* _sl;
	int m_iFrame;
	MString m_tmpPath;
 };

