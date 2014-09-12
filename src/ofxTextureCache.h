#pragma once

#include "ofMain.h"
#include "ofxTexture.h"

class ofxTextureCache
{
public :
	static ofxTextureCache*		s_Instance;
	static ofxTextureCache*		GetInstance();
	static void					DestroyInstance();
private:
	map<string, ofxTexture*>	m_ResourceMap;
	ofxTexture*					m_DefaultResource;
public:
	ofxTextureCache();
	~ofxTextureCache();
	ofxTexture*					GetResource(string resource_file);
	bool						PushResource(ofxTexture* resource, string resource_name);
	void						Clean();
};
#define ofxTEXTURECACHE ofxTextureCache::GetInstance()