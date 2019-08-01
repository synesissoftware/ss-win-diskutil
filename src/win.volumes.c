
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
 * types
 */

typedef SSWinDiskUtil_VolumeDescriptor_t        internal_descriptor_t;

/* /////////////////////////////////////////////////////////////////////////
 * constants
 */

#define NUM_ELEMENTS_(ar)               (sizeof(ar) / sizeof(0[ar]))

#define CCH_VOLID_                      (MAX_PATH)
#define CCH_VOLLABEL_                   (MAX_PATH)

#define CAP_JUMP_                       (8)

static char const s_guard[8] = { '~', '~', '~', '~', '~', '~', '~', '~', };

/* /////////////////////////////////////////////////////////////////////////
 * macros
 */

/* /////////////////////////////////////////////////////////////////////////
 * helper function declarations
 */

static
void*
internal_allocate_(
    size_t cb
);

static
void*
internal_reallocate_(
    void*   pv
,   size_t  cb
);

static
void
internal_free_(
    void*   pv
);

static
SSWinDiskUtil_VolumeDescriptors_t*
internal_create_(
    wchar_t const*                      volId
,   wchar_t const*                      volLabel
,   uint64_t                            capacityBytes
,   uint64_t                            systemFreeBytes
,   uint64_t                            callerFreeBytes
);

static
SSWinDiskUtil_VolumeDescriptors_t*
internal_append_(
    SSWinDiskUtil_VolumeDescriptors_t*  current
,   wchar_t const*                      volId
,   wchar_t const*                      volLabel
,   uint64_t                            capacityBytes
,   uint64_t                            systemFreeBytes
,   uint64_t                            callerFreeBytes
);

static
void
internal_destroy_(
    SSWinDiskUtil_VolumeDescriptors_t const*    current
);

static
HANDLE
internal_FindFirstVolume_(
    wchar_t*    volumeId
,   DWORD       cchVolumeId
,   wchar_t*    volumeLabel
,   DWORD       cchVolumeLabel
,   uint64_t*   capacityBytes
,   uint64_t*   systemFreeBytes
,   uint64_t*   callerFreeBytes
);

static
BOOL
internal_FindNextVolume_(
    HANDLE      hFind
,   wchar_t*    volumeId
,   DWORD       cchVolumeId
,   wchar_t*    volumeLabel
,   DWORD       cchVolumeLabel
,   uint64_t*   capacityBytes
,   uint64_t*   systemFreeBytes
,   uint64_t*   callerFreeBytes
);

static
uint64_t
external_n_(
    uint64_t    numVolumes
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
    WCHAR       volId[CCH_VOLID_ + 1];
    WCHAR       volLabel[CCH_VOLLABEL_ + 1];
    uint64_t    capacityBytes;
    uint64_t    systemFreeBytes;
    uint64_t    callerFreeBytes;
    HANDLE      h;

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

    h = internal_FindFirstVolume_(&volId[0], NUM_ELEMENTS_(volId), &volLabel[0], NUM_ELEMENTS_(volLabel), &capacityBytes, &systemFreeBytes, &callerFreeBytes);

    if (INVALID_HANDLE_VALUE == h)
    {
        return 0;
    }
    else
    {
        SSWinDiskUtil_VolumeDescriptors_t* volumes = internal_create_(volId, volLabel, capacityBytes, systemFreeBytes, callerFreeBytes);

        if (NULL == volumes)
        {
            FindVolumeClose(h);

            return 0;
        }
        else
        {
            int n = 1;

            for (; internal_FindNextVolume_(h, &volId[0], NUM_ELEMENTS_(volId), &volLabel[0], NUM_ELEMENTS_(volLabel), &capacityBytes, &systemFreeBytes, &callerFreeBytes); ++n)
            {
                SSWinDiskUtil_VolumeDescriptors_t* newVolumes = internal_append_(volumes, volId, volLabel, capacityBytes, systemFreeBytes, callerFreeBytes);

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

            volumes->numVolumes = external_n_(volumes->numVolumes);

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
void*
internal_allocate_(
    size_t cb
)
{
    void* pv = malloc(cb);

    if (NULL != pv)
    {
        ;
    }

    return pv;
}

static
void*
internal_reallocate_(
    void*   pv
,   size_t  cb
)
{
    return realloc(pv, cb);
}

static
void
internal_free_(
    void*   pv
)
{
    free(pv);
}

static
void
internal_copychars_(
    wchar_t**       pp
,   wchar_t const*  s
,   size_t          n
)
{
    _ASSERTE(NULL != pp);
    _ASSERTE(NULL != *pp);
    _ASSERTE(0 == n || NULL != s);

    if (0 != n)
    {
        CopyMemory(*pp, s, sizeof(wchar_t) * n);
        (*pp) += n;
    }

    **pp = L'\0';
    (*pp)++;
}

static
size_t
internal_num_logical_drives_()
{
    DWORD const ld = GetLogicalDrives();
    size_t      i;
    size_t      r;

    for (i = 0, r = 0; 32 != i; ++i)
    {
        if (ld & (0x1 << i))
        {
            ++r;
        }
    }

    return r;
}

static
uint64_t
internal_n_(
    uint64_t    cap
,   uint64_t    n
)
{
    _ASSERTE(cap < 0x100000000);
    _ASSERTE(n < 0x100000000);

    return (cap << 32) | (n & 0xffffffff);
}

static
uint64_t
external_n_(
    uint64_t    numVolumes
)
{
    return numVolumes & 0xffffffff;
}


static
SSWinDiskUtil_VolumeDescriptors_t*
internal_create_(
    wchar_t const*                      volId
,   wchar_t const*                      volLabel
,   uint64_t                            capacityBytes
,   uint64_t                            systemFreeBytes
,   uint64_t                            callerFreeBytes
)
{
    /* While building, we use the top 32-bits of the descriptors' numVolumes
     * for the offset
     */

    size_t const                        nLogical    =   1 /* internal_num_logical_drives_() */;

    size_t const                        newCap      =   nLogical;
    size_t const                        newN        =   1;

    size_t const                        cchId       =   lstrlenW(volId);
    size_t const                        cchLabel    =   lstrlenW(volLabel);

    /* Allocate space for:
     *
     * - 1 x SSWinDiskUtil_VolumeDescriptors_t
     * - (newCap - 1) x SSWinDiskUtil_VolumeDescriptor_t
     * - volId (+ nul)
     * - volLabel (+ nul)
     */

    size_t                              cbCtrl      =   0
                                                    +   sizeof(SSWinDiskUtil_VolumeDescriptors_t)
                                                    +   sizeof(SSWinDiskUtil_VolumeDescriptor_t) * (newCap - 1)
                                                    ;
    size_t                              cbText      =   0
                                                    +   sizeof(wchar_t) * (1 + cchId)
                                                    +   sizeof(wchar_t) * (1 + cchLabel)
                                                    ;
    size_t const                        cbGuard     =   8;
    size_t                              cb          =   0
                                                    +   cbCtrl
                                                    +   cbText
                                                    +   cbGuard
                                                    ;
    SSWinDiskUtil_VolumeDescriptors_t*  p           =   (SSWinDiskUtil_VolumeDescriptors_t*)internal_allocate_(cb);

    if (NULL != p)
    {
        SSWinDiskUtil_VolumeDescriptor_t* const pVol0   =   &p->volumes[0];
        void* const                             pGuard  =   (char*)p + (cb - cbGuard);
        wchar_t*                                pText   =   (wchar_t*)&p->volumes[newCap];


        /* guard: */

        FillMemory(pGuard, cbGuard, '~');


        /* zero: */

        ZeroMemory(pVol0, sizeof(0[pVol0]));


        /* Id: */

        pVol0->id.len           =   cchId;
        pVol0->id.ptr           =   pText;

        internal_copychars_(&pText, volId, cchId);


        /* Label: */

        pVol0->friendlyName.len =   cchLabel;
        pVol0->friendlyName.ptr =   pText;

        internal_copychars_(&pText, volLabel, cchLabel);


        /* stats: */

        pVol0->capacityBytes    =   capacityBytes;
        pVol0->systemFreeBytes  =   systemFreeBytes;
        pVol0->callerFreeBytes  =   callerFreeBytes;


        /* guard: */

        _ASSERTE(0 == memcmp(pGuard, s_guard, cbGuard));



        p->numVolumes           =   internal_n_(newCap, newN);
    }

    return p;
}

static
SSWinDiskUtil_VolumeDescriptors_t*
internal_append_(
    SSWinDiskUtil_VolumeDescriptors_t*  current
,   wchar_t const*                      volId
,   wchar_t const*                      volLabel
,   uint64_t                            capacityBytes
,   uint64_t                            systemFreeBytes
,   uint64_t                            callerFreeBytes
)
{
    size_t const                        currCap     =   (current->numVolumes >> 32) & 0xffffffff;
    size_t const                        currN       =   (current->numVolumes >>  0) & 0xffffffff;

    size_t const                        newCap      =   (currN < currCap) ? currCap : currCap + CAP_JUMP_;
    size_t const                        newN        =   currN + 1;

    size_t const                        cchId       =   lstrlenW(volId);
    size_t const                        cchLabel    =   lstrlenW(volLabel);

    /* Reallocate space for:
     *
     * - 1 x SSWinDiskUtil_VolumeDescriptors_t
     * - (newCap - 1) x SSWinDiskUtil_VolumeDescriptor_t
     * - currently used text space
     * - volId (+ nul)
     * - volLabel (+ nul)
     */

    size_t                              cbCtrl      =   0
                                                    +   sizeof(SSWinDiskUtil_VolumeDescriptors_t)
                                                    +   sizeof(SSWinDiskUtil_VolumeDescriptor_t) * (newCap - 1)
                                                    ;
    internal_descriptor_t* const        pVolL       =   &current->volumes[currN - 1];
    internal_descriptor_t* const        pVolN       =   &current->volumes[currCap];
    size_t const                        cbCurrText  =   0
                                                    +   (char*)(pVolL->friendlyName.ptr + pVolL->friendlyName.len + 1)
                                                    -   (char*)pVolN
                                                    ;
    size_t                              cbNewText   =   0
                                                    +   sizeof(wchar_t) * (1 + cchId)
                                                    +   sizeof(wchar_t) * (1 + cchLabel)
                                                    ;
    size_t                              cbText      =   0
                                                    +   cbCurrText
                                                    +   cbNewText
                                                    ;
    size_t const                        cbGuard     =   8;
    size_t                              cb          =   0
                                                    +   cbCtrl
                                                    +   cbText
                                                    +   cbGuard
                                                    ;
    SSWinDiskUtil_VolumeDescriptors_t*  p           =   (SSWinDiskUtil_VolumeDescriptors_t*)internal_reallocate_(current, cb);

    if (NULL != p)
    {
        internal_descriptor_t* const    pVolN       =   &p->volumes[currN];
        void* const                     pGuard      =   (char*)p + (cb - cbGuard);
        wchar_t*                        pCurrText   =   (wchar_t*)(&p->volumes[currCap]);
        wchar_t*                        pNewText    =   (wchar_t*)(&p->volumes[newCap]);
        wchar_t*                        pText       =   (wchar_t*)((char*)p + (cb - (cbNewText + cbGuard)));


        /* guard: */

        FillMemory(pGuard, cbGuard, '~');


        /* move down and adjust all extant (if necessary): */

        if (newCap != currCap)
        {
            MoveMemory(pNewText, pCurrText, cbCurrText);
        }

        if (current != p || newCap != currCap)
        {
            ptrdiff_t const                 d       =   (char*)p - (char*)current + (newCap - currCap) * sizeof(0[pVolN]);
            internal_descriptor_t*          vol     =   &p->volumes[0];
            size_t                          i;

            for (i = 0; i != currN; ++i, ++vol)
            {
                vol->id.ptr             +=  d / sizeof(wchar_t);
                vol->friendlyName.ptr   +=  d / sizeof(wchar_t);

                _ASSERTE(vol->id.len == lstrlenW(vol->id.ptr));
                _ASSERTE(vol->friendlyName.len == lstrlenW(vol->friendlyName.ptr));
            }
        }


        /* zero: */

        ZeroMemory(pVolN, sizeof(0[pVolN]));


        /* Id: */

        pVolN->id.len           =   cchId;
        pVolN->id.ptr           =   pText;

        internal_copychars_(&pText, volId, cchId);


        /* Label: */

        pVolN->friendlyName.len =   cchLabel;
        pVolN->friendlyName.ptr =   pText;

        internal_copychars_(&pText, volLabel, cchLabel);


        /* stats: */

        pVolN->capacityBytes    =   capacityBytes;
        pVolN->systemFreeBytes  =   systemFreeBytes;
        pVolN->callerFreeBytes  =   callerFreeBytes;


        /* guard: */

        _ASSERTE(0 == memcmp(pGuard, s_guard, cbGuard));


        p->numVolumes           =   internal_n_(newCap, newN);
    }

    return p;
}

static
void
internal_destroy_(
    SSWinDiskUtil_VolumeDescriptors_t const*    current
)
{
    void* p = (void*)current;

    internal_free_(p);
}

static
void
internal_get_label_and_spaces_(
    wchar_t const*  volumeId
,   wchar_t*        volumeLabel
,   DWORD           cchVolumeLabel
,   uint64_t*       capacityBytes
,   uint64_t*       systemFreeBytes
,   uint64_t*       callerFreeBytes
)
{
    ULARGE_INTEGER      freeBytesAvailableToCaller;
    ULARGE_INTEGER      totalNumberOfBytes;
    ULARGE_INTEGER      totalNumberOfFreeBytes;

    if (!GetVolumeInformationW(volumeId, volumeLabel, cchVolumeLabel, NULL, NULL, NULL, NULL, 0))
    {
        volumeLabel[0] = L'\0';
    }

    if (GetDiskFreeSpaceExW(volumeId, &freeBytesAvailableToCaller, &totalNumberOfBytes, &totalNumberOfFreeBytes))
    {
        *capacityBytes      =   totalNumberOfBytes.QuadPart;
        *systemFreeBytes    =   totalNumberOfFreeBytes.QuadPart;
        *callerFreeBytes    =   freeBytesAvailableToCaller.QuadPart;
    }
}

static
HANDLE
internal_FindFirstVolume_(
    wchar_t*    volumeId
,   DWORD       cchVolumeId
,   wchar_t*    volumeLabel
,   DWORD       cchVolumeLabel
,   uint64_t*   capacityBytes
,   uint64_t*   systemFreeBytes
,   uint64_t*   callerFreeBytes
)
{
    HANDLE hFind = FindFirstVolumeW(volumeId, cchVolumeId);

    if (INVALID_HANDLE_VALUE != hFind)
    {
        internal_get_label_and_spaces_(volumeId, volumeLabel, cchVolumeLabel, capacityBytes, systemFreeBytes, callerFreeBytes);
    }

    return hFind;
}

static
BOOL
internal_FindNextVolume_(
    HANDLE      hFind
,   wchar_t*    volumeId
,   DWORD       cchVolumeId
,   wchar_t*    volumeLabel
,   DWORD       cchVolumeLabel
,   uint64_t*   capacityBytes
,   uint64_t*   systemFreeBytes
,   uint64_t*   callerFreeBytes
)
{
    BOOL b = FindNextVolumeW(hFind, volumeId, cchVolumeId);

    if (b)
    {
        internal_get_label_and_spaces_(volumeId, volumeLabel, cchVolumeLabel, capacityBytes, systemFreeBytes, callerFreeBytes);
    }

    return b;
}

/* ///////////////////////////// end of file //////////////////////////// */

