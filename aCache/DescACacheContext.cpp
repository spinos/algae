#include "DescACacheContext.h"

void* DescACacheContext::creator()
{
    return new DescACacheContext;
}


void DescACacheContext::copy ( const MPxData& other )
{
	_pDesc = ((const DescACacheContext&)other)._pDesc;
}

MTypeId DescACacheContext::typeId() const
{
	return DescACacheContext::id;
}

MString DescACacheContext::name() const
{ 
	return DescACacheContext::typeName;
}

MStatus DescACacheContext::readASCII(  const MArgList& args,
                                unsigned& lastParsedElement )
{
    return MS::kSuccess;
}

MStatus DescACacheContext::writeASCII( ostream& out )
{
    //out << fValue << " ";
    return MS::kSuccess;
}

MStatus DescACacheContext::readBinary( istream& in, unsigned )
{
    //in.read( (char*) &fValue, sizeof( fValue ));
    //return in.fail() ? MS::kFailure : MS::kSuccess;
    return MS::kSuccess;
}

MStatus DescACacheContext::writeBinary( ostream& out )
{
    //out.write( (char*) &fValue, sizeof( fValue));
    //return out.fail() ? MS::kFailure : MS::kSuccess;
    return MS::kSuccess;;
}

//
// this is the unique type id used to identify the new user-defined type.
//
const MTypeId DescACacheContext::id( 0x0002525c );
const MString DescACacheContext::typeName( "aCacheContextDesc" );
