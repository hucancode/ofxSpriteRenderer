#include "ofxShaderCache.h"

ofxShaderCache* ofxShaderCache::s_Instance = 0;
ofxShaderCache* ofxShaderCache::GetInstance()
{
	return s_Instance;
}
void ofxShaderCache::DestroyInstance()
{
	if(!s_Instance) return;
	delete s_Instance;
	s_Instance = 0;
}
ofxShaderCache::ofxShaderCache()
{
	if(s_Instance) return;
	s_Instance = this;
}
ofxShaderCache::~ofxShaderCache()
{
	map<string, ofxShader*>::iterator it = m_ResourceMap.begin();
	for(;it != m_ResourceMap.end();)
	{
		ofxShader* material = (*it).second;
		delete material;
	}
}
ofxShader* ofxShaderCache::GetResource(string resource_file)
{
	if(m_ResourceMap[resource_file] == 0)
	{
		ofxShader* resource = new ofxShader;
		resource->Load(resource_file);
		m_ResourceMap[resource_file] = resource;
	}
	return m_ResourceMap[resource_file];
}
void ofxShaderCache::Clean()
{
	map<string, ofxShader*>::iterator it = m_ResourceMap.begin();
	while(it != m_ResourceMap.end())
	{
		ofxShader* material = (*it).second;
		if(material->IsUnused())
		{
			delete material;
			it = m_ResourceMap.erase(it);
		}
		else
		{
			it++;
		}
	}
}