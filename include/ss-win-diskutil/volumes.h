/* /////////////////////////////////////////////////////////////////////////
 * File:    ss-win-diskutil/volumes.h
 *
 * Purpose: Synesis Software Disk Utility library, for Windows: Volumes API.
 *
 * Created: 4th April 2019
 * Updated: 6th February 2024
 *
 * Home:    http://github.com/synesissoftware/ss-win-diskutil
 *
 * Copyright (c) 2019-2024, Matthew Wilson and Synesis Information Systems
 * Copyright (c) 2019, Matthew Wilson and Synesis Software
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * - Neither the name(s) of Matthew Wilson and Synesis Software nor the
 *   names of any contributors may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ////////////////////////////////////////////////////////////////////// */


#ifndef SS_WIN_DISKUTIL_h_volumes
#define SS_WIN_DISKUTIL_h_volumes

/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <stddef.h>
#include <stdint.h>


/* /////////////////////////////////////////////////////////////////////////
 * types
 */

/** String slice type
 *
 * @note A string slice type such as this is not required to refer to a
 *   nul-terminated C-style string. Refer to the documentation of the
 *   particular construct to inform whether the use(s) involve
 *   nul-terminated strings
 */
struct SSWinDiskUtil_slice_w_t
{
    size_t                              len;    /*!< The length of the string */
    wchar_t const*                      ptr;    /*!< Pointer to the first character of the string */
};
#ifndef __cplusplus
typedef struct SSWinDiskUtil_slice_w_t                      SSWinDiskUtil_slice_w_t;
#endif /* !__cplusplus */

struct SSWinDiskUtil_VolumeDescriptor_t
{
    SSWinDiskUtil_slice_w_t             id;                 /*!< The volume identifier */
    SSWinDiskUtil_slice_w_t             friendlyName;       /*!< The volume friendly name */
    SSWinDiskUtil_slice_w_t             reserved0[5];
    uint64_t                            capacityBytes;      /*!< The volume capacity */
    uint64_t                            systemFreeBytes;    /*!< The volume free-space */
    uint64_t                            callerFreeBytes;    /*!< The volume free-space as available to the caller */
    uint64_t                            statusFlags;        /*!< Status flags */
};
#ifndef __cplusplus
typedef struct SSWinDiskUtil_VolumeDescriptor_t             SSWinDiskUtil_VolumeDescriptor_t;
#endif /* !__cplusplus */

/** Volume descriptors structure
 *
 */
struct SSWinDiskUtil_VolumeDescriptors_t
{
    uint64_t                            numVolumes;         /*!< The number of volumes */
    SSWinDiskUtil_VolumeDescriptor_t    volumes[1];         /*!< The array of volumes, of the size [numVolumes] */
};
#ifndef __cplusplus
typedef struct SSWinDiskUtil_VolumeDescriptors_t            SSWinDiskUtil_VolumeDescriptors_t;
#endif /* !__cplusplus */

/** Volume descriptions handle
 *
 */
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

/** Releases all resources associated with the volumes
 *
 * @param reserved [in] (void*) Reserved. Must be \c nullptr
 * @param volumes [in] The descriptions handle
 *
 * @retval 0
 */
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

/** Obtains the volumes information for the host
 *
 * @see SSWinDiskUtil_LoadVolumes()
 */
inline
int
LoadVolumes(
    int64_t                             flags
,   SSWinDiskUtil_VolumeDescriptions_t* pvolumes
)
{
    return SSWinDiskUtil_LoadVolumes(nullptr, flags, pvolumes);
}

/** Releases all resources associated with the volumes
 *
 * @see SSWinDiskUtil_ReleaseVolumes()
 */
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
 * STLSoft string access shims
 */

#ifdef __cplusplus
namespace stlsoft
{
    inline
    size_t
    c_str_len_w(
        SSWinDiskUtil_slice_w_t const& sl
    )
    {
        return sl.len;
    }

    inline
    wchar_t const*
    c_str_data_w(
        SSWinDiskUtil_slice_w_t const& sl
    )
    {
        return sl.ptr;
    }

} /* namespace stlsoft */
#endif /* __cplusplus */


/* /////////////////////////////////////////////////////////////////////////
 * inclusion control
 */

#ifdef STLSOFT_CF_PRAGMA_ONCE_SUPPORT
# pragma once
#endif /* STLSOFT_CF_PRAGMA_ONCE_SUPPORT */

#endif /* !SS_WIN_DISKUTIL_h_volumes */

/* ///////////////////////////// end of file //////////////////////////// */

