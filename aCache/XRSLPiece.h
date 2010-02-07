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

enum XRSLParameterAccess {
	Internal = 0,
	Uniform = 1,
	Varying = 2,
	Output = 3
};

enum XRSLParameterDetail {
	Simple = 0,
	Slider = 1,
	Switch = 2,
	Connection = 3
};

enum XRSLParameterType {
	RSLFloat = 0,
	RSLColor = 1,
	RSLString = 2,
	RSLVector = 3,
	RSLPoint = 4
};

struct XRSLParameter
{
	string name;
	float r, g, b;
	float min, max;
	string v;
	XRSLParameterType type;
	XRSLParameterDetail detail;
	XRSLParameterAccess access;
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
	const char* getIcon() const {return m_icon.c_str();}
private:
	string m_sl_path;
	string m_name;
	string m_type;
	string m_source;
	string m_icon;
	ParamList m_param;
	char m_isValid;
};