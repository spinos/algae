#include "meshData.h"
#include "../shared/SHelper.h"
#include "../shared/FXMLMesh.h"
#include <iostream>
using namespace std;
#define MAX_NUMPARAM 256

/*
	args list is:
	scene name
	mesh name
	frame number
	shutter open
	shutter close
	subdiv or not
*/
			
meshData::meshData(std::string& parameter):
m_i_hdr_shadowed(0), m_i_hdr_indirect(0), m_i_hdr_scat(0), m_i_hdr_backscat(0),
m_i_lightsrc_shadowed(0),m_b_bake(0)
{
	int n = sscanf(parameter.c_str(), "%s %s %d %f %f %d", 
	m_cache_name, 
	m_mesh_name,
	&m_frame,
	&m_mesh_0, 
	&m_mesh_1,
	&m_b_bake);
}

meshData::~meshData() 
{
}

void meshData::generateRIB(RtFloat detail)
{
	RiSides(2);
	
	FXMLMesh* pMesh = new FXMLMesh(m_cache_name, m_mesh_name);
	
	RtToken tags[] = {"creasemethod", "interpolateboundary"};
	RtInt nargs[] = {0,0,1,1,0,0};
	RtInt intargs[] = {1};
	RtFloat* floatargs;
	RtToken stringargs[] = {"chaikin"};
	
	RtToken paramname[MAX_NUMPARAM];
	RtPointer paramvalue[MAX_NUMPARAM];
	
	//string sname(m_prt_name);

	if(!pMesh->isNull()) {
		string scache(m_cache_name);
		SHelper::changeFrameNumber(scache, SHelper::safeConvertToInt(m_frame));
		printf("anemone loaded %s from %s\n", m_mesh_name, scache.c_str());
		pMesh->updateFrom(scache.c_str());
		//if(pMesh->getNumUVSet()<1) RiHierarchicalSubdivisionMesh("catmull-clark", (RtInt)pMesh->nfaces(),  (RtInt*)pMesh->nverts(), (RtInt*)pMesh->verts(), (RtInt)2, tags, nargs, intargs, floatargs, stringargs, "P", (RtPoint*)pMesh->points(), RI_NULL );
		//else
		//{
		int bSubdiv = 1;
		if( pMesh->hasAttrib("noSubdiv") || m_b_bake == 1 ) bSubdiv = 0;
			
		paramname[0] = "P";
		paramvalue[0] =(RtPoint*)pMesh->points();
			
		int nparam = 1;
// no subdiv mesh needs face normal
		if(pMesh->hasAttrib("noSubdiv")) {
			paramname[nparam] = "facevarying normal N";
			paramvalue[nparam] = (RtFloat*)pMesh->getFaceNormals();
			nparam++;
		}
		else { // subdiv use vertex normal
			paramname[nparam] = "vertex normal N";
			paramvalue[nparam] = (RtFloat*)pMesh->getNormals();
			nparam++;
		}
			//printf("%d uv set ", pMesh->getNumUVSet());
			for(int i=0; i<pMesh->getNumUVSet(); i++) {
				paramname[nparam] = (RtToken)pMesh->getSNameById(i);
				paramvalue[nparam] = (RtFloat*)pMesh->getSById(i);
				nparam++;
				paramname[nparam] = (RtToken)pMesh->getTNameById(i);
				paramvalue[nparam] = (RtFloat*)pMesh->getTById(i);
				nparam++;
			}

/*			
			if(m_i_hdr_shadowed == 1.0f || m_i_lightsrc_shadowed == 1.0f)
			{
				SHelper::changeFilenameExtension(sname, "prtu");
				pMesh->appendFloatSet("vertex float coeffu", sname.c_str());
				
				if(m_i_hdr_shadowed == 1.0f)
				{
					SHelper::changeFilenameExtension(sname, "prt");
					pMesh->appendFloatSet("vertex float coeff", sname.c_str());
				}
				if(m_i_lightsrc_shadowed == 1.0f)
				{
					SHelper::changeFilenameExtension(sname, "prtl");
					pMesh->appendColorSet("vertex color coeffl", sname.c_str());
					SHelper::changeFilenameExtension(sname, "prtj");
					pMesh->appendColorSet("vertex color coeffj", sname.c_str());
				}
			}
			
			if(m_i_hdr_indirect == 1.0f)
			{
				SHelper::changeFilenameExtension(sname, "prti");
				pMesh->appendColorSet("vertex color coeffi", sname.c_str());
			}
			
			if(m_i_hdr_scat == 1.0f)
			{
				SHelper::changeFilenameExtension(sname, "prts");
				pMesh->appendFloatSet("vertex float coeffs", sname.c_str());
				SHelper::changeFilenameExtension(sname, "prte");
				pMesh->appendFloatSet("vertex float coeffe", sname.c_str());
			}
			
			if(m_i_hdr_backscat == 1)
			{
				SHelper::changeFilenameExtension(sname, "prtb");
				pMesh->appendFloatSet("vertex float coeffb", sname.c_str());
			}
			
			for(int i=0; i< pMesh->getNumColorSet(); i++)
			{
				paramname[nparam] = (RtToken)pMesh->getColorSetNameById(i);
				paramvalue[nparam] = (RtFloat*)pMesh->getColorSetById(i);
				nparam++;
			}
			
			for(int i=0; i< pMesh->getNumFloatSet(); i++)
			{
				paramname[nparam] = (RtToken)pMesh->getFloatSetNameById(i);
				paramvalue[nparam] = (RtFloat*)pMesh->getFloatSetById(i);
				nparam++;
			}
*/			
			
			if( bSubdiv==1 ) RiHierarchicalSubdivisionMeshV("catmull-clark", (RtInt)pMesh->nfaces(),  (RtInt*)pMesh->nverts(), (RtInt*)pMesh->verts(), (RtInt)2, tags, nargs, intargs, floatargs, stringargs, (RtInt)nparam, paramname, paramvalue );
			else RiPointsPolygonsV( (RtInt)pMesh->nfaces(), (RtInt*)pMesh->nverts(), (RtInt*)pMesh->verts(), (RtInt)nparam, paramname, paramvalue);

/*			else {
				pMesh->setMotion(m_mesh_0, m_mesh_1);
				RiMotionBegin(2, (RtFloat)m_shutter_open, (RtFloat)m_shutter_close);
				
				paramvalue[0] =(RtPoint*)pMesh->pointsOpen();
					RiHierarchicalSubdivisionMeshV("catmull-clark", (RtInt)pMesh->nfaces(),  (RtInt*)pMesh->nverts(), (RtInt*)pMesh->verts(), (RtInt)2, tags, nargs, intargs, floatargs, stringargs, (RtInt)nparam, paramname, paramvalue );
				paramvalue[0] =(RtPoint*)pMesh->pointsClose();
					RiHierarchicalSubdivisionMeshV("catmull-clark", (RtInt)pMesh->nfaces(),  (RtInt*)pMesh->nverts(), (RtInt*)pMesh->verts(), (RtInt)2, tags, nargs, intargs, floatargs, stringargs, (RtInt)nparam, paramname, paramvalue );
					
				RiMotionEnd();
			}*/
		//}
	}
  printf("anemone cleanup %s \n", m_mesh_name);
	delete pMesh;

	return;
}
//:~