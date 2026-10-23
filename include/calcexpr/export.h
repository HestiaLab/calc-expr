#ifndef CALCEXPR_EXPORT_H
#define CALCEXPR_EXPORT_H

#if defined(_WIN32) && defined(CALCEXPR_SHARED)
#if defined(CALCEXPR_BUILDING_LIBRARY)
#define CALCEXPR_API __declspec(dllexport)
#else
#define CALCEXPR_API __declspec(dllimport)
#endif
#else
#define CALCEXPR_API
#endif

#endif /* CALCEXPR_EXPORT_H */
