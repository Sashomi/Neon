#pragma once

#ifndef ZRENDERER_HPP
#define ZRENDERER_HPP

#include <memory>

#include <Zanix/ZUtils.hpp>

namespace Zx
{
	class ZString;
	template <typename T>
	class ZPlugin;

	class ZANIX_EXPORT ZRenderer
	{
	public : 
		ZRenderer() = default;

		static void LoadRenderer(const ZString& libraryName);
		static void UnloadRenderer();

		static ZRenderer& GetRenderer();

		virtual void BeginScene() const = 0;
		virtual void EndScene() const = 0;
	private :
		static ZRenderer* s_instance;
	};
}

#endif //ZRENDERER_HPP