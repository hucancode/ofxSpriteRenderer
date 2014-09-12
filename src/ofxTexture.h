#pragma once
#include "ofMain.h"
#include "ofxResource.h"
#include "FreeImage.h"

class ofxBitmapFont;

class ofxTexture
	: public ofxResource
{
	friend class ofxBitmapFont;
private:
	FIBITMAP*			m_ImageData;
	GLuint				m_TextureId;
	ofVec2f				m_Dimension;
public:
	ofxTexture();
	virtual ~ofxTexture();
	virtual bool		Load(string texture_file);
	void				SubmitChanges();
	void				Bind(GLuint slot=0);
	void				Unbind(GLuint slot=0);
	ofVec2f				GetDimension();
public:
	void				Allocate(unsigned int width, unsigned int height);
	ofColor				GetPixel(ofVec2f position);
	void				SetPixel(ofVec2f position, ofColor color);
	void				FlipX();
	void				FlipY();
	void				BlockTransfer(ofxTexture* source, ofRectangle source_rect, ofVec2f dest_pos, int alpha = 255);
	void				StretchTransfer(ofxTexture* source, ofRectangle source_rect, ofRectangle dest_rect, int alpha = 255);
	void				Fill(ofColor color, ofRectangle dest_rect);
	void				Clear(ofRectangle dest_rect);
	void				Clear();
	void				DrawString(string text, ofxBitmapFont* font, ofVec2f dest_pos, unsigned char font_size = 0);
	void				DrawString(string text, ofxBitmapFont* font, ofRectangle dest_rect, unsigned char font_size = 0);
private:
	FIBITMAP*			GetImageData();
};
typedef vector<ofxTexture*> ofxTextures;