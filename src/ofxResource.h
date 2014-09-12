#pragma once
#include "ofMain.h"
class ofxResource
{
protected:
	unsigned int		m_ReferenceCount;
public:
	ofxResource();
	virtual ~ofxResource();
	virtual bool		Load(string resource_file);
	virtual void		IncreaseReference();
	virtual void		DecreaseReference();
	virtual bool		IsUnused();
};