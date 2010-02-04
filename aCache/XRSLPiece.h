/*
 *  XSLPiece.h
 *  acache
 *
 *  Created by jian zhang on 1/29/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <string>
#include <vector>
using namespace std;

enum XRSLParameterDetail {
	Simple = 0,
	Slider = 1,
	Switch = 2,
	Connection = 3,
	Output = 4,
	Varying = 5
};

enum XRSLParameterType {
	Float = 0,
	Color = 1,
	String = 2
};

struct XRSLParameter
{
	string name;
	float r, g, b;
	float min, max;
	string v;
	XRSLParameterType type;
	XRSLParameterDetail detail;
};

typedef vector<XRSLParameter*>ParamList;

class XRSLPiece
{
public:
	XRSLPiece();
	~XRSLPiece();
	char isValid() const {return m_isValid;}
	char load(const char* filename);
	
// query
	const char* getName() const {return m_name.c_str();}
	const char* getType() const {return m_type.c_str();}
	ParamList& getAttrib() {return m_param;}
	const char* getBody() const {return m_source.c_str();}
private:
	string m_sl_path;
	string m_name;
	string m_type;
	string m_source;
	ParamList m_param;
	char m_isValid;
};