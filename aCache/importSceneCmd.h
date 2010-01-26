/// :
// meshCacheCmd.h
// Version 1.3.1
// Mesh cache file writer
// updated: 05/07/08
//
#include <maya/MGlobal.h>
#include <maya/MPxCommand.h>
#include <maya/MPxDeformerNode.h> 
#include <maya/MPointArray.h>
#include <maya/MFloatArray.h>
#include <maya/MVectorArray.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>

class XMLSceneCmd : public MPxCommand
 {
 public:
 				XMLSceneCmd();
	virtual		~XMLSceneCmd();
 	MStatus doIt( const MArgList& args );
 	static void* creator();
	static MSyntax newSyntax();

  private:
	MStatus parseArgs ( const MArgList& args );
	MStatus getSceneInfo();
	MStatus getSelectedUV();
 };
 //:~