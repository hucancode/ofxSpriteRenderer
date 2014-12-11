#include "ofxShaderProgram.h"
#include "ofxShaderCache.h"
#include "ofxSpriteRenderer.h"

ofxShaderProgram::ofxShaderProgram()
{
	m_ProgramId = glCreateProgram();
}
ofxShaderProgram::~ofxShaderProgram()
{
	glDetachShader(m_ProgramId, m_VertexShader->GetShaderId());
	glDetachShader(m_ProgramId, m_FragmentShader->GetShaderId());
	glDeleteProgram(m_ProgramId);
}
bool ofxShaderProgram::Load(string path)
{
	return Load(path+".vert", path+".frag");
}
bool ofxShaderProgram::Load(string vs_path, string fs_path)
{
	m_VertexShader = ofxSHADERCACHE->GetResource(vs_path);
	m_FragmentShader = ofxSHADERCACHE->GetResource(fs_path);
	// link program
	glAttachShader(m_ProgramId, m_VertexShader->GetShaderId());
	glAttachShader(m_ProgramId, m_FragmentShader->GetShaderId());
	glLinkProgram(m_ProgramId);
	GLint link_status;
	glGetProgramiv(m_ProgramId, GL_LINK_STATUS, &link_status);
	if(!link_status)
	{
#ifdef _DEBUG
		GLint len = 0;
		glGetProgramiv(m_ProgramId, GL_INFO_LOG_LENGTH, &len);
		if(len > 1)
		{
			char* info = new char[len];
			glGetProgramInfoLog(m_ProgramId, len, NULL, info);
			printf("Error linking program[%s,%s]:\n%s\n", vs_path.c_str(), fs_path.c_str(), info);
			delete[] info;
		}
#endif
		glDeleteProgram(m_ProgramId);
		return false;
	}
	BuildReferenceMap();
	return true;
}
void ofxShaderProgram::Bind()
{
	glUseProgram(m_ProgramId);
	BindReferenceMap();
}
void ofxShaderProgram::BuildReferenceMap()
{
	m_AttributeMap["a_position"]			= glGetAttribLocation(m_ProgramId, "a_position");
	m_AttributeMap["a_uv"]					= glGetAttribLocation(m_ProgramId, "a_uv");
	m_AttributeMap["a_color"]				= glGetAttribLocation(m_ProgramId, "a_color");
	m_AttributeMap["a_color_intensity"]		= glGetAttribLocation(m_ProgramId, "a_color_intensity");
	m_AttributeMap["a_opacity"]				= glGetAttribLocation(m_ProgramId, "a_opacity");
	m_UniformMap["u_texture"]				= glGetUniformLocation(m_ProgramId, "u_texture");
	m_UniformMap["u_modelview_matrix"]		= glGetUniformLocation(m_ProgramId, "u_modelview_matrix");
	m_UniformMap["u_projection_matrix"]		= glGetUniformLocation(m_ProgramId, "u_projection_matrix");
	m_UniformMap["u_transform_matrix"]		= glGetUniformLocation(m_ProgramId, "u_transform_matrix");
	m_UniformMap["u_cam_inverse_matrix"]	= glGetUniformLocation(m_ProgramId, "u_cam_inverse_matrix");
}
void ofxShaderProgram::BindReferenceMap()
{
	int stride = sizeof(ofxVertex);
	glEnableVertexAttribArray	(m_AttributeMap["a_position"]);
	glVertexAttribPointer		(m_AttributeMap["a_position"],			3, GL_FLOAT, GL_FALSE, 
		stride, (GLvoid*) offsetof(ofxVertex, x));
	glEnableVertexAttribArray	(m_AttributeMap["a_uv"]);
	glVertexAttribPointer		(m_AttributeMap["a_uv"],				2, GL_FLOAT, GL_FALSE, 
		stride, (GLvoid*) offsetof(ofxVertex, u));
	glEnableVertexAttribArray	(m_AttributeMap["a_color"]);
	glVertexAttribPointer		(m_AttributeMap["a_color"],				3, GL_FLOAT, GL_FALSE, 
		stride, (GLvoid*) offsetof(ofxVertex, r));
	glEnableVertexAttribArray	(m_AttributeMap["a_color_intensity"]);
	glVertexAttribPointer		(m_AttributeMap["a_color_intensity"],	1, GL_FLOAT, GL_FALSE, 
		stride, (GLvoid*) offsetof(ofxVertex, color_intensity));
	glEnableVertexAttribArray	(m_AttributeMap["a_opacity"]);
	glVertexAttribPointer		(m_AttributeMap["a_opacity"],			1, GL_FLOAT, GL_FALSE, 
		stride, (GLvoid*) offsetof(ofxVertex, opacity));
	// --------------
	glUniform1i					(m_UniformMap["u_texture"],				0);
	glUniformMatrix4fv			(m_UniformMap["u_modelview_matrix"],	1, GL_FALSE, 
		ofxRENDERER->GetModelViewMatrix().getPtr());
	glUniformMatrix4fv			(m_UniformMap["u_projection_matrix"],	1, GL_FALSE, 
		ofxRENDERER->GetProjectionMatrix().getPtr());
	glUniformMatrix4fv			(m_UniformMap["u_transform_matrix"],	1, GL_FALSE, 
		ofxRENDERER->GetTransformation().getPtr());
	glUniformMatrix4fv			(m_UniformMap["u_cam_inverse_matrix"],	1, GL_FALSE, 
		ofxRENDERER->GetInverseModelViewMatrix().getPtr());
}
void ofxShaderProgram::Unbind()
{
	glUseProgram(0);
}
void ofxShaderProgram::IncreaseReference()
{
	ofxResource::IncreaseReference();
	m_VertexShader->IncreaseReference();
	m_FragmentShader->IncreaseReference();
}
void ofxShaderProgram::DecreaseReference()
{
	ofxResource::DecreaseReference();
	m_VertexShader->DecreaseReference();
	m_FragmentShader->DecreaseReference();
}