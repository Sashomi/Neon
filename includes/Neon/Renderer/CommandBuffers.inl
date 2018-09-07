namespace Zx
{
	inline const VkCommandPool& CommandBuffers::GetCommandPool() const
	{
		return m_commandPool;
	}

	inline std::vector<RenderingResourcesData>& CommandBuffers::GetRenderingResources()
	{
		return m_renderingResources;
	}
}