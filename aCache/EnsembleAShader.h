#include <maya/MPxNode.h> 


 
class EnsembleAShaderNode : public MPxNode
{
public:
						EnsembleAShaderNode();
	virtual				~EnsembleAShaderNode(); 

	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );

	static  void*		creator();
	static  MStatus		initialize();

public:
	static  MObject		asurface;
	static  MObject		adisplacement;
	static  MObject		aribbox;
	static  MObject		aoperation;
	static MObject atracebias;
	static  MObject		output;        // The output value.
	static	MTypeId		id;
};