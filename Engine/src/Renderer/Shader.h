#pragma once

#include <glm/glm.hpp>

typedef unsigned int GLenum;

namespace VoxelEngine {

	class Shader
	{
	public:
		Shader(const std::string& filepath);
		~Shader();

		void Bind() const;
		void Unbind() const;

		const std::string& GetName() const { return m_name; }

		void SetInt(const std::string& name, int value) const;
		void SetIntArray(const std::string& name, int* values, uint32_t count) const;
		void SetFloat(const std::string& name, float value) const;
		void SetFloat2(const std::string& name, const glm::vec2& value) const;
		void SetFloat3(const std::string& name, const glm::vec3& value) const;
		void SetFloat4(const std::string& name, const glm::vec4& value) const;
		void SetMat4(const std::string& name, const glm::mat4& value) const;

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(std::unordered_map<GLenum, std::string> shaderSources);

	private:
		uint32_t m_rendererID;
		std::string m_name;
	};
}
