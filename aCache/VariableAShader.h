#include <maya/MPxNode.h> 


 
class VariableAShaderNode : public MPxNode
{
public:
						VariableAShaderNode();
	virtual				~VariableAShaderNode(); 

	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );

	static  void*		creator();
	static  MStatus		initialize();

public:
	static  MObject		ahandle;
	static  MObject		output;
	static  MObject		outputC;
	static  MObject		outputS;        // The output value.
	static	MTypeId		id;
};