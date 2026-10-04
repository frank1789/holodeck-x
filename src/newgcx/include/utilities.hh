#ifndef HOLODECK_X_NEW_CGX_UTILILIES_HH_
#define HOLODECK_X_NEW_CGX_UTILILIES_HH_

#if defined(_WIN32) || defined(_WIN64)
#define HDX_WINDOWS
#elif defined(__linux__)
#define HDX_LINUX
#elif defined(__APPLE__) && defined(__MACH__)
#define HDX_MACOS
#elif defined(__FreeBSD__)
#define HDX_FREEBSD
#else
#warning \
    "Unsupported platform. This project is only supported on Windows, Linux, macOS, and FreeBSD."
#endif

#endif  // HOLODECK_X_NEW_CGX_UTILILIES_HH_
