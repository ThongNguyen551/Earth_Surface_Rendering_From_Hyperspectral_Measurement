#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct ShaderProgramSource
{
	std::string vertexSource;
	std::string fragmentSource;
	std::string geometrySource;
};

class Shader
{
public:
	GLuint RendererID;
	std::string filepath;

	GLuint GetUniformLocation(const std::string& name);
	GLint CompileShader(const std::string& source, unsigned int type);
	GLint CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader);
	ShaderProgramSource ParseShader(const std::string& filepath);

public:
	Shader(const std::string fpath);
	~Shader();

	void Attach() const;
	void Detach() const;
	void SetUniformMatrix4fv(const std::string& name, GLuint n, bool to_norm, glm::mat4 proj);
	void SetUniform1f(const std::string& name, float s);
	void SetUniform3f(const std::string& name, glm::vec3 s);
};