/*
 *  GHelper.cpp
 *  acache
 *
 *  Created by jian zhang on 1/25/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "GHelper.h"

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

void GHelper::drawBoundingBox(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax)
{
	glBegin(GL_LINES);

	glVertex3f(xmin,ymin,zmin);
	glVertex3f(xmax,ymin,zmin);
	glVertex3f(xmax,ymin,zmin);
	glVertex3f(xmax,ymax,zmin);
	glVertex3f(xmax,ymax,zmin);
	glVertex3f(xmin,ymax,zmin);
	glVertex3f(xmin,ymax,zmin);
	glVertex3f(xmin,ymin,zmin);

	glVertex3f(xmin,ymin,zmax);
	glVertex3f(xmax,ymin,zmax);
	glVertex3f(xmax,ymin,zmax);
	glVertex3f(xmax,ymax,zmax);
	glVertex3f(xmax,ymax,zmax);
	glVertex3f(xmin,ymax,zmax);
	glVertex3f(xmin,ymax,zmax);
	glVertex3f(xmin,ymin,zmax);

	glVertex3f(xmax,ymin,zmax);
	glVertex3f(xmax,ymin,zmin);
	glVertex3f(xmax,ymax,zmax);
	glVertex3f(xmax,ymax,zmin);
	glVertex3f(xmin,ymin,zmax);
	glVertex3f(xmin,ymin,zmin);
	glVertex3f(xmin,ymax,zmax);
	glVertex3f(xmin,ymax,zmin);

	glEnd();
}

void GHelper::drawLocator(float x, float y, float z)
{
	glBegin(GL_LINES);
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(x-1.f, y, z);
		glVertex3f(x+1.f, y, z);
		glColor3f(0.f, 1.f, 0.f);
		glVertex3f(x, y-1.f, z);
		glVertex3f(x, y+1.f, z);
		glColor3f(0.f, 0.f, 1.f);
		glVertex3f(x, y, z-1.f);
		glVertex3f(x, y, z+1.f);
	glEnd();
}