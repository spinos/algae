/*
 *  FColorSet.cpp
 *  
 *
 *  Created by zhang on 08-10-23.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "FColorSet.h"

FColorSet::FColorSet() : data(0)
{

}

FColorSet::~FColorSet()
{
	if(data) delete[] data;
}

