/*
 *  mshUtil.h
 *  
 *
 *  Created by zhang on 08-1-12.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <maya/MGlobal.h>
#include <maya/MPxCommand.h>
#include <maya/MPxDeformerNode.h> 
#include <maya/MPointArray.h>
#include <maya/MFloatArray.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>

class mshUtil : public MPxCommand
{
public :
			mshUtil();
	virtual		~mshUtil();
 	MStatus doIt( const MArgList& args );
 	static void* creator();
	static MSyntax newSyntax();


  private:

};