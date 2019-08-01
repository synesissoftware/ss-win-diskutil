
#include <ss-win-diskutil/volumes.h>

#include <windows.h>

#include <stdlib.h>

#include <crtdbg.h>

/* /////////////////////////////////////////////////////////////////////////
 * compatibility
 */

#ifndef _WIN32
# error Requires Windows
#endif

/* /////////////////////////////////////////////////////////////////////////
 * constants
 */

#define NUM_ELEMENTS_(ar)       (sizeof(ar) / sizeof(0[ar]))

#define CCH_VOLID_              (MAX_PATH)
//#define CCH_VOLID_              (64)
#define CCH_VOLNAME_            (MAX_PATH)

/* /////////////////////////////////////////////////////////////////////////
 * types
 */

//struct SSWinDiskUtil_VolumeDescriptor_t
//{
//};
//typedef 


/* /////////////////////////////////////////////////////////////////////////
 * helper function declarations
 */

static
SSWinDiskUtil_VolumeDescriptors_t*
internal_create_(
    wchar_t const*  volume0
);

static
SSWinDiskUtil_VolumeDescriptors_t*
internal_append_(
    SSWinDiskUtil_VolumeDescriptors_t*  current
,   wchar_t const*                      volumeN
);

static
void
internal_destroy_(
    SSWinDiskUtil_VolumeDescriptors_t const*    current
);

static
BOOL
internal_get_volume_spaces_(
    SSWinDiskUtil_slice_w_t const*  volume
,   uint64_t*                       capacityBytes
,   uint64_t*                       systemFreeBytes
,   uint64_t*                       callerFreeBytes
);

/* /////////////////////////////////////////////////////////////////////////
 * API functions
 */

int
SSWinDiskUtil_LoadVolumes(
    void*                               reserved
,   int64_t                             flags
,   SSWinDiskUtil_VolumeDescriptions_t* pvolumes
)
{
    WCHAR   sz[MAX_PATH + 1];
    HANDLE  h;

    if (NULL != reserved)
    {
        SetLastError(ERROR_INVALID_PARAMETER);

        return 0;
    }

    if (0 != flags)
    {
        SetLastError(ERROR_INVALID_PARAMETER);

        return 0;
    }

    _ASSERTE(NULL != pvolumes);

    h = FindFirstVolumeW(&sz[0], NUM_ELEMENTS_(sz));

    if (INVALID_HANDLE_VALUE == h)
    {
#ifdef _DEBUG

        LONG const le = GetLastError();
#endif

        return 0;
    }
    else
    {
        SSWinDiskUtil_VolumeDescriptors_t* volumes = internal_create_(sz);

        if (NULL == volumes)
        {
            FindVolumeClose(h);

            return 0;
        }
        else
        {
            int n = 1;

            for (; FindNextVolumeW(h, &sz[0], NUM_ELEMENTS_(sz)); ++n)
            {
                SSWinDiskUtil_VolumeDescriptors_t* newVolumes = internal_append_(volumes, sz);

                if (NULL == newVolumes)
                {
                    internal_destroy_(volumes);

                    FindVolumeClose(h);

                    return 0;
                }
                else
                {
                    volumes = newVolumes;
                }
            }

            FindVolumeClose(h);

            *pvolumes = volumes;

            return n;
        }
    }
}

int
SSWinDiskUtil_ReleaseVolumes(
    void*                               reserved
,   SSWinDiskUtil_VolumeDescriptions_t  volumes
)
{
    ((void)reserved);

    internal_destroy_(volumes);

    return 0;
}

/* /////////////////////////////////////////////////////////////////////////
 * helper function declarations
 */

static
SSWinDiskUtil_VolumeDescriptors_t*
internal_create_(
    wchar_t const*  volume0
)
{
    size_t const                        cchT    =   (1 + CCH_VOLID_);
    size_t const                        cbT     =   cchT * sizeof(wchar_t);
    size_t const                        n       =   1;
    size_t                              cb      =   sizeof(SSWinDiskUtil_VolumeDescriptors_t) + (n * cbT);
    SSWinDiskUtil_VolumeDescriptors_t*  p       =   (SSWinDiskUtil_VolumeDescriptors_t*)malloc(cb);

    if (NULL != p)
    {
        SSWinDiskUtil_VolumeDescriptor_t*   pVol0   =   &p->volumes[0];
        wchar_t*                            pText   =   (wchar_t*)&p->volumes[1];
        size_t const                        cchTxt  =   lstrlenW(volume0);

        ZeroMemory(pVol0, sizeof(0[pVol0]));
        ZeroMemory(pText, cbT);

        CopyMemory(pText, volume0, cchTxt * sizeof(wchar_t));

        pVol0->id.len       =   cchTxt;
        pVol0->id.ptr       =   pText;

        internal_get_volume_spaces_(&pVol0->id, &pVol0->capacityBytes, &pVol0->systemFreeBytes, &pVol0->callerFreeBytes);

        p->numVolumes   =   1;
    }

    return p;
}

static
SSWinDiskUtil_VolumeDescriptors_t*
internal_append_(
    SSWinDiskUtil_VolumeDescriptors_t*  current
,   wchar_t const*                      volumeN
)
{
    size_t const                        cchT    =   (1 + CCH_VOLID_);
    size_t const                        cbT     =   cchT * sizeof(wchar_t);
    size_t const                        cbVD    =   sizeof(SSWinDiskUtil_VolumeDescriptor_t);
    size_t const                        n       =   current->numVolumes + 1;
    size_t                              cb      =   sizeof(SSWinDiskUtil_VolumeDescriptors_t) + (n * (cbVD + cbT));
    SSWinDiskUtil_VolumeDescriptors_t*  p       =   (SSWinDiskUtil_VolumeDescriptors_t*)realloc(current, cb);

    if (NULL != p)
    {
        size_t const                            nOld        =   p->numVolumes;
        SSWinDiskUtil_VolumeDescriptor_t* const pVol0       =   &p->volumes[0];
        SSWinDiskUtil_VolumeDescriptor_t* const pVolN       =   &p->volumes[nOld];
        wchar_t* const                          pTextOld    =   (wchar_t*)&p->volumes[n - 1];
        wchar_t* const                          pTextNew    =   (wchar_t*)&p->volumes[n - 0];
        wchar_t* const                          pTextN      =   pTextNew + (nOld * cchT);
        wchar_t*                                pText;
        SSWinDiskUtil_VolumeDescriptor_t*       pVol;
        size_t                                  i;

        /* Move down text */

        MoveMemory(pTextNew, pTextOld, nOld * cbT);

        /* Move all existing pointers down */

        for (i = 0, pVol = pVol0, pText = pTextNew; i != nOld; ++i, ++pVol, pText += cchT)
        {
            pVol->id.ptr = pText;

            //if (NULL != pVol->friendlyName)
            //{
            //    pVol->friendlyName += cchT;
            //}
        }

        /* Insert the new element */

        ZeroMemory(pVolN, sizeof(0[pVolN]));
        ZeroMemory(pTextN, cbT);

        lstrcpyW(pTextN, volumeN);

        pVolN->id.len       =   lstrlenW(pTextN);
        pVolN->id.ptr       =   pTextN;

        internal_get_volume_spaces_(&pVolN->id, &pVolN->capacityBytes, &pVolN->systemFreeBytes, &pVolN->callerFreeBytes);

        p->numVolumes   =   n;
    }

    return p;
}

static
void
internal_destroy_(
    SSWinDiskUtil_VolumeDescriptors_t const*    current
)
{
    free(current);
}

static
BOOL
internal_get_volume_spaces_2_(
    SSWinDiskUtil_slice_w_t const*  volId
,   SSWinDiskUtil_slice_w_t const*  volRoot
,   uint64_t*                       capacityBytes
,   uint64_t*                       systemFreeBytes
,   uint64_t*                       callerFreeBytes
);

static
BOOL
internal_get_volume_spaces_(
    SSWinDiskUtil_slice_w_t const*  volume
,   uint64_t*                       capacityBytes
,   uint64_t*                       systemFreeBytes
,   uint64_t*                       callerFreeBytes
)
{
    WCHAR                       volId_[1 + MAX_PATH];
    WCHAR                       volRoot_[1 + MAX_PATH];
    SSWinDiskUtil_slice_w_t     volId;
    SSWinDiskUtil_slice_w_t     volRoot;

    _ASSERTE(L'\0' == volume->len[volume->ptr]);

    CopyMemory(&volId_[0], volume->ptr, volume->len * sizeof(0[volume->ptr]));
    CopyMemory(&volRoot_[0], volume->ptr, volume->len * sizeof(0[volume->ptr]));

    volId.ptr       =   &volId_[0];
    volRoot.ptr     =   &volRoot_[0];

    if (L'\\' == volume->ptr[volume->len - 1])
    {
        volId.len = volume->len - 1;
        volRoot.len = volume->len;
    }
    else
    {
        volId.len = volume->len;
        volRoot.len = volume->len + 1;

        volRoot_[volRoot.len - 1] = L'\\';
    }
    volId_[volId.len] = L'\0';
    volRoot_[volRoot.len] = L'\0';

    return internal_get_volume_spaces_2_(&volId, &volRoot, capacityBytes, systemFreeBytes, callerFreeBytes);
}

static
BOOL
internal_get_volume_spaces_2_(
    SSWinDiskUtil_slice_w_t const*  volId
,   SSWinDiskUtil_slice_w_t const*  volRoot
,   uint64_t*                       capacityBytes
,   uint64_t*                       systemFreeBytes
,   uint64_t*                       callerFreeBytes
)
{
    HANDLE              h;
    VOLUME_DISK_EXTENTS vdExtents;
    DWORD               dwWritten;

    ULARGE_INTEGER      FreeBytesAvailableToCaller;
    ULARGE_INTEGER      TotalNumberOfBytes;
    ULARGE_INTEGER      TotalNumberOfFreeBytes;
    WCHAR               driveName[1 + MAX_PATH];
    DWORD               cchDriveName = NUM_ELEMENTS_(driveName);

    LONG                le;

    WCHAR               volName[CCH_VOLNAME_ + 1];

    _ASSERTE(L'\0' == volId->len[volId->ptr]);
    _ASSERTE(L'\0' == volRoot->len[volRoot->ptr]);


    if (!GetVolumePathNamesForVolumeNameW(volRoot->ptr, driveName, cchDriveName, &cchDriveName))
    {
        if (ERROR_MORE_DATA != GetLastError())
        {
            return FALSE;
        }
    }

    if (!GetDiskFreeSpaceExW(volRoot->ptr, &FreeBytesAvailableToCaller, &TotalNumberOfBytes, &TotalNumberOfFreeBytes))
    {
        return FALSE;
    }

    *capacityBytes      =   TotalNumberOfBytes.QuadPart;
    *systemFreeBytes    =   TotalNumberOfFreeBytes.QuadPart;
    *callerFreeBytes    =   FreeBytesAvailableToCaller.QuadPart;

    return TRUE;
}

/* ///////////////////////////// end of file //////////////////////////// */

