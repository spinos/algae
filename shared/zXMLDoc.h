/*
 *  zXMLDoc.h
 *  xmlTest
 *
 *  Created by zhang on 07-11-12.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _ZXMLDOC_H
#define _ZXMLDOC_H
#include <stdio.h>
#include <string.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <libxml/xmlreader.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

class ZXMLDoc
{
public:
	ZXMLDoc();
	~ZXMLDoc();

	void begin();
	void end(const char *file);
	
	void elementBegin(const char* name);
	void elementEnd();
	
	void addAttribute(const char* name, const char* value);
	void addAttribute(const char* name, float value);
	void addAttribute(const char* name, float x, float y, float z);
	void addAttribute(const char* name, int value);

	void addContent(const char* value);
	void addContent(float value);
	void addContent(int value);
	void addContent(float r, float g, float b);
	
	int load(const char* file);
	void free();
	void reset();
	int isEmpty();
	void setParent();
	void setChildren();
	void nextNode();
	int isLastNode();
	int getChildByName(const char* name);
	int checkNodeName(const char* name);
	const char* getNodeName();
	const char* getNodeContent();
	int getIntNodeContent();
	float getFloatNodeContent();
	const char* getAttribByName(const char* name);
	int getIntAttribByName(const char* name);
	float getFloatAttribByName(const char* name);
	void getFloat3AttribByName(const char* name, float& x, float& y, float& z);
	
private:
	
	xmlTextWriterPtr writer;
    xmlDocPtr doc;
	int rc;
	xmlChar *tmp;
	xmlNodePtr cur;
};
#endif
//:~