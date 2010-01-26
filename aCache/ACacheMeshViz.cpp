#include "ACacheMeshViz.h"
#include "RenderACache.h"
#include "../shared/AHelper.h"
#include "../shared/SHelper.h"
#include "../shared/FXMLMesh.h"
#include "../shared/TVectors.h"
#include "DescACacheContext.h"
#include "GHelper.h"

MTypeId ACacheMeshViz::id( 0x00025256 );

MObject ACacheMeshViz::aindesc;
MObject ACacheMeshViz::ameshname;
MObject ACacheMeshViz::acachename;
MObject ACacheMeshViz::aframe;
MObject ACacheMeshViz::aminframe;
MObject ACacheMeshViz::amaxframe;
MObject ACacheMeshViz::amode;
MObject ACacheMeshViz::aoutval;

ACacheMeshViz::ACacheMeshViz():m_pMesh(0),m_pRender(0),m_mode(-1),m_program(0)
{
}

ACacheMeshViz::~ACacheMeshViz()
{
	if(m_pMesh) delete m_pMesh;
	if(m_pRender) delete m_pRender;
}

MStatus ACacheMeshViz::compute( const MPlug& plug, MDataBlock& data )
{ 
	if(plug == aoutval)
	{
		/*int hascoe = 0;
		MVectorArray vcoe = AHelper::getVectorArrayAttr(data, aincoe);
		XYZ hdrCoeff[16];
		if(vcoe.length() == 16)
		{
			hascoe = 1;
			for(unsigned i=0; i<16; i++) hdrCoeff[i] = XYZ(vcoe[i].x, vcoe[i].y, vcoe[i].z);
		}*/
		MDataHandle hdesc = data.inputValue(aindesc);
		MObject odesc = hdesc.data();
		MFnPluginData fdesc(odesc);
		DescACacheContext* dDesc = (DescACacheContext*) fdesc.data();
		if(dDesc) {
			ACacheContextData* pDesc = dDesc->getDesc();
			m_program = pDesc->program;
		}
		
		
		double dtime = data.inputValue( aframe ).asDouble();

		string sbuf(m_cachename.asChar());
		SHelper::changeFrameNumber(sbuf, SHelper::safeConvertToInt(dtime));
		if(m_pMesh) {
			m_pMesh->updateFrom(sbuf.c_str());
			//if(hascoe==1) m_pMesh->setHDRLighting(hdrCoeff);
			//else m_pMesh->resetHDRLighting();
			//m_pMesh->updateColor(m_mode);
		}
		else {
			m_cachename =  data.inputValue( acachename ).asString();
			m_meshname =  data.inputValue( ameshname ).asString();
			
			if(m_cachename != "" && m_meshname != "") {
				m_pMesh = new FXMLMesh(m_cachename.asChar(), m_meshname.asChar());

				if(m_pMesh->isNull()) {
					delete m_pMesh;
					m_pMesh = 0;
				}
			}
		}
	}
	return MS::kUnknownParameter;
}

void ACacheMeshViz::draw( M3dView & view, const MDagPath & /*path*/, 
							 M3dView::DisplayStyle style,
							 M3dView::DisplayStatus status )
{ 
	view.beginGL(); 
	
	if(!m_pRender) {
		m_pRender = new RenderACache();
		m_pRender->initialize();
	}

	if(m_pMesh) {
		/*glPushAttrib( GL_ALL_ATTRIB_BITS );
		if(m_mode<0) glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

#ifdef LINUX	
		glFrontFace(GL_CCW);
#else
		glFrontFace(GL_CW);
#endif

		glShadeModel(GL_SMOOTH);
		if(m_mode<-2) m_pMesh->drawUV(-3-m_mode);
		else if(m_mode<-1) 
		{
			glEnable(GL_CULL_FACE);
			m_pMesh->drawNoColor();
			glDisable(GL_CULL_FACE);
			m_pMesh->drawTangentSpace();
		}
		else if(m_mode<0) 
		{
			glEnable(GL_CULL_FACE);
			m_pMesh->drawNoColor();
			glDisable(GL_CULL_FACE);
		}
		else m_pMesh->draw();
		glPopAttrib();*/
		m_pRender->setTriangles(m_pMesh->triangles(), m_pMesh->getNumTriangle()*3);
		m_pRender->setP((const float *)m_pMesh->points(), m_pMesh->getNumVertex());
		m_pRender->enableProgram(m_program);
		m_pRender->draw();
		m_pRender->disableProgram();
		
		GHelper::drawBoundingBox( m_pMesh->getBBox0X(), m_pMesh->getBBox0Y(), m_pMesh->getBBox0Z(), m_pMesh->getBBox1X(), m_pMesh->getBBox1Y(), m_pMesh->getBBox1Z());
	
	}

	view.endGL();
}

bool ACacheMeshViz::isBounded() const
{ 
	return true;
}

MBoundingBox ACacheMeshViz::boundingBox() const
{ 
	MPoint corner1( 0,0,0 );
	MPoint corner2( 1,1,1 );
	
	if(m_mode>-3) {
		if(m_pMesh) {
			corner1.x = m_pMesh->getBBox0X();
			corner1.y = m_pMesh->getBBox0Y();
			corner1.z = m_pMesh->getBBox0Z();
			corner2.x = m_pMesh->getBBox1X();
			corner2.y = m_pMesh->getBBox1Y();
			corner2.z = m_pMesh->getBBox1Z();
		}
	}

	return MBoundingBox( corner1, corner2 );
}

void* ACacheMeshViz::creator()
{
	return new ACacheMeshViz();
}

MStatus ACacheMeshViz::initialize()
{ 
	MStatus				stat;
	MFnNumericAttribute numAttr;
	
	aframe = numAttr.create( "currentTime", "ct", MFnNumericData::kDouble, 1.0 );
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	addAttribute( aframe );
	
	aminframe = numAttr.create( "minFrame", "mnf", MFnNumericData::kInt, 1 );
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	addAttribute( aminframe );
	
	amaxframe = numAttr.create( "maxFrame", "mxf", MFnNumericData::kInt, 24 );
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	addAttribute( amaxframe );
	
	aoutval = numAttr.create( "outval", "ov", MFnNumericData::kInt, 1 );
	numAttr.setStorable(false);
	numAttr.setWritable(false);
	numAttr.setKeyable(false);
	addAttribute( aoutval );
	
	MFnTypedAttribute   stringAttr;
	acachename = stringAttr.create( "cachePath", "cp", MFnData::kString );
 	stringAttr.setStorable(true);
	stringAttr.setInternal(true);
	addAttribute( acachename );
	
	ameshname = stringAttr.create( "meshName", "mn", MFnData::kString );
 	stringAttr.setStorable(true);
	stringAttr.setInternal(true);
	addAttribute(ameshname);
	
	amode = numAttr.create( "drawMode", "dm", MFnNumericData::kInt, -1 );
	numAttr.setStorable(true);
	numAttr.setKeyable(true);
	numAttr.setInternal(true);
	addAttribute( amode );
	
	MFnTypedAttribute tAttr;
	
	aindesc = tAttr.create("inDesc", "inDesc", MFnData::kPlugin);
	tAttr.setStorable(false);
	tAttr.setConnectable(true);
	addAttribute(aindesc);
	
	attributeAffects( aframe, aoutval );
	attributeAffects( aindesc, aoutval );
    	
	return MS::kSuccess;
}

/* virtual */
bool 
ACacheMeshViz::setInternalValueInContext( const MPlug& plug,
												  const MDataHandle& handle,
												  MDGContext&)
{
	bool handledAttribute = false;
	if (plug == acachename) {
		handledAttribute = true;
		m_cachename = (MString) handle.asString();
		
		if(m_pMesh) delete m_pMesh;
		m_pMesh = new FXMLMesh(m_cachename.asChar(), m_meshname.asChar());

		if(m_pMesh->isNull()) {
			delete m_pMesh;
			m_pMesh = 0;
		}
		//else m_pMesh->updateColor(m_mode);
	}
	
	if (plug == ameshname) {
		handledAttribute = true;
		m_meshname = (MString) handle.asString();
	}
	
	if (plug == amode)
	{
		handledAttribute = true;
		m_mode = handle.asInt();
		//if(m_pMesh) m_pMesh->updateColor(m_mode);
	}

	return handledAttribute;
}

/* virtual */
bool
ACacheMeshViz::getInternalValueInContext( const MPlug& plug,
											  MDataHandle& handle,
											  MDGContext&)
{
	bool handledAttribute = false;
	if (plug == acachename)
	{
		handledAttribute = true;
		handle.set( m_cachename );
	}
	
	if (plug == ameshname)
	{
		handledAttribute = true;
		handle.set( m_meshname );
	}
	
	if (plug == amode)
	{
		handledAttribute = true;
		handle.set( m_mode );
	}
	
	return handledAttribute;
}

//:~
