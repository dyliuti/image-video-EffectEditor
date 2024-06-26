﻿#pragma once
#include "glutil.h"

namespace render::gl {
class RENDER_EXPORT ShaderProgram : public OpenGLBase {
public:
    using OpenGLBase::OpenGLBase;
    ~ShaderProgram();
    bool createProgram(const char* vertex_shader, const char* fragment_shader);
    bool createProgram(QString vertexFile, QString fragmentFile);
    GLuint programId() const
    {
        return m_programId;
    }
    void useProgram()
    {
        m_gl->glUseProgram(m_programId);
    }

protected:
    GLuint loadShader(GLenum type, const char* source);
    bool checkShaderCompilation(const char* message, GLuint shader);
    bool checkProgramLink(const char* message, GLuint program);

private:
    GLuint m_programId = 0;
};
} // namespace render::gl
