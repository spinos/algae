#include <maya/MPxNode.h> 


 
class AdaptEnsembleAShaderNode : public MPxNode
{
public:
						AdaptEnsembleAShaderNode();
	virtual				~AdaptEnsembleAShaderNode(); 

	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );

	static  void*		creator();
	static  MStatus		initialize();

public:
	static MObject ancondition;
	static  MObject		aensemble;
	static  MObject		acondition;
	static  MObject		ahandle;
	static  MObject		output;        // The output value.
	static	MTypeId		id;
};