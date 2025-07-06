#include "ResourceClasses/Shader.h"

#include <iostream>

Shader::Shader()
{
    ID = 0;
};

Shader& Shader::Use()
{
    glUseProgram(ID);
    return *this;
}

const char* Shader::getFragmentSource()
{
    return !m_fShaderFile.empty() ? m_fShaderFile.c_str() : nullptr;
}

const char* Shader::getVertexSource()
{
    return !m_vShaderFile.empty() ? m_vShaderFile.c_str() : nullptr;
}

const char* Shader::getGeometrySource()
{
    return !m_gShaderFile.empty() ? m_gShaderFile.c_str() : nullptr;
}

void Shader::setFragmentSource(const char* fragmentSourceFile)
{
    if (fragmentSourceFile != nullptr)
    {
        m_fShaderFile = std::string(fragmentSourceFile);
    }
}

void Shader::setVertexSource(const char* vertexSourceFile)
{
    if (vertexSourceFile != nullptr)
    {
        m_vShaderFile = std::string(vertexSourceFile);
    }
}

void Shader::setGeometrySource(const char* geometrySourceFile)
{
    if (geometrySourceFile != nullptr)
    {
        m_gShaderFile = std::string(geometrySourceFile);
        Logger::info(
            MESSAGE("Saved filename: " + m_gShaderFile)
        );
    }
}

void Shader::setSources(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
{
    setFragmentSource(fragmentSource);
    setGeometrySource(geometrySource);
    setVertexSource(vertexSource);
}

void Shader::Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
{
    // create all the shader ID's
    unsigned int sVertex, sFragment, gShader;
    // vertex Shader
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertexSource, NULL);
    glCompileShader(sVertex);
    checkCompileErrors(sVertex, "VERTEX");

    // fragment Shader
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragmentSource, NULL);
    glCompileShader(sFragment);
    checkCompileErrors(sFragment, "FRAGMENT");

    // if geometry shader source code is given, also compile geometry shader
    if (geometrySource != nullptr)
    {
        gShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gShader, 1, &geometrySource, NULL);
        glCompileShader(gShader);
        checkCompileErrors(gShader, "GEOMETRY");
    }
    
    // shader program (linking of the shaders)
    ID = glCreateProgram();
    glAttachShader(ID, sVertex);
    glAttachShader(ID, sFragment);
    if (geometrySource != nullptr)
    {
        glAttachShader(ID, gShader);
    }
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
    if (geometrySource != nullptr)
    {
        glDeleteShader(gShader);
    }
}

void Shader::SetFloat(const char* name, float value, bool useShader)
{
    if (useShader)
        Use();
    glUniform1f(glGetUniformLocation(ID, name), value);
}
void Shader::SetInteger(const char* name, int value, bool useShader)
{
    if (useShader)
        Use();
    glUniform1i(glGetUniformLocation(ID, name), value);
}
void Shader::SetVector2f(const char* name, float x, float y, bool useShader)
{
    if (useShader)
        Use();
    glUniform2f(glGetUniformLocation(ID, name), x, y);
}
void Shader::SetVector2f(const char* name, const glm::vec2& value, bool useShader)
{
    if (useShader)
        Use();
    glUniform2f(glGetUniformLocation(ID, name), value.x, value.y);
}
void Shader::SetVector3f(const char* name, float x, float y, float z, bool useShader)
{
    if (useShader)
        Use();
    glUniform3f(glGetUniformLocation(ID, name), x, y, z);
}
void Shader::SetVector3f(const char* name, const glm::vec3& value, bool useShader)
{
    if (useShader)
        Use();
    glUniform3f(glGetUniformLocation(ID, name), value.x, value.y, value.z);
}
void Shader::SetVector4f(const char* name, float x, float y, float z, float w, bool useShader)
{
    if (useShader)
        Use();
    glUniform4f(glGetUniformLocation(ID, name), x, y, z, w);
}
void Shader::SetVector4f(const char* name, const glm::vec4& value, bool useShader)
{
    if (useShader)
        Use();
    glUniform4f(glGetUniformLocation(ID, name), value.x, value.y, value.z, value.w);
}
void Shader::SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader)
{
    if (useShader)
        Use();
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, false, glm::value_ptr(matrix));
}


void Shader::checkCompileErrors(unsigned int object, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            Logger::error(
                MESSAGE("SHADER: Compile-time error: Type: " + type + "\n" + infoLog + "\n -- --------------------------------------------------- -- ")
            );
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            Logger::error(
                MESSAGE("SHADER: Link-time error: Type: " + type + "\n" + infoLog + "\n -- --------------------------------------------------- -- ")
            );
        }
    }
}


