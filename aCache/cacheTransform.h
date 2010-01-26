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
// Define declarations
//
#include <maya/MPxTransform.h>
#include <maya/MPxTransformationMatrix.h>
#include <maya/MTransformationMatrix.h>

#define kRockingTransformNodeID 0x8101B
#define kRockingTransformMatrixID 0x8101C

#define ReturnOnError(status)			   \
		if (MS::kSuccess != status) {	   \
			return status;				  \
		}

//
// Class declarations -- matrix and transform
//

class rockingTransformMatrix : public MPxTransformationMatrix
{
	// A really simple implementation of MPxTransformationMatrix.
	// The methods include:
	// - Two accessor methods for getting and setting the 
	// rock
	// - The virtual asMatrix() method which passes the matrix 
	// back to Maya when the command "xform -q -ws -m" is invoked

	public:
		rockingTransformMatrix();
		static void *creator();
		
		virtual MMatrix asMatrix() const;
		//virtual MMatrix	asMatrix(double percent) const;
		//virtual MMatrix	asRotateMatrix() const;
		
		// Degrees
		double	getRockInX() const;
		void	setRockInX( double rock, const MString& scene, const MString& mesh);
		
		static	MTypeId	id;
	protected:		
		typedef MPxTransformationMatrix ParentClass;
		// Degrees
		double rockXValue;
		float fm[4][4];
		float fm1[4][4];
};

class rockingTransformNode : public MPxTransform 
{
	// A really simple custom transform.
	public:
		rockingTransformNode();
		rockingTransformNode(MPxTransformationMatrix *);
		virtual ~rockingTransformNode();

		virtual MPxTransformationMatrix *createTransformationMatrix();
					
		virtual void postConstructor();

		virtual MStatus validateAndSetValue(const MPlug& plug,
					const MDataHandle& handle, const MDGContext& context);
					
		virtual void  resetTransformation (MPxTransformationMatrix *);
		virtual void  resetTransformation (const MMatrix &);
					
		// Utility for getting the related rock matrix pointer
		rockingTransformMatrix *getRockingTransformMatrix();
				
		const char* className();
		static	void * 	creator();
		static  MStatus	initialize();
		
		static	MTypeId	id;
	protected:
		// Degrees
		static	MObject aRockInX;
		static MObject ameshname;
		static MObject acachename;
		static MObject aframe;
		static MObject aminframe;
		static MObject amaxframe;
		double rockXValue;
		typedef MPxTransform ParentClass;
};

/**
 *class DegreeRadianConverter
 *{
 *	public:
 *		double degreesToRadians( double degrees );
 *		double radiansToDegrees( double radians );
 *};
 */

