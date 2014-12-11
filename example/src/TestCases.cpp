#include "TestCases.h"
#include "ofxTextureCache.h"
#include "ofxShaderCache.h"
#include "ofxShaderProgramCache.h"
#include "ofxBitmapFontCache.h"
#include "ofxParticleEffect2D.h"
#include "IL/il.h"

Test* current_test = new Particle2DBenchmarkTest();
ofxBaseSprite* spriteObstacle;
void Test::Setup()
{

}
void Test::Update()
{
}
void Test::Render()
{
}
void SpriteTest::Setup()
{
	ofxSpriteQuad* sprite = new ofxSpriteQuad();
	sprite->SetTexture("data/psk_actor_idle.png");
	spriteObstacle = sprite;
}
void SpriteTest::Update()
{
	ofxRENDERER->Update();
}
void SpriteTest::Render()
{
	ofxRENDERER->Render();
}
void RendererTest::Setup()
{
	for(int i=-20;i<20;i++)
	{
		for(int j=-20;j<20;j++)
		{
			ofxSpriteQuad* sprite = new ofxSpriteQuad();
			sprite->SetTexture("data/psk_actor_idle.png");
			sprite->MoveTo(i*192.0f,0.0f,j*192.0f);
			if(i==0 && j==0) spriteObstacle = sprite;
		}
	}
}
void RendererTest::Update()
{
	ofxRENDERER->Update();
}
void RendererTest::Render()
{
	ofxRENDERER->Render();
}

void SortingTest::Setup()
{
	float space_x = 192.0f;
	float space_y = 192.0f;
	for(int i=-40;i<40;i++)
	{
		for(int j=-40;j<40;j++)
		{
			ofxSpriteQuad* sprite = new ofxSpriteQuad();
			sprite->SetTexture("data/psk_actor_idle.png");
			sprite->MoveTo(i*space_x,0.0f,j*space_y);
			if(i==0 && j==0) spriteA = sprite;
			if(i==1 && j==1) spriteB = sprite;
		}
	}
	spriteObstacle = spriteA;
	
}
void SortingTest::Update()
{
	ofxRENDERER->Update();
	if(spriteA->GetPosition().z < 0.2f)
	{
		spriteA->MoveBy(0.0f,0.0f,0.0001f);
	}
	else
	{
		spriteA->MoveBy(0.0f,0.0f,-0.4f);
	}
	if(spriteB->GetPosition().z > -0.2f)
	{
		spriteB->MoveBy(0.0f,0.0f,-0.0001f);
	}
	else
	{
		spriteB->MoveBy(0.0f,0.0f,0.4f);
	}
}
void SortingTest::Render()
{
	ofxRENDERER->Render();
}
void TextureTest::Setup()
{
	float space_x = 192.0f;
	float space_y = 192.0f;
	for(int i=-20;i<20;i++)
	{
		for(int j=-20;j<20;j++)
		{
			ofxSpriteQuad* sprite = new ofxSpriteQuad();
			sprite->SetTexture(("data/sprite"+ofToString(abs(j)%5+1)+".png").c_str());
			sprite->MoveTo(i*space_x,0.0f,j*space_y);
			if(i==0 && j==0) spriteObstacle = sprite;
		}
	}
}
void TextureTest::Update()
{
	ofxRENDERER->Update();
}
void TextureTest::Render()
{
	ofxRENDERER->Render();
}

void AnimationTest::Setup()
{
	ofxSpriteAnimation* animation = new ofxSpriteAnimation();
	animation->SetTexture("data/psk_actor.png");
	animation->MoveTo(0.0f,0.0f,0.0f);
	animation->SetFrameCount(24);
	animation->SetSequenceCount(2);
	animation->SetFrameTime(0,0.1f);
	animation->SetFrameTime(1,0.1f);
	{
		int i=0;
		// run
		for(int x=0;x<3;x++)
		{
			for(int y=0;y<4;y++)
			{
				animation->SetFrameData(i++,x*170,y*170,170,170,0,0,170,170);
			}
		}
		animation->SetSequenceData(0,0,11);
		// respawn
		for(int x=3;x<6;x++)
		{
			for(int y=0;y<4;y++)
			{
				animation->SetFrameData(i++,x*170,y*170,170,170,0,0,170,170);
			}
		}
		animation->SetSequenceData(1,12,23);
	}
	animation->SetSequence(0);
	spriteObstacle = animation;
}
void AnimationTest::Update()
{
	ofxRENDERER->Update();
}
void AnimationTest::Render()
{
	ofxRENDERER->Render();
}
void TextSpriteTest::Setup()
{
	string header = "this is a header, so i draw it big";
	string text = "hello world, this is a long long long text.";
	ofxBitmapFont* font = ofxBITMAPFONTCACHE->GetResource("data/verdana.xml");
	ofxTexture* texture = new ofxTexture();
	ofVec2f dimension_header = font->GetTextDimension(header, 22);
	ofVec2f dimension_text = font->GetTextDimension(text);
	texture->Allocate(max(dimension_header.x, dimension_text.x), dimension_header.y+dimension_text.y);
	texture->DrawString(header, font, ofVec2f(0,0), 22);
	texture->DrawString(text, font, ofVec2f(0,dimension_header.y));
	texture->SubmitChanges();
	std::string texture_name = "txt_verdana"+text;
	ofxTEXTURECACHE->PushResource(texture, texture_name);
	ofxSpriteQuad* sprite = new ofxSpriteQuad();
	sprite->SetTexture(texture_name);
	//sprite->SetOpacity(100);
	spriteObstacle = sprite;
}
void TextSpriteTest::Update()
{
	ofxRENDERER->Update();
}
void TextSpriteTest::Render()
{
	ofxRENDERER->Render();
}
void Particle2DTest::Setup()
{
	ofxParticleEffect2D::LoadSharedParticleTexture();
	ofxParticleEffect2D::BuildForceTable();
	ofxParticleEffect2D* sprite = new ofxParticleEffect2D();
	ofxEmitter2D* emitter = new ofxEmitter2D();
	emitter->position = ofVec2f(0,0);
	emitter->angle = 0;
	emitter->angle_var = TWO_PI;
	emitter->emission_rate = 5;
	emitter->emission_rate_var = 0;
	emitter->emission_time = 0.1f;
	emitter->emission_time_var = 0.0f;
	emitter->life = 3.0f;
	emitter->life_var = 0.0f;
	emitter->radial_accel = 1.0f;
	emitter->radial_accel_var = 1.0f;
	emitter->tangental_accel = -2.0f;
	emitter->tangental_accel_var = 1.0f;
	emitter->speed = 15.0f;
	emitter->speed_var = 20.0f;
	emitter->accel = 10.0f;
	emitter->accel_var = 15.0f;
	emitter->radius = 0.0f;
	emitter->radius_var = 20.0f;
	// TODO: color
	emitter->size = 15.0f;
	emitter->size_var = 35.0f;
	emitter->size_accel = 10.0f;
	emitter->opacity = 0.5f;
	emitter->opacity_var = 0.5f;
	emitter->opacity_accel = -0.2f;
	emitter->color_intensity = 0.0f;
	emitter->color_intensity_var = 1.0f;
	emitter->color = ofColor(0.0f,0.0f,0.0f);
	emitter->color_var = ofColor(1.0f,1.0f,1.0f);
	emitter->color_accel = ofColor(0.1f,0.1f,0.1f);


	sprite->AddEmitter(emitter);
	spriteObstacle = sprite;
}
void Particle2DTest::Update()
{
	ofxRENDERER->Update();
}
void Particle2DTest::Render()
{
	ofxRENDERER->Render();
}
void TextureCompressionTest::Setup()
{
	//ofxSpriteQuad* sprite1 = new ofxSpriteQuad();
	//sprite1->SetTexture("data/sprite1.png");
	ofxSpriteQuad* sprite2 = new ofxSpriteQuad();
	sprite2->MoveBy(200.0f,0.0f,0.0f);
	sprite2->SetTexture("data/tianwang3_0.png");
	sprite2->GetTexture()->SetCompressed(true);
	sprite2->GetTexture()->SubmitChanges();
	spriteObstacle = sprite2;
}
void TextureCompressionTest::Update()
{
	ofxRENDERER->Update();
}
void TextureCompressionTest::Render()
{
	ofxRENDERER->Render();
}
void SpriteBenchmarkTest::Setup()
{
	float space_x = 192.0f;
	float space_y = 192.0f;
	for(int i=-40;i<40;i++)
	{
		for(int j=-50;j<50;j++)
		{
			ofxSpriteQuad* sprite = new ofxSpriteQuad();
			sprite->SetTexture("data/psk_actor_idle.png");
			sprite->MoveTo(i*space_x,0.0f,j*space_y);
			if(i==0 && j==0) spriteObstacle = sprite;
		}
	}
}
void SpriteBenchmarkTest::Update()
{
	ofxRENDERER->Update();
}
void SpriteBenchmarkTest::Render()
{
	ofxRENDERER->Render();
}
void SortBenchmarkTest::Setup()
{
	float space_x = 192.0f;
	float space_y = 192.0f;
	for(int i=-50;i<50;i++)
	{
		for(int j=-50;j<50;j++)
		{
			ofxSpriteQuad* sprite = new ofxSpriteQuad();
			sprite->SetTexture(("data/sprite"+ofToString(abs(j)%5+1)+".png").c_str());
			sprite->MoveTo(i*space_x,0.0f,j*space_y);
			if(i==0 && j==0) spriteObstacle = sprite;
			if(i==0 && j==0) spriteA = sprite;
			if(i==1 && j==1) spriteB = sprite;
		}
	}
}
void SortBenchmarkTest::Update()
{
	ofxRENDERER->Update();
	if(spriteA->GetPosition().z < 1000.0f)
	{
		spriteA->MoveBy(0.0f,0.0f,15.0f);
	}
	else
	{
		spriteA->MoveBy(0.0f,0.0f,-2000.0f);
	}
	spriteB->MoveTo(ofxRENDERER->GetCamera()->getPosition());
	int a = 10;
}
void SortBenchmarkTest::Render()
{
	ofxRENDERER->Render();
	printf("update time = %u\nrender time = %u\n",
		ofxRENDERER->GetUpdateTimeMilisecond(),
		ofxRENDERER->GetRenderTimeMilisecond());
}
void AnimationBenchmarkTest::Setup()
{
	float space_x = 192.0f;
	float space_y = 192.0f;
	for(int i=-50;i<50;i++)
	{
		for(int j=-50;j<50;j++)
		{
			ofxSpriteAnimation* animation = new ofxSpriteAnimation();
			animation->SetTexture("data/psk_actor.png");
			animation->MoveTo(i*space_x,0.0f,j*space_y);
			animation->SetFrameCount(24);
			animation->SetSequenceCount(2);
			animation->SetFrameTime(0,0.1f);
			animation->SetFrameTime(1,0.1f);
			{
				int i=0;
				// run
				for(int x=0;x<3;x++)
				{
					for(int y=0;y<4;y++)
					{
						animation->SetFrameData(i++,x*170,y*170,170,170,0,0,170,170);
					}
				}
				animation->SetSequenceData(0,0,11);
				// respawn
				for(int x=3;x<6;x++)
				{
					for(int y=0;y<4;y++)
					{
						animation->SetFrameData(i++,x*170,y*170,170,170,0,0,170,170);
					}
				}
				animation->SetSequenceData(1,12,23);
			}
			animation->SetSequence(0);
			if(i==0 && j==0) spriteObstacle = animation;
			if(i==0 && j==0) spriteA = animation;
			if(i==1 && j==1) spriteB = animation;
		}
	}
}
void AnimationBenchmarkTest::Update()
{
	ofxRENDERER->Update();
	if(spriteA->GetPosition().z < 1000.0f)
	{
		spriteA->MoveBy(0.0f,0.0f,15.0f);
	}
	else
	{
		spriteA->MoveBy(0.0f,0.0f,-2000.0f);
	}
	spriteB->MoveTo(ofxRENDERER->GetCamera()->getPosition());
}
void AnimationBenchmarkTest::Render()
{
	ofxRENDERER->Render();
	printf("update time = %u\nrender time = %u\n",
		ofxRENDERER->GetUpdateTimeMilisecond(),
		ofxRENDERER->GetRenderTimeMilisecond());
}
void Particle2DBenchmarkTest::Setup()
{
	ofxParticleEffect2D::LoadSharedParticleTexture();
	ofxParticleEffect2D::BuildForceTable();
	for(int i=0;i<40;i++)
	{
		ofxParticleEffect2D* sprite = new ofxParticleEffect2D();
		ofxEmitter2D* emitter = new ofxEmitter2D();
		emitter->position = ofVec2f(0,0);
		emitter->angle = 0;
		emitter->angle_var = TWO_PI;
		emitter->emission_rate = 5;
		emitter->emission_rate_var = 0;
		emitter->emission_time = 0.1f;
		emitter->emission_time_var = 0.0f;
		emitter->life = 3.0f;
		emitter->life_var = 0.0f;
		emitter->radial_accel = 1.0f;
		emitter->radial_accel_var = 1.0f;
		emitter->tangental_accel = -2.0f;
		emitter->tangental_accel_var = 1.0f;
		emitter->speed = 15.0f;
		emitter->speed_var = 20.0f;
		emitter->accel = 10.0f;
		emitter->accel_var = 15.0f;
		emitter->radius = 0.0f;
		emitter->radius_var = 20.0f;
		// TODO: color
		emitter->size = 15.0f;
		emitter->size_var = 35.0f;
		emitter->size_accel = 10.0f;
		emitter->opacity = 0.5f;
		emitter->opacity_var = 0.5f;
		emitter->opacity_accel = -0.2f;
		emitter->color_intensity = 0.0f;
		emitter->color_intensity_var = 1.0f;
		emitter->color = ofColor(0.0f,0.0f,0.0f);
		emitter->color_var = ofColor(1.0f,1.0f,1.0f);
		emitter->color_accel = ofColor(0.1f,0.1f,0.1f);
		sprite->AddEmitter(emitter);
		sprite->SetGrounded(true);
		spriteObstacle = sprite;
	}
	
}
void Particle2DBenchmarkTest::Update()
{
	ofxRENDERER->Update();
}
void Particle2DBenchmarkTest::Render()
{
	ofxRENDERER->Render();
}
void TextureBenchmarkTest::Setup()
{
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_attack_dual_weapon_crit.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_attack_flying_dagger.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_attack_horse_no_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_attack_horse_no_weapon_crit.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_attack_no_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_attack_one_hand_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_attack_one_hand_weapon_crit.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_attack_two_hand_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_attack_two_hand_weapon_crit.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_fall_dual_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_fall_horse_no_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_fall_no_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_fall_one_hand_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_fall_two_hand_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_hurt_dual_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_hurt_horse_no_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_hurt_no_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_hurt_one_hand_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_hurt_two_hand_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_idle_horse_no_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_idle_no_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_idle_one_hand_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_jump.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_magic_dual_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_magic_horse_no_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_magic_no_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_magic_one_hand_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_magic_two_hand_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_ready_dual_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_ready_no_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_ready_one_hand_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_ready_two_hand_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_run_dual_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_run_horse_no_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_run_no_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_run_one_hand_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_run_two_hand_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_walk_dual_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_walk_horse_no_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_walk_no_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_walk_one_hand_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_walk_two_hand_weapon.png"); sprite->GetTexture()->Lock();}
	{ofxSpriteQuad* sprite = new ofxSpriteQuad();sprite->SetTexture("data/tianwang_cloth/tianwang3_zen.png"); sprite->GetTexture()->Lock();}
	{
		ofxSpriteQuad* sprite = new ofxSpriteQuad();
		sprite->SetTexture("data/tianwang_cloth/tianwang3_attack_dual_weapon.png");
		spriteObstacle = sprite;
	}
}
void TextureBenchmarkTest::Update()
{
	ofxRENDERER->Update();
}
void TextureBenchmarkTest::Render()
{
	ofxRENDERER->Render();
}