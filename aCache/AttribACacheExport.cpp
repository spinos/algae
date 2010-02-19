#include "AttribACacheExport.h"
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h> 
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>

MTypeId     AttribACacheExportNode::id( 0x00025257 );
MObject		AttribACacheExportNode::anochange;
MObject		AttribACacheExportNode::anosubdiv;
/*MObject		AttribACacheExportNode::askipscat;
MObject		AttribACacheExportNode::askipepid;
MObject		AttribACacheExportNode::aaslight;    
MObject		AttribACacheExportNode::aklight; 
MObject		AttribACacheExportNode::anoshadow;
MObject		AttribACacheExportNode::aasghost;  */ 
MObject     AttribACacheExportNode::output;       

AttribACacheExportNode::AttribACacheExportNode() {}
AttribACacheExportNode::~AttribACacheExportNode() {}

MStatus AttribACacheExportNode::compute( const MPlug& plug, MDataBlock& data )
{
	
	MStatus returnStatus;
 
	if( plug == output )
	{
		float result = 1.0f;
		MDataHandle outputHandle = data.outputValue( AttribACacheExportNode::output );
		outputHandle.set( result );
		data.setClean(plug);
		
	} else {
		return MS::kUnknownParameter;
	}

	return MS::kSuccess;
}

void* AttribACacheExportNode::creator()
{
	return new AttribACacheExportNode();
}

MStatus AttribACacheExportNode::initialize()
{
	MFnNumericAttribute numAttr;
	MStatus				stat;
	
	anochange = numAttr.create( "noChange", "ncg", MFnNumericData::kBoolean, 0 );
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	addAttribute( anochange );
	
	anosubdiv = numAttr.create( "asSubdiv", "asd", MFnNumericData::kBoolean, 1 );
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	addAttribute( anosubdiv );
/*	
	askipepid = numAttr.create( "skipScatter", "skscat", MFnNumericData::kBoolean, 0 );
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	addAttribute( askipepid );
	
	askipscat = numAttr.create( "skipBackscatter", "skbscat", MFnNumericData::kBoolean, 0 );
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	addAttribute( askipscat );
	
	aaslight = numAttr.create( "asLightsource", "asl", MFnNumericData::kBoolean, 0 );
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	addAttribute( aaslight );
	
	aklight = numAttr.create( "lightIntensity", "li", MFnNumericData::kDouble, 2.0 );
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	addAttribute( aklight );
	
	anoshadow = numAttr.create( "castNoShadow", "cns", MFnNumericData::kBoolean, 0 );
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	addAttribute( anoshadow );
	
	aasghost = numAttr.create( "asGhost", "asg", MFnNumericData::kBoolean, 0 );
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	addAttribute( aasghost );
*/
	output = numAttr.create( "output", "out", MFnNumericData::kFloat, 0.0 );
	numAttr.setWritable(false);
	numAttr.setStorable(false);
	stat = addAttribute( output );
		if (!stat) { stat.perror("addAttribute"); return stat;}
		
	attributeAffects( anochange, output );
	attributeAffects( anosubdiv, output );
/*	attributeAffects( askipepid, output );
	attributeAffects( askipscat, output );
	attributeAffects( aaslight, output );
	attributeAffects( aklight, output );
	attributeAffects( aasghost, output );
	attributeAffects( anoshadow, output );*/

	return MS::kSuccess;
}
