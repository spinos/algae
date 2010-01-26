//
// Copyright (C) YiLi
// 
// File: ContextACache.cpp
//
// Dependency Graph Node: ContextACache
//
// Author: Maya Plug-in Wizard 2.0
//

#include "ContextACache.h"
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MTime.h>
#include <maya/MVector.h>
#include <maya/MMatrix.h>
#include <maya/MFnMatrixData.h>
#include <maya/MGlobal.h>

#include "../shared/AHelper.h"
#include "../shared/GHelper.h"

#ifndef __APPLE__
#include "../shared/gExtension.h"
#endif

MTypeId     ContextACache::id( 0x00025258 );
MObject     ContextACache::aclipnear;
MObject     ContextACache::aclipfar;

MObject     ContextACache::frame;
MObject     ContextACache::adensity;

MObject 	ContextACache::aviewattrib;
MObject     ContextACache::adrawtype;
MObject     ContextACache::input;

MObject ContextACache::anoise;
MObject     ContextACache::adiffuse;
MObject     ContextACache::alightposx;
MObject     ContextACache::alightposy;
MObject     ContextACache::alightposz;
MObject     ContextACache::ameanradius;
MObject     ContextACache::aoutval;
MObject     ContextACache::akkey;
MObject     ContextACache::akback;
MObject     ContextACache::acloudr;
MObject     ContextACache::acloudg;
MObject     ContextACache::acloudb;
MObject     ContextACache::alacunarity;
MObject     ContextACache::adimension;
MObject ContextACache::asaveimage;
MObject ContextACache::aresolutionx;
MObject ContextACache::aresolutiony;
MObject ContextACache::acameraname;

ContextACache::ContextACache() : f_type(0), fSaveImage(0),
fImageWidth(800), fImageHeight(600), fSupported(0),m_pShader(0)
{
	_pDesc = new ACacheContextData();
	_pDesc->enable = 1;
}

ContextACache::~ContextACache() 
{
	if(_pDesc) delete _pDesc;
	if(m_pShader) delete m_pShader;
}

MStatus ContextACache::compute( const MPlug& plug, MDataBlock& data )
//
//	Description:
//		This method computes the value of the given output plug based
//		on the values of the input attributes.
//
//	Arguments:
//		plug - the plug to compute
//		data - object that provides access to the attributes for this node
//
{
	if(plug == aoutval) {
		MStatus status;
		/*MString path =  data.inputValue( input ).asString();
		fEyeCamera = data.inputValue( acameraname ).asString();

	    double time = data.inputValue( frame ).asTime().value();
	    int minfrm = data.inputValue( aminframe ).asInt();
		f_type = data.inputValue( adrawtype ).asInt();
		fSaveImage = data.inputValue( asaveimage ).asInt();
	    
		if( time < minfrm ) time = minfrm;
		int frame_lo = time + 0.005;
		char filename[512];
		sprintf( filename, "%s.%d.pmap", path.asChar(), frame_lo );
		sprintf( exrname, "%s.%d.exr", path.asChar(), frame_lo );
		
		float kwei = data.inputValue(anoise).asFloat();
		
		
		
		kwei = data.inputValue(adiffuse).asFloat();
		
				
		kwei = data.inputValue(adensity).asFloat();
		
				
		kwei = data.inputValue(ameanradius).asFloat();
		
			
		kwei = data.inputValue(akkey).asFloat();
				
		kwei = data.inputValue(akback).asFloat();
				
		kwei = data.inputValue(alacunarity).asFloat();
				
		kwei = data.inputValue(adimension).asFloat();
		
		
		float r, g, b;
		r = data.inputValue(alightposx).asFloat();
		g = data.inputValue(alightposy).asFloat();
		b = data.inputValue(alightposz).asFloat();
		
		
		
		r = data.inputValue(acloudr).asFloat();
		g = data.inputValue(acloudg).asFloat();
		b = data.inputValue(acloudb).asFloat();
		
				
		fImageWidth = data.inputValue(aresolutionx).asInt();
		fImageHeight = data.inputValue(aresolutiony).asInt();*/
		
		_pDesc->enable = data.inputValue( adrawtype ).asInt();
		
		if(m_pShader) {
			GLSLACachePARAM param;
			param.base_r = param.base_g = param.base_b = 1.f;
			param.clip_near =  data.inputValue(aclipnear).asFloat();
			param.clip_far =  data.inputValue(aclipfar).asFloat();
			m_pShader->setParam(param);
			_pDesc->program = m_pShader->getProgram();
		}
		else _pDesc->program = 0;
		
		MFnPluginData fnPluginData;
		MObject newDataObject = fnPluginData.create(DescACacheContext::id, &status);
		
		DescACacheContext* pData = (DescACacheContext*) fnPluginData.data(&status);
		
		if(pData) pData->setDesc(_pDesc);
	
		MGlobal::displayInfo("update cache ctx desc");
		
		MDataHandle outDescHandle = data.outputValue(aoutval);
		status = outDescHandle.set(pData);

		data.setClean(plug);
		
		return MS::kSuccess;
	}
	
	return MS::kUnknownParameter;
}

void* ContextACache::creator()
//
//	Description:
//		this method exists to give Maya a way to create new objects
//      of this type. 
//
//	Return Value:
//		a new object of this type
//
{
	return new ContextACache();
}

bool ContextACache::isBounded() const
{ 
    return false;
}

MBoundingBox ContextACache::boundingBox() const
{
	MPoint corner1( -1,-1,-1 );
	MPoint corner2( 1,1,1 );
	
	
	return MBoundingBox( corner1, corner2 ); 
}

void ContextACache::draw( M3dView & view, const MDagPath & path, 
                                 M3dView::DisplayStyle style,M3dView::DisplayStatus status )
{ 	
	MDagPath camera;
	view = M3dView::active3dView();
	view.getCamera(camera);

	view.beginGL(); 
	
	int port[4];
	glGetIntegerv(GL_VIEWPORT, port);
	
	GHelper::drawLocator(0.f,0.f,0.f);
	
	if(!m_pShader) {
		m_pShader = new GLSLACache();
		if(!m_pShader->isValid()) MGlobal::displayInfo("shader failed to initialize");
	}
/*test	
	m_pShader->enable();
	glBegin(GL_TRIANGLES);
	glVertex3f(0,0,0);
	glVertex3f(1,0,0);
	glVertex3f(0,0,1);
	glEnd();
	m_pShader->disable();
	*/
	view.endGL();
}

MStatus ContextACache::initialize()
//
//	Description:
//		This method is called to create and initialize all of the attributes
//      and attribute dependencies for this node type.  This is only called 
//		once when the node type is registered with Maya.
//
//	Return Values:
//		MS::kSuccess
//		MS::kFailure
//		
{
	// This sample creates a single input float attribute and a single
	// output float attribute.
	//
    MStatus stat;
	MFnNumericAttribute nAttr;
	MFnUnitAttribute uAttr;
	
	aclipnear = nAttr.create( "nearClip", "nrc", MFnNumericData::kFloat, 1.f );
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	addAttribute( aclipnear );
	
	aclipfar = nAttr.create( "farClip", "frc", MFnNumericData::kFloat, 1000.f );
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	addAttribute( aclipfar );
	
	frame = uAttr.create("currentTime", "ct", MFnUnitAttribute::kTime, 1.0);
	uAttr.setKeyable(true);
	uAttr.setAffectsWorldSpace(true);
	uAttr.setStorable(true);
	addAttribute( frame );
	
	
	
	adensity = nAttr.create( "density", "den", MFnNumericData::kFloat, 1.0);
	nAttr.setMin(0.1);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	addAttribute( adensity );
	
	MFnTypedAttribute   stringAttr;
	input = stringAttr.create( "cachePath", "cp", MFnData::kString );
 	stringAttr.setStorable(true);
	addAttribute( input );
	
	aviewattrib = stringAttr.create( "viewAttrib", "va", MFnData::kString );
 	stringAttr.setStorable(true);
	addAttribute( aviewattrib );
	
	adrawtype = nAttr.create( "drawType", "dt", MFnNumericData::kInt, 1 );
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	addAttribute( adrawtype );
	
		
	anoise = nAttr.create( "KNoise", "knoi", MFnNumericData::kFloat, 0.5);
	nAttr.setMin(0.0);
	//nAttr.setMax(1.0);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	addAttribute(anoise);
	
	adiffuse = nAttr.create( "KDiffuse", "kdif", MFnNumericData::kFloat, 0.2);
	nAttr.setMin(0.0);
	nAttr.setMax(1.0);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	addAttribute(adiffuse);
	
	alightposx = nAttr.create( "LightPosX", "lposx", MFnNumericData::kFloat, 100.0);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	addAttribute(alightposx);
	
	alightposy = nAttr.create( "LightPosY", "lposy", MFnNumericData::kFloat, 100.0);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	addAttribute(alightposy);
	
	alightposz = nAttr.create( "LightPosZ", "lposz", MFnNumericData::kFloat, 100.0);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	addAttribute(alightposz);
	
	ameanradius = nAttr.create( "MeanRadius", "mnr", MFnNumericData::kFloat, 4.0);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	addAttribute(ameanradius);
	
	acloudr = nAttr.create( "CloudColorR", "cclr", MFnNumericData::kFloat, 0.1);
	nAttr.setMin(0.0);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	addAttribute(acloudr);
	
	acloudg = nAttr.create( "CloudColorG", "cclg", MFnNumericData::kFloat, 0.1);
	nAttr.setMin(0.0);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	addAttribute(acloudg);
	
	acloudb = nAttr.create( "CloudColorB", "cclb", MFnNumericData::kFloat, 0.1);
	nAttr.setMin(0.0);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	addAttribute(acloudb);
	
	akkey = nAttr.create( "KKey", "kky", MFnNumericData::kFloat, 1.0);
	nAttr.setMin(0.0);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	addAttribute(akkey);
	
	akback = nAttr.create( "KBack", "kbc", MFnNumericData::kFloat, 1.0);
	nAttr.setMin(0.0);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	addAttribute(akback);
	
	alacunarity = nAttr.create( "Lacunarity", "lcu", MFnNumericData::kFloat, 1.25);
	nAttr.setMin(1.25);
	nAttr.setMax(4.0);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	addAttribute(alacunarity);
	
	adimension = nAttr.create( "Dimension", "dem", MFnNumericData::kFloat, 0.0);
	nAttr.setMin(0.0);
	nAttr.setMax(3.5);
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	addAttribute(adimension);
	
	asaveimage = nAttr.create( "saveImage", "smg", MFnNumericData::kInt, 0); 
	nAttr.setStorable(true);
	addAttribute(asaveimage);
	
	aresolutionx = nAttr.create( "resolutionX", "rzx", MFnNumericData::kInt, 800); 
	nAttr.setStorable(true);
	addAttribute(aresolutionx);
	
	aresolutiony = nAttr.create( "resolutionY", "rzy", MFnNumericData::kInt, 600); 
	nAttr.setStorable(true);
	addAttribute(aresolutiony);
	
	acameraname = stringAttr.create( "cameraName", "cmn", MFnData::kString );
 	stringAttr.setStorable(true);
	addAttribute( acameraname );
	
	MFnTypedAttribute tAttr;
	
	aoutval = tAttr.create("outval", "ov", MFnData::kPlugin);
	tAttr.setStorable(false);
	tAttr.setConnectable(true);
	addAttribute(aoutval);
    
	attributeAffects( aclipnear, aoutval );
	attributeAffects( aclipfar, aoutval );
	
	attributeAffects( input, aoutval );
	attributeAffects( frame, aoutval );
	
	attributeAffects( aviewattrib, aoutval );
	attributeAffects( adrawtype, aoutval );
	
	attributeAffects( adensity, aoutval );
	attributeAffects( anoise, aoutval );
	attributeAffects( adiffuse, aoutval );
	attributeAffects( alightposx, aoutval );
	attributeAffects( alightposy, aoutval );
	attributeAffects( alightposz, aoutval );
	attributeAffects( ameanradius, aoutval );
	attributeAffects( akkey, aoutval );
	attributeAffects( akback, aoutval );
	attributeAffects( acloudr, aoutval );
	attributeAffects( acloudg, aoutval );
	attributeAffects( acloudb, aoutval );
	attributeAffects( alacunarity, aoutval );
	attributeAffects( adimension, aoutval );
	attributeAffects( asaveimage, aoutval );
	return MS::kSuccess;

}
//~: