#include <maya/MPxNode.h> 


 
class AdaptVariableAShaderNode : public MPxNode
{
public:
						AdaptVariableAShaderNode();
	virtual				~AdaptVariableAShaderNode(); 

	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );

	static  void*		creator();
	static  MStatus		initialize();

public:
	static MObject ancondition;
	static  MObject		ainput;
	static  MObject		ainputC;
	static  MObject		ainputS;
	static  MObject		acondition;
	static  MObject		ahandle;
	static  MObject		aoperation;
	static  MObject		output;
	static  MObject		outputC;
	static  MObject		outputS;         // The output value.
	static	MTypeId		id;
};