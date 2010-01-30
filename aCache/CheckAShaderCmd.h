/*
 *  CheckAShaderCmd.h
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

class CheckAShader : public MPxCommand
 {
 public:
 				CheckAShader();
	virtual		~CheckAShader();
 	MStatus doIt( const MArgList& args );
 	static void* creator();
	static MSyntax newSyntax();

private:
	MStatus parseArgs ( const MArgList& args );
 };

