#ifndef _ContextACache_H
#define _ContextACache_H
//
// Copyright (C) YiLi
// 
// File: ContextACache.h
//
// Dependency Graph Node: ContextACache
//
// Author: Maya Plug-in Wizard 2.0
//

#include <maya/MPxLocatorNode.h> 
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h> 
#include <maya/MPointArray.h>
#include <maya/MVectorArray.h>
#include <maya/MPoint.h>
#include <maya/MFnDagNode.h>
#include <iostream>
#include <fstream>
#include <string>
#include <maya/MFnCamera.h>
#include <maya/MDagPath.h>
#include "DescACacheContext.h"
#include "GLSLACache.h"
 
class ContextACache : public MPxLocatorNode
{
public:
						  ContextACache();
	virtual				  ~ContextACache(); 

	virtual MStatus		  compute( const MPlug& plug, MDataBlock& data );
	virtual void          draw( M3dView & view, const MDagPath & path, 
                                M3dView::DisplayStyle style,M3dView::DisplayStatus status );
    virtual bool          isBounded() const;
    virtual MBoundingBox  boundingBox() const; 

	static  void*		creator();
	static  MStatus		initialize();

public:

	static  	MTypeId		id;
	static  	MObject 	aclipnear;
	static  	MObject 	aclipfar;
	static      MObject     adrawtype;
	
	/*static  	MObject		input;
	static  	MObject		frame;

	static      MObject     adensity;
	static      MObject     anoise;
	static      MObject     adiffuse;
	static      MObject     alightposx;
	static      MObject     alightposy;
	static      MObject     alightposz;
	static  	MObject 	aviewattrib;
	static      MObject     ameanradius;
	static      MObject     acloudr;
	static      MObject     acloudg;
	static      MObject     acloudb;
	static      MObject     akkey;
	static      MObject     akback;
	static      MObject     alacunarity;
	static      MObject     adimension;
	
	static MObject asaveimage;
	static MObject aresolutionx;
	static MObject aresolutiony;
	static MObject acameraname;*/
	static MObject aoutval;
private:
	int f_type, fSaveImage;
	
	char exrname[512];
	int fImageWidth, fImageHeight;
	
	MString fEyeCamera;
	char fSupported;
	
	ACacheContextData *_pDesc;
	GLSLACache *m_pShader;
};

#endif
