#pragma once
#include "ofMain.h"
#include "ofxResource.h"
#include "IL/il.h"
#include "IL/ilu.h"

class ofxBitmapFont;

class ofxTexture
	: public ofxResource
{
	friend class ofxBitmapFont;
private:
	ILuint				m_ImageId;
	GLuint				m_TextureId;
	GLfloat				m_UnitWidth;
	GLfloat				m_UnitHeight;
	GLuint				m_Width;
	GLuint				m_Height;
	ILubyte				m_BytePerPixel; 
	bool				m_Locked;
	bool				m_Compressed;
public:
	ofxTexture();
	virtual ~ofxTexture();
	void				SetCompressed(bool value);
	bool				IsCompressed();
	virtual bool		Load(string texture_file);
	void				SubmitChanges();
	void				Bind(GLuint slot=0);
	void				Unbind(GLuint slot=0);
	GLuint				GetWidth();
	GLuint				GetHeight();
	GLfloat				GetUnitWidth();
	GLfloat				GetUnitHeight();
public:
	void				Lock();
	bool				IsLocked();
public:
	void				Allocate(unsigned int width, unsigned int height);
	ofColor				GetPixel(ofVec2f position);
	void				SetPixel(ofVec2f position, ofColor color);
	void				FlipX();
	void				FlipY();
	void				BlockTransfer(ofxTexture* source, ofRectangle source_rect, ofVec2f dest_pos, int alpha = 255);
	void				StretchTransfer(ofxTexture* source, ofRectangle source_rect, ofRectangle dest_rect, int alpha = 255);
	void				Fill(ofFloatColor color, ofRectangle dest_rect);
	void				Clear(ofRectangle dest_rect);
	void				Clear();
	void				DrawString(string text, ofxBitmapFont* font, ofVec2f dest_pos, unsigned char font_size = 0);
	void				DrawString(string text, ofxBitmapFont* font, ofRectangle dest_rect, unsigned char font_size = 0);
private:
	ILuint				GetDevilId();
};
typedef vector<ofxTexture*> ofxTextures;