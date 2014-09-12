#pragma once

#include "ofMain.h"
#include "ofxBitmapFont.h"

class ofxBitmapFontCache
{
public :
	static ofxBitmapFontCache*	s_Instance;
	static ofxBitmapFontCache*	GetInstance();
	static void					DestroyInstance();
private:
	map<string, ofxBitmapFont*>	m_ResourceMap;
	ofxBitmapFont*				m_DefaultResource;
public:
	ofxBitmapFontCache();
	~ofxBitmapFontCache();
	ofxBitmapFont*				GetResource(string resource_file);
	void						Clean();
};
#define ofxBITMAPFONTCACHE ofxBitmapFontCache::GetInstance()