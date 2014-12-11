#pragma once
#include "ofMain.h"
/*
note: 
if you gonna render something complicated, and sprite isn't enough good to do it, then:
- create a class inherrit from both ofxBaseCommand, and ofxBaseSprite
- set it's position, by modifiying m_Position member. so renderer can sort it among of sprite list.
- set it's dimension, by modifying m_Dimension member, so renderer can discard it when it out of screen.
- do magic in Render() function.
- use your custom sprite like normal sprite.
- remember, this is not recommended for mass renderring. 1 custom sprite, cost 1 draw call.
*/
class ofxBaseCommand
{
protected:
	unsigned int m_VerticesSize;
public:
	ofxBaseCommand();
	virtual ~ofxBaseCommand();
	virtual void Render();
	unsigned int GetRenderedVertices();
};
typedef vector<ofxBaseCommand*> ofxBaseCommands;