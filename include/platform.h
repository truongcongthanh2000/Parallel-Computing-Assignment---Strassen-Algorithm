#pragma once

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
	#define PLATFORM_NAME "windows" // Windows
#elif defined(__ANDROID__)
	#define PLATFORM_NAME "android" // Android (implies Linux, so it must come first)
#elif defined(__linux__)
	#define PLATFORM_NAME "linux" // Debian, Ubuntu, Gentoo, Fedora, openSUSE, RedHat, Centos and other
#elif defined(__unix__) || !defined(__APPLE__) && defined(__MACH__)
	#include <sys/param.h>

	#if defined(BSD)
		#define PLATFORM_NAME "bsd" // FreeBSD, NetBSD, OpenBSD, DragonFly BSD
	#endif
#elif defined(__hpux)
	#define PLATFORM_NAME "hp-ux" // HP-UX
#elif defined(_AIX)
	#define PLATFORM_NAME "aix" // IBM AIX
#elif defined(__APPLE__) && defined(__MACH__) // Apple OSX and iOS (Darwin)
	#include <TargetConditionals.h>

	#if TARGET_IPHONE_SIMULATOR == 1
		#define PLATFORM_NAME "ios" // Apple iOS
	#elif TARGET_OS_IPHONE == 1
		#define PLATFORM_NAME "ios" // Apple iOS
	#elif TARGET_OS_MAC == 1
		#define PLATFORM_NAME "osx" // Apple OSX
	#endif
#elif defined(__sun) && defined(__SVR4)
	#define PLATFORM_NAME "solaris" // Oracle Solaris, Open Indiana
#else
	#define PLATFORM_NAME NULL
#endif

// Return a name of platform, if determined, otherwise - an empty string
const char* get_platform_name() {
	return (PLATFORM_NAME == "") ? "" : PLATFORM_NAME;
}
