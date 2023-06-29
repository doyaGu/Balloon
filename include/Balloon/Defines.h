/**
 * @file Defines.h
 * @brief General helper and export macros.
 */
#ifndef BALLOON_DEFINES_H
#define BALLOON_DEFINES_H

#include "Version.h"
#include "Export.h"

#ifndef BALLOON_BEGIN_CDECLS
#   ifdef __cplusplus
#       define BALLOON_BEGIN_CDECLS extern "C" {
#   else
#       define BALLOON_BEGIN_CDECLS
#   endif
#endif // !BALLOON_BEGIN_CDECLS

#ifndef BALLOON_END_CDECLS
#   ifdef __cplusplus
#       define BALLOON_END_CDECLS }
#   else
#       define BALLOON_END_CDECLS
#   endif
#endif // !BALLOON_END_CDECLS

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#define TOSTRING2(arg) #arg
#define TOSTRING(arg) TOSTRING2(arg)

#define BALLOON_UNUSED(x) (void)(x)

#define BALLOON_ARRAY_LENGTH(a) (sizeof(a) / sizeof((a)[0]))

#define BALLOON_IS_ALIGNED_TO(x, align) (((x) & ((align) - 1)) == 0)
#define BALLOON_ALIGN_UP(x, align) (((x) + (align) - 1) & ~((align) - 1))
#define BALLOON_ALIGN_DOWN(x, align) (((x) - 1) & ~((align) - 1))

#endif // BALLOON_DEFINES_H
