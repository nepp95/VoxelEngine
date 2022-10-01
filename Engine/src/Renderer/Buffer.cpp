#include "pch.h"
#include "Buffer.h"

#include <glad/glad.h>

namespace VoxelEngine
{
	BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements)
		: m_elements(elements)
	{
		CalculateOffsetsAndStride();
	}

	void BufferLayout::CalculateOffsetsAndStride()
	{
		size_t offset{ 0 };
		m_stride = 0;

		for (auto& element : m_elements)
		{
			element.Offset = offset;
			offset += element.Size;
			m_stride += element.Size;
		}
	}

	VertexBuffer::VertexBuffer(uint32_t size)
	{
		glCreateBuffers(1, &m_rendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	VertexBuffer::VertexBuffer(float* vertices, uint32_t size)
	{
		glCreateBuffers(1, &m_rendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	VertexBuffer::VertexBuffer(void* data, uint32_t size)
	{
		glCreateBuffers(1, &m_rendererID);
		glNamedBufferData(m_rendererID, size, data, GL_STATIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_rendererID);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	IndexBuffer::IndexBuffer(uint32_t count)
		: m_count(count)
	{
		glCreateBuffers(1, &m_rendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW);
	}

	IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count)
		: m_count(count)
	{
		glCreateBuffers(1, &m_rendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	IndexBuffer::IndexBuffer(void* data, uint32_t size)
		: m_count(size / sizeof(uint32_t))
	{
		glCreateBuffers(1, &m_rendererID);
		glNamedBufferData(m_rendererID, size, data, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_rendererID);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
	}

	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void IndexBuffer::SetData(const void* data, uint32_t size, uint32_t offset /*= 0*/)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
	}
}