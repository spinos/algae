/*
 *  GLSLACache.h
 *  pmap
 *
 *  Created by jian zhang on 12/20/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _GLSL_ANEMONE_H
#define _GLSL_ANEMONE_H

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glext.h>
#endif

#ifdef LINUX
#include <GL/glx.h>
#include <GL/glxext.h>
#include <GL/glext.h>
#endif

struct GLSLACachePARAM
{
	float clip_near, clip_far;
	float base_r, base_g, base_b;
};

class GLSLACache
{
public:
	GLSLACache();
	~GLSLACache();
	
	char initProgram();
	char isValid() const {return m_isValid;}
	void enable();
	void disable();
	void setParam(GLSLACachePARAM& param);
	GLuint getProgram() const {return program;}
	
private:
	GLuint program;
	char m_isValid;
};
#endif