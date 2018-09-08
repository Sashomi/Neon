namespace Zx
{
	inline const VkCommandPool& CommandBuffers::GetCommandPool() const
	{
		return m_commandPool;
	}

	inline const std::vector<RenderingResourcesData>& CommandBuffers::GetRenderingResources() const
	{
		return m_renderingResources;
	}
}