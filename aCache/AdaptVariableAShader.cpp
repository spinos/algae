#include "AdaptVariableAShader.h"
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MTypeId.h> 
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>

MTypeId     AdaptVariableAShaderNode::id( 0x00026254 );
MObject		AdaptVariableAShaderNode::ancondition;
MObject		AdaptVariableAShaderNode::ainput;
MObject		AdaptVariableAShaderNode::ainputC;
MObject		AdaptVariableAShaderNode::ainputS;
MObject		AdaptVariableAShaderNode::acondition;
MObject		AdaptVariableAShaderNode::ahandle;
MObject		AdaptVariableAShaderNode::aoperation;
MObject     AdaptVariableAShaderNode::output;  
MObject     AdaptVariableAShaderNode::outputC;  
MObject     AdaptVariableAShaderNode::outputS;      

AdaptVariableAShaderNode::AdaptVariableAShaderNode() {}
AdaptVariableAShaderNode::~AdaptVariableAShaderNode() {}

MStatus AdaptVariableAShaderNode::compute( const MPlug& plug, MDataBlock& data )
{
	
	MStatus returnStatus;
 
	if( plug == output )
	{
		float result = 1.0f;
		MDataHandle outputHandle = data.outputValue( output );
		outputHandle.set( result );
		data.setClean(plug);
		
	} else {
		return MS::kUnknownParameter;
	}

	return MS::kSuccess;
}

void* AdaptVariableAShaderNode::creator()
{
	return new AdaptVariableAShaderNode();
}

MStatus AdaptVariableAShaderNode::initialize()
{
	MFnNumericAttribute numAttr;
	MFnTypedAttribute tAttr;
	MStatus				stat;
	
	ancondition = numAttr.create( "numCondition", "nct", MFnNumericData::kInt, 1);
	numAttr.setStorable(true);
	addAttribute( ancondition );
	
	ainput = numAttr.create( "input", "in", MFnNumericData::kFloat );
	numAttr.setStorable(true);
	numAttr.setArray(true);
	addAttribute( ainput );
	
	ainputC = numAttr.createColor( "inputC", "inc" );
	numAttr.setStorable(true);
	numAttr.setArray(true);
	addAttribute( ainputC );
	
	ainputS = tAttr.create( "inputS", "ins", MFnData::kString );
	tAttr.setStorable(true);
	tAttr.setArray(true);
	addAttribute( ainputS );
	
	acondition = tAttr.create( "condition", "cdt", MFnData::kString );
	tAttr.setStorable(true);
	tAttr.setArray(true);
	addAttribute( acondition );
	
	ahandle = tAttr.create( "handle", "hdl", MFnData::kString );
	tAttr.setStorable(true);
	addAttribute( ahandle );
	
	aoperation = tAttr.create( "operation", "opt", MFnData::kString );
	tAttr.setStorable(true);
	addAttribute( aoperation );
	
	output = numAttr.create( "output", "out", MFnNumericData::kFloat, 0.0 );
	numAttr.setWritable(false);
	numAttr.setStorable(false);
	addAttribute( output );
	
	outputC = numAttr.createColor( "outputC", "ouc" );
	numAttr.setWritable(false);
	numAttr.setStorable(false);
	addAttribute( outputC );
	
	outputS = tAttr.create( "outputS", "ous", MFnData::kString );
	tAttr.setWritable(false);
	tAttr.setStorable(false);
	addAttribute( outputS );
		
	attributeAffects( ainput, output );
	attributeAffects( ainputC, outputC );
	attributeAffects( ainputS, outputS );
	attributeAffects( acondition, output );
	attributeAffects( acondition, outputC );
	attributeAffects( acondition, outputS );

	return MS::kSuccess;
}
