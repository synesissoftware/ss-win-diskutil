
#include <ss-win-diskutil/volumes.h>

#include <windows.h>

#include <assert.h>
#include <stdlib.h>

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

#define CCH_VOL_                (MAX_PATH)

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

    assert(NULL != pvolumes);

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
    size_t const                        cchT    =   (1 + CCH_VOL_);
    size_t const                        cbT     =   cchT * sizeof(wchar_t);
    size_t const                        n       =   1;
    size_t                              cb      =   sizeof(SSWinDiskUtil_VolumeDescriptors_t) + (n * cbT);
    SSWinDiskUtil_VolumeDescriptors_t*  p       =   (SSWinDiskUtil_VolumeDescriptors_t*)malloc(cb);

    if (NULL != p)
    {
        SSWinDiskUtil_VolumeDescriptor_t*   pVol0   =   &p->volumes[0];
        wchar_t*                            pText   =   (wchar_t*)&p->volumes[1];

        ZeroMemory(pVol0, sizeof(*pVol0));
        ZeroMemory(pText, cbT);

        lstrcpyW(pText, volume0);

        pVol0->id       =   pText;

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
    size_t const                        cchT    =   (1 + CCH_VOL_);
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
            pVol->id = pText;

            //if (NULL != pVol->friendlyName)
            //{
            //    pVol->friendlyName += cchT;
            //}
        }

        /* Insert the new element */

        ZeroMemory(pVolN, sizeof(*pVolN));
        ZeroMemory(pTextN, cbT);

        lstrcpyW(pTextN, volumeN);

        pVolN->id       =   pTextN;


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

/* ///////////////////////////// end of file //////////////////////////// */

