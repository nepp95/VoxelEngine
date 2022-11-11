#pragma once

#include "Engine/Renderer/Buffer.h"

namespace VoxelEngine
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void Bind() const;
		void Unbind() const;

		void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer);
		void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer);

		const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const { return m_vertexBuffers; }
		const Ref<IndexBuffer>& GetIndexBuffer() const { return m_indexBuffer; }

	private:
		uint32_t m_rendererID;
		uint32_t m_vertexBufferIndex{ 0 };
		std::vector<Ref<VertexBuffer>> m_vertexBuffers;
		Ref<IndexBuffer> m_indexBuffer;
	};
}
