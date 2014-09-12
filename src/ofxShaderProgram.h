#pragma once
#include "ofMain.h"
#include "ofxResource.h"
#include "ofxShader.h"

/*
TODO: make this class more dynamic, allow user to add custom uniform
*/
class ofxShaderProgram
	:public ofxResource
{
private:
	ofxShader*			m_VertexShader;
	ofxShader*			m_FragmentShader;
private:
	GLuint				m_ProgramId;
	map<string, GLint>	m_AttributeMap;
	map<string, GLint>	m_UniformMap;
public:
	ofxShaderProgram();
	virtual ~ofxShaderProgram();
	virtual bool		Load(string program_file);
	virtual void		IncreaseReference();
	virtual void		DecreaseReference();
	bool				Load(string vs_file, string fs_file);
	void				Bind();
	void				Unbind();
};