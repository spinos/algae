/*
 *  GHelper.h
 *  acache
 *
 *  Created by jian zhang on 1/25/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _G_HELPER_H
#define _G_HELPER_H

class GHelper
{
public :
	static void drawBoundingBox(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax);
	static void drawLocator(float x, float y, float z);
};

#endif