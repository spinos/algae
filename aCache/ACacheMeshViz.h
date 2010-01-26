/*
 *  ACacheMeshViz.h
 *  ACacheMeshViz
 *
 *  Created by zhang on 07-10-12.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include <maya/MPxLocatorNode.h> 
#include <maya/MString.h> 
#include <maya/MTypeId.h> 
#include <maya/MPlug.h>
#include <maya/MVector.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MColor.h>
#include <maya/M3dView.h>
#include <maya/MDistance.h>
#include <maya/MFnUnitAttribute.h>

class FXMLMesh;
class RenderACache;

class ACacheMeshViz : public MPxLocatorNode
{
public:
	ACacheMeshViz();
	virtual ~ACacheMeshViz(); 

    virtual MStatus   		compute( const MPlug& plug, MDataBlock& data );

	virtual void            draw( M3dView & view, const MDagPath & path, 
								  M3dView::DisplayStyle style,
								  M3dView::DisplayStatus status );

	virtual bool            isBounded() const;
	virtual MBoundingBox    boundingBox() const;

	static  void *          creator();
	static  MStatus         initialize();
	
		// Internally cached attribute handling routines
	virtual bool getInternalValueInContext( const MPlug&,
											  MDataHandle&,
											  MDGContext&);
    virtual bool setInternalValueInContext( const MPlug&,
											  const MDataHandle&,
											  MDGContext&);

		// Attributes
	static MObject aindesc;
	static MObject ameshname;
	static MObject acachename;
	static MObject aframe;
	static MObject aminframe;
	static MObject amaxframe;
	static MObject amode;
	static MObject aoutval;
	static MTypeId id;
	
		// Cached internal values
	double m_time;
	//float m_size;
	MString m_cachename, m_meshname;
	const FXMLMesh* getMeshPointer() const { return m_pMesh; }
	
private :
	FXMLMesh* m_pMesh;
	RenderACache* m_pRender;
	bool	mAttributesChanged;
	int m_mode;
	unsigned int m_program;
};