#pragma once
#include "ofxBaseSprite.h"

/*
note: this effect has all of it's particle moving freely in 3d space
*/
/* ----------------------------------
// WIP
---------------------------------- */

const int MAX_PARTICLE3D_COUNT = 200;

struct ofxEmitter3D
{
	ofVec3f			position;
	int				emission_rate;
	int				emission_rate_var;
	float			emission_time;
	float			emission_time_var;
	float			cooldown;
	float			life;
	float			life_var;
	float			radius;
	float			radius_var;
	float			speed;
	float			speed_var;
	float			accel;
	float			accel_var;
	float			angle;
	float			angle_var;
	float			radial_accel;
	float			radial_accel_var;
	float			tangental_x_accel;
	float			tangental_x_accel_var;
	float			tangental_y_accel;
	float			tangental_y_accel_var;
	float			size;
	float			size_var;
	float			size_accel;
	float			opacity;
	float			opacity_var;
	float			opacity_accel;
	float			color_intensity;
	float			color_intensity_var;
	ofFloatColor	color;
	ofFloatColor	color_var;
	ofFloatColor	color_accel;
};

// TODO: ofxParticle3D inherrit from ofxBaseSprite
class ofxParticle3D
	: public ofxBaseSprite
{
	friend class ofxParticleEffect3D;
private:
	static ofxTexture*			m_SharedParticleTexture;
	static vector<ofRectangle>	m_SharedParticleUVs;
public:
	static void					LoadSharedParticleTexture();
private:
	ofxEmitter3D*	m_Emitter;
	float			m_Size;
	float			m_Life;
	float			m_Speed;
	float			m_Accel;
	float			m_RadialAccel;
	float			m_TangentalXAccel;
	float			m_TangentalYAccel;
public:
	ofxParticle3D();
	~ofxParticle3D();
	void Initialize(ofxEmitter3D* emitter);
	virtual void Update(float delta_time);
	virtual void SubmitChanges();
};

typedef vector<ofxEmitter3D*> ofxEmitter3Ds;
class ofxParticleEffect3D
{
private:
	ofVec3f						m_Position;
	ofxEmitter3Ds				m_Emitters;
	ofxParticle3D*				m_ParticlePool[MAX_PARTICLE3D_COUNT];
	unsigned short				m_ParticleCount;
	bool						m_Paused;
	bool						m_Stopped;
public:
	ofxParticleEffect3D();
	~ofxParticleEffect3D();
	void						Load(string xml_file);
	void						AddEmitter(ofxEmitter3D* emitter);
	void						Update(float delta_time);
public:
	void						PauseResume();
	bool						IsPaused();
	void						Stop();
	void						ForceStop();
	bool						IsStopped();
};
#define DEFAULT_PARTICLE3D_SHADER "sprite2d"