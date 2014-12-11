#include "ofxBaseCommand.h"
ofxBaseCommand::ofxBaseCommand()
{
	m_VerticesSize = 0;
}
ofxBaseCommand::~ofxBaseCommand()
{
}
void ofxBaseCommand::Render()
{
}
unsigned int ofxBaseCommand::GetRenderedVertices()
{
	return m_VerticesSize;
}