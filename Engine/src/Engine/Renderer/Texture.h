#pragma once

#include "Engine/Asset/Asset.h"

typedef unsigned int GLenum;

namespace VoxelEngine
{
	class Texture : public Asset
	{
	public:
		Texture(uint32_t width, uint32_t height);
		Texture(const std::filesystem::path& filepath);
		~Texture();

		void LoadTexture();

		uint32_t GetWidth() const { return m_width; }
		uint32_t GetHeight() const { return m_height; }
		uint32_t GetRendererID() const { return m_rendererID; }
		const std::filesystem::path& GetFilepath() const { return m_filepath; }

		void SetData(void* data, uint32_t size);
		void Bind(uint32_t slot = 0) const;

		bool IsLoaded() const { return m_isLoaded; }
		bool operator==(const Texture& other) const { return m_rendererID == other.GetRendererID(); }

		static AssetType GetStaticType() { return AssetType::Texture; }

	private:
		uint32_t m_rendererID;
		GLenum m_internalFormat, m_dataFormat;

		uint32_t m_width{ 0 }, m_height{ 0 };
		std::filesystem::path m_filepath;
		bool m_isLoaded{ false };
	};
}
