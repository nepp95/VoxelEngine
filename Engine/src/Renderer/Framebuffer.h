#pragma once

namespace VoxelEngine
{
	enum class FramebufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,
		RED_INTEGER,

		// Depth/Stencil
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: TextureFormat(format)
		{}

		FramebufferTextureFormat TextureFormat{ FramebufferTextureFormat::None };
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments)
		{}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	struct FramebufferSpecification
	{
		FramebufferAttachmentSpecification Attachments;
		uint32_t Width{ 0 }, Height{ 0 };
		uint32_t Samples{ 1 };

		bool SwapchainTarget{ false };
	};

	class Framebuffer
	{
	public:
		Framebuffer(const FramebufferSpecification& specification);
		~Framebuffer();

		void Invalidate();

		void Bind();
		void Unbind();
		void Resize(uint32_t width, uint32_t height);

		void ClearAttachment(uint32_t attachmentIndex, int value);
		uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const;

		const FramebufferSpecification& GetSpecification() const { return m_specification; }

	private:
		uint32_t m_rendererID;

		FramebufferSpecification m_specification;
		std::vector<FramebufferTextureSpecification> m_colorAttachmentSpecifications;
		FramebufferTextureSpecification m_depthAttachmentSpecification{ FramebufferTextureFormat::None };

		std::vector<uint32_t> m_colorAttachments;
		uint32_t m_depthAttachment{ 0 };
	};
}
