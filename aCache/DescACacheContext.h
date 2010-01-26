#ifndef __A_CACHE_CONTEXT_DESC_DATA_INCLUDED__
#define __A_CACHE_CONTEXT_DESC_DATA_INCLUDED__

#include <maya/MPxData.h>
#include <maya/MTypeId.h>
#include <maya/MString.h>

struct ACacheContextData
{
	int enable;
	unsigned int program;
};

class DescACacheContext : public MPxData
{
public:
						DescACacheContext() {}
    virtual					~DescACacheContext() {}

    virtual MStatus         readASCII( const MArgList&, unsigned& lastElement );
    virtual MStatus         readBinary( istream& in, unsigned length );
    virtual MStatus         writeASCII( ostream& out );
    virtual MStatus         writeBinary( ostream& out );

	virtual void			copy( const MPxData& );
	MTypeId                 typeId() const; 
	MString                 name() const;

    	ACacheContextData* getDesc() const { return _pDesc; }
    	void setDesc(ACacheContextData* pDesc) { _pDesc = pDesc; }


	static const MString	typeName;
    static const MTypeId    id;
	static void*            creator();

private:
    ACacheContextData* _pDesc; 
};
#endif