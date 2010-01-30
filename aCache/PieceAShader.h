#include <maya/MPxNode.h> 
#include "XRSLPiece.h"

 
class PieceAShaderNode : public MPxNode
{
public:
						PieceAShaderNode();
	virtual				~PieceAShaderNode(); 

	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );

	static  void*		creator();
	static  MStatus		initialize();
	
		virtual bool getInternalValueInContext( const MPlug&,
											  MDataHandle&,
											  MDGContext&);
    virtual bool setInternalValueInContext( const MPlug&,
											  const MDataHandle&,
											  MDGContext&);

public:
	static  MObject		axslpath;
	static  MObject		output;        // The output value.
	static  MObject		outputc;
	static  MObject		outputs;
	static	MTypeId		id;
	
	XRSLPiece* getPiece() const {return m_rsl;}
private:	
	MString m_xsl_path;
	XRSLPiece *m_rsl;
};