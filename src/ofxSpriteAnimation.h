#pragma once
#include "ofxSpriteQuad.h"
class ofxSpriteAnimation
	:public ofxSpriteQuad
{
	friend ofxSpriteCommand;
private:
	unsigned short			m_FrameCount;
	unsigned short**		m_TextureRectTable;
	unsigned short**		m_SpriteRectTable;
	unsigned short			m_SequenceCount;
	unsigned short*			m_SequenceBegin;
	unsigned short*			m_SequenceEnd;
	float*					m_SequenceTime;
	float*					m_FrameTime;
	float					m_AnimationTime;

	unsigned int			m_FrameIndex;
	unsigned int			m_SequenceIndex;
	bool					m_Paused;
public:
	ofxSpriteAnimation();
	~ofxSpriteAnimation();
	void					SetSequenceTime(unsigned short sequence_index, float time);
	void					SetFrameTime(unsigned short sequence_index, float time);

	void					SetFrame(unsigned short index);
	void					SetFrameCount(unsigned short count);
	void					SetFrameData(unsigned short index, 
								unsigned short texture_rect_x, unsigned short texture_rect_y, 
								unsigned short texture_rect_w, unsigned short texture_rect_h,
								unsigned short sprite_rect_x, unsigned short sprite_rect_y, 
								unsigned short sprite_rect_w, unsigned short sprite_rect_h);

	void					SetSequence(unsigned short index);
	void					SetSequenceCount(unsigned short count);
	void					SetSequenceData(unsigned short index, unsigned short begin, unsigned short end);

	virtual void			Update(const float delta_time);
	void					PauseResume();
	bool					IsPaused();
};