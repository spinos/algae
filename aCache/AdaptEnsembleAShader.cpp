#include "AdaptEnsembleAShader.h"
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MTypeId.h> 
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>

MTypeId     AdaptEnsembleAShaderNode::id( 0x00026252 );
MObject		AdaptEnsembleAShaderNode::aensemble;
MObject		AdaptEnsembleAShaderNode::acondition;
MObject		AdaptEnsembleAShaderNode::ahandle;
MObject AdaptEnsembleAShaderNode::ancondition;
MObject     AdaptEnsembleAShaderNode::output;       

AdaptEnsembleAShaderNode::AdaptEnsembleAShaderNode() {}
AdaptEnsembleAShaderNode::~AdaptEnsembleAShaderNode() {}

MStatus AdaptEnsembleAShaderNode::compute( const MPlug& plug, MDataBlock& data )
{
	
	MStatus returnStatus;
 
	if( plug == output )
	{
		float result = 1.0f;
		MDataHandle outputHandle = data.outputValue( AdaptEnsembleAShaderNode::output );
		outputHandle.set( result );
		data.setClean(plug);
		
	} else {
		return MS::kUnknownParameter;
	}

	return MS::kSuccess;
}

void* AdaptEnsembleAShaderNode::creator()
{
	return new AdaptEnsembleAShaderNode();
}

MStatus AdaptEnsembleAShaderNode::initialize()
{
	MFnNumericAttribute numAttr;
	MFnTypedAttribute tAttr;
	MStatus				stat;
	
	ancondition = numAttr.create( "numCondition", "nct", MFnNumericData::kInt, 1);
	numAttr.setStorable(true);
	addAttribute( ancondition );
	
	aensemble = numAttr.create( "ensemble", "ens", MFnNumericData::kFloat );
	numAttr.setStorable(true);
	numAttr.setArray(true);
	addAttribute( aensemble );
	
	acondition = tAttr.create( "condition", "cdt", MFnData::kString );
	tAttr.setStorable(true);
	tAttr.setArray(true);
	addAttribute( acondition );
	
	ahandle = tAttr.create( "handle", "hdl", MFnData::kString );
	tAttr.setStorable(true);
	addAttribute( ahandle );
	
	output = numAttr.create( "output", "out", MFnNumericData::kFloat, 0.0 );
	numAttr.setWritable(false);
	numAttr.setStorable(false);
	stat = addAttribute( output );
		if (!stat) { stat.perror("addAttribute"); return stat;}
		
	attributeAffects( aensemble, output );
	attributeAffects( acondition, output );

	return MS::kSuccess;
}
