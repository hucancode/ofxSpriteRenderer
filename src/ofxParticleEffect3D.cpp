#include "ofxParticleEffect3D.h"
#include "ofxTextureCache.h"
#include "ofxSpriteRenderer.h"
#include "math.h"

ofxParticle3D::ofxParticle3D()
{
	ofxBaseSprite::ofxBaseSprite();
	m_Texture = m_SharedParticleTexture;
	m_VerticesSize = 4;
	m_Vertices = new ofxVertex[m_VerticesSize];
	LoadShader(DEFAULT_PARTICLE3D_SHADER);
	ofxRENDERER->PushSprite(this);
}
ofxParticle3D::~ofxParticle3D()
{
	delete[] m_Vertices;
}
void ofxParticle3D::SubmitChanges()
{
	//TODO: implement
}
void ofxParticle3D::Update(float delta_time)
{
	{// force
		// unit force
		ofVec3f u_radial, u_tangental_x, u_tangental_y;
		u_radial = m_Position - m_Emitter->position;
		if(u_radial != ofVec3f::zero())
		{
			u_radial = u_radial.normalized();
			if(u_radial.z != 0)
			{
				u_tangental_y = ofVec3f(0, 1, u_radial.y/u_radial.z);
				u_tangental_y = u_tangental_y.normalized();
			}
			else if(u_radial.y != 0)
			{
				u_tangental_y = ofVec3f(0, u_radial.z/u_radial.y, 1);
				u_tangental_y = u_tangental_y.normalized();
			}
			else
			{
				u_tangental_y = ofVec3f::zero();
			}
			if(u_radial.z != 0)
			{
				u_tangental_x = ofVec3f(1, 0, u_radial.x/u_radial.z);
				u_tangental_x = u_tangental_x.normalized();
			}
			else if(u_radial.x != 0)
			{
				u_tangental_x = ofVec3f(1, 0, u_radial.z/u_radial.x);
				u_tangental_x = u_tangental_x.normalized();
			}
			else
			{
				u_tangental_x = ofVec3f::zero();
			}
			// real force
			m_Speed += delta_time*m_Accel;
			float distance = delta_time*m_Speed;
			ofVec3f radial_force = distance*m_RadialAccel*u_radial;
			ofVec3f tangental_x_force = distance*m_TangentalXAccel*u_tangental_x;
			ofVec3f tangental_y_force = distance*m_TangentalYAccel*u_tangental_y;
			m_Position += radial_force;
			m_Position += tangental_x_force;
			m_Position += tangental_y_force;
		}
		m_Size += delta_time*m_Emitter->size_accel;
		float half_size = m_Size*0.5f;
		m_Vertices[0].x = m_Position.x - half_size;
		m_Vertices[0].y = m_Position.y - half_size;
		m_Vertices[0].z = m_Position.z;
		m_Vertices[1].x = m_Vertices[0].x + m_Size;
		m_Vertices[1].y = m_Vertices[0].y;
		m_Vertices[1].z = m_Vertices[0].z;
		m_Vertices[2].x = m_Vertices[1].x;
		m_Vertices[2].y = m_Vertices[1].y + m_Size;
		m_Vertices[2].z = m_Vertices[1].z;
		m_Vertices[3].x = m_Vertices[0].x;
		m_Vertices[3].y = m_Vertices[2].y;
		m_Vertices[3].z = m_Vertices[2].z;
	}
	{// color
		float opa_accel = m_Emitter->opacity_accel*delta_time;
		float r_accel = m_Emitter->color_accel.r*delta_time;
		float g_accel = m_Emitter->color_accel.r*delta_time;
		float b_accel = m_Emitter->color_accel.r*delta_time;
		m_Vertices[0].r += r_accel;
		m_Vertices[1].r += r_accel;
		m_Vertices[2].r += r_accel;
		m_Vertices[3].r += r_accel;
		m_Vertices[0].g += g_accel;
		m_Vertices[1].g += g_accel;
		m_Vertices[2].g += g_accel;
		m_Vertices[3].g += g_accel;
		m_Vertices[0].b += b_accel;
		m_Vertices[1].b += b_accel;
		m_Vertices[2].b += b_accel;
		m_Vertices[3].b += b_accel;
		m_Vertices[0].opacity += opa_accel;
		m_Vertices[1].opacity += opa_accel;
		m_Vertices[2].opacity += opa_accel;
		m_Vertices[3].opacity += opa_accel;
	}
}
void ofxParticle3D::Initialize(ofxEmitter3D* emitter)
{
	// speed
	m_Emitter = emitter;
	m_Life = emitter->life + ofRandom(emitter->life_var);
	m_Speed = emitter->speed + ofRandom(emitter->speed_var);
	m_Accel = emitter->accel + ofRandom(emitter->accel_var);
	m_RadialAccel = emitter->radial_accel + ofRandom(emitter->radial_accel_var);
	m_TangentalXAccel = emitter->tangental_x_accel + ofRandom(emitter->tangental_x_accel_var);
	m_TangentalYAccel = emitter->tangental_y_accel + ofRandom(emitter->tangental_y_accel_var);
	// position
	float radius = emitter->radius + ofRandom(emitter->radius_var);
	float angle = emitter->angle + ofRandom(emitter->angle_var);
	m_Position = radius*ofVec2f(1,1).rotateRad(angle);
	m_Size = emitter->size + ofRandom(emitter->size_var);
	// uv
	ofRectangle texture_rect = m_SharedParticleUVs[(int)ofRandom(m_SharedParticleUVs.size()-1)];
	m_Vertices[0].u = texture_rect.getMinX();
	m_Vertices[0].v = texture_rect.getMinY();
	m_Vertices[1].u = texture_rect.getMaxX();
	m_Vertices[1].v = texture_rect.getMinY();
	m_Vertices[2].u = texture_rect.getMaxX();
	m_Vertices[2].v = texture_rect.getMaxY();
	m_Vertices[3].u = texture_rect.getMinX();
	m_Vertices[3].v = texture_rect.getMaxY();
	// color
	float r = emitter->color.r + ofRandom(emitter->color_var.r);
	float g = emitter->color.g + ofRandom(emitter->color_var.g);
	float b = emitter->color.b + ofRandom(emitter->color_var.b);
	float opacity = emitter->opacity + ofRandom(emitter->opacity_var);
	float intensity = emitter->color_intensity + ofRandom(emitter->color_intensity_var);
	m_Vertices[0].r = r;
	m_Vertices[1].r = r;
	m_Vertices[2].r = r;
	m_Vertices[3].r = r;
	m_Vertices[0].g = g;
	m_Vertices[1].g = g;
	m_Vertices[2].g = g;
	m_Vertices[3].g = g;
	m_Vertices[0].b = b;
	m_Vertices[1].b = b;
	m_Vertices[2].b = b;
	m_Vertices[3].b = b;
	m_Vertices[0].color_intensity = intensity;
	m_Vertices[1].color_intensity = intensity;
	m_Vertices[2].color_intensity = intensity;
	m_Vertices[3].color_intensity = intensity;
	m_Vertices[0].opacity = opacity;
	m_Vertices[1].opacity = opacity;
	m_Vertices[2].opacity = opacity;
	m_Vertices[3].opacity = opacity;
}
ofxParticleEffect3D::ofxParticleEffect3D()
{
	m_ParticleCount = 0;
	m_Paused = false;
	m_Stopped = false;
	for(int i=0;i<MAX_PARTICLE3D_COUNT;i++)
	{
		m_ParticlePool[i] = 0;
	}
}
ofxParticleEffect3D::~ofxParticleEffect3D()
{
}
void ofxParticleEffect3D::Load(string xml_file)
{
}
void ofxParticleEffect3D::AddEmitter(ofxEmitter3D* emitter)
{
	m_Emitters.push_back(emitter);
}
void ofxParticleEffect3D::Update(float delta_time)
{
	if(m_Paused) return;
	for(int i = 0;i < m_Emitters.size();i++)
	{
		ofxEmitter3D* e = m_Emitters[i];
		e->cooldown -= delta_time;
		if(e->cooldown <= 0)
		{
			int particle_to_emit = e->emission_rate + ofRandom(e->emission_rate_var);
			int j = m_ParticleCount;
			m_ParticleCount += particle_to_emit;
			if(m_ParticleCount > MAX_PARTICLE3D_COUNT)
			{
				m_ParticleCount = MAX_PARTICLE3D_COUNT;
			}
			ofVec3f offset = m_Position + e->position;
			//ofLogNotice() << "emitting "<<m_ParticleCount<<endl;
			for(;j<m_ParticleCount;j++)
			{
				ofxParticle3D* item;
				if(m_ParticlePool[j] == 0)
				{
					m_ParticlePool[j] = new ofxParticle3D();
					item = m_ParticlePool[j];
				}
				item->Initialize(e);
				item->m_Position += offset;
			}
			e->cooldown = e->emission_time + ofRandom(e->emission_time_var);
		}
	}
	for(int i=0;i<m_ParticleCount;i++)
	{
		ofxParticle3D* item = m_ParticlePool[i];
		item->m_Life -= delta_time;
		while(item->m_Life <= 0 || item->m_Vertices[0].opacity <= 0)// life
		{
			m_ParticleCount--;
			//ofLogNotice() << "destroying "<<m_ParticleCount<<endl;
			if(m_ParticleCount == 0) break;
			ofxParticle3D* p = m_ParticlePool[m_ParticleCount];
			m_ParticlePool[m_ParticleCount] = m_ParticlePool[i];
			m_ParticlePool[i] = p;
		}
		if(m_ParticleCount == 0) break;
		item->Update(delta_time);
	}
}

void ofxParticleEffect3D::PauseResume()
{
	m_Paused = !m_Paused;
}
bool ofxParticleEffect3D::IsPaused()
{
	return m_Paused;
}
// shared texture
vector<ofRectangle>	ofxParticle3D::m_SharedParticleUVs;
ofxTexture* ofxParticle3D::m_SharedParticleTexture = 0;
void ofxParticle3D::LoadSharedParticleTexture()
{
	m_SharedParticleTexture = new ofxTexture();
	m_SharedParticleTexture->Load("data/particle_sheet.png");
	m_SharedParticleUVs.push_back(ofRectangle(0,0,1,1));
}