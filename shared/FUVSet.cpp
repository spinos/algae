/*
 *  FUVSet.cpp
 *  
 *
 *  Created by zhang on 08-10-23.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "FUVSet.h"

FUVSet::FUVSet() : s(0), t(0)
{

}

FUVSet::~FUVSet()
{
	if(s) delete[] s;
	if(t) delete[] t;
}

