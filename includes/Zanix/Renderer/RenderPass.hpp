#ifndef RENDERPASS_HPP
#define RENDERPASS_HPP

namespace Zx
{
	class RenderPass
	{
	public:
		static void CreateRenderPass();
		static void CreateFramebuffer();

	private:
		struct RenderPasss;

		static std::shared_ptr<RenderPasss> s_renderPass;

		struct RenderPasss
		{
			inline RenderPasss() : renderPass(VK_NULL_HANDLE), framebuffer()
			{}

			VkRenderPass renderPass;
			std::vector<VkFramebuffer> framebuffer;
		};
	private:
	};
}

#endif //RENDERPASS_HPP