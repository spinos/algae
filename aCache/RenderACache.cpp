/*
 *  RenderACache.cpp
 *  acache
 *
 *  Created by jian zhang on 1/25/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "RenderACache.h"

RenderACache::RenderACache():m_isInitialized(0) {}
RenderACache::~RenderACache() 
{
	uninitialize();
}

void RenderACache::initialize()
{
	glGenBuffers(1, &ibo);
	glGenBuffers(1, &vbo);
	
	m_isInitialized = 1;
}

void RenderACache::uninitialize()
{
	glDeleteBuffers(1, &ibo);
	glDeleteBuffers(1, &vbo);
}

void RenderACache::setTriangles(const int *triidx, unsigned int num_idx)
{
	m_n_draw = num_idx;
	unsigned int size = num_idx * sizeof(int);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, triidx, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void RenderACache::setP(const float *p, unsigned int num_vert)
{
	unsigned int size = num_vert * sizeof(float) * 3;
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, p, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RenderACache::draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glEnableClientState(GL_VERTEX_ARRAY);
		
	//glBindBuffer(GL_ARRAY_BUFFER, tbo);
	//glTexCoordPointer(4, GL_FLOAT, 0, 0);
	//glClientActiveTexture(GL_TEXTURE0);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glDrawElements(GL_TRIANGLES, m_n_draw, GL_UNSIGNED_INT, 0 );
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);	
	//glClientActiveTexture(GL_TEXTURE0);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RenderACache::enableProgram(GLuint p)
{
	glUseProgram((GLuint)p);
}

void RenderACache::disableProgram()
{
	glUseProgram(0);
}
//:~