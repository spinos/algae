#include "VariableAShader.h"
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MTypeId.h> 
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>

MTypeId     VariableAShaderNode::id( 0x00026253 );
MObject		VariableAShaderNode::ahandle;
MObject     VariableAShaderNode::output;
MObject     VariableAShaderNode::outputC; 
MObject     VariableAShaderNode::outputS;        

VariableAShaderNode::VariableAShaderNode() {}
VariableAShaderNode::~VariableAShaderNode() {}

MStatus VariableAShaderNode::compute( const MPlug& plug, MDataBlock& data )
{
	
	MStatus returnStatus;
 
	if( plug == output )
	{
		float result = 1.0f;
		MDataHandle outputHandle = data.outputValue( VariableAShaderNode::output );
		outputHandle.set( result );
		data.setClean(plug);
		
	} else {
		return MS::kUnknownParameter;
	}

	return MS::kSuccess;
}

void* VariableAShaderNode::creator()
{
	return new VariableAShaderNode();
}

MStatus VariableAShaderNode::initialize()
{
	MFnNumericAttribute numAttr;
	MFnTypedAttribute tAttr;
	MStatus				stat;
	
	ahandle = tAttr.create( "handle", "hdl", MFnData::kString );
	tAttr.setStorable(true);
	addAttribute( ahandle );
	
	output = numAttr.create( "output", "out", MFnNumericData::kFloat, 0.0 );
	numAttr.setWritable(false);
	numAttr.setStorable(false);
	stat = addAttribute( output );
		if (!stat) { stat.perror("addAttribute"); return stat;}
		
	outputC = numAttr.createColor( "outputC", "outc");
	numAttr.setWritable(false);
	numAttr.setStorable(false);
	stat = addAttribute( outputC );
	
	outputS = tAttr.create( "outputS", "outs", MFnData::kString );
	tAttr.setWritable(false);
	tAttr.setStorable(false);
	addAttribute( outputS );
		
	attributeAffects( ahandle, output );
	attributeAffects( ahandle, outputC );
	attributeAffects( ahandle, outputS );

	return MS::kSuccess;
}
