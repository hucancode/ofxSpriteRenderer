#pragma once
#include "ofMain.h"
#include "ofxShaderProgram.h"
#include "ofxTexture.h"

struct ofxVertex
{
	float x,y,z;			// position
	float u,v;				// texture uv
	float r, g, b;			// color
	float color_intensity;	// color intensity
	float opacity;			// vertex opacity
};
enum SPRITE_OCCLUSION
{
	SPRITE_OCCLUSION_IN_SCREEN,		// need to update occlusion, but not too fast
	SPRITE_OCCLUSION_OFF_SCREEN,	// need to update occlusion, now
	SPRITE_OCCLUSION_FAR_SCREEN,	// need to update occlusion, but not today
	SPRITE_OCCLUSION_UNKNOWN,		// need to update occlusion, now
};

class ofxSpriteBase
{
	friend class ofxSpriteRenderer;
	friend class ofxSpriteCommand;
protected:
	SPRITE_OCCLUSION	m_Occlusion;
	ofVec3f				m_Position;
	ofRectangle			m_Dimension;
	bool				m_Visible;
	bool				m_PositionChange;
	bool				m_DimensionChange;
	ofxTexture*			m_Texture;
	ofxShaderProgram*	m_Shader;
	ofxVertex*			m_Vertices;
	GLsizei				m_VerticesSize;
	int					m_ID;
public:
	ofxSpriteBase();
	~ofxSpriteBase();
	virtual void		Update(const float delta_time);
	virtual void		SubmitChanges();
	ofVec3f				GetPosition();
	void				SetVisible(bool value);
	bool				IsVisible();
	void				MoveTo(const float x, const float y, const float z);
	void				MoveTo(const ofVec3f position);
	void				MoveBy(const float x, const float y, const float z);
	void				MoveBy(const ofVec3f accelerator);
	ofxShaderProgram*	GetShader();
	virtual void		LoadShader(string shader_path);
	virtual void		SetTexture(string texture_path);
	ofxTexture*			GetTexture();
private:
	void				SetID(int id);
	int					GetID();
	SPRITE_OCCLUSION	GetOcclusion();
	ofxVertex*			GetVertices();
	GLsizei				GetVerticesSize();
	void				SetPositionChange(bool value);
	bool				GetPositionChange();
	void				SetDimensionChange(bool value);
	bool				GetDimensionChange();
};
typedef vector<ofxSpriteBase*> ofxSpriteBases;