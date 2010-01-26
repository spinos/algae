/*
 *  FFloatSet.cpp
 *  
 *
 *  Created by zhang on 08-10-23.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "FFloatSet.h"

FFloatSet::FFloatSet() : data(0)
{

}

FFloatSet::~FFloatSet()
{
	if(data) delete[] data;
}

