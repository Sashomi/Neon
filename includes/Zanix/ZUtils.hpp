#ifndef ZUTILS_HPP
#define ZUTILS_HPP

#include <Zanix/Core/ZException.hpp>

#if !defined NDEBUG
	#define ZDEBUG
#endif

#if defined (_WIN32)
	#define ZANIX_WINDOWS

	#define ZANIX_EXPORT __declspec(dllexport)
	#define ZANIX_IMPORT __declspec(dllimport)
#endif

#endif //ZUTILS_HPP