#ifndef CALCEXPR_COMPILER_H
#define CALCEXPR_COMPILER_H

/*
 * Compiler and platform detection macros.
 *
 * This header provides a reference pattern for portable C code across the
 * four supported compiler families (MSVC, GCC, Clang, AppleClang) and three
 * target platforms (Windows, Linux, macOS).
 */

/* ---- Compiler family detection ---- */

#define CALCEXPR_COMPILER_MSVC 0
#define CALCEXPR_COMPILER_APPLECLANG 0
#define CALCEXPR_COMPILER_CLANG 0
#define CALCEXPR_COMPILER_GCC 0

#if defined(_MSC_VER)
#undef CALCEXPR_COMPILER_MSVC
#define CALCEXPR_COMPILER_MSVC 1
#elif defined(__clang__) && defined(__APPLE__)
#undef CALCEXPR_COMPILER_APPLECLANG
#define CALCEXPR_COMPILER_APPLECLANG 1
#elif defined(__clang__)
#undef CALCEXPR_COMPILER_CLANG
#define CALCEXPR_COMPILER_CLANG 1
#elif defined(__GNUC__)
#undef CALCEXPR_COMPILER_GCC
#define CALCEXPR_COMPILER_GCC 1
#else
#warning "Unknown compiler. Portable macros may not be available."
#endif

/* ---- Platform detection ---- */

#define CALCEXPR_PLATFORM_WINDOWS 0
#define CALCEXPR_PLATFORM_MACOS 0
#define CALCEXPR_PLATFORM_LINUX 0

#if defined(_WIN32)
#undef CALCEXPR_PLATFORM_WINDOWS
#define CALCEXPR_PLATFORM_WINDOWS 1
#elif defined(__APPLE__)
#undef CALCEXPR_PLATFORM_MACOS
#define CALCEXPR_PLATFORM_MACOS 1
#elif defined(__linux__)
#undef CALCEXPR_PLATFORM_LINUX
#define CALCEXPR_PLATFORM_LINUX 1
#endif

/* ---- Portable function attributes ---- */

#if CALCEXPR_COMPILER_MSVC
#define CALCEXPR_NORETURN __declspec(noreturn)
#define CALCEXPR_DEPRECATED(msg) __declspec(deprecated(msg))
#else
#define CALCEXPR_NORETURN __attribute__((__noreturn__))
#define CALCEXPR_DEPRECATED(msg) __attribute__((__deprecated__(msg)))
#endif

#endif /* CALCEXPR_COMPILER_H */
