/*
 *  zXMLDoc.cpp
 *  xmlTest
 *
 *  Created by zhang on 07-11-12.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "zXMLDoc.h"

//#define MY_ENCODING "ISO-8859-1"
#define MY_ENCODING "UTF-8"

xmlChar* ConvertInput(const char *in, const char *encoding)
{
	 xmlChar *out;
    int ret;
    int size;
    int out_size;
    int temp;
    xmlCharEncodingHandlerPtr handler;

    if (in == 0)
        return 0;

    handler = xmlFindCharEncodingHandler(encoding);

    if (!handler) {
        printf("ConvertInput: no encoding handler found for '%s'\n",
               encoding ? encoding : "");
        return 0;
    }

    size = (int) strlen(in) + 1;
    out_size = size * 2 - 1;
    out = (unsigned char *) xmlMalloc((size_t) out_size);

    if (out != 0) {
        temp = size - 1;
        ret = handler->input(out, &out_size, (const xmlChar *) in, &temp);
        if ((ret < 0) || (temp - size + 1)) {
            if (ret < 0) {
                printf("ConvertInput: conversion wasn't successful.\n");
            } else {
                printf
                    ("ConvertInput: conversion wasn't successful. converted: %i octets.\n",
                     temp);
            }

            xmlFree(out);
            out = 0;
        } else {
            out = (unsigned char *) xmlRealloc(out, out_size + 1);
            out[out_size] = 0;  /*null terminating out */
        }
    } else {
        printf("ConvertInput: no mem\n");
    }

    return out;

}

ZXMLDoc::ZXMLDoc():tmp(0)
{

}

ZXMLDoc::~ZXMLDoc()
{
	if(tmp) xmlFree(tmp);
}

void ZXMLDoc::begin()
{
	/* Create a new XmlWriter for DOM, with no compression. */
    writer = xmlNewTextWriterDoc(&doc, 0);
    if (writer == NULL) {
        printf("testXmlwriterDoc: Error creating the xml writer\n");
        return;
    }
	
	/* Start the document with the xml default for the version,
     * encoding ISO 8859-1 and the default for the standalone
     * declaration. */
    rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
    if (rc < 0) {
        printf("testXmlwriterDoc: Error at xmlTextWriterStartDocument\n");
        return;
    }
}

void ZXMLDoc::end(const char *file)
{

    /* Here we could close the elements ORDER and EXAMPLE using the
     * function xmlTextWriterEndElement, but since we do not want to
     * write any other elements, we simply call xmlTextWriterEndDocument,
     * which will do all the work.
    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0) {
        printf("testXmlwriterDoc: Error at xmlTextWriterEndDocument\n");
        return;
    } */

    xmlFreeTextWriter(writer);
	
    //xmlSaveFileEnc(file, doc, MY_ENCODING);
	xmlSaveFormatFile (file, doc, 1);

    xmlFreeDoc(doc);

}

void ZXMLDoc::elementBegin(const char* name)
{
	rc = xmlTextWriterStartElement(writer, BAD_CAST name);
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterStartElement\n");
        return;
    }
}

void ZXMLDoc::elementEnd()
{
	rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf("testXmlwriterTree: Error at xmlTextWriterEndElement\n");
        return;
    }
}

void ZXMLDoc::addAttribute(const char* name, const char* value)
{
	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST name,
                                     BAD_CAST value);
    if (rc < 0) 
	{
        printf("testXmlwriterDoc: Error at xmlTextWriterWriteAttribute\n");
        return;
    }
}

void ZXMLDoc::addAttribute(const char* name, float value)
{
	rc = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST name,
                                     "%f", value);
    if (rc < 0) 
	    {
        printf("testXmlwriterDoc: Error at xmlTextWriterWriteAttribute\n");
        return;
    }
}

void ZXMLDoc::addAttribute(const char* name, float x, float y, float z)
{
	rc = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST name,
                                     "%f %f %f", x, y, z);
    if (rc < 0) 
	    {
        printf("testXmlwriterDoc: Error at xmlTextWriterWriteAttribute\n");
        return;
    }
}

void ZXMLDoc::addAttribute(const char* name, int value)
{
	rc = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST name,
                                     "%d", value);

    if (rc < 0) 
	    {
        printf("testXmlwriterDoc: Error at xmlTextWriterWriteAttribute\n");
        return;
    }
}

void ZXMLDoc::addContent(const char* value)
{
	rc = xmlTextWriterWriteFormatRaw(writer, "%s", value);

	if (rc < 0) 
	{
		printf("testXmlwriterDoc: Error at xmlTextWriterWriteElement\n");
		return;
	}
}

void ZXMLDoc::addContent(float value)
{
	rc = xmlTextWriterWriteFormatRaw(writer, "%f", value);

	if (rc < 0) 
	{
		printf("testXmlwriterDoc: Error at xmlTextWriterWriteElement\n");
		return;
	}
}

void ZXMLDoc::addContent(int value)
{
	rc = xmlTextWriterWriteFormatRaw(writer, "%d", value);

	if (rc < 0) 
	{
		printf("testXmlwriterDoc: Error at xmlTextWriterWriteElement\n");
		return;
	}
}

void ZXMLDoc::addContent(float r, float g, float b)
{
	char sbuf[64];
	sprintf(sbuf, "%f %f %f", r, g, b);
	
	rc = xmlTextWriterWriteFormatRaw(writer, "%s", sbuf);

	if (rc < 0) 
	{
		printf("testXmlwriterDoc: Error at xmlTextWriterWriteElement\n");
		return;
	}
}

int ZXMLDoc::load(const char* file)
{
	doc = xmlParseFile(file);
	if(!doc) return 0;
	
	cur = xmlDocGetRootElement(doc);
	
	if(!cur) return 0;
	
	return 1;
}

void ZXMLDoc::free()
{
	xmlFreeDoc(doc);
}

void ZXMLDoc::reset()
{
	cur = xmlDocGetRootElement(doc);
}

int ZXMLDoc::isEmpty()
{
	if(cur->xmlChildrenNode == NULL) return 1;
	else return 0;
}

void ZXMLDoc::setParent()
{
	cur = cur->parent;
}

void ZXMLDoc::setChildren()
{
	cur = cur->xmlChildrenNode;
}

void ZXMLDoc::nextNode()
{
	cur = cur->next;
}

int ZXMLDoc::isLastNode()
{
	if(cur->next == NULL) return 1;
	else return 0;
}

const char* ZXMLDoc::getNodeName()
{
	return (const char*)(cur->name);
}

const char* ZXMLDoc::getNodeContent()
{
	if(tmp) xmlFree(tmp);
	tmp = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
	return (const char*)(tmp);
}

int ZXMLDoc::getIntNodeContent()
{
	if(tmp) xmlFree(tmp);
	tmp = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
	int val;
	sscanf( (const char*)(tmp), "%d", &val);
	return val;
}

float ZXMLDoc::getFloatNodeContent()
{
	if(tmp) xmlFree(tmp);
	tmp = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
	float val;
	sscanf( (const char*)(tmp), "%f", &val);
	return val;
}

int ZXMLDoc::getChildByName(const char* name)
{
	//if(isEmpty() == 1) return 0;
	
	cur = cur->xmlChildrenNode;
	while (isLastNode() != 1) 
	{
		if ((!xmlStrcmp(cur->name, (const xmlChar *)name))) return 1;
		nextNode();
	}
	return 0;
}

int ZXMLDoc::checkNodeName(const char* name)
{
	if ((!xmlStrcmp(cur->name, (const xmlChar *)name))) return 1;
	else return 0;
}

const char* ZXMLDoc::getAttribByName(const char* name)
{
	return (const char*)xmlGetProp(cur, (const xmlChar *)name);
}

int ZXMLDoc::getIntAttribByName(const char* name)
{
	int val;
	sscanf( (const char*)xmlGetProp(cur, (const xmlChar *)name), "%d", &val);
	return val;
}

float ZXMLDoc::getFloatAttribByName(const char* name)
{
	float val;
	sscanf( (const char*)xmlGetProp(cur, (const xmlChar *)name), "%f", &val);
	return val;
}

void ZXMLDoc::getFloat3AttribByName(const char* name, float& x, float& y, float& z)
{
	sscanf( (const char*)xmlGetProp(cur, (const xmlChar *)name), "%f %f %f", &x, &y, &z);
}
//:~

