#include "PieceAShader.h"
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MTypeId.h> 
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MGlobal.h>

MTypeId     PieceAShaderNode::id( 0x00026251 );
MObject		PieceAShaderNode::axslpath;
MObject		PieceAShaderNode::adisplacement;
MObject     PieceAShaderNode::output;       

PieceAShaderNode::PieceAShaderNode() 
{
	m_rsl = new XRSLPiece();
}

PieceAShaderNode::~PieceAShaderNode() {}

MStatus PieceAShaderNode::compute( const MPlug& plug, MDataBlock& data )
{
	
	MStatus returnStatus;
 
	if( plug == output ) {
		m_xsl_path = data.inputValue( axslpath).asString();
		MGlobal::displayInfo(m_xsl_path);
		if(!m_rsl->isValid()) {
			m_rsl->load(m_xsl_path.asChar());
			if( m_rsl->isValid()) {
				MGlobal::displayInfo(MString("loaded ")+m_rsl->getType()+" \""+m_rsl->getName()+"\" from "+m_xsl_path);
			}
			else MGlobal::displayWarning(MString("cannot load ")+m_xsl_path);
		}
		float result = 1.0f;
		MDataHandle outputHandle = data.outputValue( PieceAShaderNode::output );
		outputHandle.set( result );
		data.setClean(plug);
		
	} else {
		return MS::kUnknownParameter;
	}

	return MS::kSuccess;
}

void* PieceAShaderNode::creator()
{
	return new PieceAShaderNode();
}

MStatus PieceAShaderNode::initialize()
{
	MFnNumericAttribute numAttr;
	MFnTypedAttribute tAttr;
	MStatus				stat;
	
	axslpath = tAttr.create( "xslPath", "xsp", MFnData::kString );
	numAttr.setStorable(true);
	numAttr.setInternal(true);
	addAttribute( axslpath );
	
	adisplacement = numAttr.create( "displacementShader", "dls", MFnNumericData::kFloat, 0.0 );
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	addAttribute( adisplacement );
	
	output = numAttr.create( "output", "out", MFnNumericData::kFloat, 0.0 );
	numAttr.setWritable(false);
	numAttr.setStorable(false);
	stat = addAttribute( output );
		if (!stat) { stat.perror("addAttribute"); return stat;}
		
	attributeAffects( axslpath, output );
	attributeAffects( adisplacement, output );
	

	return MS::kSuccess;
}

/* virtual */
bool 
PieceAShaderNode::setInternalValueInContext( const MPlug& plug,
												  const MDataHandle& handle,
												  MDGContext&)
{
	bool handledAttribute = false;
	if (plug == axslpath) {
		handledAttribute = true;
		m_xsl_path = (MString) handle.asString();
		m_rsl->load(m_xsl_path.asChar());
		if(m_rsl->isValid()) {
			MGlobal::displayInfo(m_xsl_path);
			MGlobal::displayInfo(m_rsl->getName());
			MGlobal::displayInfo(m_rsl->getType());
		}
		/*
		if(m_pMesh) delete m_pMesh;
		m_pMesh = new FXMLMesh(m_cachename.asChar(), m_meshname.asChar());

		if(m_pMesh->isNull()) {
			delete m_pMesh;
			m_pMesh = 0;
		}*/
	}

	return handledAttribute;
}

/* virtual */
bool
PieceAShaderNode::getInternalValueInContext( const MPlug& plug,
											  MDataHandle& handle,
											  MDGContext&)
{
	bool handledAttribute = false;
	if (plug == axslpath)
	{
		handledAttribute = true;
		handle.set( m_xsl_path );
	}
	
	return handledAttribute;
}