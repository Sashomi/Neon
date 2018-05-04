#pragma once

#ifndef ZIRENDERER_HPP
#define ZIRENDERER_HPP

namespace Zx
{
	class ZIRenderer
	{
	public : 
		virtual void BeginScene() const = 0;
		virtual void EndScene() const = 0;
	private : 
	};
}

#endif