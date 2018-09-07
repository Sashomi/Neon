#include <Neon/Renderer/SwapChain.hpp>
#include <Neon/Renderer/Window.hpp>
#include <Neon/Renderer/Renderer.hpp>

namespace Zx
{
	inline const std::shared_ptr<Device::Devices>& Device::GetDevice() const
	{
		return m_device;
	}

	inline void Device::SetSwapChain(const SwapChain& swapChain)
	{
		m_swapChain = std::make_shared<SwapChain>(swapChain);
	}

	inline void Device::SetWindow(const Window& window)
	{
		m_window = std::make_shared<Window>(window);
	}

	inline void Device::SetRenderer(const Renderer& renderer)
	{
		m_renderer = std::make_shared<Renderer>(renderer);
	}
}


