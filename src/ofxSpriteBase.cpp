#include "ofxSpriteBase.h"
#include "ofxSpriteRenderer.h"
#include "ofxShaderProgramCache.h"
#include "ofxTextureCache.h"
ofxSpriteBase::ofxSpriteBase()
{
	m_VerticesSize = 0;
	m_Vertices = 0;
	m_Shader = 0;
	m_Occlusion = SPRITE_OCCLUSION_UNKNOWN;
	m_Texture = 0;
	m_PositionChange = true;
	m_DimensionChange = true;
}
ofxSpriteBase::~ofxSpriteBase()
{
	if(m_Shader) m_Shader->DecreaseReference();
	if(m_Texture) m_Texture->DecreaseReference();
}
void ofxSpriteBase::MoveTo(const float x, const float y, const float z)
{
	MoveTo(ofVec3f(x,y,z));
}
void ofxSpriteBase::MoveTo(const ofVec3f position)
{
	m_Position = position;
	m_PositionChange = true;
	m_Occlusion = SPRITE_OCCLUSION_UNKNOWN;
}
void ofxSpriteBase::MoveBy(const float x, const float y, const float z)
{
	MoveBy(ofVec3f(x,y,z));
}
void ofxSpriteBase::MoveBy(const ofVec3f accelerator)
{
	m_Position += accelerator;
	m_PositionChange = true;
	if(accelerator.x > FAR_SCREEN_SPEED_THRESHOLD || 
		accelerator.y > FAR_SCREEN_SPEED_THRESHOLD)
	{
		m_Occlusion = SPRITE_OCCLUSION_UNKNOWN;
	}
}
void ofxSpriteBase::SetVisible(bool value)
{
	m_Visible = value;
}
bool ofxSpriteBase::IsVisible()
{
	return m_Visible;
}
SPRITE_OCCLUSION ofxSpriteBase::GetOcclusion()
{
	return m_Occlusion;
}
ofVec3f ofxSpriteBase::GetPosition()
{
	return m_Position;
}
void ofxSpriteBase::Update(const float delta_time)
{
}
void ofxSpriteBase::SubmitChanges()
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

void ofxSpriteBase::LoadShader(string shader_path)
{
	if(m_Shader)
	{
		m_Shader->DecreaseReference();
	}
	m_Shader = ofxSHADERPROGRAMCACHE->GetResource(shader_path);
	m_Shader->IncreaseReference();
}
ofxShaderProgram* ofxSpriteBase::GetShader()
{
	return m_Shader;
}
void ofxSpriteBase::SetTexture(string texture_path)
{
	if(m_Texture)
	{
		m_Texture->DecreaseReference();
	}
	m_Texture = ofxTEXTURECACHE->GetResource(texture_path);
	m_Texture->IncreaseReference();
}
ofxTexture* ofxSpriteBase::GetTexture()
{
	return m_Texture;
}
void ofxSpriteBase::SetID(int id)
{
	m_ID = id;
}
int ofxSpriteBase::GetID()
{
	return m_ID;
}
ofxVertex* ofxSpriteBase::GetVertices()
{
	return m_Vertices;
}
GLsizei ofxSpriteBase::GetVerticesSize()
{
	return m_VerticesSize;
}
void ofxSpriteBase::SetPositionChange(bool value)
{
	m_PositionChange = value;
}
bool ofxSpriteBase::GetPositionChange()
{
	return m_PositionChange;
}
void ofxSpriteBase::SetDimensionChange(bool value)
{
	m_DimensionChange = value;
}
bool ofxSpriteBase::GetDimensionChange()
{
	return m_DimensionChange;
}