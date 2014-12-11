#include "ofxTexture.h"
#include "ofxBitmapFont.h"
#include "IL/il.h"
#include "IL/ilu.h"

ofxTexture::ofxTexture()
	:ofxResource()
{
	glGenTextures(1, &m_TextureId);
	m_ImageId = ilGenImage();
	m_Width = 0;
	m_Height = 0;
	m_BytePerPixel = 0;
	m_Compressed = false;
	m_Locked = false;
}
ofxTexture::~ofxTexture()
{
	glDeleteTextures(1, &m_TextureId);
	ilDeleteImage(m_ImageId); 
}
void ofxTexture::SetCompressed(bool value)
{
	m_Compressed = value;
}
bool ofxTexture::IsCompressed()
{
	return m_Compressed;
}
bool ofxTexture::Load(string texture_file)
{
	ilBindImage(m_ImageId);
	ILboolean loaded = ilLoadImage(texture_file.c_str());
	if (loaded == IL_FALSE)
	{
		ILenum error = ilGetError();
		ofLogError() <<"DevIL failed to load image "<<texture_file.c_str()<<endl<<"error code "<<error;
		return false; 
	}
	m_Locked = false;
	SubmitChanges();
	return true;
}
void ofxTexture::SubmitChanges()
{
	if(m_Locked) return;
	ilBindImage(m_ImageId);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TextureId);
	{
		ILinfo info;
		iluGetImageInfo(&info);
		m_BytePerPixel = info.Bpp;
		m_Width = info.Width;
		m_Height = info.Height;
		m_UnitWidth = m_Width>0?1.0f/m_Width:0.0f;
		m_UnitHeight = m_Height>0?1.0f/m_Height:0.0f;
	}
	{
		GLenum format = m_BytePerPixel==3?GL_RGB:GL_RGBA;
		ILubyte* pixel_data = ilGetData();
		if(m_Compressed)
		{
			ILuint compressed_size;
			ILubyte* compressed_data;
			compressed_data = ilCompressDXT(pixel_data, m_Width, m_Height, 1, IL_DXT3, &compressed_size);
			glCompressedTexImage2DARB(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, 
				m_Width, m_Height, 0, compressed_size, compressed_data);
#ifdef _DEBUG
			ILuint uncompressed_size = m_Width*m_Height*m_BytePerPixel;
			float ratio = (float)compressed_size/uncompressed_size*100;
			ofLogNotice() <<"compressed texture, m_TextureId = "<<m_TextureId
				<<endl<<"before "<<uncompressed_size/1024.0<<" Kbytes, after "<<compressed_size/1024.0<<" Kbytes, ratio = "<<ratio<<"%";
#endif
			//delete[] compressed_data;
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, pixel_data);
		}
	}
	{
		GLint param = GL_CLAMP_TO_EDGE;
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,param);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,param);
	}
	{
		GLint param = GL_LINEAR;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
		if(param == GL_LINEAR_MIPMAP_LINEAR || GL_LINEAR_MIPMAP_NEAREST)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}
}
/*----------------------------------
ofxTexture::Lock()
------------------------------------
- store texture data at GPU
- save alot of RAM
- a texture locked can't be manipulated or used as material to manipulate other textures
- only used for render
----------------------------------*/
void ofxTexture::Lock()
{
	m_Locked = true;
	ilDeleteImage(m_ImageId);
}
bool ofxTexture::IsLocked()
{
	return m_Locked;
}
void ofxTexture::Bind(GLuint slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_TextureId);
}
void ofxTexture::Unbind(GLuint slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}
GLuint ofxTexture::GetWidth()
{
	return m_Width;
}
GLuint ofxTexture::GetHeight()
{
	return m_Height;
}
GLfloat ofxTexture::GetUnitWidth()
{
	return m_UnitWidth;
}
GLfloat ofxTexture::GetUnitHeight()
{
	return m_UnitHeight;
}
/* ----------------------------------
texture operation
---------------------------------- */
void ofxTexture::Allocate(unsigned int width, unsigned int height)
{
	ilBindImage(m_ImageId);
	ilTexImage(width, height, 0, 4, IL_RGBA , IL_UNSIGNED_BYTE, NULL); 
	ilClearColour(0.0f, 0.0f, 0.0f, 0.0f);
	ilClearImage();
	m_Width = width;
	m_Height = height;
	m_Locked = false;
}
ofColor ofxTexture::GetPixel(ofVec2f position)
{
	if(m_Locked) return ofColor(0.0f, 0.0f, 0.0f);
	ilBindImage(m_ImageId);
	ILubyte* data;
	ofColor color;
	data = new ILubyte[m_BytePerPixel];
	ilCopyPixels(position.x, position.y, 0, 1, 1, 1, m_BytePerPixel==3?IL_RGB:IL_RGBA, IL_UNSIGNED_BYTE, data);
	color.r = data[0];
	color.g = data[1];
	color.b = data[2];
	if(m_BytePerPixel == 3)
	{
		color.a = 255;
	}
	else
	{
		color.a = data[3];
	}
	delete[] data;
	return color;
}
void ofxTexture::SetPixel(ofVec2f position, ofColor color)
{
	if(m_Locked) return;
	ilBindImage(m_ImageId);
	ILubyte* data;
	data = new ILubyte[m_BytePerPixel];
	data[0] = color.r;
	data[1] = color.g;
	data[2] = color.b;
	if(m_BytePerPixel == 4)
	{
		data[3] = color.a;
	}
	ilSetPixels(position.x, position.y, 0, 1, 1, 1, m_BytePerPixel==3?IL_RGB:IL_RGBA, IL_UNSIGNED_BYTE, data);
	delete[] data;
}
void ofxTexture::FlipX()
{
	if(m_Locked) return;
	ilBindImage(m_ImageId);
	ILboolean success = iluRotate(PI);
	if(success != IL_TRUE)
	{
		ofLogNotice() <<"ofxTexture::FlipX() failed, image id = "<<m_ImageId;
	}
}
void ofxTexture::FlipY()
{
	if(m_Locked) return;
	ilBindImage(m_ImageId);
	ILboolean success = iluFlipImage();
	if(success != IL_TRUE)
	{
		ofLogNotice() <<"ofxTexture::FlipY() failed, image id = "<<m_ImageId;
	}
}
void ofxTexture::BlockTransfer(ofxTexture* source, ofRectangle source_rect, ofVec2f dest_pos, int alpha)
{
	if(m_Locked || source->IsLocked()) return;
	ilBindImage(m_ImageId);
	ilDisable(IL_BLIT_BLEND);
	ilBlit(source->GetDevilId(), dest_pos.x, dest_pos.y, 0, 
		source_rect.x, source_rect.y, 0, source_rect.width, source_rect.height, 1);
	ilEnable(IL_BLIT_BLEND);
}
void ofxTexture::StretchTransfer(ofxTexture* source, ofRectangle source_rect, ofRectangle dest_rect, int alpha)
{
	if(m_Locked || source->IsLocked()) return;
	ILuint dummy = ilGenImage();
	ilBindImage(dummy);
	ilCopyImage(source->GetDevilId());
	iluScale(dest_rect.width, dest_rect.height, 1);
	ilBindImage(m_ImageId);
	ilDisable(IL_BLIT_BLEND);
	ilBlit(dummy, dest_rect.x, dest_rect.y, 0, 
		source_rect.x, source_rect.y, 0, source_rect.width, source_rect.height, 1);
	ilEnable(IL_BLIT_BLEND);
	ilDeleteImage(dummy);
}
void ofxTexture::Fill(ofFloatColor color, ofRectangle dest_rect)
{
	if(m_Locked) return;
	ILuint dummy = ilGenImage();
	ilBindImage(dummy);
	ilTexImage(dest_rect.width, dest_rect.height, 0, 4, IL_RGBA , IL_UNSIGNED_BYTE, NULL); 
	ilClearColour(color.r, color.g, color.b, color.a);
	ilClearImage();
	ilBindImage(m_ImageId);
	ilDisable(IL_BLIT_BLEND);
	ilBlit(dummy, dest_rect.x, dest_rect.y, 0, 0, 0, 0, dest_rect.width, dest_rect.height, 1);
	ilEnable(IL_BLIT_BLEND);
	ilDeleteImage(dummy);
}
void ofxTexture::Clear(ofRectangle dest_rect)
{
	if(m_Locked) return;
	Fill(ofFloatColor(0.0f, 0.0f, 0.0f, 0.0f), dest_rect);
}
void ofxTexture::Clear()
{
	if(m_Locked) return;
	ilBindImage(m_ImageId);
	ilClearColour(0.0f, 0.0f, 0.0f, 0.0f);
	ilClearImage();
}
void ofxTexture::DrawString(string text, ofxBitmapFont* font, ofVec2f dest_pos, unsigned char font_size)
{
	if(m_Locked) return;
	float scale;
	if(font_size == 0)
	{
		scale = 1.0f;
	}
	else
	{
		scale = (float)font_size/font->GetFontSize();
	}
	ofVec2f cursor(dest_pos);
	ilDisable(IL_BLIT_BLEND);
	for (int i = 0; i < text.size(); i++)
	{
		ofVec2f draw_region = scale*font->GetRect(text[i]);
		ILuint character_image = font->GetImageId(text[i]);
		ILuint dummy = ilGenImage();
		ilBindImage(dummy);
		ilCopyImage(character_image);
		iluScale(draw_region.x, draw_region.y, 1);
		ilBindImage(m_ImageId);
		ilBlit(dummy, cursor.x, cursor.y, 0, 0, 0, 0, draw_region.x, draw_region.y, 1);
		ilDeleteImage(dummy);
		cursor.x += draw_region.x;
	}
	ilEnable(IL_BLIT_BLEND);
}
void ofxTexture::DrawString(string text, ofxBitmapFont* font, ofRectangle dest_rect, unsigned char font_size)
{
	if(m_Locked) return;
	// TODO: implement draw text with boundary
	float scale;
	if(font_size == 0)
	{
		scale = 1.0f;
	}
	else
	{
		scale = (float)font_size/font->GetFontSize();
	}
	ofVec2f cursor(dest_rect.x, dest_rect.y);
	ilDisable(IL_BLIT_BLEND);
	for (int i = 0; i < text.size(); i++)
	{
		ofVec2f draw_region = scale*font->GetRect(text[i]);
		ILuint character_image = font->GetImageId(text[i]);
		ILuint dummy = ilGenImage();
		ilBindImage(dummy);
		ilCopyImage(character_image);
		iluScale(draw_region.x, draw_region.y, 1);
		ilBindImage(m_ImageId);
		ilBlit(dummy, cursor.x, cursor.y, 0, 0, 0, 0, draw_region.x, draw_region.y, 1);
		ilDeleteImage(dummy);
		cursor.x += draw_region.x;
	}
	ilEnable(IL_BLIT_BLEND);
	
}
ILuint ofxTexture::GetDevilId()
{
	return m_ImageId;
}