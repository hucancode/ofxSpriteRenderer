#pragma once

#include "ofMain.h"

class ofxOrthoCamera 
	:public ofEasyCam 
{
private:
	float			m_Scale;
	float			m_ScaleX;
	float			m_ScaleY;
	ofRectangle		m_Viewport;
	ofMatrix4x4		m_ProjectionMatrix;
	ofMatrix4x4		m_LocalTransformMatrix;
	ofMatrix4x4		m_ModelViewMatrix;
	ofMatrix4x4		m_ModelViewProjectionMatrix;
	ofMatrix4x4		m_InverseCameraMatrix;
	ofMatrix4x4		m_InverseModelViewMatrix;
	bool			m_ProjectionUpdated;
	bool			ModelViewUpdated();
	bool			m_ModelViewProjectionnUpdated;
	bool			m_InverseCameraUpdated;
	bool			m_InverseModelViewUpdated;
public:
	ofxOrthoCamera();
	~ofxOrthoCamera();
	virtual void	begin(ofRectangle rect = ofGetWindowRect());
	ofMatrix4x4		GetProjectionMatrix();
	ofMatrix4x4		GetModelViewMatrix();
	ofMatrix4x4		GetModelViewProjectionMatrix();
	ofMatrix4x4		GetInverseCameraMatrix();
	ofMatrix4x4		GetInverseModelViewMatrix();
	ofVec3f			OrthoScreenToWorld(ofVec3f ScreenXYZ, ofRectangle viewport=ofGetWindowRect());
	void			SetScale(float scale);
	float			GetScale();
};