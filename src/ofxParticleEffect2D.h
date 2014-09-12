#pragma once
#include "ofxSpriteBase.h"

/*
note: this effect has all of it's particle stick to a plane
*/

const int MAX_PARTICLE2D_COUNT = 200;

struct ofxParticle2D
{
	ofVec2f		position;
	float		life;
	float		speed;
	float		angle;
	float		radial_accel;
	float		tangental_accel;
	ofxVertex	vertices[4];
};
struct ofxEmitter2D
{
	ofVec2f		position;
	int			emission_rate;
	int			emission_rate_var;
	float		emission_time;
	float		emission_time_var;
	float		life;
	float		life_var;
	float		radius;
	float		radius_var;
	float		speed;
	float		speed_var;
	float		angle;
	float		angle_var;
	float		radial_accel;
	float		radial_accel_var;
	float		tangental_accel;
	float		tangental_accel_var;
	float		begin_opacity;
	float		begin_opacity_var;
	float		begin_color_intensity;
	float		begin_color_intensity_var;
	ofColor		begin_color;
	ofColor		begin_color_var;
	float		end_opacity;
	float		end_opacity_var;
	float		end_color_intensity;
	float		end_color_intensity_var;
	ofColor		end_color;
	ofColor		end_color_var;
};
typedef vector<ofxEmitter2D*> ofxEmitter2Ds;
class ofxParticleEffect2D
	:public ofxSpriteBase
{
private:
	ofxEmitter2Ds			m_Emitters;
	ofxParticle2D			m_ParticlePool[MAX_PARTICLE2D_COUNT];
	unsigned short			m_ParticleCount;
public:
	ofxParticleEffect2D();
	~ofxParticleEffect2D();
	void					Load(string xml_file);
	void					AddEmitter(ofxEmitter2D* emitter);
	virtual void			Update(float delta_time);
	void					PauseResume();
	bool					IsPaused();
};