//-
// ==========================================================================
// Copyright (C) 1995 - 2005 Alias Systems Corp. and/or its licensors.  All 
// rights reserved. 
// 
// The coded instructions, statements, computer programs, and/or related 
// material (collectively the "Data") in these files are provided by Alias 
// Systems Corp. ("Alias") and/or its licensors for the exclusive use of the 
// Customer (as defined in the Alias Software License Agreement that 
// accompanies this Alias software). Such Customer has the right to use, 
// modify, and incorporate the Data into other products and to distribute such 
// products for use by end-users.
//  
// THE DATA IS PROVIDED "AS IS".  ALIAS HEREBY DISCLAIMS ALL WARRANTIES 
// RELATING TO THE DATA, INCLUDING, WITHOUT LIMITATION, ANY AND ALL EXPRESS OR 
// IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE. IN NO EVENT SHALL ALIAS BE LIABLE FOR ANY DAMAGES 
// WHATSOEVER, WHETHER DIRECT, INDIRECT, SPECIAL, OR PUNITIVE, WHETHER IN AN 
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, OR IN EQUITY, 
// ARISING OUT OF ACCESS TO, USE OF, OR RELIANCE UPON THE DATA.
// ==========================================================================
//+

//
// Example custom transform:
//	This plug-in implements an example custom transform that
//	can be used to perform a rocking motion around the X axix.
//	Geometry of any rotation can be made a child of this transform
//	to demonstrate the effect.
//	The plug-in contains two pieces:
//	1. The custom transform node -- rockingTransformNode
//	2. The custom transformation matrix -- rockingTransformMatrix
//	These classes are used together in order to implement the
//	rocking motion.  Note that the rock attribute is stored outside
//	of the regular transform attributes.
//
// MEL usage:
/*
	// Create a rocking transform and make a rotated plane
	// its child.
	loadPlugin rockingTransform;
	file -f -new;
	polyPlane -w 1 -h 1 -sx 10 -sy 10 -ax 0 1 0 -tx 1 -ch 1;
	select -r pPlane1 ;
	rotate -r -ws -15 -15 -15 ;
	createNode rockingTransform;
	parent pPlane1 rockingTransform1;
	setAttr rockingTransform1.rockx 55;
*/
//

#include <maya/MPxTransform.h>
#include <maya/MPxTransformationMatrix.h>
#include <maya/MGlobal.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MIOStream.h>
#include <maya/MFnTypedAttribute.h>
#include "cacheTransform.h"
#include "../shared/TVectors.h"
#include "../shared/AHelper.h"
#include "../shared/SHelper.h"
#include "../shared/XMLUtil.h"
#ifndef M_PI
#include <math.h>
#endif

//
// Initialize our static class variables
//
MObject rockingTransformNode::ameshname;
MObject rockingTransformNode::acachename;
MObject rockingTransformNode::aframe;
MObject rockingTransformNode::aminframe;
MObject rockingTransformNode::amaxframe;
MObject rockingTransformNode::aRockInX;
MTypeId rockingTransformNode::id(0x00055055);
MTypeId rockingTransformMatrix::id(0x00021215);

//
// Implementation of our custom transformation matrix
//

//
//	Constructor for matrix
//
rockingTransformMatrix::rockingTransformMatrix()
{
	rockXValue = 0.0;
	fm[0][0] =1; fm[0][1] =0; fm[0][2] =0; fm[0][3] =0; 
	fm[1][0] =0; fm[1][1] =1; fm[1][2] =0; fm[1][3] =0; 
	fm[2][0] =0; fm[2][1] =0; fm[2][2] =1; fm[2][3] =0; 
	fm[3][0] =0; fm[3][1] =0; fm[3][2] =0; fm[3][3] =1; 
}

//
// Creator for matrix
//
void *rockingTransformMatrix::creator()
{
	return new rockingTransformMatrix();
}

//
//	Utility method for getting the rock
//	motion in the X axis
//
double rockingTransformMatrix::getRockInX() const
{
	return rockXValue;
}

//
//	Utility method for setting the rcok 
//	motion in the X axis
//
void rockingTransformMatrix::setRockInX( double rock, const MString& scene, const MString& mesh)
{
	if(scene=="" || mesh=="") return;
	double delta = rock - SHelper::safeConvertToInt(rock);
	
	std::string sbuf(scene.asChar());
	SHelper::changeFrameNumber(sbuf, SHelper::safeConvertToInt(rock));
	
	ZXMLDoc doc;
	XYZ a, b, c;
	float size;
	
	if(XMLUtil::findByNameAndType(sbuf.c_str(), mesh.asChar(), "transform", doc))
	{
		doc.getFloat3AttribByName("X", fm[0][0], fm[0][1], fm[0][2]);
		doc.getFloat3AttribByName("Y", fm[1][0], fm[1][1], fm[1][2]);
		doc.getFloat3AttribByName("Z", fm[2][0], fm[2][1], fm[2][2]);
		doc.getFloat3AttribByName("W", fm[3][0], fm[3][1], fm[3][2]);
		
		doc.free();	
	}
	else if(XMLUtil::findByNameAndType(sbuf.c_str(), mesh.asChar(), "camera", doc))
	{
		doc.getFloat3AttribByName("X", fm[0][0], fm[0][1], fm[0][2]);
		doc.getFloat3AttribByName("Y", fm[1][0], fm[1][1], fm[1][2]);
		doc.getFloat3AttribByName("Z", fm[2][0], fm[2][1], fm[2][2]);
		doc.getFloat3AttribByName("W", fm[3][0], fm[3][1], fm[3][2]);
		
		doc.free();	
	}
	else MGlobal::displayWarning(MString("cannot load ")+sbuf.c_str());
	
	if(delta >0)
	{
		SHelper::changeFrameNumber(sbuf, SHelper::safeConvertToInt(rock+1.0));
		
		char found = 0;
		if(XMLUtil::findByNameAndType(sbuf.c_str(), mesh.asChar(), "transform", doc))
		{
			found = 1;
			doc.getFloat3AttribByName("X", fm1[0][0], fm1[0][1], fm1[0][2]);
			doc.getFloat3AttribByName("Y", fm1[1][0], fm1[1][1], fm1[1][2]);
			doc.getFloat3AttribByName("Z", fm1[2][0], fm1[2][1], fm1[2][2]);
			doc.getFloat3AttribByName("W", fm1[3][0], fm1[3][1], fm1[3][2]);
			
			doc.free();	
		}
		else if(XMLUtil::findByNameAndType(sbuf.c_str(), mesh.asChar(), "camera", doc))
		{
			found = 1;
			doc.getFloat3AttribByName("X", fm1[0][0], fm1[0][1], fm1[0][2]);
			doc.getFloat3AttribByName("Y", fm1[1][0], fm1[1][1], fm1[1][2]);
			doc.getFloat3AttribByName("Z", fm1[2][0], fm1[2][1], fm1[2][2]);
			doc.getFloat3AttribByName("W", fm1[3][0], fm1[3][1], fm1[3][2]);
			
			doc.free();	
		}
		
		if(found)
		{
			a.x = fm[0][0];
			a.y = fm[0][1];
			a.z = fm[0][2];
			size = a.length();
			b.x = fm1[0][0];
			b.y = fm1[0][1];
			b.z = fm1[0][2];
			c = a + (b-a)*delta;
			c.normalize();
			c *= size;
			fm[0][0] =c.x; 
			fm[0][1] =c.y; 
			fm[0][2] =c.z; 
			
			a.x = fm[1][0];
			a.y = fm[1][1];
			a.z = fm[1][2];
			size = a.length();
			b.x = fm1[1][0];
			b.y = fm1[1][1];
			b.z = fm1[1][2];
			c = a + (b-a)*delta;
			c.normalize();
			c *= size;
			fm[1][0] =c.x; 
			fm[1][1] =c.y; 
			fm[1][2] =c.z;

			a.x = fm[2][0];
			a.y = fm[2][1];
			a.z = fm[2][2];
			size = a.length();
			b.x = fm1[2][0];
			b.y = fm1[2][1];
			b.z = fm1[2][2];
			c = a + (b-a)*delta;
			c.normalize();
			c *= size;
			fm[2][0] =c.x; 
			fm[2][1] =c.y; 
			fm[2][2] =c.z; 
			
			a.x = fm[3][0];
			a.y = fm[3][1];
			a.z = fm[3][2];
			b.x = fm1[3][0];
			b.y = fm1[3][1];
			b.z = fm1[3][2];
			c = a + (b-a)*delta;
			fm[3][0] =c.x; 
			fm[3][1] =c.y; 
			fm[3][2] =c.z;
		}	
		
	}
	//rockXValue = rock;
}

//
// This method will be used to return information to
// Maya.  Use the attributes which are outside of
// the regular transform attributes to build a new
// matrix.  This new matrix will be passed back to
// Maya.
//
MMatrix rockingTransformMatrix::asMatrix() const
{
	// Get the current transform matrix
	MMatrix m = ParentClass::asMatrix();
	// Initialize the new matrix we will calculate
	MTransformationMatrix tm( m );
	// Find the current rotation as a quaternion
	MQuaternion quat = rotation();
	// Convert the rocking value in degrees to radians
	//DegreeRadianConverter conv;
	//double newTheta = conv.degreesToRadians( getRockInX() );
	//quat.setToXAxis( newTheta );
	// Apply the rocking rotation to the existing rotation
	//tm.addRotationQuaternion( quat.x, quat.y, quat.z, quat.w, MSpace::kTransform );
	//MVector tv(0,getRockInX(),0);
	
	//fm[0][0] =1; fm[0][1] =0; fm[0][2] =0; fm[0][3] =0; 
	//fm[1][0] =0; fm[1][1] =1; fm[1][2] =0; fm[1][3] =0; 
	//fm[2][0] =0; fm[2][1] =0; fm[2][2] =1; fm[2][3] =0; 
	//fm[3][0] =0; fm[3][1] =getRockInX(); fm[3][2] =0; fm[3][3] =1; 
	MMatrix mm(fm);
	//tm.setTranslation( tv,MSpace::kTransform);
	tm = mm;
	// Let Maya know what the matrix should be
	return tm.asMatrix();
}
/*
MMatrix rockingTransformMatrix::asMatrix(double percent) const
{
	MPxTransformationMatrix m(*this);

	//	Apply the percentage to the matrix components
	MVector trans = m.translation();
	trans *= percent;
	m.translateTo( trans );
	MPoint rotatePivotTrans = m.rotatePivot();
	rotatePivotTrans = rotatePivotTrans * percent;
	m.setRotatePivot( rotatePivotTrans );
	MPoint scalePivotTrans = m.scalePivotTranslation();
	scalePivotTrans = scalePivotTrans * percent;
	m.setScalePivotTranslation( scalePivotTrans );

	//	Apply the percentage to the rotate value.  Same
	// as above + the percentage gets applied
	MQuaternion quat = rotation();
	DegreeRadianConverter conv;
	double newTheta = conv.degreesToRadians( getRockInX() );
	quat.setToXAxis( newTheta );
	m.rotateBy( quat );
	MEulerRotation eulRotate = eulerRotation();
	m.rotateTo(  eulRotate * percent, MSpace::kTransform);

	//	Apply the percentage to the scale
	MVector s(scale(MSpace::kTransform));
	s.x = 1.0 + (s.x - 1.0)*percent;
	s.y = 1.0 + (s.y - 1.0)*percent;
	s.z = 1.0 + (s.z - 1.0)*percent;
	m.scaleTo(s, MSpace::kTransform);

	return m.asMatrix();
}

MMatrix	rockingTransformMatrix::asRotateMatrix() const
{
	// To be implemented
	return ParentClass::asRotateMatrix();
}
*/

//
// Implementation of our custom transform
//

//
//	Constructor of the transform node
//
rockingTransformNode::rockingTransformNode()
: ParentClass()
{
	rockXValue = 0.0;
}

//
//	Constructor of the transform node
//
rockingTransformNode::rockingTransformNode(MPxTransformationMatrix *tm)
: ParentClass(tm)
{
	rockXValue = 0.0;
}

//
//	Post constructor method.  Have access to *this.  Node setup
//	operations that do not go into the initialize() method should go
//	here.
//
void rockingTransformNode::postConstructor()
{
	//	Make sure the parent takes care of anything it needs.
	//
	ParentClass::postConstructor();

	// 	The baseTransformationMatrix pointer should be setup properly 
	//	at this point, but just in case, set the value if it is missing.
	//
	if (NULL == baseTransformationMatrix) {
		MGlobal::displayWarning("NULL baseTransformationMatrix found!");
		baseTransformationMatrix = new MPxTransformationMatrix();
	}

	MPlug aRockInXPlug(thisMObject(), aRockInX);
}

//
//	Destructor of the rocking transform
//
rockingTransformNode::~rockingTransformNode()
{
}

//
//	Method that returns the new transformation matrix
//
MPxTransformationMatrix *rockingTransformNode::createTransformationMatrix()
{
	return new rockingTransformMatrix();
}

//
//	Method that returns a new transform node
//
void *rockingTransformNode::creator()
{
	return new rockingTransformNode();
}

//
//	Node initialize method.  We configure node
//	attributes here.  Static method so
//	*this is not available.
//
MStatus rockingTransformNode::initialize()
{
	MFnNumericAttribute numFn;
	aRockInX = numFn.create("RockInX", "rockx", MFnNumericData::kDouble, 0.0);
	numFn.setKeyable(true);
	numFn.setAffectsWorldSpace(true);
	addAttribute(aRockInX);
	
	MStatus				stat;
	MFnNumericAttribute numAttr;
	
	aframe = numAttr.create( "currentTime", "ct", MFnNumericData::kDouble, 1.0 );
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	addAttribute( aframe );
	
	aminframe = numAttr.create( "minFrame", "mnf", MFnNumericData::kInt, 1 );
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	addAttribute( aminframe );
	
	amaxframe = numAttr.create( "maxFrame", "mxf", MFnNumericData::kInt, 24 );
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	addAttribute( amaxframe );
	
	MFnTypedAttribute   stringAttr;
	acachename = stringAttr.create( "cachePath", "cp", MFnData::kString );
 	stringAttr.setStorable(true);
	stringAttr.setInternal(true);
	addAttribute( acachename );
	
	ameshname = stringAttr.create( "meshName", "mn", MFnData::kString );
 	stringAttr.setStorable(true);
	stringAttr.setInternal(true);
	addAttribute(ameshname);

	//	This is required so that the validateAndSet method is called
	//mustCallValidateAndSet(aRockInX);
	mustCallValidateAndSet(aframe);
	return MS::kSuccess;
}

//
//	Debugging method
//
const char* rockingTransformNode::className() 
{
	return "rockingTransformNode";
}

//
//	Reset transformation
//
void  rockingTransformNode::resetTransformation (const MMatrix &matrix)
{
	ParentClass::resetTransformation( matrix );
}

//
//	Reset transformation
//
void  rockingTransformNode::resetTransformation (MPxTransformationMatrix *resetMatrix )
{
	ParentClass::resetTransformation( resetMatrix );
}

//
// A very simple implementation of validAndSetValue().  No lock
// or limit checking on the rocking attribute is done in this method.
// If you wish to apply locks and limits to the rocking attribute, you
// would follow the approach taken in the rockingTransformCheck example.
// Meaning you would implement methods similar to:
//	* applyRotationLocks();
//	* applyRotationLimits();
//	* checkAndSetRotation();  
// but for the rocking attribute.  The method checkAndSetRotation()
// would be called below rather than updating the rocking attribute
// directly.
//
MStatus rockingTransformNode::validateAndSetValue(const MPlug& plug,
												const MDataHandle& handle,
												const MDGContext& context)
{
	MStatus status = MS::kSuccess;

	//	Make sure that there is something interesting to process.
	//
	if (plug.isNull())
		return MS::kFailure;

	MDataBlock block = forceCache(*(MDGContext *)&context);
	MDataHandle blockHandle = block.outputValue(plug, &status);
	ReturnOnError(status);
	
	MString cachename =  block.inputValue( acachename ).asString();
	MString meshname =  block.inputValue( ameshname ).asString();
	
/*	
	if ( plug == aRockInX )
	{
		// Update our new rock in x value
		double rockInX = handle.asDouble();
		blockHandle.set(rockInX);
		rockXValue = rockInX;
		
		// Update the custom transformation matrix to the
		// right rock value.  
		rockingTransformMatrix *ltm = getRockingTransformMatrix();
		if (ltm)
			ltm->setRockInX(rockXValue);
		else 
			MGlobal::displayError("Failed to get rock transform matrix");
			
		blockHandle.setClean();
		
		// Mark the matrix as dirty so that DG information
		// will update.
		dirtyMatrix();		
	}
*/	
	if ( plug == aframe )
	{
		// Update our new rock in x value
		double rockInX = handle.asDouble();
		blockHandle.set(rockInX);
		rockXValue = rockInX;
		
		// Update the custom transformation matrix to the
		// right rock value.  
		rockingTransformMatrix *ltm = getRockingTransformMatrix();
		if (ltm)
			ltm->setRockInX(rockXValue, cachename, meshname);
		else 
			MGlobal::displayError("Failed to get rock transform matrix");
			
		blockHandle.setClean();
		
		// Mark the matrix as dirty so that DG information
		// will update.
		dirtyMatrix();		
	}
	
	// Allow processing for other attributes
	return ParentClass::validateAndSetValue(plug, handle, context);
}

//
//	Method for returning the current rocking transformation matrix
//
rockingTransformMatrix *rockingTransformNode::getRockingTransformMatrix()
{
	rockingTransformMatrix *ltm = (rockingTransformMatrix *) baseTransformationMatrix;
	return ltm;
}

//
// Utility class
//
/**
 *double DegreeRadianConverter::degreesToRadians( double degrees )
 *{
 *	 return degrees * ( M_PI/ 180.0 );
 *}
 *double DegreeRadianConverter::radiansToDegrees( double radians )
 *{
 *	return radians * (180.0/M_PI);
 *}	

 */
 