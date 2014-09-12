#include "ofxShader.h"
#include "ofxSpriteQuad.h"
#include "ofxSpriteRenderer.h"
#include "ofxShaderCache.h"
ofxShader::ofxShader()
	:ofxResource()
{
	m_ShaderId = 0;
}
ofxShader::~ofxShader()
{
	glDeleteShader(m_ShaderId);
}
bool ofxShader::Load(string shader_path)
{
	GLenum type;
	string extension = shader_path.substr(shader_path.length()-4,4);
	if(extension == "vert")
	{
		bool result = Load(shader_path, GL_VERTEX_SHADER);
		if(!result) return result;
	}
	else if(extension == "frag")
	{
		bool result = Load(shader_path, GL_FRAGMENT_SHADER);
		if(!result) return result;
	}
	else
	{
		return false;
	}
	return true;
}
bool ofxShader::Load(string shader_path, GLenum type)
{
	m_ShaderId = glCreateShader(type);
	ofBuffer shader_file = ofBufferFromFile(shader_path);
	string shader_source = shader_file.getText();
	const char* shader_source_raw = shader_source.c_str();
	int shader_source_length = shader_source.size()+1;
	glShaderSource(m_ShaderId, 1, &shader_source_raw, &shader_source_length);
	glCompileShader(m_ShaderId);
	GLint compile_status;
	glGetShaderiv(m_ShaderId, GL_COMPILE_STATUS, &compile_status);
	if(!compile_status)
	{
#ifdef _DEBUG
		GLint len = 0;
		glGetShaderiv(m_ShaderId, GL_INFO_LOG_LENGTH, &len);
		if(len > 1)
		{
			char* info = new char[len];
			glGetShaderInfoLog(m_ShaderId, len, NULL, info);
			printf("Error compiling shader[%s]:\n%s\n", shader_path.c_str(), info);
			delete[] info;
		}
#endif
		glDeleteShader(m_ShaderId);
		return false;
	}
	return true;
}
GLuint ofxShader::GetShaderId()
{
	return m_ShaderId;
}