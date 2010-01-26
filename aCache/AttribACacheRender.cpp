#include "AttribACacheRender.h"
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h> 
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>

MTypeId     AttribACacheRenderNode::id( 0x0002525b );
MObject		AttribACacheRenderNode::asurfacecolor;
MObject		AttribACacheRenderNode::askipidir;
MObject     AttribACacheRenderNode::output;       

AttribACacheRenderNode::AttribACacheRenderNode() {}
AttribACacheRenderNode::~AttribACacheRenderNode() {}

MStatus AttribACacheRenderNode::compute( const MPlug& plug, MDataBlock& data )
{
	
	MStatus returnStatus;
 
	if( plug == output )
	{
		float result = 1.0f;
		MDataHandle outputHandle = data.outputValue( AttribACacheRenderNode::output );
		outputHandle.set( result );
		data.setClean(plug);
		
	} else {
		return MS::kUnknownParameter;
	}

	return MS::kSuccess;
}

void* AttribACacheRenderNode::creator()
{
	return new AttribACacheRenderNode();
}

MStatus AttribACacheRenderNode::initialize()
{
	MFnNumericAttribute numAttr;
	MStatus				stat;
	
	asurfacecolor = numAttr.createColor( "surfaceColor", "cs" );
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	numAttr.setDefault(1.f, 1.f, 1.f);
	addAttribute( asurfacecolor );
	
	askipidir = numAttr.create( "skipIndirect", "skind", MFnNumericData::kBoolean, 0 );
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	addAttribute( askipidir );
	
	
	output = numAttr.create( "output", "out", MFnNumericData::kFloat, 0.0 );
	numAttr.setWritable(false);
	numAttr.setStorable(false);
	stat = addAttribute( output );
		if (!stat) { stat.perror("addAttribute"); return stat;}
		
	attributeAffects( asurfacecolor, output );
	attributeAffects( askipidir, output );
	

	return MS::kSuccess;
}
