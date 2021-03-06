/*
 *  XRSLPiece.cpp
 *  acache
 *
 *  Created by jian zhang on 1/29/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "XRSLPiece.h"

#include "../shared/zXMLDoc.h"

XRSLPiece::XRSLPiece() : m_isValid(0) {}
XRSLPiece::~XRSLPiece() {}

char XRSLPiece::load(const char* filename)
{
	m_icon = "anm_default.xpm";
	m_sl_path = filename;
	
	ZXMLDoc doc;
	if(doc.load(filename) != 1) return 0;
	
	m_name = doc.getAttribByName("name");
	m_type = doc.getAttribByName("type");
	m_description = "no description";
	
	doc.setChildren();
	
	while(doc.isLastNode() != 1) {
		if(doc.checkNodeName("icon") == 1) {
			m_icon = doc.getNodeContent();
		}
		else if(doc.checkNodeName("source") == 1) {
			m_source = doc.getNodeContent();
			//printf(m_source.c_str());
		}
		else if(doc.checkNodeName("description") == 1) {
			m_description = doc.getNodeContent();
		}
		else if(doc.checkNodeName("parameter") == 1) {
			XRSLParameter *aparam = new XRSLParameter();
			aparam->detail = Simple;
			aparam->access = Internal;
			aparam->min = 0.f;
			aparam->max = 1.f;
			aparam->name = doc.getAttribByName("name");
			
			const char* typestr = doc.getAttribByName("type");
			if(strcmp(typestr, "float") == 0) {
				aparam->type = RSLFloat;
				aparam->r = doc.getFloatAttribByName("value");
			}
			else if(strcmp(typestr, "color") == 0) {
				aparam->type = RSLColor;
				doc.getFloat3AttribByName("value", aparam->r, aparam->g, aparam->b);
			}
			else if(strcmp(typestr, "string") == 0) {
				aparam->type = RSLString;
				aparam->v = doc.getAttribByName("value");
			}
			else if(strcmp(typestr, "vector") == 0) {
				aparam->type = RSLVector;
				doc.getFloat3AttribByName("value", aparam->r, aparam->g, aparam->b);
			}
			else if(strcmp(typestr, "point") == 0) {
				aparam->type = RSLPoint;
				doc.getFloat3AttribByName("value", aparam->r, aparam->g, aparam->b);
			}
			
			if(doc.isEmpty() != 1) {
				doc.setChildren();
				while(doc.isLastNode()!=1)
				{
					if(doc.checkNodeName("range") ==1) {
						aparam->min = doc.getFloatAttribByName("min");
						aparam->max = doc.getFloatAttribByName("max");
						//printf("set range");
					}
					else if(doc.checkNodeName("detail") ==1) {
						if(strcmp(doc.getAttribByName("value"), "slider") == 0) aparam->detail = Slider;	
						else if(strcmp(doc.getAttribByName("value"), "switch") == 0) aparam->detail = Switch;
						else if(strcmp(doc.getAttribByName("value"), "connection") == 0) aparam->detail = Connection;
					}
					else if(doc.checkNodeName("access") ==1) {
						if(strcmp(doc.getAttribByName("value"), "uniform") == 0) aparam->access = Uniform;	
						else if(strcmp(doc.getAttribByName("value"), "varying") == 0) aparam->access = Varying;
						else if(strcmp(doc.getAttribByName("value"), "output") == 0) aparam->access = Output;
					}
					doc.nextNode();
				}
				doc.setParent();
			}
			m_param.push_back(aparam);
		}
		doc.nextNode();
	}
	
	doc.setParent();
	doc.free();
	
	m_isValid = 1;
	return m_isValid;
}