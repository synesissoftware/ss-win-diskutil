
#include <ss-win-diskutil.h>

#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static
void
show_usage(
    FILE*           stm
,   wchar_t const*  bn
);

static
wchar_t const*
basename(
    wchar_t const*  path
);

int wmain(int argc, wchar_t* argv[])
{
    int                     i;
    int                     showLabels  =   0;
    int                     showSpaces  =   0;
    wchar_t const* const    bn          =   basename(argv[0]);

    SSWinDiskUtil_VolumeDescriptions_t  volumes;

    for (i = 1; argc != i; ++i)
    {
        wchar_t const* const arg = argv[i];

        if (0 == wcscmp(L"--help", arg))
        {
            show_usage(stdout, bn);

            return EXIT_SUCCESS;
        }
        else if(0 == wcscmp(L"--label", arg))
        {
            showLabels = 1;
        }
        else if(0 == wcscmp(L"--spaces", arg))
        {
            showSpaces = 1;
        }
        else
        {
            fwprintf(stderr, L"%s: unrecognised argument '%s'; use --help for usage\n", bn, arg);

            return EXIT_FAILURE;
        }
    }

    if (0 == SSWinDiskUtil_LoadVolumes(NULL, 0, &volumes))
    {
        LONG const le = GetLastError();

        fwprintf(stderr, L"%s: failed to load volumes: %d\n", bn, le);

        return EXIT_FAILURE;
    }
    else
    {
        size_t j;

        fwprintf(stdout, L"%lu volume(s)\n", volumes->numVolumes);

        for (j = 0; volumes->numVolumes != j; ++j)
        {
            SSWinDiskUtil_VolumeDescriptor_t const* const volume = &volumes->volumes[j];

            fwprintf(
                stdout
            ,   L"%lu: id=%.*s label=\"%.*s\" free=%I64u capacity=%I64u\n"
            ,   j
            ,   (int)volume->id.len, volume->id.ptr
            ,   (int)volume->friendlyName.len, volume->friendlyName.ptr
            ,   volume->callerFreeBytes
            ,   volume->capacityBytes
            );
        }

        SSWinDiskUtil_ReleaseVolumes(NULL, volumes);

        return EXIT_SUCCESS;
    }
}

static
wchar_t const*
basename(
    wchar_t const*  path
)
{
    wchar_t const* const  fslash  =   wcsrchr(path, L'/');
    wchar_t const* const  bslash  =   wcsrchr(path, L'\\');

    if (NULL == fslash)
    {
        if (NULL == bslash)
        {
            return path;
        }
        else
        {
            return bslash + 1;
        }
    }
    else
    {
        if (NULL == bslash)
        {
            return fslash + 1;
        }
        else
        {
            if (bslash < fslash)
            {
                return fslash + 1;
            }
            else
            {
                return bslash + 1;
            }
        }
    }
}

static
void
show_usage(
    FILE*           stm
,   wchar_t const*  bn
)
{
    fwprintf(stm, L"USAGE: %s { --help | [ --label ] [ --spaces ] }\n", bn);
}

