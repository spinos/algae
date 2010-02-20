/*
 *  RenderACache.h
 *  acache
 *
 *  Created by jian zhang on 1/25/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _RENDER_A_CACHE_H
#define _RENDER_A_CACHE_H

#ifdef WIN32
#include <windows.h>
#endif


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

class RenderACache
{
public:
	RenderACache();
	~RenderACache();
	
	void initialize();
	void uninitialize();
	
	char isInitialized() const { return m_isInitialized; }
	
	void setTriangles(int *triidx, unsigned int num_idx);
	void setP(float *p, unsigned int num_vert);
	void draw();
	void enableProgram(GLuint p);
	void disableProgram();
	
private:
	char m_isInitialized;
	GLuint ibo;
	GLuint vbo;
	unsigned int m_n_draw;
};
#endif