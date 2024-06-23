﻿#pragma once
#include "glutil.h"
#include <QImage>

namespace render::gl {
class UTIL_EXPORT Texture : public OpenGLBase {
public:
    Texture(const QColor& color, int width, int height, QImage::Format format = QImage::Format_RGBA8888);
    Texture(QString imagePath);
    Texture(const QImage& image);
    Texture(QImage&& image);
    virtual ~Texture();
    void setMipmapEnable(bool enable) {m_mipmapEnable = enable;}
    void setWrapMode(GLint wrapMode) {m_wrapMode = wrapMode;}
    void setFilterMode(GLint filterMode) {m_filterMode = filterMode;}
    GLuint textureId() { return m_textureId;}

    void release()
    {
        if (m_textureId != 0) {
            m_gl->glDeleteTextures(1, &m_textureId);
            m_textureId = 0;
        }
    }

protected:
    void genTexture(const QImage& image);
    QImage convertQImageFormatToGL(const QImage& image, GLuint& glForamt, bool& needConvert);

private:
    GLuint m_textureId = 0;
    int m_width = 0;
    int m_height = 0;
    GLint m_wrapMode = GL_REPEAT;
    GLint m_filterMode = GL_LINEAR;
    bool m_mipmapEnable = false;
};
} // namespace render::gl
