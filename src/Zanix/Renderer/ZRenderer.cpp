#include <Zanix/Core/ZString.hpp>
#include <Zanix/Core/ZPlugin.hpp>
#include <Zanix/Renderer/ZRenderer.hpp>

namespace Zx
{
	/*
	@brief : Load a renderer library
	@param : The name of the renderer library to load
	*/
	void ZRenderer::LoadRenderer(const ZString& libraryName)
	{
		s_instance = ZPlugin<ZRenderer>::LoadPlugin(libraryName);
	}

	/*
	@brief : Unload a renderer library
	*/
	void ZRenderer::UnloadRenderer()
	{
		ZPlugin<ZRenderer>::UnloadPlugin();
	}

	/*
	@brief : Return the instance of this ZRenderer
	@note : Require the renderer library is load
	*/
	ZRenderer& ZRenderer::GetRenderer()
	{
		ZAssert(s_instance, "No renderer instance load");
		return (*s_instance);
	}

	ZRenderer* ZRenderer::s_instance = nullptr;
}