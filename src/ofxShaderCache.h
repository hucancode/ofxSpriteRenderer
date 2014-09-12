#pragma once
#include "ofMain.h"
#include "ofxShader.h"

/*

note: this class is used internally in ofxSpriteRenderer, it's not really useful for end users

*/

class ofxShaderCache
{
public :
	static ofxShaderCache*		s_Instance;
	static ofxShaderCache*		GetInstance();
	static void					DestroyInstance();
private:
	map<string, ofxShader*>		m_ResourceMap;
public:
	ofxShaderCache();
	~ofxShaderCache();
	ofxShader*					GetResource(string resource_file);
	void						Clean();
};
#define ofxSHADERCACHE ofxShaderCache::GetInstance()