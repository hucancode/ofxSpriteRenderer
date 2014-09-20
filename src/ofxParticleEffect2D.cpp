#include "ofxParticleEffect2D.h"
#include "ofxTextureCache.h"
#include "ofxSpriteRenderer.h"
#include "math.h"

ofxParticleEffect2D::ofxParticleEffect2D()
{
	ofxSpriteBase::ofxSpriteBase();
	m_Vertices = new ofxVertex[MAX_PARTICLE2D_COUNT*4];
	m_ParticleCount = 0;
	m_Paused = false;
	m_Texture = m_SharedParticleTexture;
	LoadShader(DEFAULT_PARTICLE_SHADER);
	ofxRENDERER->PushSprite(this);
}
ofxParticleEffect2D::~ofxParticleEffect2D()
{
	delete[] m_Vertices;
}
void ofxParticleEffect2D::Load(string xml_file)
{
}
void ofxParticleEffect2D::AddEmitter(ofxEmitter2D* emitter)
{
	m_Emitters.push_back(emitter);
}
void ofxParticleEffect2D::Update(float delta_time)
{
	if(m_Paused) return;
	for(int i = 0;i < m_Emitters.size();i++)
	{
		ofxEmitter2D* e = m_Emitters[i];
		e->cooldown -= delta_time;
		if(e->cooldown <= 0)
		{
			int particle_to_emit = e->emission_rate + ofRandom(e->emission_rate_var);
			int j = m_ParticleCount;
			m_ParticleCount += particle_to_emit;
			if(m_ParticleCount > MAX_PARTICLE2D_COUNT)
			{
				m_ParticleCount = MAX_PARTICLE2D_COUNT;
			}
			//ofLogNotice() << "emitting "<<m_ParticleCount<<endl;
			for(;j<m_ParticleCount;j++)
			{
				ofxParticle2D& item = m_ParticlePool[j];
				// speed
				item.emitter = e;
				item.life = e->life + ofRandom(e->life_var);
				item.speed = e->speed + ofRandom(e->speed_var);
				item.accel = e->accel + ofRandom(e->accel_var);
				item.radial_accel = e->radial_accel + ofRandom(e->radial_accel_var);
				item.tangental_accel = e->tangental_accel + ofRandom(e->tangental_accel_var);
				// position
				float radius = e->radius + ofRandom(e->radius_var);
				float angle = e->angle + ofRandom(e->angle_var);
				item.position = radius*ofVec2f(1,1).rotateRad(angle);
				item.size = e->size + ofRandom(e->size_var);
				// uv
				ofRectangle texture_rect = m_SharedParticleUVs[(int)ofRandom(m_SharedParticleUVs.size()-1)];
				item.vertices[0].u = texture_rect.getMinX();
				item.vertices[0].v = texture_rect.getMinY();
				item.vertices[1].u = texture_rect.getMaxX();
				item.vertices[1].v = texture_rect.getMinY();
				item.vertices[2].u = texture_rect.getMaxX();
				item.vertices[2].v = texture_rect.getMaxY();
				item.vertices[3].u = texture_rect.getMinX();
				item.vertices[3].v = texture_rect.getMaxY();
				// TODO: color
				float r = e->color.r + ofRandom(e->color_var.r);
				float g = e->color.g + ofRandom(e->color_var.g);
				float b = e->color.b + ofRandom(e->color_var.b);
				float opacity = e->opacity + ofRandom(e->opacity_var);
				float intensity = e->color_intensity + ofRandom(e->color_intensity_var);
				item.vertices[0].r = r;
				item.vertices[1].r = r;
				item.vertices[2].r = r;
				item.vertices[3].r = r;
				item.vertices[0].g = g;
				item.vertices[1].g = g;
				item.vertices[2].g = g;
				item.vertices[3].g = g;
				item.vertices[0].b = b;
				item.vertices[1].b = b;
				item.vertices[2].b = b;
				item.vertices[3].b = b;
				item.vertices[0].color_intensity = intensity;
				item.vertices[1].color_intensity = intensity;
				item.vertices[2].color_intensity = intensity;
				item.vertices[3].color_intensity = intensity;
				item.vertices[0].opacity = opacity;
				item.vertices[1].opacity = opacity;
				item.vertices[2].opacity = opacity;
				item.vertices[3].opacity = opacity;
			}
			e->cooldown = e->emission_time + ofRandom(e->emission_time_var);
		}
	}
	for(int i=0;i<m_ParticleCount;i++)
	{
		ofxParticle2D& item = m_ParticlePool[i];
		item.life -= delta_time;
		while(item.life <= 0 || item.vertices[0].opacity <= 0)// life
		{
			m_ParticleCount--;
			//ofLogNotice() << "destroying "<<m_ParticleCount<<endl;
			if(m_ParticleCount == 0) break;
			ofxParticle2D p = m_ParticlePool[m_ParticleCount];
			m_ParticlePool[m_ParticleCount] = m_ParticlePool[i];
			m_ParticlePool[i] = p;
		}
		if(m_ParticleCount == 0) break;
		{// force
			int radial = ofVec2f(1,0).angle(item.position - item.emitter->position);
			if(radial < 0) radial += 360;
			int tangental = radial + 90;
			if(tangental >= 360) tangental -= 360;
			item.speed += delta_time*item.accel;
			item.size += delta_time*item.emitter->size_accel;
			float distance = delta_time*item.speed;
			ofVec2f radial_force = distance*item.radial_accel*GetForceFromAngle(radial);
			ofVec2f tangental_force = distance*item.tangental_accel*GetForceFromAngle(tangental);
			item.position += radial_force;
			item.position += tangental_force;

			float half_size = item.size*0.5f;
			item.vertices[0].x = item.position.x - half_size;
			item.vertices[0].y = item.position.y - half_size;
			item.vertices[0].z = m_Position.z;
			item.vertices[1].x = item.vertices[0].x + item.size;
			item.vertices[1].y = item.vertices[0].y;
			item.vertices[1].z = item.vertices[0].z;
			item.vertices[2].x = item.vertices[1].x;
			item.vertices[2].y = item.vertices[1].y + item.size;
			item.vertices[2].z = item.vertices[1].z;
			item.vertices[3].x = item.vertices[0].x;
			item.vertices[3].y = item.vertices[2].y;
			item.vertices[3].z = item.vertices[2].z;
		}
		{// TODO: color
			float opa_accel = item.emitter->opacity_accel*delta_time;
			float r_accel = item.emitter->color_accel.r*delta_time;
			float g_accel = item.emitter->color_accel.r*delta_time;
			float b_accel = item.emitter->color_accel.r*delta_time;
			item.vertices[0].r += r_accel;
			item.vertices[1].r += r_accel;
			item.vertices[2].r += r_accel;
			item.vertices[3].r += r_accel;
			item.vertices[0].g += g_accel;
			item.vertices[1].g += g_accel;
			item.vertices[2].g += g_accel;
			item.vertices[3].g += g_accel;
			item.vertices[0].b += b_accel;
			item.vertices[1].b += b_accel;
			item.vertices[2].b += b_accel;
			item.vertices[3].b += b_accel;
			item.vertices[0].opacity += opa_accel;
			item.vertices[1].opacity += opa_accel;
			item.vertices[2].opacity += opa_accel;
			item.vertices[3].opacity += opa_accel;
		}
	}
}
void ofxParticleEffect2D::SubmitChanges()
{
	m_VerticesSize = m_ParticleCount*4;
	size_t size = sizeof(ofxVertex)*4;
	for(int i=0;i<m_ParticleCount;i++)
	{
		memcpy(m_Vertices+i*4, m_ParticlePool[i].vertices, size);
	}
}
void ofxParticleEffect2D::PauseResume()
{
	m_Paused = !m_Paused;
}
bool ofxParticleEffect2D::IsPaused()
{
	return m_Paused;
}
// shared texture
vector<ofRectangle>	ofxParticleEffect2D::m_SharedParticleUVs;
ofxTexture* ofxParticleEffect2D::m_SharedParticleTexture = 0;
void ofxParticleEffect2D::LoadSharedParticleTexture()
{
	m_SharedParticleTexture = new ofxTexture();
	m_SharedParticleTexture->Load("data/particle_sheet.png");
	m_SharedParticleUVs.push_back(ofRectangle(0,0,1,1));
}

// shared math utilities
const float ofxParticleEffect2D::DEGREE_TO_RADIAL = 0.01745329251994329577;
const float ofxParticleEffect2D::RADIAL_TO_DEGREE = 57.295779513082320877;
ofVec2f ofxParticleEffect2D::m_ForceTable[360];

void ofxParticleEffect2D::BuildForceTable()
{
	for(int i=0;i<360;i++)
	{
		float i_rad = i*DEGREE_TO_RADIAL;
		m_ForceTable[i] = ofVec2f(cos(i_rad),sin(i_rad));
	}
}
ofVec2f ofxParticleEffect2D::GetForceFromAngle(int angle)
{
	return m_ForceTable[angle];
}
float ofxParticleEffect2D::GetAngleFromForceRadial(ofVec2f force)
{
	return atan2(force.y, force.x);
}
int ofxParticleEffect2D::GetAngleFromForce(ofVec2f force)
{
	return GetAngleFromForceRadial(force)*RADIAL_TO_DEGREE;
}