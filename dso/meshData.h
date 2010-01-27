#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <ri.h>

class meshData {
public:
	meshData(std::string& parameter);
    ~meshData();
	void generateRIB(RtFloat detail);
    
private:
	char m_cache_name[1024];
	char m_mesh_name[1024];
	char m_prt_name[1024];
	int m_i_hdr_shadowed, m_i_hdr_indirect, m_i_hdr_scat, m_i_hdr_backscat;
	int m_i_lightsrc_shadowed;
	float m_shutter_open, m_shutter_close;
	int m_frame;
	float m_mesh_0, m_mesh_1;
	int m_i_double_sided;
};
