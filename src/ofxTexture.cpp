#include "ofxTexture.h"
#include "ofxBitmapFont.h"
ofxTexture::ofxTexture()
	:ofxResource()
{
	glGenTextures(1, &m_TextureId);
}
ofxTexture::~ofxTexture()
{
	FreeImage_Unload(m_ImageData);
	glDeleteTextures(1, &m_TextureId);
	FreeImage_Unload(m_ImageData);
}
bool ofxTexture::Load(string texture_file)
{
	m_ImageData = FreeImage_Load(FIF_PNG, texture_file.c_str(), PNG_DEFAULT);
	FreeImage_FlipVertical(m_ImageData);
	SubmitChanges();
	return true;
}
void ofxTexture::SubmitChanges()
{
	unsigned int bpp = FreeImage_GetBPP(m_ImageData);
	unsigned int width = FreeImage_GetWidth(m_ImageData);
	unsigned int height = FreeImage_GetHeight(m_ImageData);
	BYTE* pixel_data = FreeImage_GetBits(m_ImageData);
	{
		GLenum format = bpp==24?GL_RGB:GL_RGBA;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_TextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixel_data);
		m_Dimension.x = width;
		m_Dimension.y = height;
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
ofVec2f ofxTexture::GetDimension()
{
	return m_Dimension; 
}
/* ----------------------------------
texture operation
---------------------------------- */
void ofxTexture::Allocate(unsigned int width, unsigned int height)
{
	m_ImageData = FreeImage_Allocate(width, height, 32);
	m_Dimension.x = width;
	m_Dimension.y = height;
}
ofColor ofxTexture::GetPixel(ofVec2f position)
{
	RGBQUAD fi_color;
	ofColor color;
	bool success = FreeImage_SetPixelColor(m_ImageData, position.x, position.y, &fi_color);
	color.r = fi_color.rgbRed;
	color.g = fi_color.rgbGreen;
	color.b = fi_color.rgbBlue;
	if(!success)
	{
		ofLog(OF_LOG_WARNING, "ofxTexture::GetPixel failed, position = (%d, %d)", position.x, position.y);
	}
	return color;
}
void ofxTexture::SetPixel(ofVec2f position, ofColor color)
{
	RGBQUAD fi_color;
	fi_color.rgbRed = color.r;
	fi_color.rgbGreen = color.g;
	fi_color.rgbBlue = color.b;
	fi_color.rgbReserved = 0;
	bool success = FreeImage_SetPixelColor(m_ImageData, position.x, position.y, &fi_color);
	if(!success)
	{
		ofLog(OF_LOG_WARNING, "ofxTexture::SetPixel failed, position = (%d, %d)", position.x, position.y);
	}
}
void ofxTexture::FlipX()
{
	FreeImage_FlipHorizontal(m_ImageData);
}
void ofxTexture::FlipY()
{
	FreeImage_FlipVertical(m_ImageData);
}
void ofxTexture::BlockTransfer(ofxTexture* source, ofRectangle source_rect, ofVec2f dest_pos, int alpha)
{
	FIBITMAP* image_piece = FreeImage_Copy(source->m_ImageData, source_rect.x, source_rect.y, 
		source_rect.x + source_rect.width, source_rect.y + source_rect.height);
	FreeImage_Paste(m_ImageData, image_piece, dest_pos.x, dest_pos.y, alpha);
	FreeImage_Unload(image_piece);
}
void ofxTexture::StretchTransfer(ofxTexture* source, ofRectangle source_rect, ofRectangle dest_rect, int alpha)
{
	FIBITMAP* image_piece = FreeImage_Copy(source->m_ImageData, source_rect.x, source_rect.y, 
		source_rect.x + source_rect.width, source_rect.y + source_rect.height);
	FIBITMAP* image_piece_rescale = FreeImage_Rescale(image_piece, dest_rect.width, dest_rect.height, FILTER_BILINEAR);
	FreeImage_Unload(image_piece);
	FreeImage_Paste(m_ImageData, image_piece_rescale, dest_rect.x, dest_rect.y, alpha);
	FreeImage_Unload(image_piece_rescale);
}
void ofxTexture::Fill(ofColor color, ofRectangle dest_rect)
{
	FIBITMAP* image_piece = FreeImage_Allocate(dest_rect.width, dest_rect.height, 
		FreeImage_GetBPP(m_ImageData), color.r, color.g, color.b);
	FreeImage_Paste(m_ImageData, image_piece, dest_rect.x, dest_rect.y, color.a);
	FreeImage_Unload(image_piece);
}
void ofxTexture::Clear(ofRectangle dest_rect)
{
	RGBQUAD fi_color;
	fi_color.rgbRed = 0;
	fi_color.rgbGreen = 0;
	fi_color.rgbBlue = 0;
	fi_color.rgbReserved = 0;
	FIBITMAP* dummy = FreeImage_AllocateEx(dest_rect.width, dest_rect.height, FreeImage_GetBPP(m_ImageData), &fi_color);
	FreeImage_Paste(m_ImageData, dummy, dest_rect.x, dest_rect.y, 255);
	FreeImage_Unload(dummy);
}
void ofxTexture::Clear()
{
	Clear(ofRectangle(0, 0, m_Dimension.x, m_Dimension.y));
}
void ofxTexture::DrawString(string text, ofxBitmapFont* font, ofVec2f dest_pos, unsigned char font_size)
{
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
	for (int i = 0; i < text.size(); i++)
	{
		ofVec4f draw_region = font->GetCharacterRect(text[i]);
		int width = (draw_region.z - draw_region.x)*scale;
		int height = (draw_region.w - draw_region.y)*scale;
		FIBITMAP* character_bitmap = font->GetCharacterBitmap(text[i]);
		FIBITMAP* character_bitmap_rescale = FreeImage_Rescale(character_bitmap, width, height, FILTER_BILINEAR);
		FreeImage_Paste(m_ImageData, character_bitmap_rescale, cursor.x, cursor.y, 255);
		FreeImage_Unload(character_bitmap_rescale);
		cursor.x += width;
	}
	FreeImage_Save(FIF_PNG, m_ImageData, "text.png");
}
void ofxTexture::DrawString(string text, ofxBitmapFont* font, ofRectangle dest_rect, unsigned char font_size)
{
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
	for (int i = 0; i < text.size(); i++)
	{
		ofVec4f draw_region = font->GetCharacterRect(text[i]);
		int width = (draw_region.z - draw_region.x)*scale;
		int height = (draw_region.w - draw_region.y)*scale;
		FIBITMAP* character_bitmap = font->GetCharacterBitmap(text[i]);
		FIBITMAP* character_bitmap_rescale = FreeImage_Rescale(character_bitmap, width, height, FILTER_BILINEAR);
		FreeImage_Paste(m_ImageData, character_bitmap_rescale, cursor.x, cursor.y, 255);
		FreeImage_Unload(character_bitmap_rescale);
		cursor.x += width;
	}
	
}
FIBITMAP* ofxTexture::GetImageData()
{
	return m_ImageData;
}