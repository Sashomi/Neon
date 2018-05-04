#ifndef ZUTILS_HPP
#define ZUTILS_HPP

#if defined(_WIN32)
	#define EXPORT_ZANIX __declspec(dllexport)
	#define IMPORT_ZANIX __declspec(dllimport)
#endif

#endif //ZUTILS_HPP