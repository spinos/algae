/*
 *  XMLUtil.h
 *  hairBase
 *
 *  Created by zhang on 08-12-17.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _XML_UTILITY_H
#define _XML_UTILITY_H


#include "TVectors.h"
#include <string>
#include "../shared/zXMLDoc.h"
#include <fstream>
#include <vector>
using namespace std;

class XMLUtil
{
public:
	XMLUtil() {}
	~XMLUtil() {}

	static char findByNameAndType(const char* filename, const char* name, const char* type, ZXMLDoc& doc);
	static char findByNameAndType(const char* name, const char* type, ZXMLDoc& doc);
	static char readBBox(XYZ& bbmin, XYZ& bbmax, ZXMLDoc& doc);
	static char readDynBBox(const char* filename, XYZ& bbmin, XYZ& bbmax);
	static char readBBox(const char* filename, XYZ& bbmin, XYZ& bbmax);
	static void getMeshes(const char* filename, vector<string>& res);
};
#endif
//:~