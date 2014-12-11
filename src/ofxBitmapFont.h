#pragma once
#include "ofMain.h"
#include "ofxResource.h"
#include "ofxTexture.h"
#include "IL/il.h"
#include "IL/ilu.h"

class ofxBitmapFont
	: public ofxResource
{
private:
	map<char, ofVec2f>		m_CharacterMap;
	map<char, ILuint>		m_BitmapCache;
	unsigned char			m_FontSize;
public:
	ofxBitmapFont();
	virtual ~ofxBitmapFont();
	virtual bool			Load(string xml_file);
	unsigned char			GetFontSize();
public:
	bool					HaveCharacter(char c);
	ofVec2f					GetRect(char c);
	ofVec2f					GetTextDimension(string text, unsigned char font_size = 0);
	ILuint					GetImageId(char c);
};
typedef vector<ofxBitmapFont*> ofxBitmapFonts;