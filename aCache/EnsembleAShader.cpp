#include "EnsembleAShader.h"
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MTypeId.h> 
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>

MTypeId     EnsembleAShaderNode::id( 0x00026250 );
MObject		EnsembleAShaderNode::asurface;
MObject		EnsembleAShaderNode::adisplacement;
MObject		EnsembleAShaderNode::aribbox;
MObject		EnsembleAShaderNode::aoperation;
MObject EnsembleAShaderNode::atracebias;
MObject     EnsembleAShaderNode::output;       

EnsembleAShaderNode::EnsembleAShaderNode() {}
EnsembleAShaderNode::~EnsembleAShaderNode() {}

MStatus EnsembleAShaderNode::compute( const MPlug& plug, MDataBlock& data )
{
	
	MStatus returnStatus;
 
	if( plug == output )
	{
		float result = 1.0f;
		MDataHandle outputHandle = data.outputValue( EnsembleAShaderNode::output );
		outputHandle.set( result );
		data.setClean(plug);
		
	} else {
		return MS::kUnknownParameter;
	}

	return MS::kSuccess;
}

void* EnsembleAShaderNode::creator()
{
	return new EnsembleAShaderNode();
}

MStatus EnsembleAShaderNode::initialize()
{
	MFnNumericAttribute numAttr;
	MFnTypedAttribute tAttr;
	MStatus				stat;
	
	asurface = numAttr.create( "surfaceShader", "sfs", MFnNumericData::kFloat, 0.0 );
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	addAttribute( asurface );
	
	adisplacement = numAttr.create( "displacementShader", "dls", MFnNumericData::kFloat, 0.0 );
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	addAttribute( adisplacement );
	
	aribbox = tAttr.create( "ribBox", "rbx", MFnData::kString);
	tAttr.setStorable(false);
	addAttribute( aribbox );
	
	aoperation = numAttr.create( "operation", "opt", MFnNumericData::kInt, 0 );
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	addAttribute( aoperation );
	
	atracebias = numAttr.create( "traceBias", "tbs", MFnNumericData::kFloat, 0.1 );
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	addAttribute( atracebias );
	
	output = numAttr.create( "output", "out", MFnNumericData::kFloat, 0.0 );
	numAttr.setWritable(false);
	numAttr.setStorable(false);
	stat = addAttribute( output );
		if (!stat) { stat.perror("addAttribute"); return stat;}
		
	attributeAffects( asurface, output );
	attributeAffects( adisplacement, output );
	attributeAffects( aribbox, output );
	attributeAffects( aoperation, output );
	attributeAffects( atracebias, output );

	return MS::kSuccess;
}
