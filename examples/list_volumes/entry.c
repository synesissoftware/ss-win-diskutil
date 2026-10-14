
#include <ss-win-diskutil.h>

#include <diagnosticism/tracing.h>
#include <woad/woad.h>

#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>


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

/* Writes a narrow, ASCII-only string (such as a woad SGR sequence, which
 * is empty when colour is not warranted for the stream) to a stream that
 * is otherwise used with wide-character functions.
 */
static
void
put_sequence(
    FILE*           stm
,   char const*     seq
);

int wmain(int argc, wchar_t* argv[])
{
    int                     i;
    int                     showLabels  =   0;
    int                     showSpaces  =   0;
    int                     verbose     =   0;
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
        else if(0 == wcscmp(L"--verbose", arg))
        {
            verbose = 1;
        }
        else
        {
            put_sequence(stderr, WOAD_FG_RED_FOR(stderr));
            fwprintf(stderr, L"%s: unrecognised argument '%s'; use --help for usage", bn, arg);
            put_sequence(stderr, WOAD_RESET_FOR(stderr));
            fputwc(L'\n', stderr);

            return EXIT_FAILURE;
        }
    }

    if (verbose)
    {
        diagnosticism_trace(stderr, "loading volumes ...");
    }

    if (0 == SSWinDiskUtil_LoadVolumes(NULL, 0, &volumes))
    {
        LONG const le = GetLastError();

        put_sequence(stderr, WOAD_FG_RED_FOR(stderr));
        fwprintf(stderr, L"%s: failed to load volumes: %d", bn, le);
        put_sequence(stderr, WOAD_RESET_FOR(stderr));
        fputwc(L'\n', stderr);

        return EXIT_FAILURE;
    }
    else
    {
        size_t j;

        if (verbose)
        {
            diagnosticism_trace(stderr, "obtained information for %lu volume(s)", (unsigned long)volumes->numVolumes);
        }

        fwprintf(stdout, L"%I64u volume(s):\n", volumes->numVolumes);

        for (j = 0; volumes->numVolumes != j; ++j)
        {
            SSWinDiskUtil_VolumeDescriptor_t const* const volume = &volumes->volumes[j];

            fwprintf(stdout, L"%lu: %ls", (unsigned int)j, (showLabels || showSpaces) ? L"id=" : L"");

            put_sequence(stdout, WOAD_FG_CYAN_FOR(stdout));
            fwprintf(stdout, L"%.*s", (int)volume->id.len, volume->id.ptr);
            put_sequence(stdout, WOAD_RESET_FOR(stdout));

            if (showLabels)
            {
                fwprintf(stdout, L" label=\"%.*s\"", (int)volume->friendlyName.len, volume->friendlyName.ptr);
            }

            if (showSpaces)
            {
                fwprintf(stdout, L" free=%I64u capacity=%I64u", volume->callerFreeBytes, volume->capacityBytes);
            }

            fputwc(L'\n', stdout);
        }

        SSWinDiskUtil_ReleaseVolumes(NULL, volumes);

        return EXIT_SUCCESS;
    }
}

static
void
put_sequence(
    FILE*           stm
,   char const*     seq
)
{
    for (; '\0' != *seq; ++seq)
    {
        fputwc((wchar_t)(unsigned char)*seq, stm);
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
    fwprintf(stm, L"USAGE: %s { --help | [ --label ] [ --spaces ] [ --verbose ] }\n", bn);
}


/* ///////////////////////////// end of file //////////////////////////// */

