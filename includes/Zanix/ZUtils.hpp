#ifndef ZUTILS_HPP
#define ZUTILS_HPP

#if !defined NDEBUG
	#define ZDEBUG
#endif

#if defined (_WIN32)
	#define ZANIX_WINDOWS
	#define VK_USE_PLATFORM_WIN32_KHR

	#define ZANIX_EXPORT __declspec(dllexport)
	#define ZANIX_IMPORT __declspec(dllimport)
#endif

#endif //ZUTILS_HPP