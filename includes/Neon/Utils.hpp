#ifndef UTILS_HPP
#define UTILS_HPP

#if !defined NDEBUG
	#define DEBUG
#endif

#if defined (_WIN32)
	#define NEON_WINDOWS
	#define VK_USE_PLATFORM_WIN32_KHR

	#define NEON_EXPORT __declspec(dllexport)
	#define NEON_IMPORT __declspec(dllimport)
#endif

#endif //UTILS_HPP