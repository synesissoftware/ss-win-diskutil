
#ifndef SS_WIN_DISKUTIL_h_volumes
#define SS_WIN_DISKUTIL_h_volumes

#include <stdint.h>

/* /////////////////////////////////////////////////////////////////////////
 * types
 */

struct SSWinDiskUtil_VolumeDescriptor_t
{
    wchar_t const*  id;
    wchar_t const*  friendlyName;
    uint64_t        capacityBytes;
    uint64_t        freeBytes;
    uint64_t        statusFlags;
};
#ifndef __cplusplus
typedef struct SSWinDiskUtil_VolumeDescriptor_t             SSWinDiskUtil_VolumeDescriptor_t;
#endif /* !__cplusplus */

struct SSWinDiskUtil_VolumeDescriptors_t
{
    size_t                              numVolumes; /*!< The number of volumes */
    SSWinDiskUtil_VolumeDescriptor_t    volumes[1]; /*!< The array of volumes, of the size [numVolumes] */
};
#ifndef __cplusplus
typedef struct SSWinDiskUtil_VolumeDescriptors_t            SSWinDiskUtil_VolumeDescriptors_t;
#endif /* !__cplusplus */

typedef SSWinDiskUtil_VolumeDescriptors_t const*            SSWinDiskUtil_VolumeDescriptions_t;

/* /////////////////////////////////////////////////////////////////////////
 * API functions
 */

#ifdef __cplusplus
extern "C"
{
#endif

/** Obtains the volumes information for the host
 *
 * @param reserved [in] (void*) Reserved. Must be \c nullptr
 * @param flags [in] (int64_t) Flags. Currently must be \c 0
 * @param pvolumes [out] Pointer to handle (of type
 *   \c SSWinDiskUtil_VolumeDescriptions_t, which is
 *   <code>SSWinDiskUtil_VolumeDescriptors_t const*</code), to receive the
 *   volumes information. Must NOT be \c nullptr
 *
 * @retval 0 The function failed. Use GetLastError() for further information
 * @retval >0 The number of volumes for which information is obtained
 */
int
SSWinDiskUtil_LoadVolumes(
    void*                                   reserved
,   int64_t                                 flags
,   SSWinDiskUtil_VolumeDescriptions_t*     pvolumes
);

int
SSWinDiskUtil_ReleaseVolumes(
    void*                                   reserved
,   SSWinDiskUtil_VolumeDescriptions_t      volumes
);

#ifdef __cplusplus
} /* extern "C" */
#endif

/* /////////////////////////////////////////////////////////////////////////
 * namespace
 */

#ifdef __cplusplus

namespace SynesisSoftware {
namespace Windows {
namespace DiskUtil {
#endif /* __cplusplus */

/* /////////////////////////////////////////////////////////////////////////
 * C++ types
 */

#ifdef __cplusplus

typedef SSWinDiskUtil_VolumeDescriptor_t                    VolumeDescriptor_t;

typedef SSWinDiskUtil_VolumeDescriptions_t                  VolumeDescriptions_t;
#endif /* __cplusplus */

/* /////////////////////////////////////////////////////////////////////////
 * API functions (C++)
 */

#ifdef __cplusplus

inline
int
LoadVolumes(
    int64_t                             flags
,   SSWinDiskUtil_VolumeDescriptions_t* pvolumes
)
{
    return SSWinDiskUtil_LoadVolumes(nullptr, flags, pvolumes);
}

inline
int
ReleaseVolumes(
    SSWinDiskUtil_VolumeDescriptions_t  volumes
)
{
    return SSWinDiskUtil_ReleaseVolumes(nullptr, volumes);
}
#endif /* __cplusplus */

/* /////////////////////////////////////////////////////////////////////////
 * namespace
 */

#ifdef __cplusplus
} /* namespace DiskUtil */
} /* namespace Windows */
} /* namespace SynesisSoftware */
#endif /* __cplusplus */

/* /////////////////////////////////////////////////////////////////////////
 * inclusion control
 */

#ifdef STLSOFT_CF_PRAGMA_ONCE_SUPPORT
# pragma once
#endif /* STLSOFT_CF_PRAGMA_ONCE_SUPPORT */

#endif /* !SS_WIN_DISKUTIL_h_volumes */

/* ///////////////////////////// end of file //////////////////////////// */

