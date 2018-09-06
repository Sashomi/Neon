namespace Zx
{
	inline bool SwapChain::IsRenderAvailable() const
	{
		return m_isRenderAvailable;
	}

	inline const std::shared_ptr<SwapChain::SwapChains>& SwapChain::GetSwapChain() const
	{
		return m_swapChain;
	}

	inline void SwapChain::SetDevice(const Device& device)
	{
		m_device = std::make_shared<Device>(device);
	}

	inline void SwapChain::SetWindow(const Window& window)
	{
		m_window = std::make_shared<Window>(window);
	}
}