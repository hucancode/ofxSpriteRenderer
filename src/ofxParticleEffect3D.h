#pragma once
#include "ofxSpriteBase.h"
#include "ofxSpriteQuad.h"
/*
note: this effect has all of it's particle moving around 3d space
*/

const int MAX_PARTICLE3D_COUNT = 100;

struct ofxParticle3D
{
	float			life;
	float			angle;
	float			radial_accel;
	float			tangental_accel;
	ofxSpriteQuad*	sprite;
};
struct ofxEmitter3D
{
	ofVec3f		position;
	int			emission_rate;
	int			emission_rate_var;
	float		emission_time;
	float		emission_time_var;
	float		life;
	float		life_var;
	float		radius;
	float		radius_var;
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
typedef vector<ofxEmitter3D*> ofxEmitter3Ds;
class ofxParticleEffect3D
	:public ofxSpriteBase
{
private:
	ofxEmitter3Ds m_Emitters;
	ofxParticle3D m_ParticlePool[MAX_PARTICLE3D_COUNT];
	unsigned short m_ParticleCount;
public:
	ofxParticleEffect3D();
	~ofxParticleEffect3D();
	void AddEmitter(ofxEmitter3D* emitter);
	virtual void Update(float delta_time);
};