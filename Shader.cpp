#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "render.h"
#include "Shader.h"

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1, GEOMETRY = 2
    };

    std::string line;
    std::stringstream ss[3];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                //set mode to vertex
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                //set mode to fragment
                type = ShaderType::FRAGMENT;
            else if (line.find("geometry") != std::string::npos)
                //set mode to geometry
                type = ShaderType::GEOMETRY;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }

    }

    return { ss[0].str(),ss[1].str(),ss[2].str()};
}

GLint Shader::CompileShader(const std::string& source, unsigned int type)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)malloc(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}
GLint Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    unsigned int gs;
    if (geometryShader != "")
    {
        gs = CompileShader(geometryShader, GL_GEOMETRY_SHADER);
    }

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    if (geometryShader != "")
    {
        glAttachShader(program, gs);
    }
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    if (geometryShader != "")
    {
        glDeleteShader(gs);
    }

    return program;
}


Shader::Shader(const std::string fpath) : filepath(fpath), RendererID(0)
{
    //Shaders from a single file
    ShaderProgramSource source = ParseShader(filepath);
    std::cout << "vertex shader" << std::endl;
    std::cout << source.vertexSource << std::endl;
    std::cout << "fragment shader" << std::endl;
    std::cout << source.fragmentSource << std::endl;
    std::cout << "geometry shader" << std::endl;
    std::cout << source.geometrySource << std::endl;

    RendererID = CreateShader(source.vertexSource, source.fragmentSource, source.geometrySource);
}

Shader::~Shader()
{
    glDeleteProgram(RendererID);
}

void Shader::Attach() const
{
    glUseProgram(RendererID);
}

void Shader::Detach() const
{
    glUseProgram(0);
}

GLuint Shader::GetUniformLocation(const std::string& name)
{
    GLint loc = glGetUniformLocation(RendererID, name.c_str());
    if (loc == -1)
        std::cout << "The uniform " << name.c_str() << "is not declared in the shader." << std::endl;
    return loc;
}

void Shader::SetUniform1f(const std::string& name, float s)
{
    glUniform1f(GetUniformLocation(name), s);
}
void Shader::SetUniform3f(const std::string& name, glm::vec3 s)
{
    glUniform3f(GetUniformLocation(name), s[0], s[1], s[2]);
}

void Shader::SetUniformMatrix4fv(const std::string& name, GLuint n, bool to_norm, glm::mat4 matx)
{
    glUniformMatrix4fv(GetUniformLocation(name), n, to_norm, glm::value_ptr(matx));
}
