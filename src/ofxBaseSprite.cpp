#include "ofxBaseSprite.h"
#include "ofxSpriteRenderer.h"
#include "ofxShaderProgramCache.h"
#include "ofxTextureCache.h"
ofxBaseSprite::ofxBaseSprite()
{
	m_VerticesSize = 0;
	m_Vertices = 0;
	m_Shader = 0;
	m_Occlusion = SPRITE_OCCLUSION_UNKNOWN;
	m_Texture = 0;
	m_PositionChange = true;
	m_DimensionChange = true;
	m_CustomRendered = false;
}
ofxBaseSprite::~ofxBaseSprite()
{
	if(m_Shader) m_Shader->DecreaseReference();
	if(m_Texture) m_Texture->DecreaseReference();
}
void ofxBaseSprite::MoveTo(const float x, const float y, const float z)
{
	MoveTo(ofVec3f(x,y,z));
}
void ofxBaseSprite::MoveTo(const ofVec3f position)
{
	m_Position = position;
	m_PositionChange = true;
	m_Occlusion = SPRITE_OCCLUSION_UNKNOWN;
}
void ofxBaseSprite::MoveBy(const float x, const float y, const float z)
{
	MoveBy(ofVec3f(x,y,z));
}
void ofxBaseSprite::MoveBy(const ofVec3f accelerator)
{
	m_Position += accelerator;
	m_PositionChange = true;
	if(accelerator.x > FAR_SCREEN_SPEED_THRESHOLD || 
		accelerator.y > FAR_SCREEN_SPEED_THRESHOLD)
	{
		m_Occlusion = SPRITE_OCCLUSION_UNKNOWN;
	}
}
void ofxBaseSprite::SetVisible(bool value)
{
	m_Visible = value;
}
bool ofxBaseSprite::IsVisible()
{
	return m_Visible;
}
SPRITE_OCCLUSION ofxBaseSprite::GetOcclusion()
{
	return m_Occlusion;
}
ofVec3f ofxBaseSprite::GetPosition()
{
	return m_Position;
}
void ofxBaseSprite::Update(const float delta_time)
{
}
void ofxBaseSprite::SubmitChanges()
{
	if(!(ofxRENDERER->IsCameraMove() || ofxRENDERER->IsCameraForce() || m_PositionChange))
		return;
	if(!ofxRENDERER->IsCameraForce() && 
		(m_Occlusion == SPRITE_OCCLUSION_FAR_SCREEN && ofGetFrameNum() % FAR_SCREEN_UPDATE_SEQUENCE != 0))
		return;
	if(!ofxRENDERER->IsCameraForce() && 
		(m_Occlusion == SPRITE_OCCLUSION_IN_SCREEN && ofGetFrameNum() % IN_SCREEN_UPDATE_SEQUENCE != 0))
		return;

	ofRectangle sprite_dimension(m_Dimension); sprite_dimension.translate(m_Position.x, m_Position.z);
	ofRectangle screen_dimension(ofxRENDERER->GetWorldRect());
	ofRectangle screen_expanded_dimension(ofxRENDERER->GetExpandedWorldRect());
	m_Occlusion = SPRITE_OCCLUSION_FAR_SCREEN;
	if(sprite_dimension.intersects(screen_expanded_dimension))
	{
		m_Occlusion = SPRITE_OCCLUSION_OFF_SCREEN;
		if(sprite_dimension.intersects(screen_dimension))
		{
			m_Occlusion = SPRITE_OCCLUSION_IN_SCREEN;
		}
	}
}

void ofxBaseSprite::LoadShader(string shader_path)
{
	if(m_Shader)
	{
		m_Shader->DecreaseReference();
	}
	m_Shader = ofxSHADERPROGRAMCACHE->GetResource(shader_path);
	m_Shader->IncreaseReference();
}
ofxShaderProgram* ofxBaseSprite::GetShader()
{
	return m_Shader;
}
void ofxBaseSprite::SetTexture(string texture_path)
{
	if(m_Texture)
	{
		m_Texture->DecreaseReference();
	}
	m_Texture = ofxTEXTURECACHE->GetResource(texture_path);
	m_Texture->IncreaseReference();
}
ofxTexture* ofxBaseSprite::GetTexture()
{
	return m_Texture;
}
bool ofxBaseSprite::IsCustomRendered()
{
	return m_CustomRendered;
}
void ofxBaseSprite::SetID(int id)
{
	m_ID = id;
}
int ofxBaseSprite::GetID()
{
	return m_ID;
}
ofxVertex* ofxBaseSprite::GetVertices()
{
	return m_Vertices;
}
GLsizei ofxBaseSprite::GetVerticesSize()
{
	return m_VerticesSize;
}
void ofxBaseSprite::SetPositionChange(bool value)
{
	m_PositionChange = value;
}
bool ofxBaseSprite::GetPositionChange()
{
	return m_PositionChange;
}
void ofxBaseSprite::SetDimensionChange(bool value)
{
	m_DimensionChange = value;
}
bool ofxBaseSprite::GetDimensionChange()
{
	return m_DimensionChange;
}