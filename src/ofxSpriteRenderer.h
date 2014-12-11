#pragma once
#include "ofMain.h"
#include "ofxOrthoCamera.h"
#include "ofxBaseSprite.h"
#include "ofxSpriteCommand.h"

#define FAR_SCREEN_DISTANCE_THRESHOLD 6000.0
#define FAR_SCREEN_SPEED_THRESHOLD 30.0
#define FAR_SCREEN_UPDATE_SEQUENCE 100
#define IN_SCREEN_UPDATE_SEQUENCE 20

class ofxSpriteRenderer
{
	friend ofxSpriteCommand;
	friend ofxBaseSprite;
public:
	static ofxSpriteRenderer*	s_Instance;
	static ofxSpriteRenderer*	GetInstance();
	static void					DestroyInstance();
private:
	ofxBaseSprites			m_Sprites;
	ofxBaseCommands			m_Commands;
#ifdef _DEBUG
	int						m_DrawnBatches;
	int						m_DrawnVertices;
	unsigned int			m_UpdateTimeMilisecond;
	unsigned int			m_RenderTimeMilisecond;
#endif
	bool					m_CameraMove;
	bool					m_CameraForce;
	ofRectangle				m_WorldRect;
	ofRectangle				m_ExpandedWorldRect;
	ofRectangle				m_RenderRect;
	ofxOrthoCamera*			m_Camera;
	ofMatrix4x4				m_TransformMatrix;
public:
	ofxSpriteRenderer();
	~ofxSpriteRenderer();
	void					Render();
	void					PushSprite(ofxBaseSprite* sprite);
	void					EraseSprite(ofxBaseSprite* sprite);
	void					Update();
	void					SetRenderSize(unsigned int width, unsigned int height);
private:
	void					BuildCommands();
public:
	void					MoveCamera(float x, float z);
	void					MoveCamera(ofVec2f accelerator);
	void					FocusCamera(ofVec2f position);
	void					Make2DCamera();
	ofxOrthoCamera*			GetCamera();
	bool					IsCameraMove();
	bool					IsCameraForce();
	ofMatrix4x4				GetProjectionMatrix();
	ofMatrix4x4				GetModelViewMatrix();
	ofMatrix4x4				GetModelViewProjectionMatrix();
	ofMatrix4x4				GetInverseCameraMatrix();
	ofMatrix4x4				GetTransformation();
	ofMatrix4x4				GetInverseModelViewMatrix();
	ofRectangle				GetRenderRect();
	ofRectangle				GetWorldRect();
	ofRectangle				GetExpandedWorldRect();
#ifdef _DEBUG
	unsigned int			GetSpriteNumber();
	int						GetDrawCall();
	int						GetDrawVertices();
	unsigned int			GetUpdateTimeMilisecond();
	unsigned int			GetRenderTimeMilisecond();
#endif
};

#define ofxRENDERER ofxSpriteRenderer::GetInstance()