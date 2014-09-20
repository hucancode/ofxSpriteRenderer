#pragma once
#include "ofMain.h"
#include "ofxSpriteBase.h"

/*

note: this class is used internally in ofxSpriteRenderer, it's not really useful for end users

*/

#define COMMAND_VERTEX_CAPACITY 20000
class ofxSpriteCommand
{
#ifdef _DEBUG
	friend class ofxSpriteRenderer;
#endif
private:
	ofxTexture*				m_Texture;
	ofxShaderProgram*		m_Shader;
	ofxVertex*				m_Vertices;
	GLuint					m_VBOId;
	GLsizei					m_VerticesSize;
public:
	ofxSpriteCommand();
	~ofxSpriteCommand();
public:
	void					Render();
	bool					PushSprite(ofxSpriteBase* sprite);
	ofxTexture*				GetTexture();
	ofxShaderProgram*		GetShader();
	void					SetTexture(ofxTexture* texture);
	void					SetShader(ofxShaderProgram* shader);
private:
	ofxVertex*				GetVertices();
	GLsizei					GetVerticesSize();
};
typedef vector<ofxSpriteCommand*> ofxSpriteCommands;