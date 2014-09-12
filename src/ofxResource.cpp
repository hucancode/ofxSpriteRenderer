#include "ofxResource.h"
ofxResource::ofxResource()
{
	m_ReferenceCount = 0;
}
ofxResource::~ofxResource()
{
}
bool ofxResource::Load(string resource_file)
{
	return false;
}
void ofxResource::IncreaseReference()
{
	m_ReferenceCount++;
}
void ofxResource::DecreaseReference()
{
	m_ReferenceCount--;
}
bool ofxResource::IsUnused()
{
	return m_ReferenceCount == 0;
}