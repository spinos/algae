/*
 *  mshUtil.cpp
 *  
 *
 *  Created by zhang on 08-1-12.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#include <maya/MItSelectionList.h>
#include <maya/MDagPath.h>
#include <maya/MFnMesh.h>
#include "mshUtil.h"
#include "../shared/TVectors.h"

mshUtil::mshUtil() {}

mshUtil::~mshUtil() {}

MStatus mshUtil::doIt( const MArgList& args ) 
{
	MString objname, mcfname;
	MArgDatabase argData(syntax(), args);
	
	MSelectionList selList;
	MGlobal::getActiveSelectionList ( selList );

	if ( selList.length() < 2 ) {
		MGlobal:: displayError ( "Not sufficient selection!" );
		return MS::kSuccess;
	}
	
	MItSelectionList iter( selList );
	
	iter.reset();
	
	MDagPath meshPath;		
	iter.getDagPath( meshPath );
	
	meshPath.extendToShape();
	
	MObject meshObj = meshPath.node();

	MString surface = meshPath.partialPathName();
	
	MStatus status;
	MFnMesh meshFn(meshPath, &status );
	
	if(!status) {
		MGlobal:: displayError ( "Not sufficient selection!" );
		return MS::kSuccess;
	}
	
	MPointArray vxa;
	meshFn.getPoints (vxa, MSpace::kWorld);
	
	MPoint cenfrom(0.f, 0.f, 0.f);
	for(unsigned i=0; i<vxa.length(); i++) cenfrom += vxa[i];
	
	cenfrom = cenfrom/(float)vxa.length();
	
	iter.next();
	iter.getDagPath( meshPath );
	
	meshPath.extendToShape();
	
	MFnMesh mesh1Fn(meshPath, &status );
	
	if(!status) {
		MGlobal:: displayError ( "Not sufficient selection!" );
		return MS::kSuccess;
	}
	
	vxa.clear();
	mesh1Fn.getPoints (vxa, MSpace::kWorld);
	
	MPoint cento(0.f, 0.f, 0.f);
	for(unsigned i=0; i<vxa.length(); i++) cento += vxa[i];
	
	cento = cento/(float)vxa.length();
	
	MVector diff = cento - cenfrom;
	
	MDoubleArray res;
	res.append(diff.x);
	res.append(diff.y);
	res.append(diff.z);
	
	setResult ( res );
	
	/*	
	if ( !argData.isFlagSet("-i") || !argData.isFlagSet("-o") ) {
		//MGlobal::displayError( "No .obj or .mcf file specified to translate! Example: mshUtil -i filename.obj -o filename.mcf" );
		return MS::kSuccess;
	}
	else {
		argData.getFlagArgument("-i", 0, objname);
		argData.getFlagArgument("-o", 0, mcfname);
		
				
		return MS::kSuccess;
	}
*/
	return MS::kSuccess;
}

void* mshUtil::creator()
{
 return new mshUtil;
}

MSyntax mshUtil::newSyntax()
{
	MSyntax syntax;

	syntax.addFlag("-i", "-input", MSyntax::kString);
	syntax.addFlag("-o", "-output", MSyntax::kString);

	syntax.enableQuery(false);
	syntax.enableEdit(false);

   return syntax;
}