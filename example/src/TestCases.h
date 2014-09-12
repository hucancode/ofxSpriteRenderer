#pragma once
#include "ofxSpriteRenderer.h"
#include "ofxSpriteQuad.h"
#include "ofxSpriteAnimation.h"
class Test
{
public:
	virtual void Setup();
	virtual void Update();
	virtual void Render();
};
//-----------------------------------------------------------
// POSIBILITY TEST
// engine runs under finest environment, and it must works. 
// no need to work well, as long as it works, it's acceptable.
//-----------------------------------------------------------
// NOTE: try to draw 1 sprite, use any possible configuration
class SpriteTest: public Test
{
private:

public:
	virtual void Setup();
	virtual void Update();
	virtual void Render();
};
// NOTE: try to draw 1600 sprite, use any possible configuration
class RendererTest: public Test
{
private:

public:
	virtual void Setup();
	virtual void Update();
	virtual void Render();
};
// NOTE: try to draw 1600 sprite. move 2 sprite up & down
class SortingTest: public Test
{
private:
	ofxSpriteQuad* spriteA;
	ofxSpriteQuad* spriteB;
public:
	virtual void Setup();
	virtual void Update();
	virtual void Render();
};
// NOTE: try to draw 1000 sprite. random 5 material
class TextureTest: public Test
{
public:
	virtual void Setup();
	virtual void Update();
	virtual void Render();
};
// NOTE: try to draw animation.
class AnimationTest: public Test
{
public:
	virtual void Setup();
	virtual void Update();
	virtual void Render();
};
class TextSpriteTest: public Test
{
public:
	virtual void Setup();
	virtual void Update();
	virtual void Render();
};
//-----------------------------------------------------------
// STRESS TEST
// engine runs under strictly environemt, and it must works well
//-----------------------------------------------------------
class SpriteBenchmarkTest: public Test
{
public:
	virtual void Setup();
	virtual void Update();
	virtual void Render();
};
class SortBenchmarkTest: public Test
{
private:
	ofxSpriteQuad* spriteA;
	ofxSpriteQuad* spriteB;
public:
	virtual void Setup();
	virtual void Update();
	virtual void Render();
};
class AnimationBenchmarkTest: public Test
{
private:
	ofxSpriteQuad* spriteA;
	ofxSpriteQuad* spriteB;
public:
	virtual void Setup();
	virtual void Update();
	virtual void Render();
};
