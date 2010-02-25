/*
 *  FXMLMesh.cpp
 *  vertexCache
 *
 *  Created by zhang on 08-10-13.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#include "FXMLMesh.h"

#include "SHelper.h"
//#include <maya/MGlobal.h>
/*
const float constantCoeff[16] = { 3.543211, 0.000605, 0.000152, -0.003217, 
								0.000083, -0.002813, -0.000021, -0.001049,
								0.000144, -0.001950, -0.000261, 0.004375,
								0.001578, -0.004627, 0.001557, -0.000312};
								
const float hdrCoeff[48] = { 0.545337,0.615628,0.650480,
-0.410697,-0.509181,-0.586825,
-0.089776,-0.110368,-0.129048,
-0.134925,-0.125774,-0.088575,
0.065117,0.033042,-0.019713,
0.061583,0.089115,0.122649,
-0.100385,-0.116865,-0.129929,
0.112412,0.132619,0.135714,
0.022489,-0.020040,-0.085217,
-0.200834,-0.207858,-0.188677,
-0.072292,-0.082221,-0.080390,
-0.025514,-0.030657,-0.032725,
0.029278,0.033616,0.044302,
-0.003408,-0.020655,-0.049015,
-0.021611,-0.010354,0.013596,
-0.121166,-0.160075,-0.194946};
*/
/*
FXMLMesh::FXMLMesh():
//pos_s(0), 
//pos_d(0),
m_faceCount(0), 
m_vertices(0), 
m_triangleConn(0),
m_cvs(0),
//m_subd_cvs(0), 
m_normals(0),
//m_tangents(0), m_binormals(0),
m_draw_color(0), m_color(0),
m_numFace(0), 
m_numFaceVertex(0), 
m_numVertex(0),
m_i_skip_second(0), m_i_skip_third(0),
m_pOpen(0), m_pClose(0),
m_grd(0),
m_isNull(1)
{}
*/
FXMLMesh::FXMLMesh(const char* filename, const char* meshname) :
m_faceCount(0), 
m_vertices(0),
m_triangleConn(0), 
m_cvs(0),
m_normals(0),
m_facevarying_normals(0),
m_numFace(0), 
m_numFaceVertex(0), 
m_numVertex(0),
m_i_skip_second(0), m_i_skip_third(0),
m_pOpen(0), m_pClose(0),
m_grd(0),
m_isNull(1)
{
	//resetHDRLighting();
	if(load(filename, meshname) == 1) m_isNull = 0;
}

FXMLMesh::~FXMLMesh()
{
	free();
}
/*
void FXMLMesh::initMesh(const char* name, int num_triangles, float area)
{
	doc.begin();
	doc.elementBegin("mesh");
	doc.addAttribute("name", name);
	doc.addAttribute("num_triangle", num_triangles);
	doc.addAttribute("area", area);
}

void FXMLMesh::endMesh(const char* path)
{
	doc.elementEnd();
	doc.end(path);
}

void FXMLMesh::addAttribute(const char* name, int val)
{
	doc.elementBegin("attribute");
	doc.addAttribute("name", name);
	doc.addAttribute("value", val);
	doc.elementEnd();
}

void FXMLMesh::staticBegin(const char* path)
{
	doc.elementBegin("static");
	static_file.open(path, ios::out | ios::binary);
}

void FXMLMesh::staticEnd()
{
	doc.elementEnd();
	static_file.close();
}
	
void FXMLMesh::dynamicBegin(const char* path)
{
	doc.elementBegin("dynamic");
	//doc.addAttribute("path", path);
	
	dynamic_file.open(path, ios::out | ios::binary);
}

void FXMLMesh::dynamicEnd()
{
	doc.elementEnd();
	dynamic_file.close();
}

void FXMLMesh::addFaceCount(int val, const int* data)
{
	int offset = (int)sizeof(int)*val;
	doc.elementBegin("faceCount");
	doc.addAttribute("loc", pos_s);
	doc.addAttribute("size", offset);
	doc.addAttribute("count", val);
	doc.elementEnd();
	
	pos_s += offset;
	
	static_file.write((char*)data, val*sizeof(int));
}

void FXMLMesh::addFaceConnection(int val, const int* data)
{
	int offset = (int)sizeof(int)*val;
	doc.elementBegin("faceConnection");
	doc.addAttribute("loc", pos_s);
	doc.addAttribute("size", offset);
	doc.addAttribute("count", val);
	doc.elementEnd();
	
	pos_s += offset;
	
	static_file.write((char*)data, val*sizeof(int));
}

void FXMLMesh::addTriangleConnection(int val, const int* data)
{
	int offset = (int)sizeof(int)*val;
	doc.elementBegin("triangleConnection");
	doc.addAttribute("loc", pos_s);
	doc.addAttribute("size", offset);
	doc.addAttribute("count", val);
	doc.elementEnd();
	
	pos_s += offset;
	
	static_file.write((char*)data, val*sizeof(int));
}

void FXMLMesh::addP(int num, const XYZ* data)
{
	int offset = (int)sizeof(XYZ)*num;
	doc.elementBegin("P");
	doc.addAttribute("loc", pos_d);
	doc.addAttribute("size", offset);
	doc.addAttribute("count", num);
	doc.elementEnd();
	
	pos_d += offset;
	
	dynamic_file.write((char*)data, sizeof(XYZ)*num);
}

void FXMLMesh::addN(int num, const XYZ* data)
{
	int offset = (int)sizeof(XYZ)*num;
	doc.elementBegin("N");
	doc.addAttribute("loc", pos_d);
	doc.addAttribute("size", offset);
	doc.addAttribute("count", num);
	doc.elementEnd();
	
	pos_d += offset;
	
	dynamic_file.write((char*)data, sizeof(XYZ)*num);
}

void FXMLMesh::addTangent(int num, const XYZ* data)
{
	int offset = (int)sizeof(XYZ)*num;
	doc.elementBegin("Tangent");
	doc.addAttribute("loc", pos_d);
	doc.addAttribute("size", offset);
	doc.addAttribute("count", num);
	doc.elementEnd();
	
	pos_d += offset;
	
	dynamic_file.write((char*)data, sizeof(XYZ)*num);
}

void FXMLMesh::addStaticVectorEntry(const char* name, int num, const XYZ* data)
{
	int offset = (int)sizeof(XYZ)*num;
	doc.elementBegin(name);
	doc.addAttribute("loc", pos_s);
	doc.addAttribute("size", offset);
	doc.addAttribute("count", num);
	doc.elementEnd();
	
	pos_s += offset;
	
	static_file.write((char*)data, sizeof(XYZ)*num);
}

void FXMLMesh::addDynamicVectorEntry(const char* name, int num, const XYZ* data)
{
	int offset = (int)sizeof(XYZ)*num;
	doc.elementBegin(name);
	doc.addAttribute("loc", pos_d);
	doc.addAttribute("size", offset);
	doc.addAttribute("count", num);
	doc.elementEnd();
	
	pos_d += offset;
	
	dynamic_file.write((char*)data, sizeof(XYZ)*num);
}

void FXMLMesh::addIntAttrib(const char* name, int val)
{
	doc.addAttribute(name, val);
}

void FXMLMesh::addFloatParam(const char* name, int num, const float* data)
{
	int offset = (int)sizeof(float)*num;
	doc.elementBegin("parameter");
	doc.addAttribute("name", name);
	doc.addAttribute("loc", pos_s);
	doc.addAttribute("size", offset);
	doc.addAttribute("count", num);
	doc.elementEnd();
	
	pos_s += offset;
	
	static_file.write((char*)data, num*sizeof(float));
}

void FXMLMesh::uvSetBegin(const char* name)
{
	doc.elementBegin("uvSet");
	doc.addAttribute("name", name);
}

void FXMLMesh::uvSetEnd()
{
	doc.elementEnd();
}

void FXMLMesh::addS(const char* name, int num, const float* data)
{
	int offset = (int)sizeof(float)*num;
	doc.elementBegin("s");
	doc.addAttribute("name", name);
	doc.addAttribute("loc", pos_s);
	doc.addAttribute("size", offset);
	doc.addAttribute("count", num);
	doc.elementEnd();
	
	pos_s += offset;
	
	static_file.write((char*)data, num*sizeof(float));
}

void FXMLMesh::addT(const char* name, int num, const float* data)
{
	int offset = (int)sizeof(float)*num;
	doc.elementBegin("t");
	doc.addAttribute("name", name);
	doc.addAttribute("loc", pos_s);
	doc.addAttribute("size", offset);
	doc.addAttribute("count", num);
	doc.elementEnd();
	
	pos_s += offset;
	
	static_file.write((char*)data, num*sizeof(float));
}

void FXMLMesh::addVertexFloat(const char* name, int num, const float* data)
{
	int offset = (int)sizeof(float)*num;
	doc.elementBegin("vertex_float");
	doc.addAttribute("name", name);
	doc.addAttribute("loc", pos_s);
	doc.addAttribute("size", offset);
	doc.addAttribute("count", num);
	doc.elementEnd();
	
	pos_s += offset;
	
	static_file.write((char*)data, num*sizeof(float));
}

void FXMLMesh::addBBox(float x0, float y0, float z0, float x1, float y1, float z1)
{
	doc.elementBegin("bbox");
	doc.addAttribute("low", x0, y0, z0);
	doc.addAttribute("high", x1, y1, z1);
	doc.elementEnd();
}

void FXMLMesh::addSpace(float space[4][4])
{
	doc.elementBegin("space");
	doc.addAttribute("X", space[0][0], space[0][1], space[0][2]);
	doc.addAttribute("Y", space[1][0], space[1][1], space[1][2]);
	doc.addAttribute("Z", space[2][0], space[2][1], space[2][2]);
	doc.addAttribute("W", space[3][0], space[3][1], space[3][2]);
	doc.elementEnd();
}
*/
void FXMLMesh::free()
{
	if(m_isNull==0)
	{
		if(m_faceCount) delete[] m_faceCount;
		if(m_vertices) delete[] m_vertices;
		if(m_triangleConn) delete[] m_triangleConn;
		if(m_cvs) delete[] m_cvs;
		if(m_normals) delete[] m_normals;
		if(m_facevarying_normals) delete[] m_facevarying_normals;
		if(m_pOpen) delete[] m_pOpen;
		if(m_pClose) delete[] m_pClose;
		if(m_grd) delete[] m_grd;
		for(unsigned i=0; i<m_uvSet.size(); i++) {
			delete[] m_uvSet[i]->s;
			delete[] m_uvSet[i]->t;
		}
		m_uvSet.clear();
		for(unsigned i=0; i<m_colorSet.size(); i++) {
			delete[] m_colorSet[i]->data;
		}
		m_colorSet.clear();
		for(unsigned i=0; i<m_floatSet.size(); i++) {
			delete[] m_floatSet[i]->data;
		}
		m_floatSet.clear();
		m_isNull = 1;
	}
}

int FXMLMesh::findMesh(const char* meshname)
{
	while(doc.isLastNode() != 1)
	{
		if(doc.checkNodeName("mesh") == 1)
		{
			if(strcmp ( doc.getAttribByName("name"), meshname )==0) return 1;
		}
		doc.nextNode();
	}
	return 0;
}

int FXMLMesh::load(const char* filename, const char* meshname)
{
	if(string(meshname)=="") return 0;
	free();
	
	//MGlobal::displayInfo(filename);
	//MGlobal::displayInfo(meshname);
	m_xml_name = string(filename);
	if(doc.load(filename) != 1) { printf("cannot open scene %s",filename); return 0;}
	
	doc.setChildren();
	
	if(findMesh(meshname) !=1) { printf("cannot find mesh %s",meshname); return 0;}

	ifstream ffin;
	int pos, size;
	
	m_area = doc.getFloatAttribByName("area");
	m_numTriangle = doc.getIntAttribByName("num_triangle");
	m_meshName = string(doc.getAttribByName("name"));
	
	m_bbox0.x = m_bbox0.y = m_bbox0.z = 0.0f;
	m_bbox1.x = m_bbox1.y = m_bbox1.z = 1.0f;

/*get arributes*/
	doc.setChildren();
	while(doc.isLastNode()!=1) {
			if(doc.checkNodeName("attribute") ==1) {
				//printf("mesh attrib: %s\n", doc.getAttribByName("name"));
				m_attrib.push_back(doc.getAttribByName("name"));
			}
			if(doc.checkNodeName("bbox") ==1)
			{
				doc.getFloat3AttribByName("low", m_bbox0.x, m_bbox0.y, m_bbox0.z);
				doc.getFloat3AttribByName("high", m_bbox1.x, m_bbox1.y, m_bbox1.z);
			}
			doc.nextNode();
		}
		doc.setParent();

		doc.getChildByName("static");
		
		m_staticName = m_xml_name;
		SHelper::cutByLastSlash(m_staticName);
		m_staticName += "/models/";
		m_staticName += meshname;
		
		//MGlobal::displayInfo(m_staticName.c_str());
	
		ffin.open(m_staticName.c_str(), ios::in | ios::binary);
		
			doc.getChildByName("faceCount");
			m_numFace = doc.getIntAttribByName("count");
			pos = doc.getIntAttribByName("loc");
			size = doc.getIntAttribByName("size");
			
			m_faceCount = new int[m_numFace];
			ffin.seekg( pos, ios::beg );
			ffin.read((char*)m_faceCount, size);
			
		doc.setParent();
			doc.getChildByName("faceConnection");
			m_numFaceVertex = doc.getIntAttribByName("count");
			pos = doc.getIntAttribByName("loc");
			size = doc.getIntAttribByName("size");
			
			m_vertices = new int[m_numFaceVertex];
			ffin.seekg( pos, ios::beg );
			ffin.read((char*)m_vertices, size);
			
		doc.setParent();
		
			doc.getChildByName("triangleConnection");
			pos = doc.getIntAttribByName("loc");
			size = doc.getIntAttribByName("size");
			
			m_triangleConn = new int[m_numTriangle*3];
			ffin.seekg( pos, ios::beg );
			ffin.read((char*)m_triangleConn, size);
			
			
		doc.setParent();
		
		doc.setChildren();
		
		while(doc.isLastNode()!=1)
		{
			if(doc.checkNodeName("uvSet") ==1)
			{
				//printf("uvSet: %s\n", doc.getAttribByName("name"));
				
				FUVSet* uv = new FUVSet();
				uv->name = doc.getAttribByName("name");
				uv->s = new float[m_numFaceVertex];
				uv->t = new float[m_numFaceVertex];
				
				if(doc.isEmpty() != 1)
				{
					if(doc.getChildByName("s") == 1)
					{
						uv->sname = doc.getAttribByName("name");
						pos = doc.getIntAttribByName("loc");
						size = doc.getIntAttribByName("size");
						ffin.seekg( pos, ios::beg );
						ffin.read((char*)uv->s, size);
					}
					doc.setParent();
					
					if(doc.getChildByName("t") == 1)
					{
						uv->tname = doc.getAttribByName("name");
						pos = doc.getIntAttribByName("loc");
						size = doc.getIntAttribByName("size");
						ffin.seekg( pos, ios::beg );
						ffin.read((char*)uv->t, size);
					}
					doc.setParent();
				}
				
				m_uvSet.push_back(uv);
			}
			doc.nextNode();
		}
		
		doc.setParent();
		
		doc.getChildByName("P");
			m_numVertex = doc.getIntAttribByName("count");
			pos = doc.getIntAttribByName("loc");
			size = doc.getIntAttribByName("size");
			
			m_cvs = new XYZ[m_numVertex];
			
			ffin.seekg( pos, ios::beg );
			ffin.read((char*)m_cvs, size);
			
			//printf("%f %f %f \n", m_cvs[1].x, m_cvs[1].y, m_cvs[1].z);
		
		doc.setParent();
		
			m_normals = new XYZ[m_numVertex];
			
			if(doc.getChildByName("N") != 0)
			{
				pos = doc.getIntAttribByName("loc");
				size = doc.getIntAttribByName("size");
				ffin.seekg( pos, ios::beg );
				ffin.read((char*)m_normals, size);
			}
		doc.setParent();

			m_grd = new float[m_numVertex];
// set average grid size in case no data is saved
			float avg_sgrd = sqrt(m_area/m_numTriangle)/2;
			for(unsigned iv = 0; iv<m_numVertex; iv++) m_grd[iv] = avg_sgrd;
			
			if(doc.getChildByName("GridSize") != 0) {
				pos = doc.getIntAttribByName("loc");
				size = doc.getIntAttribByName("size");
				ffin.seekg( pos, ios::beg );
				ffin.read((char*)m_grd, size);
			}
		doc.setParent();
		
		if(hasAttrib("noSubdiv") == 1) {
			m_facevarying_normals = new XYZ[m_numFaceVertex];
			
			if(doc.getChildByName("FaceVaryingN") != 0) {
				pos = doc.getIntAttribByName("loc");
				size = doc.getIntAttribByName("size");
				ffin.seekg( pos, ios::beg );
				ffin.read((char*)m_facevarying_normals, size);
			}
			doc.setParent();
		}
		
		/*
			if(doc.getChildByName("vertex_color") != 0)
			{
				m_has_color = 1;
				m_numVertex = doc.getIntAttribByName("count");
				m_draw_color = new XYZ[m_numVertex];
				m_color = new XYZ[m_numVertex];
				
				pos = doc.getIntAttribByName("loc");
				size = doc.getIntAttribByName("size");
				ffin.seekg( pos, ios::beg );
				ffin.read((char*)m_color, size);
			}
			doc.setParent();*/
			ffin.close();
	doc.setParent();
	if(hasAttrib("noChange") == 0) {
	
		doc.getChildByName("dynamic");
		
		m_dynamicName = m_staticName;
		int ifrm = SHelper::getFrameNumber(m_xml_name);
		char sbuf[8];
		sprintf(sbuf, ".%i", ifrm);
		m_dynamicName.append(sbuf);
		
		ffin.open(m_dynamicName.c_str(), ios::in | ios::binary);
		
			doc.getChildByName("P");
			m_numVertex = doc.getIntAttribByName("count");
			pos = doc.getIntAttribByName("loc");
			size = doc.getIntAttribByName("size");
			
			//m_cvs = new XYZ[m_numVertex];
			
			ffin.seekg( pos, ios::beg );
			ffin.read((char*)m_cvs, size);
			
			//printf("%f %f %f \n", m_cvs[1].x, m_cvs[1].y, m_cvs[1].z);
		
		doc.setParent();
		
			//m_normals = new XYZ[m_numVertex];
			
			if(doc.getChildByName("N") != 0)
			{
				pos = doc.getIntAttribByName("loc");
				size = doc.getIntAttribByName("size");
				ffin.seekg( pos, ios::beg );
				ffin.read((char*)m_normals, size);
			}
		doc.setParent();
		
		if(hasAttrib("noSubdiv") == 1) {
			if(doc.getChildByName("FaceVaryingN") != 0) {
				pos = doc.getIntAttribByName("loc");
				size = doc.getIntAttribByName("size");
				ffin.seekg( pos, ios::beg );
				ffin.read((char*)m_facevarying_normals, size);
			}
			doc.setParent();
		}
		
			
		
			//m_grd = new float[m_numVertex];
// set average grid size in case no data is saved
			float avg_sgrd = sqrt(m_area/m_numTriangle)/2;
			for(unsigned iv = 0; iv<m_numVertex; iv++) m_grd[iv] = avg_sgrd;
			
			if(doc.getChildByName("GridSize") != 0) {
				pos = doc.getIntAttribByName("loc");
				size = doc.getIntAttribByName("size");
				ffin.seekg( pos, ios::beg );
				ffin.read((char*)m_grd, size);
			}
		doc.setParent();
		ffin.close();
		//resetColor();

		doc.setParent();
		}
			
		doc.setParent();
		
	doc.free();
	m_isNull = 0;

	return 1;
}

void FXMLMesh::updateFrom(const char* filename)
{
	if(hasAttrib("noChange") == 1) return;
	
	m_xml_name = string(filename);
	m_staticName = m_xml_name;
	SHelper::cutByLastSlash(m_staticName);
	m_staticName += "/models/";
	m_staticName += m_meshName;
	
	m_dynamicName = m_staticName;
	int ifrm = SHelper::getFrameNumber(m_xml_name);
	char sbuf[8];
	sprintf(sbuf, ".%i", ifrm);
	m_dynamicName.append(sbuf);
		
	ifstream ffin;
	int pos, size;
	if(doc.load(filename) != 1) return;
	
	doc.setChildren();
	
	if(findMesh(m_meshName.c_str()) !=1) return;
	
		if(doc.getChildByName("bbox") != 0)
		{
			doc.getFloat3AttribByName("low", m_bbox0.x, m_bbox0.y, m_bbox0.z);
			doc.getFloat3AttribByName("high", m_bbox1.x, m_bbox1.y, m_bbox1.z);
		}
		
		doc.setParent();

		doc.getChildByName("dynamic");
			
		ffin.open(m_dynamicName.c_str(), ios::in | ios::binary);
		
			doc.getChildByName("P");
			m_numVertex = doc.getIntAttribByName("count");
			pos = doc.getIntAttribByName("loc");
			size = doc.getIntAttribByName("size");
			
			ffin.seekg( pos, ios::beg );
			ffin.read((char*)m_cvs, size);
		
		doc.setParent();

			if(doc.getChildByName("N") != 0)
			{
				pos = doc.getIntAttribByName("loc");
				size = doc.getIntAttribByName("size");
				ffin.seekg( pos, ios::beg );
				ffin.read((char*)m_normals, size);
			}
		doc.setParent();

		ffin.close();

	doc.free();

}

void FXMLMesh::setMotion(float open, float close)
{
	m_pOpen = new XYZ[m_numVertex];
	m_pClose = new XYZ[m_numVertex];
	
	for(int i=0; i<m_numVertex; i++) m_pOpen[i] = m_pClose[i] = m_cvs[i];
	
	SHelper::changeFrameNumber(m_xml_name, int(close+1));
	
	ifstream ffin;
	int pos, size;
	if(doc.load(m_xml_name.c_str()) != 1) return;
	
	doc.setChildren();
	
	if(findMesh(m_meshName.c_str()) !=1) return;
	
		doc.getChildByName("dynamic");
		m_dynamicName = m_xml_name;
		SHelper::changeFilenameExtension(m_dynamicName, "dyn");
		
		ffin.open(m_dynamicName.c_str(), ios::in | ios::binary);
		
			doc.getChildByName("P");
			m_numVertex = doc.getIntAttribByName("count");
			pos = doc.getIntAttribByName("loc");
			size = doc.getIntAttribByName("size");
			
			ffin.seekg( pos, ios::beg );
			ffin.read((char*)m_pClose, size);
		
		doc.setParent();
		
		ffin.close();
		
	doc.free();
	
	XYZ pbuf, delta_v;
	float len_f = (int)close - (int)open + 1;
	float len_o = (open - (int)open)/len_f;
	float len_c = (close - (int)open)/len_f;
	for(int i=0; i<m_numVertex; i++) 
	{
		delta_v = m_pClose[i] - m_pOpen[i];
		pbuf = m_pOpen[i];
		m_pOpen[i] = pbuf + delta_v * len_o;
		m_pClose[i] = pbuf + delta_v * len_c;
	}
}

void FXMLMesh::getCenterAndSize(XYZ& center, float& size) const
{
	center = m_bbox0 + m_bbox1;
	center = center / 2.0f;
	float dx = m_bbox1.x - m_bbox0.x;
	float dy = m_bbox1.y - m_bbox0.y;
	float dz = m_bbox1.z - m_bbox0.z;
	
	if(dx > dy && dx >dz) size = dx;
		else if(dy > dx && dy >dz) size = dy;
			else size = dz;
}

void FXMLMesh::resetColor()
{
	//if(m_has_color) for(int i = 0; i < m_numVertex; i++) m_draw_color[i] = m_color[i];
	//else for(int i = 0; i < m_numVertex; i++) m_color[i] = m_draw_color[i] = XYZ(1.0f);
}
/*
void FXMLMesh::updateColor(int opt)
{
	switch (opt)
	{
		case 0: 
			resetColor();
			break;
		case 1: 
			calcOcc();
			break;
		case 2: 
			calcHDRF("prt");
			break;
		case 3: 
			calcHDRC("prti");
			break;
		case 4: 
			calcHDRF("prts");
			break;
		case 5: 
			calcHDRF("prte");
			break;
		case 6: 
			calcHDRF("prtb");
			break;
		case 7: 
			calcSRC();
			break;
		default:       
		break;
	}
}

void FXMLMesh::calcOcc()
{
	CoeRec* vex_coe = new CoeRec[m_numVertex];
	
	string rtName = m_xml_name;
	SHelper::replacefilename(rtName, m_meshName);
	SHelper::changeFilenameExtension(rtName, "prtu");
	
	ifstream ifile;
	ifile.open(rtName.c_str(), ios::in | ios::binary | ios::ate);
	
	if(ifile.is_open())
	{
		ifstream::pos_type size = ifile.tellg();
		
		if((int)size == m_numVertex*16*sizeof(float))
		{
			ifile.seekg(0, ios::beg);
			ifile.read((char*)vex_coe, m_numVertex*16*sizeof(float));
			ifile.close();
		}
	}
	
	float ov;
	for(int i = 0; i < m_numVertex; i++) 
	{
		ov = 0;
		for(int j = 0; j < 16; j++) ov += vex_coe[i].data[j]*constantCoeff[j];
		ov /= 6.28;
		m_draw_color[i] = XYZ(ov);
	}
	
	delete[] vex_coe;
}

void FXMLMesh::calcHDRF(const char* ext)
{
	CoeRec* vex_coe = new CoeRec[m_numVertex];
	
	string rtName = m_xml_name;
	SHelper::replacefilename(rtName, m_meshName);
	SHelper::changeFilenameExtension(rtName, ext);
	
	ifstream ifile;
	ifile.open(rtName.c_str(), ios::in | ios::binary | ios::ate);
	
	if(ifile.is_open())
	{
		ifstream::pos_type size = ifile.tellg();
		
		if((int)size == m_numVertex*16*sizeof(float))
		{
			ifile.seekg(0, ios::beg);
			ifile.read((char*)vex_coe, m_numVertex*16*sizeof(float));
			ifile.close();
		}
	}
	
	float r, g, b;
	for(int i = 0; i < m_numVertex; i++) 
	{
		r =g =b = 0;
		for(int j = 0; j < 16; j++) 
		{
			r += vex_coe[i].data[j]*usr_hdrCoeff[j].x;
			g += vex_coe[i].data[j]*usr_hdrCoeff[j].y;
			b += vex_coe[i].data[j]*usr_hdrCoeff[j].z;
		}
		m_draw_color[i] = XYZ(r,g,b);
	}
	
	delete[] vex_coe;
}

void FXMLMesh::calcHDRC(const char* ext)
{
	XYZ* vex_coe = new XYZ[m_numVertex*16];
	
	string rtName = m_xml_name;
	SHelper::replacefilename(rtName, m_meshName);
	SHelper::changeFilenameExtension(rtName, ext);
	
	ifstream ifile;
	ifile.open(rtName.c_str(), ios::in | ios::binary | ios::ate);
	
	if(ifile.is_open())
	{
		ifstream::pos_type size = ifile.tellg();
		
		if((int)size == m_numVertex*16*sizeof(XYZ))
		{
			ifile.seekg(0, ios::beg);
			ifile.read((char*)vex_coe, m_numVertex*16*sizeof(XYZ));
			ifile.close();
		}
	}
	
	float r, g, b;
	for(int i = 0; i < m_numVertex; i++) 
	{
		r =g =b = 0;
		for(int j = 0; j < 16; j++) 
		{
			r += vex_coe[i*16+j].x*usr_hdrCoeff[j].x;
			g += vex_coe[i*16+j].y*usr_hdrCoeff[j].y;
			b += vex_coe[i*16+j].z*usr_hdrCoeff[j].z;
		}
		m_draw_color[i] = XYZ(r,g,b);
	}
	
	delete[] vex_coe;
}

void FXMLMesh::calcSRC()
{
	XYZ* vex_coe = new XYZ[m_numVertex*16];
	
	string rtName = m_xml_name;
	SHelper::replacefilename(rtName, m_meshName);
	SHelper::changeFilenameExtension(rtName, "prtl");
	
	ifstream ifile;
	ifile.open(rtName.c_str(), ios::in | ios::binary | ios::ate);
	
	if(ifile.is_open())
	{
		ifstream::pos_type size = ifile.tellg();
		
		if((int)size == m_numVertex*16*sizeof(XYZ))
		{
			ifile.seekg(0, ios::beg);
			ifile.read((char*)vex_coe, m_numVertex*16*sizeof(XYZ));
			ifile.close();
		}
	}
	
	float r, g, b;
	for(int i = 0; i < m_numVertex; i++) 
	{
		r =g =b = 0;
		for(int j = 0; j < 16; j++) 
		{
			r += vex_coe[i*16+j].x*constantCoeff[j];
			g += vex_coe[i*16+j].y*constantCoeff[j];
			b += vex_coe[i*16+j].z*constantCoeff[j];
		}
		m_draw_color[i] = XYZ(r,g,b);
	}
	
	delete[] vex_coe;
}
*/
void FXMLMesh::appendColorSet(const char* paramname, const char* filename)
{
	ifstream ffin;
	ffin.open(filename, ios::in | ios::binary | ios::ate);
	if(!ffin.is_open()) 
	{
		printf("ERROR: Cannot open %s\n", filename);
		return;
	}
	
	ifstream::pos_type size = ffin.tellg();
	
	char is_float = 0;
	if((int)size == sizeof(float)*16*m_numVertex) is_float = 1;
	
	ffin.seekg( 0, ios::beg );
	
	float* fbuf;

	if(is_float) 
	{
		fbuf = new float[m_numVertex*16];
		ffin.read( (char*)fbuf, sizeof(float)*16*m_numVertex );
	}
	else 
	{
		fbuf = new float[m_numVertex*48];
		ffin.read( (char*)fbuf, sizeof(float)*48*m_numVertex );
	}
	
	ffin.close();
	
	char sbuf[64];
	
	if(is_float) 
	{
		for(int j=0; j<16; j++)
		{
			FColorSet* colset = new FColorSet();
			sprintf(sbuf, "%s%i", paramname, j);
				
			colset->name = sbuf;
			colset->data = new XYZ[m_numVertex];
		
			for(int i=0; i<m_numVertex; i++) colset->data[i] = XYZ(fbuf[i*16+j]);
					
			m_colorSet.push_back(colset);
		}
	}
	else
	{
		for(int j=0; j<16; j++)
		{
			FColorSet* colset = new FColorSet();
			sprintf(sbuf, "%s%i", paramname, j);
				
			colset->name = sbuf;
			colset->data = new XYZ[m_numVertex];
		
			for(int i=0; i<m_numVertex; i++) colset->data[i] = XYZ(fbuf[i*48+j*3], fbuf[i*48+j*3+1], fbuf[i*48+j*3+2]);
					
			m_colorSet.push_back(colset);
		}
	}
	delete[] fbuf;
}

void FXMLMesh::appendFloatSet(const char* paramname, const char* filename)
{
	ifstream ffin;
	ffin.open(filename, ios::in | ios::binary | ios::ate);
	if(!ffin.is_open()) 
	{
		printf("ERROR: Cannot open %s\n", filename);
		return;
	}
	
	ifstream::pos_type size = ffin.tellg();
	
	char is_float = 0;
	if((int)size == sizeof(float)*16*m_numVertex) is_float = 1;
	
	ffin.seekg( 0, ios::beg );
	
	float* fbuf;

	if(is_float) 
	{
		fbuf = new float[m_numVertex*16];
		ffin.read( (char*)fbuf, sizeof(float)*16*m_numVertex );
	}
	else 
	{
		fbuf = new float[m_numVertex*48];
		ffin.read( (char*)fbuf, sizeof(float)*48*m_numVertex );
	}
	
	ffin.close();
	
	char sbuf[64];
	
	if(is_float) 
	{
		for(int j=0; j<16; j++)
		{
			FFloatSet* colset = new FFloatSet();
			sprintf(sbuf, "%s%i", paramname, j);
				
			colset->name = sbuf;
			colset->data = new float[m_numVertex];
		
			for(int i=0; i<m_numVertex; i++) colset->data[i] = fbuf[i*16+j];
					
			m_floatSet.push_back(colset);
		}
	}
	else
	{
		for(int j=0; j<16; j++)
		{
			FFloatSet* colset = new FFloatSet();
			sprintf(sbuf, "%s%i", paramname, j);
				
			colset->name = sbuf;
			colset->data = new float[m_numVertex];
		
			for(int i=0; i<m_numVertex; i++) colset->data[i] = fbuf[i*48+j*3];
					
			m_floatSet.push_back(colset);
		}
	}
	delete[] fbuf;
}
/*
void FXMLMesh::dice(float epsilon, pcdSample* res, int& n_res)
{
	int a, b, c;
	FTriangle ftri;
	//float min_area = epsilon*epsilon/32;
	for(int i=0; i<m_numTriangle; i++)
	{
		a = m_triangleConn[i*3];
		b = m_triangleConn[i*3+1];
		c = m_triangleConn[i*3+2];
		
		ftri.create(m_cvs[a], m_cvs[b], m_cvs[c]);
		ftri.setNormal(m_normals[a], m_normals[b], m_normals[c]);
		ftri.setColor(m_color[a], m_color[b], m_color[c]);
		ftri.setTangent(m_tangents[a], m_tangents[b], m_tangents[c]);
		
		ftri.rasterize(epsilon, res, n_res);
	}
}

void FXMLMesh::diceWithRT(float epsilon, pcdSample* res, CoeRec* coe_res, int& n_res)
{
	CoeRec* vex_coe = new CoeRec[m_numVertex];
	
	string rtName = m_xml_name;
	SHelper::replacefilename(rtName, m_meshName);
	SHelper::changeFilenameExtension(rtName, "prt");
	
	ifstream ifile;
	ifile.open(rtName.c_str(), ios::in | ios::binary | ios::ate);
	
	if(ifile.is_open())
	{
		ifstream::pos_type size = ifile.tellg();
		
		if((int)size == m_numVertex*16*sizeof(float))
		{
			ifile.seekg(0, ios::beg);
			ifile.read((char*)vex_coe, m_numVertex*16*sizeof(float));
			ifile.close();
		}
	}
	
	int a, b, c;
	FTriangle ftri;
	//float min_area = epsilon*epsilon/32;
	for(int i=0; i<m_numTriangle; i++)
	{
		a = m_triangleConn[i*3];
		b = m_triangleConn[i*3+1];
		c = m_triangleConn[i*3+2];
		
		ftri.create(m_cvs[a], m_cvs[b], m_cvs[c]);
		ftri.setNormal(m_normals[a], m_normals[b], m_normals[c]);
		ftri.setColor(m_color[a], m_color[b], m_color[c]);
		ftri.setTangent(m_tangents[a], m_tangents[b], m_tangents[c]);
		ftri.setId(a, b, c);
		
		ftri.rasterize(epsilon, vex_coe, res, coe_res, n_res);
	}	
	delete[] vex_coe;	
}

int FXMLMesh::checkExistingRT() const
{
	string rtName = m_xml_name;
	SHelper::changeFilenameExtension(rtName, "prt");
	
	ifstream ifile;
	ifile.open(rtName.c_str(), ios::in | ios::binary | ios::ate);
	
	if(!ifile.is_open()) return 0;

	ifstream::pos_type size = ifile.tellg();
	
	if((int)size < m_numVertex*48*sizeof(float)) return 0;
	
	return 1;
}
*/
int FXMLMesh::hasAttrib(const char* attribname) const
{
	AttribList list = m_attrib;
	for(AttribList::iterator it=list.begin(); it != list.end(); ++it)
	{
		if((*it)==string(attribname)) return 1;
	}
	return 0;
}
/*
void FXMLMesh::setHDRLighting(const XYZ* coeff)
{
	for(unsigned i=0; i<16; i++) usr_hdrCoeff[i] = coeff[i];
}

void FXMLMesh::resetHDRLighting()
{
	for(unsigned i=0; i<16; i++) usr_hdrCoeff[i] = XYZ(hdrCoeff[i*3], hdrCoeff[i*3+1], hdrCoeff[i*3+2]);
}
*/
void FXMLMesh::interpolateRT(int lo, int hi, int frame, float weight, const char* extension, int fpv)
{
	float* vex_coe = new float[m_numVertex*fpv];
	
	string rtName = m_xml_name;
	SHelper::replacefilename(rtName, m_meshName);
	SHelper::changeFilenameExtension(rtName, extension);
	SHelper::changeFrameNumber(rtName, lo);
	
	ifstream ifile;
	ifile.open(rtName.c_str(), ios::in | ios::binary | ios::ate);
	
	if(!ifile.is_open()) {
		delete[] vex_coe;
		return;
	}

	ifstream::pos_type size = ifile.tellg();
	
	if((int)size == m_numVertex*fpv*sizeof(float)) {
		ifile.seekg(0, ios::beg);
		ifile.read((char*)vex_coe, m_numVertex*fpv*sizeof(float));
		ifile.close();
	}
	
	if(weight < 1.f) {
		float wei0 = 1.f - weight;
		float* vex_coe1 = new float[m_numVertex*fpv];
	
		SHelper::changeFrameNumber(rtName, hi);
		
		ifile.open(rtName.c_str(), ios::in | ios::binary | ios::ate);
		
		if(ifile.is_open()) {

			ifstream::pos_type size = ifile.tellg();
			
			if((int)size == m_numVertex*fpv*sizeof(float)) {
				ifile.seekg(0, ios::beg);
				ifile.read((char*)vex_coe1, m_numVertex*fpv*sizeof(float));
				ifile.close();
			}
			
			for(unsigned i=0; i<m_numVertex*fpv; i++) {
				vex_coe[i] = vex_coe1[i] * weight + vex_coe[i] * wei0;
			}
		}
		delete[] vex_coe1;	
	}

	SHelper::changeFrameNumber(rtName, frame);
	ofstream ofile;
	ofile.open(rtName.c_str(), ios::out | ios::binary);
	if(!ofile.is_open()) return;
	
	ofile.write((char*)vex_coe, m_numVertex*fpv*sizeof(float));
	ofile.close();
	
	delete[] vex_coe;	
}
/*
void FXMLMesh::depthMap(float* data, int map_w, int map_h, MATRIX44F& space, double& fov) const
{
	XYZ pa, pb, pc;
	FTriangle ftri;
	for(int i=0; i<m_numTriangle; i++) {
		pa = m_cvs[m_triangleConn[i*3]];
		pb = m_cvs[m_triangleConn[i*3+1]];
		pc = m_cvs[m_triangleConn[i*3+2]];
		
		space.transform(pa);
		space.transform(pb);
		space.transform(pc);
		
		if(pa.z < 0.1 || pb.z < 0.1 || pc.z < 0.1) continue;
		
		ftri.createSimple(pa, pb, pc);
		ftri.project(fov);
		ftri.onScreen(data, map_w, map_h);
	}
}

void FXMLMesh::depthMapPersp(float* data, int map_w, int map_h, MATRIX44F& space, float& fov) const
{
	XYZ pa, pb, pc;
	FTriangle ftri;
	for(int i=0; i<m_numTriangle; i++) {
		pa = m_cvs[m_triangleConn[i*3]];
		pb = m_cvs[m_triangleConn[i*3+1]];
		pc = m_cvs[m_triangleConn[i*3+2]];
		
		space.transform(pa);
		space.transform(pb);
		space.transform(pc);
		
		if(pa.z < 0.1 || pb.z < 0.1 || pc.z < 0.1) continue;
		
		ftri.createSimple(pa, pb, pc);
		double dfov = fov/180*3.1415927;
		ftri.project(dfov);
		ftri.onScreen(data, map_w, map_h);
	}
}

void FXMLMesh::depthMapOrtho(float* data, int map_w, int map_h, MATRIX44F& space, float& orthow) const
{
	XYZ pa, pb, pc;
	FTriangle ftri;
	for(int i=0; i<m_numTriangle; i++) {
		pa = m_cvs[m_triangleConn[i*3]];
		pb = m_cvs[m_triangleConn[i*3+1]];
		pc = m_cvs[m_triangleConn[i*3+2]];
		
		space.transform(pa);
		space.transform(pb);
		space.transform(pc);
		
		if(pa.z < 0.1 || pb.z < 0.1 || pc.z < 0.1) continue;
		
		ftri.createSimple(pa, pb, pc);
		ftri.projectOrtho(orthow);
		ftri.onScreen(data, map_w, map_h);
	}
}*/
//~:

