#pragma once
#include "ofMain.h"
#include "ofxResource.h"

/*

note: this class is used internally in ofxSpriteRenderer, it's not really useful for end users

*/

class ofxShader
	:public ofxResource
{
	friend class ofxShaderProgram;
private:
	GLuint			m_ShaderId;
public:
	ofxShader();
	~ofxShader();
	virtual bool	Load(string shader_file);
	bool			Load(string shader_file, GLenum type);
private:
	GLuint			GetShaderId();
};