/* /////////////////////////////////////////////////////////////////////////
 * File:    test/scratch/versions/main.c
 *
 * Purpose: Prints ss-win-diskutil composite version and efferent dependencies.
 *
 * Created: 17th September 2026
 * Updated: 27th October 2026
 *
 * ////////////////////////////////////////////////////////////////////// */


#include <ss-win-diskutil/version.h>

#ifdef HAS_Diagnosticism
# include <diagnosticism/diagnosticism.h>
# include <diagnosticism/version_string.h>
#endif

#ifdef HAS_woad
# include <woad/woad.h>
#endif

#include <stdio.h>
#include <stdlib.h>


static void
version(
    FILE*       stm
,   char const* prefix
,   char const* libname
,   char const* macroname
,   unsigned    libver
)
{
#ifdef HAS_Diagnosticism

    char        buffer[101];
    size_t      n;
    int const   r = diagnosticism_calc_version_string(
        buffer
    ,   sizeof(buffer) / sizeof(buffer[0])
    ,   (int)((libver >> 24) & 0xff)
    ,   (int)((libver >> 16) & 0xff)
    ,   (int)((libver >> 8) & 0xff)
    ,   (int)((libver >> 0) & 0xff)
    ,   &n
    );

    if (0 == r)
    {
        buffer[n] = '\0';

        fprintf(
            stm
        ,   "%s%s: %s (%s = 0x%08x)\n"
        ,   prefix
        ,   libname
        ,   buffer
        ,   macroname
        ,   libver
        );

        return;
    }
#endif /* HAS_Diagnosticism */

    fprintf(
        stm
    ,   "%s%s: v%u.%u.%u.%u (%s = 0x%08x)\n"
    ,   prefix
    ,   libname
    ,   (libver >> 24) & 0xff
    ,   (libver >> 16) & 0xff
    ,   (libver >> 8) & 0xff
    ,   (libver >> 0) & 0xff
    ,   macroname
    ,   libver
    );
}


int main(int argc, char* argv[])
{
    size_t efferent_counts = 0;

    ((void)&argc);
    ((void)&argv);

    {
        unsigned const libver = (unsigned)SS_WIN_DISKUTIL_VER;

        version(stdout, "", "ss-win-diskutil", "SS_WIN_DISKUTIL_VER", libver);
    }

#ifdef HAS_Diagnosticism

    if (0 == efferent_counts++)
    {
        fputs("\nefferent dependencies:\n", stdout);
    }

    {
        unsigned const libver = (unsigned)DIAGNOSTICISM_VER;

        version(stdout, "\t", "Diagnosticism", "DIAGNOSTICISM_VER", libver);
    }
#endif

#ifdef HAS_woad

    if (0 == efferent_counts++)
    {
        fputs("\nefferent dependencies:\n", stdout);
    }

    fprintf(
        stdout
    ,   "\t%s: v%s (%s = 0x%08x)\n"
    ,   "woad"
    ,   woad_version_string()
    ,   "WOAD_VER"
    ,   (unsigned)WOAD_VER
    );
#endif

    return EXIT_SUCCESS;
}


/* ///////////////////////////// end of file //////////////////////////// */
