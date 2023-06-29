/**
 * @file Export.h
 * @brief Macros for exporting symbol.
 */
#ifndef BALLOON_EXPORT_H
#define BALLOON_EXPORT_H

#ifndef BALLOON_EXPORT
#ifdef BALLOON_EXPORTS
#define BALLOON_EXPORT __declspec(dllexport)
#else
#define BALLOON_EXPORT __declspec(dllimport)
#endif
#endif

#define MOD_EXPORT extern "C" __declspec(dllexport)

#endif // BALLOON_EXPORT_H