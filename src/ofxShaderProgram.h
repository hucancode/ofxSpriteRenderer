#pragma once
#include "ofMain.h"
#include "ofxResource.h"
#include "ofxShader.h"

class ofxShaderProgram
	:public ofxResource
{
protected:
	ofxShader*			m_VertexShader;
	ofxShader*			m_FragmentShader;
protected:
	GLuint				m_ProgramId;
	map<string, GLint>	m_AttributeMap;
	map<string, GLint>	m_UniformMap;
public:
	ofxShaderProgram();
	virtual ~ofxShaderProgram();
	virtual bool		Load(string program_file);
	virtual void		BuildReferenceMap();
	virtual void		BindReferenceMap();
	virtual void		IncreaseReference();
	virtual void		DecreaseReference();
	bool				Load(string vs_file, string fs_file);
	void				Bind();
	void				Unbind();
};