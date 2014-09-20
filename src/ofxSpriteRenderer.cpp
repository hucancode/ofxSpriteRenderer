#include "ofxSpriteRenderer.h"
#include "assert.h"
ofxSpriteRenderer* ofxSpriteRenderer::s_Instance = 0;
ofxSpriteRenderer* ofxSpriteRenderer::GetInstance()
{
	return s_Instance;
}
void ofxSpriteRenderer::DestroyInstance()
{
	if(!s_Instance) return;
	delete s_Instance;
	s_Instance = 0;
}
ofxSpriteRenderer::ofxSpriteRenderer()
{
	if(s_Instance) return;
	m_Camera = new ofxOrthoCamera();
	m_Camera->setGlobalPosition(ofVec3f(0.0f, 1.0f, 1.0f));
	m_Camera->lookAt(ofVec3f(0.0f, 0.0f, 0.0f));
	s_Instance = this;
	m_CameraMove = true;
	m_CameraForce = true;
}
ofxSpriteRenderer::~ofxSpriteRenderer()
{
	{
		ofxSpriteCommands::iterator it = m_Commands.begin();
		for(;it != m_Commands.end();it++)
		{
			ofxSpriteCommand* cmd = *it;
			delete cmd;
		}
		m_Commands.clear();
	}
	{
		ofxSpriteBases::iterator it = m_Sprites.begin();
		for(;it != m_Sprites.end();it++)
		{
			ofxSpriteBase* quad = *it;
			delete quad;
		}
		m_Sprites.clear();
	}
}
static bool QuadCompare(ofxSpriteBase* quadA, ofxSpriteBase* quadB)
{
	return quadA->GetPosition().z < quadB->GetPosition().z;
}
void ofxSpriteRenderer::Render()
{
	//printf("--------------RENDER--------------\n");
	for(ofxSpriteCommands::iterator it = m_Commands.begin();it != m_Commands.end();it++)
	{
		ofxSpriteCommand* item = *it;
		delete item;
	}
	m_Commands.clear();
	
	//unsigned long long time_start_build = ofGetSystemTime();
	sort(m_Sprites.begin(), m_Sprites.end(), QuadCompare);
	for(int i=0;i<m_Sprites.size();i++)
	{
		m_Sprites[i]->SetID(i);
	}
	//unsigned long long time_finish_sort = ofGetSystemTime();
	//printf("sort time =  %llu\n", time_finish_sort - time_start_build);
	for(ofxSpriteBases::iterator it = m_Sprites.begin();it != m_Sprites.end();it++)
	{
		ofxSpriteBase* sprite = *it;
		if(!(sprite->GetOcclusion() == SPRITE_OCCLUSION_IN_SCREEN || sprite->GetOcclusion() == SPRITE_OCCLUSION_UNKNOWN) ||
			!sprite->IsVisible())
		{
			continue;
		}
		ofxSpriteCommand* command;
		if(m_Commands.size() == 0)
		{
			command = new ofxSpriteCommand();
			command->SetShader(sprite->GetShader());
			command->SetTexture(sprite->GetTexture());
			bool push_success = command->PushSprite(sprite);
			assert(push_success);
			m_Commands.push_back(command);
		}
		else
		{
			command = m_Commands.back();
			bool push_success = command->PushSprite(sprite);
			if(command->GetShader() != sprite->GetShader() 
				|| command->GetTexture() != sprite->GetTexture() 
				|| !push_success)
			{
				command = new ofxSpriteCommand();
				command->SetShader(sprite->GetShader());
				command->SetTexture(sprite->GetTexture());
				push_success = command->PushSprite(sprite);
				m_Commands.push_back(command);
				assert(push_success);
			}
		}
	}
	//unsigned long long time_finish_build = ofGetSystemTime();
	//printf("build time =  %llu\n", time_finish_build - time_start_build);
#ifdef _DEBUG
	m_DrawnBatches = 0;
	m_DrawnVertices = 0;
	unsigned long long time_start_render = ofGetSystemTime();
#endif
	m_TransformMatrix = ofGetCurrentMatrix(OF_MATRIX_MODELVIEW)*ofGetCurrentMatrix(OF_MATRIX_PROJECTION);
	//m_Camera->begin();
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		//glDisable(GL_DEPTH_TEST);// transparent isn't work well with depth test
		glDepthMask(GL_FALSE);
		for(ofxSpriteCommands::iterator it = m_Commands.begin();it != m_Commands.end();it++)
		{
			ofxSpriteCommand* cmd = *it;
			cmd->Render();
#ifdef _DEBUG
			m_DrawnBatches++;
			m_DrawnVertices += cmd->GetVerticesSize();
#endif
		}
		//glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}
	//m_Camera->end();
#ifdef _DEBUG
	unsigned long long time_finish_render = ofGetSystemTime();
	m_RenderTimeMilisecond = time_finish_render - time_start_render;
#endif
}
void ofxSpriteRenderer::PushSprite(ofxSpriteBase* sprite)
{
	sprite->SetID(m_Sprites.size());
	m_Sprites.push_back(sprite);
}
void ofxSpriteRenderer::EraseSprite(ofxSpriteBase* sprite)
{
	m_Sprites.erase(m_Sprites.begin() + sprite->GetID());
	sprite->SetID(-1);
}

void ofxSpriteRenderer::Update()
{
	float delta_time = 0.0f;
	
	if(ofGetFrameRate() > 1)
		delta_time = 1.0/ofGetFrameRate();
#ifdef _DEBUG
	unsigned long long time_start_update = ofGetSystemTime();
#endif
	float width = m_RenderRect.width;
	float height = m_RenderRect.height*2.0;
	m_WorldRect.x = m_Camera->getPosition().x - width*0.5;
	m_WorldRect.y = m_Camera->getPosition().z - height*0.5;
	m_WorldRect.width = width;
	m_WorldRect.height = height;
	m_ExpandedWorldRect = ofRectangle(
		m_WorldRect.x - FAR_SCREEN_DISTANCE_THRESHOLD, 
		m_WorldRect.y - FAR_SCREEN_DISTANCE_THRESHOLD, 
		m_WorldRect.width + FAR_SCREEN_DISTANCE_THRESHOLD*2, 
		m_WorldRect.height + FAR_SCREEN_DISTANCE_THRESHOLD*2);
	ofxSpriteBases::iterator it = m_Sprites.begin();
	for(;it != m_Sprites.end();it++)
	{
		ofxSpriteBase* item = *it;
		item->Update(delta_time);
		item->SubmitChanges();
	}
	m_CameraMove = false;
	m_CameraForce = false;
#ifdef _DEBUG
	unsigned long long time_finish_update = ofGetSystemTime();
	m_UpdateTimeMilisecond = time_finish_update - time_start_update;
#endif
}
void ofxSpriteRenderer::SetRenderSize(unsigned int width, unsigned int height)
{
	m_CameraMove = true;
	m_CameraForce = true;
	for(int i=0;i<m_Sprites.size();i++)
	{
		m_Sprites[i]->SetDimensionChange(true);
	}
	m_Camera->SetScale(min(width, height)*0.5);
	m_RenderRect.width = width;
	m_RenderRect.height = height;
	m_RenderRect.x = -(width*0.5f);
	m_RenderRect.y = -(height*0.5f);
}
void ofxSpriteRenderer::MoveCamera(float x, float z)
{
	MoveCamera(ofVec2f(x, z));
}
void ofxSpriteRenderer::MoveCamera(ofVec2f accelerator)
{
	m_Camera->move(ofVec3f(accelerator.x, 0.0f, accelerator.y));
	m_Camera->lookAt(m_Camera->getPosition() - ofVec3f(0.0f, 1.0f, 1.0f));
	m_CameraMove = true;
	if(abs(accelerator.x) > FAR_SCREEN_SPEED_THRESHOLD || abs(accelerator.y) > FAR_SCREEN_SPEED_THRESHOLD)
		m_CameraForce = true;
}
void ofxSpriteRenderer::FocusCamera(ofVec2f position)
{
	ofVec3f position3d(position.x, 0.0f, position.y);
	m_Camera->setGlobalPosition(position3d + ofVec3f(0.0f, 1.0f, 1.0f));
	m_Camera->lookAt(position3d);
	m_CameraForce = true;
}
void ofxSpriteRenderer::Make2DCamera()
{
	ofVec2f position(m_Camera->getPosition().x, m_Camera->getPosition().z - 1.0f);
	FocusCamera(position);
}
ofxOrthoCamera* ofxSpriteRenderer::GetCamera()
{
	return m_Camera;
}
bool ofxSpriteRenderer::IsCameraMove()
{
	return m_CameraMove;
}
bool ofxSpriteRenderer::IsCameraForce()
{
	return m_CameraForce;
}
ofMatrix4x4 ofxSpriteRenderer::GetProjectionMatrix()
{
	return m_Camera->GetProjectionMatrix();
}
ofMatrix4x4 ofxSpriteRenderer::GetModelViewMatrix()
{
	return m_Camera->GetModelViewMatrix();
}
ofMatrix4x4 ofxSpriteRenderer::GetModelViewProjectionMatrix()
{
	return m_Camera->GetModelViewProjectionMatrix();
}
ofMatrix4x4 ofxSpriteRenderer::GetInverseCameraMatrix()
{
	return m_Camera->GetInverseCameraMatrix();
}
ofMatrix4x4 ofxSpriteRenderer::GetTransformation()
{
	return m_TransformMatrix;
}
ofMatrix4x4 ofxSpriteRenderer::GetInverseModelViewMatrix()
{
	return m_Camera->GetInverseModelViewMatrix();
}
ofRectangle ofxSpriteRenderer::GetRenderRect()
{
	return m_RenderRect;
}
ofRectangle ofxSpriteRenderer::GetWorldRect()
{
	return m_WorldRect;
}
ofRectangle ofxSpriteRenderer::GetExpandedWorldRect()
{
	return m_ExpandedWorldRect;
}
#ifdef _DEBUG
unsigned int ofxSpriteRenderer::GetSpriteNumber()
{
	return m_Sprites.size();
}
int	ofxSpriteRenderer::GetDrawCall()
{
	return m_DrawnBatches;
}
int	ofxSpriteRenderer::GetDrawVertices()
{
	return m_DrawnVertices;
}
unsigned int ofxSpriteRenderer::GetUpdateTimeMilisecond()
{
	return m_UpdateTimeMilisecond;
}
unsigned int ofxSpriteRenderer::GetRenderTimeMilisecond()
{
	return m_RenderTimeMilisecond;
}
#endif