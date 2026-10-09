/* /////////////////////////////////////////////////////////////////////////
 * File:    test/scratch/versions/main.c
 *
 * Purpose: Prints ss-win-diskutil composite version.
 *
 * Created: 17th September 2026
 * Updated: 17th September 2026
 *
 * ////////////////////////////////////////////////////////////////////// */


#include <ss-win-diskutil/version.h>

#include <stdio.h>
#include <stdlib.h>


#define PROGRAM_NAME                                        "versions"


static void
version(
    FILE*       stm
,   char const* prefix
,   char const* libname
,   char const* macroname
,   unsigned    libver
)
{
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
    ((void)&argc);
    ((void)&argv);

    {
        unsigned const libver = (unsigned)SS_WIN_DISKUTIL_VER;

        version(stdout, "", "ss-win-diskutil", "SS_WIN_DISKUTIL_VER", libver);
    }

    return EXIT_SUCCESS;
}


/* ///////////////////////////// end of file //////////////////////////// */
