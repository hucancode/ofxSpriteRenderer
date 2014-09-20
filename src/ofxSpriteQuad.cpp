#include "ofxSpriteQuad.h"
#include "ofxSpriteCommand.h"
#include "ofxSpriteRenderer.h"

ofxSpriteQuad::ofxSpriteQuad()
{
	ofxSpriteBase::ofxSpriteBase();
	m_VerticesSize = 4;
	
	m_Vertices = new ofxVertex[m_VerticesSize];
	m_UVChange = true;
	m_OpacityChange = true;
	m_MirrorX = false;
	m_MirrorY = false;
	m_ScaleX = 1.0;
	m_ScaleY = 1.0;
	m_Opacity = 255;
	LoadShader(DEFAULT_SHADER);
	ofxRENDERER->PushSprite(this);
}
ofxSpriteQuad::~ofxSpriteQuad()
{
	ofxSpriteBase::~ofxSpriteBase();
	ofxRENDERER->EraseSprite(this);
}
void ofxSpriteQuad::SetTexture(string texture_path)
{
	ofxSpriteBase::SetTexture(texture_path);
	SetSpriteRect(-m_Texture->GetDimension().x*0.5, 0, m_Texture->GetDimension().x, m_Texture->GetDimension().y);
	SetTextureRect(0, 0, m_Texture->GetDimension().x, m_Texture->GetDimension().y);
}
ofRectangle ofxSpriteQuad::GetTextureRect()
{
	return m_TextureRect;
}
ofRectangle ofxSpriteQuad::GetSpriteRect()
{
	return m_SpriteRect;
}
void ofxSpriteQuad::SetTextureRect(const float x, const float y, const float w, const float h)
{
	SetTextureRect(ofRectangle(x, y, w, h));
}
void ofxSpriteQuad::SetSpriteRect(const float x, const float y, const float w, const float h)
{
	SetSpriteRect(ofRectangle(x, y, w, h));
}
void ofxSpriteQuad::SetTextureRect(const ofRectangle rect)
{
	m_TextureRect = rect;
	m_UVChange = true;
}
void ofxSpriteQuad::SetSpriteRect(const ofRectangle rect)
{
	m_SpriteRect = rect;
	m_Dimension = m_SpriteRect;
	m_DimensionChange = true;
}
// in order to make the quad skew 30 degree, we must put some adjust on Y and Z
#define SKEW45
#if defined(SKEW30)
#define QUAD_GRADIENT_Z 0.4472135954999579// sqrt(0.2)
#define QUAD_GRADIENT_Y 0.8944271909999159// sqrt(0.8)
#elif defined(SKEW45)
#define QUAD_GRADIENT_Z 0.7071067811865475// sqrt(0.5)
#define QUAD_GRADIENT_Y 0.7071067811865475// sqrt(0.5)
#endif
void ofxSpriteQuad::SubmitChanges()
{
	ofxSpriteBase::SubmitChanges();
	if(m_PositionChange || m_DimensionChange)
	{
		ofRectangle rect(m_SpriteRect);
		rect.x *= m_ScaleX;
		rect.y *= m_ScaleY;
		rect.width *= m_ScaleX;
		rect.height *= m_ScaleY;
		m_Vertices[0].x = rect.x + m_Position.x;
		m_Vertices[0].y = rect.y + m_Position.y;
		m_Vertices[0].z = m_Position.z;
		m_Vertices[1].x = m_Vertices[0].x + rect.width;
		m_Vertices[1].y = m_Vertices[0].y;
		m_Vertices[1].z = m_Vertices[0].z;
		m_Vertices[2].x = m_Vertices[1].x;
		m_Vertices[2].y = m_Vertices[1].y + QUAD_GRADIENT_Y*rect.height;
		m_Vertices[2].z = m_Vertices[0].z - QUAD_GRADIENT_Z*rect.height;
		m_Vertices[3].x = m_Vertices[0].x;
		m_Vertices[3].y = m_Vertices[2].y;
		m_Vertices[3].z = m_Vertices[2].z;
	}
	if(m_UVChange)
	{
		float uv_min_x = m_TextureRect.x/m_Texture->GetDimension().x;
		float uv_min_y = m_TextureRect.y/m_Texture->GetDimension().y;
		float uv_max_x = uv_min_x + m_TextureRect.width/m_Texture->GetDimension().x;
		float uv_max_y = uv_min_y + m_TextureRect.height/m_Texture->GetDimension().y;
		swap(uv_min_y, uv_max_y);

		if(m_MirrorX)
		{
			swap(uv_max_x, uv_min_x);
		}
		if(m_MirrorY)
		{
			swap(uv_max_y, uv_min_y);
		}

		m_Vertices[0].u = uv_min_x;
		m_Vertices[0].v = uv_min_y;
		m_Vertices[1].u = uv_max_x;
		m_Vertices[1].v = uv_min_y;
		m_Vertices[2].u = uv_max_x;
		m_Vertices[2].v = uv_max_y;
		m_Vertices[3].u = uv_min_x;
		m_Vertices[3].v = uv_max_y;
	}
	if(m_OpacityChange)
	{
		m_Vertices[0].opacity = m_Opacity;
		m_Vertices[1].opacity = m_Opacity;
		m_Vertices[2].opacity = m_Opacity;
		m_Vertices[3].opacity = m_Opacity;
	}
	m_PositionChange = m_DimensionChange = m_UVChange = m_OpacityChange = false;
}
/* ----------------------------------
sprite operation
---------------------------------- */
void ofxSpriteQuad::SetScale(float value)
{
	SetScaleX(value);
	SetScaleY(value);
}
void ofxSpriteQuad::SetScaleX(float value)
{
	if(m_ScaleX == value) return;
	m_ScaleX = value;
	m_DimensionChange = true;
}
float ofxSpriteQuad::GetScaleX()
{
	return m_ScaleX;
}
void ofxSpriteQuad::SetScaleY(float value)
{
	if(m_ScaleY == value) return;
	m_ScaleY = value;
	m_DimensionChange = true;
}
float ofxSpriteQuad::GetScaleY()
{
	return m_ScaleY;
}
void ofxSpriteQuad::SetMirrorX(bool value)
{
	m_MirrorX = value;
	m_UVChange = true;
}
bool ofxSpriteQuad::IsMirrorX()
{
	return m_MirrorX;
}
void ofxSpriteQuad::SetMirrorY(bool value)
{
	m_MirrorY = value;
	m_UVChange = true;
}
bool ofxSpriteQuad::IsMirrorY()
{
	return m_MirrorY;
}
void ofxSpriteQuad::SetOpacity(float value)
{
	m_Opacity = value;
	m_OpacityChange = true;
}
float ofxSpriteQuad::GetOpacity()
{
	return m_Opacity;
}