#include "ofxSpriteCommand.h"
#include "ofxSpriteRenderer.h"

ofxSpriteCommand::ofxSpriteCommand()
{
	glGenBuffers(1, &m_VBOId);
	m_Texture = 0;
	m_Shader = 0;
	m_Vertices = new ofxVertex[COMMAND_CAPACITY*4];
	m_VerticesSize = 0;
}
ofxSpriteCommand::~ofxSpriteCommand()
{
	delete[] m_Vertices;
	glDeleteBuffers(1, &m_VBOId);
}
void ofxSpriteCommand::Render()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ofxVertex)*m_VerticesSize, &m_Vertices[0], GL_STATIC_DRAW);
	m_Shader->Bind();
	m_Texture->Bind();

	glDrawArrays(GL_QUADS, 0, m_VerticesSize);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	m_Shader->Unbind();
	m_Texture->Unbind();
}
void ofxSpriteCommand::PushSprite(ofxSpriteBase* sprite)
{
	memcpy(&m_Vertices[m_VerticesSize], sprite->GetVertices(), sizeof(ofxVertex)*sprite->GetVerticesSize());
	m_VerticesSize += sprite->GetVerticesSize();
}
ofxTexture* ofxSpriteCommand::GetTexture()
{
	return m_Texture;
}
ofxShaderProgram* ofxSpriteCommand::GetShader()
{
	return m_Shader;
}
void ofxSpriteCommand::SetTexture(ofxTexture* texture)
{
	m_Texture = texture;
}
void ofxSpriteCommand::SetShader(ofxShaderProgram* shader)
{
	m_Shader = shader;
}
ofxVertex* ofxSpriteCommand::GetVertices()
{
	return m_Vertices;
}
GLsizei ofxSpriteCommand::GetVerticesSize()
{
	return m_VerticesSize;
}