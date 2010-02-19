#include <maya/MPxNode.h> 


 
class AttribACacheExportNode : public MPxNode
{
public:
						AttribACacheExportNode();
	virtual				~AttribACacheExportNode(); 

	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );

	static  void*		creator();
	static  MStatus		initialize();

public:
	static  MObject		anochange;
	static  MObject		anosubdiv;
	/*static  MObject		askipscat;
	static  MObject		askipepid;
	static  MObject		aaslight;
	static  MObject		aklight;
	static  MObject		anoshadow;
	static  MObject		aasghost;*/
	static  MObject		output;        // The output value.
	static	MTypeId		id;
};