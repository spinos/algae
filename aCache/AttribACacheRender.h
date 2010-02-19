#include <maya/MPxNode.h> 


 
class AttribACacheRenderNode : public MPxNode
{
public:
						AttribACacheRenderNode();
	virtual				~AttribACacheRenderNode(); 

	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );

	static  void*		creator();
	static  MStatus		initialize();

public:
	static  MObject		asurfacecolor;
	//static  MObject		aassubd;
	static  MObject		output;        // The output value.
	static	MTypeId		id;
};