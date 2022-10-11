#include "pch.h"
#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace VoxelEngine {
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment")
			return GL_FRAGMENT_SHADER;

		CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	Shader::Shader(const std::string& filepath)
	{
		// Load shader file
		std::string source = ReadFile(filepath);

		// Pre process shader file
		auto shaderSources = PreProcess(source);

		// Compile shader
		Compile(shaderSources);

		// Extract name from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_name = filepath.substr(lastSlash, count);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_rendererID);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_rendererID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::SetInt(const std::string& name, int value) const
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform1i(location, value);
	}

	void Shader::SetIntArray(const std::string& name, int* values, uint32_t count) const
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void Shader::SetFloat(const std::string& name, float value) const
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform1f(location, value);
	}

	void Shader::SetFloat2(const std::string& name, const glm::vec2& value) const
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void Shader::SetFloat3(const std::string& name, const glm::vec3& value) const
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void Shader::SetFloat4(const std::string& name, const glm::vec4& value) const
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void Shader::SetMat4(const std::string& name, const glm::mat4& value) const
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	std::string Shader::ReadFile(const std::string& filepath)
	{
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		std::string result;

		if (in)
		{
			// Check size of the file and resize string accordingly
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());

			// Put file contents into string
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());

			// Close stream
			in.close();
		} else
		{
			CORE_ERROR("Could not open shader file: %", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> Shader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		// Find type token
		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		// Repeat finding it until last occurrence
		while (pos != std::string::npos) {
			// Make sure there is actual source after the type token
			size_t eol = source.find_first_of("\r\n", pos);
			CORE_ASSERT(eol != std::string::npos, "Syntax error");

			// Extract shader type
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type (%) specified!", type);

			// If there is no other type token, take the string till eof. Otherwise take it till the next type token.
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	void Shader::Compile(std::unordered_map<GLenum, std::string> shaderSources)
	{
		GLuint program = glCreateProgram();
		CORE_ASSERT(shaderSources.size() <= 2, "We only support 2 shaders for now!");
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDIndex{ 0 };

		for (auto& kv : shaderSources) {
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			// Set the vertex shader source
			const GLchar* sourceCstr = source.c_str();
			glShaderSource(shader, 1, &sourceCstr, 0);

			// Compile vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

			if (isCompiled == GL_FALSE) {
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// maxLength includes NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// Shader is not needed anymore
				glDeleteShader(shader);

				CORE_ERROR(infoLog.data());
				CORE_ASSERT(false, "Vertex shader compilation failure!");

				break;
			}

			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		// Create program
		m_rendererID = program;

		// Link our program
		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);

		if (!isLinked) {
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// maxLength includes NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// Program is not needed anymore
			glDeleteProgram(program);

			// We don't want to leak shaders
			for (auto id : glShaderIDs)
				glDeleteShader(id);

			CORE_ERROR(infoLog.data());
			CORE_ASSERT(false, "Shader link failure!");

			return;
		}

		// Detach shaders because the program is now compiled and linked
		for (auto id : glShaderIDs)
			glDetachShader(program, id);
	}
}
