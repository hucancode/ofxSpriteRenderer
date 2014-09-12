#pragma once
#include "ofMain.h"
#include "ofxResource.h"
#include "ofxTexture.h"

class ofxBitmapFont
	: public ofxResource
{
private:
	map<char, ofVec4f>		m_CharacterMap;
	map<char, FIBITMAP*>	m_BitmapCache;
	unsigned char			m_FontSize;
public:
	ofxBitmapFont();
	virtual ~ofxBitmapFont();
	virtual bool			Load(string xml_file);
	unsigned char			GetFontSize();
public:
	ofVec4f					GetCharacterRect(char character);
	ofVec2f					GetTextDimension(string text, unsigned char font_size = 0);
	FIBITMAP*				GetCharacterBitmap(char character);
};
typedef vector<ofxBitmapFont*> ofxBitmapFonts;