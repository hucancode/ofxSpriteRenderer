#pragma once
#include "ofMain.h"
#include "ofxBaseSprite.h"
#include "ofxBaseCommand.h"
/*

note: this class is used internally in ofxSpriteRenderer, it's not really useful for end users

*/

#define COMMAND_VERTEX_CAPACITY 20000
class ofxSpriteCommand
	:public ofxBaseCommand
{
private:
	ofxTexture*				m_Texture;
	ofxShaderProgram*		m_Shader;
	ofxVertex*				m_Vertices;
	GLuint					m_VBOId;
public:
	ofxSpriteCommand();
	virtual ~ofxSpriteCommand();
	virtual void			Render();
	bool					PushSprite(ofxBaseSprite* sprite);
	ofxTexture*				GetTexture();
	ofxShaderProgram*		GetShader();
	void					SetTexture(ofxTexture* texture);
	void					SetShader(ofxShaderProgram* shader);
};
typedef vector<ofxSpriteCommand*> ofxSpriteCommands;