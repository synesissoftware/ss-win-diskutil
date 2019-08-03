/* /////////////////////////////////////////////////////////////////////////
 * File:    clcp.c
 *
 * Purpose: Synesis Software Disk Utility library, for Windows: create-
 *          library-copy-program
 *
 * Created: 2nd August 2019
 * Updated: 3rd August 2019
 *
 * Home:    http://github.com/synesissoftware/ss-win-diskutil
 *
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


/* /////////////////////////////////////////////////////////////////////////
 * compatibility
 */

#ifndef _MSC_VER
# error Visual C++ required
#else

# pragma warning(disable : 4996)
#endif

/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* /////////////////////////////////////////////////////////////////////////
 * helper function declarations
 */

static
char const*
basename(
    char const* path
);

static
int
find_ver_major(
    char const* versionFile
,   char const* versionSymbol
);

/* /////////////////////////////////////////////////////////////////////////
 * main()
 */

int main(int argc, char* argv[])
{
    char const* const bn = basename(argv[0]);

    switch (argc)
    {
    default:

        fprintf(stderr, "USAGE: %s <target-path> <target-lib-dir> <version-file> <ver-major-symbol> <platform> <configuration> <project-name> <project-subname>\n", bn);

        return EXIT_FAILURE;
    case 8:
    case 9:
        {
            char const* const   targetPath      =   argv[1];
            char const* const   targetLibDir    =   argv[2];
            char const* const   versionFile     =   argv[3];
            char const* const   versionSymbol   =   argv[4];
            char const*         platform        =   argv[5];
            char const*         configuration   =   argv[6];

            char const* const   projectName     =   argv[7];
            char const*         projectSubname  =   argv[8];

            int const           ver_major       =   find_ver_major(versionFile, versionSymbol);

            if (ver_major < 0)
            {
                fprintf(stderr, "%s: could not locate symbol '%s' in version file '%s'\n", bn, versionSymbol, versionFile);

                return EXIT_FAILURE;
            }
            else
            {
                char        destPath[1 + _MAX_PATH];
                char        destName[1 + _MAX_PATH];
                char        fragVerMajor[101];
                int         compilerVer;
                char        fragCompilerVer[101];
                char        fragProjSubname[101];
                char const* config;
                char const* debug;

                /* library name format
                 *
                 * "<project-name>.<ver-major>[.<project-subname>].<compiler-version>[.<platform>][.<configuration>][.<debug>].lib"
                 */


                /* ver-major */

                sprintf(fragVerMajor, ".%d", ver_major);


                /* project-subname */

                if ('\0' != projectSubname[0])
                {
                    sprintf(fragProjSubname, ".%s", projectSubname);

                    projectSubname = fragProjSubname;
                }


                /* compiler-version */

                switch (_MSC_VER)
                {
                case 1200:
                case 1300:
                case 1400:
                case 1500:
                case 1600:
                case 1700:
                case 1800:

                    compilerVer = (_MSC_VER / 100) - 6;
                    break;
                case 1310:

                    compilerVer = 71;
                    break;
                default:

                    switch (_MSC_VER - (_MSC_VER % 100))
                    {
                    case 1900:
                        compilerVer = 14;
                        break;
                    default:

                        fprintf(stderr, "Not compatible with version of Visual C++ (_MSC_VER = %d)\n", _MSC_VER);

                        return EXIT_FAILURE;
                    }
                    break;
                }

                sprintf(fragCompilerVer, ".vc%d", compilerVer);


                /* platform */

                if (0 == stricmp("WIN32", platform))
                {
                    platform = "";
                }
                else if (0 == stricmp("x64", platform))
                {
                    platform = ".x64";
                }
                else
                {
                    fprintf(stderr, "Unsupported platform '%s'\n", platform);

                    return EXIT_FAILURE;
                }


                /* configuration */

                if (NULL != strstr(configuration, "DLL") || NULL != strstr(configuration, "Dll"))
                {
                    config = ".dll";
                }
                else if (NULL != strstr(configuration, "Multithreaded"))
                {
                    config = ".mt";
                }
                else
                {
                    config = "";
                }



                /* debug */

                if (NULL != strstr(configuration, "Debug"))
                {
                    debug = ".debug";
                }
                else
                {
                    debug = "";
                }


                sprintf(destName, "%s%s%s%s%s%s%s.lib", projectName, fragVerMajor, projectSubname, fragCompilerVer, platform, config, debug);

                strcpy(destPath, targetLibDir);
                strcat(destPath, "\\");
                strcat(destPath, destName);

                if (!CopyFileA(targetPath, destPath, FALSE))
                {
                    fprintf(stderr, "Could not copy '%s' to '%s'\n", targetPath, destPath);

                    return EXIT_FAILURE;
                }
                else
                {
                    return EXIT_SUCCESS;
                }
            }
        }
    }
}

/* /////////////////////////////////////////////////////////////////////////
 * helper function definitions
 */

static
int
find_ver_major(
    char const* versionFile
,   char const* versionSymbol
)
{
    FILE*   f = fopen(versionFile, "r");

    if (NULL != f)
    {
        char    line[1001];
        int     v = -1;

        for (; NULL != fgets(line, sizeof(line) / sizeof(0[line]), f); )
        {
            char const* vs;

            if (NULL != (vs = strstr(line, versionSymbol)))
            {
                vs += strlen(versionSymbol);

                for (; '\0' != *vs && isspace(*vs); ++vs)
                {}

                if (isdigit(*vs))
                {
                    return atoi(vs);
                }
            }
        }

        fclose(f);

        return v;
    }

    return -1;
}


static
char const*
basename(
    char const* path
)
{
    char const* const   fslash  =   strrchr(path, '/');
    char const* const   bslash  =   strrchr(path, '\\');

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

/* ///////////////////////////// end of file //////////////////////////// */

