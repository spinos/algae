/*
 *  XMLUtil.cpp
 *  hairBase
 *
 *  Created by zhang on 08-12-17.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "XMLUtil.h"

char XMLUtil::readDynBBox(const char* filename, XYZ& bbmin, XYZ& bbmax)
{
	ZXMLDoc doc;
	if(doc.load(filename) != 1) return 0;
	
	char res = 0;
		doc.getChildByName("dynamic");
		
			if(doc.getChildByName("bbox") != 0)
			{
				doc.getFloat3AttribByName("low", bbmin.x, bbmin.y, bbmin.z);
				doc.getFloat3AttribByName("high", bbmax.x, bbmax.y, bbmax.z);
				res = 1;
			}
			
		doc.setParent();
		
	doc.free();

	return res;
}

char XMLUtil::readBBox(const char* filename, XYZ& bbmin, XYZ& bbmax)
{
	ZXMLDoc doc;
	if(doc.load(filename) != 1) return 0;
	
	char res = 0;
		
		
			if(doc.getChildByName("bbox") != 0)
			{
				doc.getFloat3AttribByName("low", bbmin.x, bbmin.y, bbmin.z);
				doc.getFloat3AttribByName("high", bbmax.x, bbmax.y, bbmax.z);
				res = 1;
			}
			
		
		
	doc.free();

	return res;
}

void XMLUtil::getMeshes(const char* filename, vector<string>& res)
{
	ZXMLDoc doc;
	if(doc.load(filename) != 1) return;
	doc.setChildren();
	while(doc.isLastNode()!=1)
	{
		if(doc.checkNodeName("mesh") ==1) res.push_back(doc.getAttribByName("path"));
		doc.nextNode();
	}
		
	doc.free();

	return;
}

char XMLUtil::findByNameAndType(const char* filename, const char* name, const char* type, ZXMLDoc& doc)
{
	
	if(doc.load(filename) != 1) return 0;
	doc.setChildren();
	while(doc.isLastNode() != 1)
	{
		if(doc.checkNodeName(type) == 1)
		{
			if(strcmp ( doc.getAttribByName("name"), name )==0) return 1;
		}
		doc.nextNode();
	}
	
	return 0;
}

char XMLUtil::findByNameAndType(const char* name, const char* type, ZXMLDoc& doc)
{
	while(doc.isLastNode() != 1)
	{
		if(doc.checkNodeName(type) == 1)
		{
			if(strcmp ( doc.getAttribByName("name"), name )==0) return 1;
		}
		doc.nextNode();
	}
	return 0;
}

char XMLUtil::readBBox(XYZ& bbmin, XYZ& bbmax, ZXMLDoc& doc)
{
	if(doc.getChildByName("bbox") == 0) return 0;
	
	doc.getFloat3AttribByName("low", bbmin.x, bbmin.y, bbmin.z);
	doc.getFloat3AttribByName("high", bbmax.x, bbmax.y, bbmax.z);

	return 1;
}
//:~