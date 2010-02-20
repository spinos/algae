/*
 *  FXMLScene.h
 *  vertexCache
 *
 *  Created by zhang on 08-11-23.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _FXMLSCENE_H
#define _FXMLSCENE_H
#include <string>
#include "zXMLDoc.h"
#include <fstream>
#include <vector>
using namespace std;

//class FQSPLAT;
class FXMLMesh;
typedef vector<FXMLMesh*>MeshList;
//typedef vector<FQSPLAT*>TreeList;

class FXMLScene
{
public:
	FXMLScene();
	~FXMLScene();
/*writing*/	
	void begin(const char* path, int frame, char bfirst);
	void end(const char* path);
	void meshBegin(const char* name, char bNoChange);
	void triangleInfo(int n_tri, float area);
	void addAttribute(const char* name, int val);
	void addAttribute(const char* name, float val);
	void meshEnd(char bNoChange);
	void staticBegin();
	void staticEnd();
	void addFaceCount(int val, const int* data);
	void addFaceConnection(int val, const int* data);
	void addTriangleConnection(int val, int* data);
	void uvSetBegin(const char* name);
	void uvSetEnd();
	void addS(const char* name, int num, const float* data);
	void addT(const char* name, int num, const float* data);
	void addVertexColor(const char* name, int num, const XYZ* data);
	void addStaticP(int num, const XYZ* data);
	void addStaticN(int num, const XYZ* data);
	void addStaticGridSize(int num, const float* data);
	void dynamicBegin();
	void dynamicEnd();
	void addP(int num, const XYZ* data);
	void addN(int num, const XYZ* data);
	//void addTangent(int num, const XYZ* data);
	void addGridSize(int num, const float* data);
	void addBBox(float x0, float y0, float z0, float x1, float y1, float z1);
	void transformBegin(const char* name, float space[4][4]);
	void transformEnd();
	void addScale(float x, float y, float z);
	void nurbssurfaceBegin(const char* name, int degreeU, int degreeV, int formU, int formV);
	void nurbssurfaceEnd();
	void addStaticVec(const char* name, int num, const XYZ* data);
	void addStaticFloat(const char* name, int num, const float* data);
	void cameraBegin(const char* name, float space[4][4]);
	void cameraEnd();
/*reading*/
	int load(const char* filename);
/*query*/
	float getSceneArea() {return m_scene_area;}
	int getSceneNumTriangle() {return m_scene_numtri;}
	int getNumMesh() const {return m_mesh.size();}
	const MeshList& getMesh() const {return m_mesh;}
	const FXMLMesh* getMesh(int id) const {return m_mesh[id];}
	//const TreeList& getShadowTree() const {return m_shadow_tree;}
	//const FQSPLAT* getRTTree() const {return m_rt_tree;}
	int getFrame() const {return m_frame;}
	void getSpaceX(XYZ& vec) const {vec.x = m_backscat_space[0][0]; vec.y = m_backscat_space[0][1]; vec.z = m_backscat_space[0][2]; vec.normalize();}
	void getSpaceY(XYZ& vec) const {vec.x = m_backscat_space[1][0]; vec.y = m_backscat_space[1][1]; vec.z = m_backscat_space[1][2]; vec.normalize();}
	void getSpaceZ(XYZ& vec) const {vec.x = m_backscat_space[2][0]; vec.y = m_backscat_space[2][1]; vec.z = m_backscat_space[2][2]; vec.normalize();}
	char hasLight() const {return m_light_numtri>0;}
	const char* getXMLName() const {return m_xml_name.c_str();}
/*dicing
	void dice(int n_cell);
	void diceWithRTandColor(int n_cell);
	void dice(int n_cell, FQSPLAT* cloud) const;
	void diceLight(int n_cell, FQSPLAT* cloud) const;
	void diceWithRT(int n_cell, FQSPLAT* cloud) const;
	void removeTree();
	void removeRTTree();*/
/*draw depth
	void depthMap(float* data, int map_w, int map_h, double fov, const float cameramatrix[4][4]) const;
	void depthMapPersp(float* data, int map_w, int map_h, const float cameramatrix[4][4], float& fov) const;
	void depthMapOrtho(float* data, int map_w, int map_h, const float cameramatrix[4][4], float& orthow) const;*/
private:
	ZXMLDoc doc;
	string m_xml_name, m_model_path;
	MeshList m_mesh, m_light;
	//TreeList m_shadow_tree;
	//FQSPLAT* m_rt_tree;

	float m_scene_area, m_light_area;
	int m_scene_numtri, m_light_numtri;
	
	ofstream static_mesh_file, dynamic_mesh_file;
	int pos_s, pos_d, m_frame;
	int m_numFace, m_numFaceVertex, m_numVertex, m_numTriangle;
	float m_backscat_space[4][4];
};
#endif
//:~
