namespace Zx
{
	inline void SetVkInstance(const VkInstance& instance)
	{
		m_instance = instance;
		m_surfacePlatform->SetVkInstance(instance);
	}

	inline const Window::VkSurfaceKHR& GetSurface() const
	{
		return m_surfacePlatform->GetSurface();
	}

	inline const Window::HWND& GetHandle() const
	{
		return m_surfacePlatform->GetHandle();
	}

	inline const Window::HINSTANCE& GetInstance() const
	{
		return m_surfacePlatform->GetInstance();
	}
}