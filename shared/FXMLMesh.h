/*
 *  FXMLMesh.h
 *  vertexCache
 *
 *  Created by zhang on 08-10-13.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _FXMLMESH_H
#define _FXMLMESH_H

#include "TVectors.h"
#include <string>
#include "../shared/zXMLDoc.h"
#include <fstream>
#include <vector>
using namespace std;
#include "FUVSet.h"
typedef vector<FUVSet*>UVList;
#include "FColorSet.h"
typedef vector<FColorSet*>ColorList;
#include "FFloatSet.h"
typedef vector<FFloatSet*>FloatList;
typedef vector<string>AttribList;

class FXMLMesh
{
public:
	//FXMLMesh();
	FXMLMesh(const char* filename, const char* meshname);
	~FXMLMesh();
/*	
	void initMesh(const char* name, int num_triangles, float area);
	void endMesh(const char* path);
	
	void staticBegin(const char* path);
	void staticEnd();
	
	void dynamicBegin(const char* path);
	void dynamicEnd();
	
	void addFaceCount(int val, const int* data);
	void addFaceConnection(int val, const int* data);
	void addTriangleConnection(int val, const int* data);
	void uvSetBegin(const char* name);
	void uvSetEnd();
	void addFloatParam(const char* name, int num, const float* data);
	void addS(const char* name, int num, const float* data);
	void addT(const char* name, int num, const float* data);
	void addVertexFloat(const char* name, int num, const float* data);
	void addP(int num, const XYZ* data);
	void addN(int num, const XYZ* data);
	void addTangent(int num, const XYZ* data);
	void addBBox(float x0, float y0, float z0, float x1, float y1, float z1);
	void addStaticVectorEntry(const char* name, int num, const XYZ* data);
	void addDynamicVectorEntry(const char* name, int num, const XYZ* data);
	void addIntAttrib(const char* name, int val);
	void addSpace(float space[4][4]);
	void addAttribute(const char* name, int val);
*/	
	void free();
	int load(const char* filename, const char* meshname);
	
	const char* getXMLName() const { return m_xml_name.c_str(); }
	const char* getMeshName() const { return m_meshName.c_str(); }	
	int getNumFace() const { return m_numFace; }
	int getNumFaceVertex() const { return m_numFaceVertex; }
	int getNumVertex() const { return m_numVertex; }
	int getNumTriangle() const {return m_numTriangle;}
	float getArea() const {return m_area;}
	float getBBox0X() const {return m_bbox0.x;}
	float getBBox1X() const {return m_bbox1.x;}
	float getBBox0Y() const {return m_bbox0.y;}
	float getBBox1Y() const {return m_bbox1.y;}
	float getBBox0Z() const {return m_bbox0.z;}
	float getBBox1Z() const {return m_bbox1.z;}

	char isNull() const {return m_isNull;}
	
	int nfaces() const {return m_numFace;}
	const int* nverts() const {return m_faceCount;}
	const int* verts() const {return m_vertices;}
	const int* triangles() const {return m_triangleConn;}
	const XYZ* points() const {return m_cvs;}
	const XYZ* pointsOpen() const {return m_pOpen;}
	const XYZ* pointsClose() const {return m_pClose;}
	
	int getNumUVSet() const { return m_uvSet.size(); }
	const char* getUVSetName(int i) const {return m_uvSet[i]->name.c_str();}
	const char* getSNameById(int i) const { return m_uvSet[i]->sname.c_str();}
	const char* getTNameById(int i) const { return m_uvSet[i]->tname.c_str();}
	const float* getSById(int i) const { return m_uvSet[i]->s;}
	const float* getTById(int i) const { return m_uvSet[i]->t;}
	
	//int getNumVertexFloatSet() const { return m_vertexFloatSet.size(); }
	//const char* getVertexFloatNameById(int i) const {return m_vertexFloatSet[i]->name.c_str();}
	//const float* getVertexFloatById(int i) const {return m_vertexFloatSet[i]->s;}
	
	void updateFrom(const char* filename);
	void setMotion(float open, float close);
	
	//void getNormal(XYZ& res, unsigned int idx ) const { res = m_normals[idx]; }
	//void getTangent(XYZ& res, unsigned int idx) const { res = m_tangents[idx]; }
	void getVertex(XYZ& res, unsigned int idx) const { res = m_cvs[idx]; }
	void getMovedVertex(XYZ& res, float bias, unsigned int idx) const { res = m_cvs[idx]; res += m_normals[idx]* bias; }
	
	char toSkipInterreflection() const { return m_i_skip_second; }
	char toSkipScattering() const { return m_i_skip_third; }
	
	//void setDrawColor(const XYZ& input, unsigned int idx) { m_draw_color[idx] = input; }
	void getCenterAndSize(XYZ& center, float& size) const;
	
	void resetColor();
	/*
	void updateColor(int opt);
	void calcOcc();
	void calcHDRF(const char* ext);
	void calcHDRC(const char* ext);
	void calcSRC();
	void setHDRLighting(const XYZ* coeff);
	void resetHDRLighting();*/
	
	const XYZ *getNormals() const {return m_normals;}
	//const XYZ *getBiNormals() const {return m_binormals;}
	//const XYZ *getTangents() const {return m_tangents;}
	
	void appendColorSet(const char* paramname, const char* filename);
	int getNumColorSet() const { return m_colorSet.size(); }
	const char* getColorSetNameById(int idx) const { return m_colorSet[idx]->name.c_str(); }
	const XYZ* getColorSetById(int idx) const { return m_colorSet[idx]->data; }
	
	void appendFloatSet(const char* paramname, const char* filename);
	int getNumFloatSet() const { return m_floatSet.size(); }
	const char* getFloatSetNameById(int idx) const { return m_floatSet[idx]->name.c_str(); }
	const float* getFloatSetById(int idx) const { return m_floatSet[idx]->data; }
	
	float getGrid(int idx) const {return m_grd[idx];}
	
/*dicing
	void dice(float epsilon, pcdSample* res, int& n_res);
	void diceWithRT(float epsilon, pcdSample* res, CoeRec* coe_res, int& n_res);*/
	//int checkExistingRT() const;
	int hasAttrib(const char* attribname) const;
	
	void interpolateRT(int lo, int hi, int frame, float weight, const char* extension, int fpv);
/*save depth
	void depthMap(float* data, int map_w, int map_h, MATRIX44F& space, double& fov) const;
	void depthMapPersp(float* data, int map_w, int map_h, MATRIX44F& space, float& fov) const;
	void depthMapOrtho(float* data, int map_w, int map_h, MATRIX44F& space, float& orthow) const;*/
private:
	ZXMLDoc doc;
	//int pos_s, pos_d;
	//ofstream static_file, dynamic_file;
	int* m_faceCount;
	int* m_vertices;
	int* m_triangleConn;
	//XYZ* m_color;
	XYZ* m_cvs;
	float* m_grd;
	XYZ* m_normals;
	//XYZ* m_tangents;
	//XYZ* m_binormals;
	//XYZ* m_draw_color;
	
	XYZ m_bbox0, m_bbox1;
	
	string m_xml_name, m_meshName, m_staticName, m_dynamicName;
	int m_numFace, m_numFaceVertex, m_numVertex, m_numTriangle;
	char m_isNull;
	//char m_has_color;
	
	UVList m_uvSet;
	ColorList m_colorSet;
	FloatList m_floatSet;
	
	XYZ* m_pOpen;
	XYZ* m_pClose;
	
	char m_i_skip_second, m_i_skip_third;
	float m_area;
	AttribList m_attrib;
	
	int findMesh(const char* meshname);
	XYZ usr_hdrCoeff[16];
};

#endif