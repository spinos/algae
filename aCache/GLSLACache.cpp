/*
 *  GLSLACache.cpp
 *  pmap
 *
 *  Created by jian zhang on 12/20/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "GLSLACache.h"

#ifndef __APPLE__
#include "../shared/gExtension.h"
#endif

const char *particleVS = 
// point size in world space 
//"uniform float pointRadius;     \n"
// scale to calculate size in pixels \n
//"uniform float pointScale;"   


//"uniform mat4 shadowMatrix;"

"varying float eyez;"

"void main()                                                 \n"
"{                                                           \n"
"	vec4 wpos = vec4(gl_Vertex.xyz, 1.0);                   \n"
"    gl_Position = gl_ModelViewMatrix * wpos;      \n"
"	eyez = -gl_Position.z;"
" gl_Position = gl_ProjectionMatrix * gl_Position;"

// calculate window-space point size                    
//"    vec4 eyeSpacePos = gl_ModelViewMatrix * wpos;           \n"
//"    float dist = length(eyeSpacePos.xyz);                   \n"
//"    gl_PointSize = 128.0/dist;       \n"

// velocity and densit

//"    gl_TexCoord[0] = gl_MultiTexCoord0; "
//"    gl_TexCoord[1] = eyeSpacePos;                           \n"

// transform point in shadow, calc ndc
//"    gl_TexCoord[2] = vec4(shadowMatrix * wpos);"
//"    gl_TexCoord[2].x = (gl_TexCoord[2].x + 1.0)*0.5;"
//"    gl_TexCoord[2].y = (gl_TexCoord[2].y + 1.0)*0.5;"

// world point for noise
//"	gl_TexCoord[3] = wpos;"

// side and up of 
//"	gl_TexCoord[4] = spriteSide;"
//"	gl_TexCoord[5] = spriteUp;"
"}";

const char *particleFS = 
"uniform vec3 baseColor;"
"uniform vec2 clipping;"

"varying float eyez;"

"void main()                                                        \n"
"{                                                                  \n"
"	vec3 cs = baseColor;"
"	float normz = clamp((eyez - clipping.x)/(clipping.y - clipping.x), 0.0, 1.0);"
"	normz = sqrt(normz);"
"    gl_FragColor = vec4(1.0-normz, normz, 0.0, 1.0);              \n"
"} ";

GLSLACache::GLSLACache() 
{
	m_isValid = initProgram();
}

GLSLACache::~GLSLACache() 
{
	glDeleteProgram( program );
}

char GLSLACache::initProgram()
{
	GLuint vertex_shader   = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &particleVS, NULL);
	glCompileShader(vertex_shader);
	
	GLint vertexshader_compiled;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, (GLint*)&vertexshader_compiled);
	if(!vertexshader_compiled) return 0;
	
	GLuint fragment_shader   = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &particleFS, NULL);
	glCompileShader(fragment_shader);
	
	GLint fragmentshader_compiled;
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, (GLint*)&fragmentshader_compiled);
	if(!fragmentshader_compiled) return 0;
	
	program = glCreateProgram();
	
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	
	glLinkProgram(program);
	
	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if(!linked) return 0;
	
	glUseProgram(program);
	//glUniform1f(glGetUniformLocation(program, "scaleDensity"), 1.0);
	//glUniform1f(glGetUniformLocation(program, "pointRadius"), 1.0);
	glUniform3f(glGetUniformLocation(program, "baseColor"), 1.0, 0.25, 0.0);
	glUniform2f(glGetUniformLocation(program, "clipping"), 1.f, 1000.f);
	/*glUniform3f(glGetUniformLocation(program, "lightColor"), 0.0, 1.0, 1.0);
	glUniform3f(glGetUniformLocation(program, "shadowColor"), 0.0, 0.0, 0.0);
	glUniform1f(glGetUniformLocation(program, "Lacunarity"), 2.0);
	glUniform1f(glGetUniformLocation(program, "Dimension"), 1.0);
	glUniform1f(glGetUniformLocation(program, "Frequency"), 1.0/256.0);
	glUniform1i(glGetUniformLocation(program, "ShadowMap"), 0);
	glUniform1i(glGetUniformLocation(program, "EarthNight"), 1);
	glUniform4f(glGetUniformLocation(program, "spriteSide"), 1.0, 0.0, 0.0, 1.0);
	glUniform4f(glGetUniformLocation(program, "spriteUp"), 0.0, 1.0, 0.0, 1.0);
	glUniform3f(glGetUniformLocation(program, "NoiseOrigin"), 0.0, 0.0, 0.0);*/
	glUseProgram(0);
	return 1;
}

void GLSLACache::enable()
{
	glUseProgram(program);
}

void GLSLACache::disable()
{
	glUseProgram(0);
}

void GLSLACache::setParam(GLSLACachePARAM& param)
{
	glUseProgram(program);
	//glUniform1f(glGetUniformLocation(program, "scaleDensity"), param.density);
	//glUniform1f(glGetUniformLocation(program, "pointRadius"), param.radius);
	glUniform3f(glGetUniformLocation(program, "baseColor"), param.base_r, param.base_g, param.base_b);
	glUniform2f(glGetUniformLocation(program, "clipping"), param.clip_near, param.clip_far);
	/*glUniform3f(glGetUniformLocation(program, "lightColor"), param.light_r, param.light_g, param.light_b);
	glUniform3f(glGetUniformLocation(program, "shadowColor"), param.shadow_r, param.shadow_g, param.shadow_b);
	glUniform4f(glGetUniformLocation(program, "spriteSide"), param.side_x, param.side_y, param.side_z, 1.0);
	glUniform4f(glGetUniformLocation(program, "spriteUp"), param.up_x, param.up_y, param.up_z, 1.0);
	glUniform1f(glGetUniformLocation(program, "Lacunarity"), param.lacunarity);
	glUniform1f(glGetUniformLocation(program, "Dimension"), param.dimension);
	glUniform1f(glGetUniformLocation(program, "Frequency"), param.frequency);
	glUniform3f(glGetUniformLocation(program, "NoiseOrigin"), param.noise_x, param.noise_y, param.noise_z);
	*/
}
