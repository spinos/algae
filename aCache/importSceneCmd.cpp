/// :
// meshCacheCmd.cpp
// Version 1.2.8
// Mesh cache file writer
// updated: 05/07/08
//
#include "../shared/AHelper.h"
#include "importSceneCmd.h"
#include <maya/MAnimControl.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MItSurfaceCV.h>
#include <maya/MItGeometry.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>
#include <maya/MItMeshEdge.h>
#include <maya/MFnMesh.h>
#include <maya/MFnMeshData.h>
#include "aCacheMeshViz.h"
#include "../shared/TVectors.h"
#include "../shared/zXMLDoc.h"
#include "../shared/FXMLMesh.h"
#include "../shared/SHelper.h"
////////////////////////////////////////
//
// cache file generation Command Class
//
////////////////////////////////////////
 
  
XMLSceneCmd::XMLSceneCmd() {}

XMLSceneCmd::~XMLSceneCmd() {}

MSyntax XMLSceneCmd::newSyntax() 
{
	MSyntax syntax;

	syntax.addFlag("-p", "-path", MSyntax::kString);
	syntax.addFlag("-m", "-mesh", MSyntax::kBoolean);
	syntax.addFlag("-t", "-transform", MSyntax::kBoolean);
	syntax.addFlag("-i", "-info", MSyntax::kBoolean);
	syntax.addFlag("-u", "-uvinfo", MSyntax::kBoolean);
	
	syntax.enableQuery(false);
	syntax.enableEdit(false);

	return syntax;
}

MStatus XMLSceneCmd::doIt( const MArgList& args ) 
{
	MStatus status = parseArgs( args );
	
	if( status != MS::kSuccess ) return status;
	
	MArgDatabase argData(syntax(), args);
	
	//MAnimControl timeControl;
	//MTime time = timeControl.currentTime();
	if (argData.isFlagSet("-i")) return getSceneInfo();
	else if (argData.isFlagSet("-u")) return getSelectedUV();

	MString proj;
	MGlobal::executeCommand( MString ("string $p = `workspace -q -fn`"), proj );
	
	MSelectionList selList;
	MGlobal::getActiveSelectionList ( selList );
	MItSelectionList iter( selList );

	MString cache_path = proj + "/data/";
	if (argData.isFlagSet("-p")) argData.getFlagArgument("-p", 0, cache_path);
	
	ZXMLDoc doc;
	if(doc.load(cache_path.asChar()) != 1) 
	{
		MGlobal::displayError(MString("Cannot open ")+cache_path);
		return MS::kSuccess;
	}
	
	doc.setChildren();
	
	if(argData.isFlagSet("-m"))
	{
		while(doc.isLastNode() != 1)
		{
			if(doc.checkNodeName("mesh") == 1)
			{
				appendToResult(doc.getAttribByName("name"));
			}
			doc.nextNode();
		}
	}
	
	if(argData.isFlagSet("-t"))
	{
		while(doc.isLastNode() != 1)
		{
			if(doc.checkNodeName("camera") == 1)
			{
				appendToResult(doc.getAttribByName("name"));
				
				MFnCamera ccam;
				MObject ocam = ccam.create();
				
				if(doc.isEmpty() != 1)
				{
					MDagPath pcam;
					MDagPath::getAPathTo(ocam, pcam);
					pcam.extendToShape();
					
					MFnCamera fcam = MFnCamera(pcam);
					doc.setChildren();
					
					while(doc.isLastNode() != 1)
					{
						if(doc.checkNodeName("attribute") == 1)
						{
							if(strcmp (doc.getAttribByName("name"), "focal_length") == 0)
							{
								fcam.setFocalLength(doc.getFloatAttribByName("value"));
							}
							else if(strcmp (doc.getAttribByName("name"), "horizontal_film_aperture") == 0)
							{
								fcam.setHorizontalFilmAperture(doc.getFloatAttribByName("value"));
							}
							else if(strcmp (doc.getAttribByName("name"), "vertical_film_aperture") == 0)
							{
								fcam.setVerticalFilmAperture(doc.getFloatAttribByName("value"));
							}
							else if(strcmp (doc.getAttribByName("name"), "near_clipping_plane") == 0)
							{
								fcam.setNearClippingPlane(doc.getFloatAttribByName("value"));
							}
							else if(strcmp (doc.getAttribByName("name"), "far_clipping_plane") == 0)
							{
								fcam.setFarClippingPlane(doc.getFloatAttribByName("value"));
							}
						}
						doc.nextNode();
					}
					
					doc.setParent();
				}
				
				appendToResult(MFnDependencyNode(ocam).name());
				appendToResult(" 1 1 1 ");
			}
			else if(doc.checkNodeName("transform") == 1)
			{
				appendToResult(doc.getAttribByName("name"));
				
				XYZ scale;
				doc.getFloat3AttribByName("scale", scale.x, scale.y, scale.z);
				
				doc.getChildByName("nurbs_surface");
				
				int degreeU = doc.getIntAttribByName("degreeU");
				int degreeV = doc.getIntAttribByName("degreeV");
				
				int formU = doc.getIntAttribByName("formU");
				int formV = doc.getIntAttribByName("formV");
				
				MFnNurbsSurface::Form fmu;
				MFnNurbsSurface::Form fmv;
				
				if(formU ==0) fmu = MFnNurbsSurface::kOpen;
				else if(formU ==1) fmu = MFnNurbsSurface::kClosed;
				else fmu = MFnNurbsSurface::kPeriodic;
				
				if(formV ==0) fmv = MFnNurbsSurface::kOpen;
				else if(formV ==1) fmv = MFnNurbsSurface::kClosed;
				else fmv = MFnNurbsSurface::kPeriodic;
				
				std::string staticName = cache_path.asChar();
				SHelper::cutByFirstDot(staticName);
				staticName += ".sta";
				
				std::ifstream ffin;
				ffin.open(staticName.c_str(), ios::in | ios::binary);
				
				doc.getChildByName("static");
				
				doc.getChildByName("cvs");
					
					int n_cvs = doc.getIntAttribByName("count");
					int pos = doc.getIntAttribByName("loc");
					int size = doc.getIntAttribByName("size");
					
					XYZ* cvs = new XYZ[n_cvs];
					ffin.seekg( pos, ios::beg );
					ffin.read((char*)cvs, size);
					
					MPointArray pcvs; pcvs.setLength(n_cvs);
					
					for(unsigned i=0; i<n_cvs; i++) pcvs[i] = MPoint(cvs[i].x, cvs[i].y, cvs[i].z);
					
					delete[] cvs;
					
				doc.setParent();
				
				doc.getChildByName("knotu");
					
					int n_knotu = doc.getIntAttribByName("count");
					pos = doc.getIntAttribByName("loc");
					size = doc.getIntAttribByName("size");
					
					float* knotu = new float[n_knotu];
					ffin.seekg( pos, ios::beg );
					ffin.read((char*)knotu, size);
					
					MDoubleArray pknotu; pknotu.setLength(n_knotu);
					
					for(unsigned i=0; i<n_knotu; i++) pknotu[i] = knotu[i];
					
					delete[] knotu;
					
				doc.setParent();
				
				doc.getChildByName("knotv");
					
					int n_knotv = doc.getIntAttribByName("count");
					pos = doc.getIntAttribByName("loc");
					size = doc.getIntAttribByName("size");
					
					float* knotv = new float[n_knotv];
					ffin.seekg( pos, ios::beg );
					ffin.read((char*)knotv, size);
					
					MDoubleArray pknotv; pknotv.setLength(n_knotv);
					
					for(unsigned i=0; i<n_knotv; i++) pknotv[i] = knotv[i];
					
					delete[] knotv;
					
				ffin.close();
					
				doc.setParent();

				MFnNurbsSurface fnurbs;
				MObject onurbs = fnurbs.create(pcvs, pknotu, pknotv, degreeU, degreeV,  fmu,  fmv, 0, MObject::kNullObj);
				MString nurbsn = MFnDependencyNode(onurbs).name();
	
				appendToResult(nurbsn);
				
				MString outscale = MString(" ") +scale.x +" "+scale.y +" "+scale.z;
				appendToResult(outscale);
				
				doc.setParent();
				doc.setParent();
			}
			doc.nextNode();
		}
	}
	
	doc.setParent();
	doc.free();

 return MS::kSuccess;
 }

void* XMLSceneCmd::creator() {
 return new XMLSceneCmd;
 }
 
MStatus XMLSceneCmd::parseArgs( const MArgList& args )
{
	// Parse the arguments.
	MStatus stat = MS::kSuccess;
	return MS::kSuccess;
}

MStatus XMLSceneCmd::getSceneInfo()
{
	//MStringArray existingnode;
	//MGlobal::executeCommand ("ls -type meshCacheViz", existingnode );
	MSelectionList selList;
	MGlobal::getActiveSelectionList ( selList );
	MItSelectionList iter( selList );
	for ( ; !iter.isDone(); iter.next() )
	{
		MDagPath vizPath;		
		iter.getDagPath( vizPath );
		vizPath.extendToShape();
	
		MFnDagNode pf(vizPath.node());
		
		ACacheMeshViz* pNode = (ACacheMeshViz*)pf.userNode();
		if(pNode)
		{
			const FXMLMesh* pMesh = pNode->getMeshPointer();
			if(pMesh)
			{
				MString res("  Mesh - ");
				res += pMesh->getMeshName();
				res += "\n num vertices: ";
				res += pMesh->getNumVertex();
				res += "\n num faces: ";
				res += pMesh->nfaces();
				res += "\n num face vertices: ";
				res += pMesh->getNumFaceVertex();
				res += "\n uv set: ";
				for(unsigned j=0; j<pMesh->getNumUVSet(); j++)
				{
					res += pMesh->getUVSetName(j);
					if(j != pMesh->getNumUVSet()-1) res += " | ";
				}
				res += "\n path: ";
				res += pMesh->getXMLName();
				appendToResult(res);
			}
		}
	}
	
	return MS::kSuccess;
}

MStatus XMLSceneCmd::getSelectedUV()
{
	MSelectionList selList;
	MGlobal::getActiveSelectionList ( selList );

	if ( selList.length() == 0 )
	{
		MGlobal:: displayError ( "Nothing is selected!" );
		return MS::kSuccess;
	}
	
	MItSelectionList iter( selList );
	
	MDagPath nodePath;		
	iter.getDagPath( nodePath );
	nodePath.extendToShape();
	MFnDagNode pf(nodePath);
	ACacheMeshViz* pNode = (ACacheMeshViz*)pf.userNode();
	const FXMLMesh* pMesh = pNode->getMeshPointer();
	if(pMesh)
	{
		MString res = pMesh->getMeshName();
		appendToResult(res);
		for(unsigned j=0; j<pMesh->getNumUVSet(); j++)
		{
			res = pMesh->getUVSetName(j);
			appendToResult(res);
		}
	}
	return MS::kSuccess;
}
//:~
