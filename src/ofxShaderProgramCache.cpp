#include "ofxShaderProgramCache.h"

ofxShaderProgramCache* ofxShaderProgramCache::s_Instance = 0;
ofxShaderProgramCache* ofxShaderProgramCache::GetInstance()
{
	return s_Instance;
}
void ofxShaderProgramCache::DestroyInstance()
{
	if(!s_Instance) return;
	delete s_Instance;
	s_Instance = 0;
}
ofxShaderProgramCache::ofxShaderProgramCache()
{
	if(s_Instance) return;
	s_Instance = this;
	m_DefaultResource = new ofxShaderProgram();
	m_DefaultResource->Load("default_shader");
}
ofxShaderProgramCache::~ofxShaderProgramCache()
{
	map<string, ofxShaderProgram*>::iterator it = m_ResourceMap.begin();
	for(;it != m_ResourceMap.end();)
	{
		ofxShaderProgram* material = (*it).second;
		delete material;
	}
	delete m_DefaultResource;
}
ofxShaderProgram* ofxShaderProgramCache::GetResource(string resource_file)
{
	if(m_ResourceMap[resource_file] == 0)
	{
		ofxShaderProgram* resource = new ofxShaderProgram();
		resource->Load(resource_file);
		m_ResourceMap[resource_file] = resource;
	}
	if(m_ResourceMap[resource_file] == 0)
	{
		return m_DefaultResource;
	}
	return m_ResourceMap[resource_file];
}
void ofxShaderProgramCache::Clean()
{
	map<string, ofxShaderProgram*>::iterator it = m_ResourceMap.begin();
	while(it != m_ResourceMap.end())
	{
		ofxShaderProgram* material = (*it).second;
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