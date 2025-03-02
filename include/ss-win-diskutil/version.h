/* /////////////////////////////////////////////////////////////////////////
 * File:    ss-win-diskutil/version.h
 *
 * Purpose: Synesis Software Disk Utility library, for Windows: version.
 *
 * Created: 2nd August 2019
 * Updated: 2nd February 2025
 *
 * Home:    http://github.com/synesissoftware/ss-win-diskutil
 *
 * Copyright (c) 2019-2025, Matthew Wilson and Synesis Information Systems
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


#ifndef SS_WIN_DISKUTIL_h_version
#define SS_WIN_DISKUTIL_h_version


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */


/* /////////////////////////////////////////////////////////////////////////
 * version
 */

/** \def SS_WIN_DISKUTIL_VER_MAJOR
 * The major version number of cstring
 */

/** \def SS_WIN_DISKUTIL_VER_MINOR
 * The minor version number of cstring
 */

/** \def SS_WIN_DISKUTIL_VER_REVISION
 * The revision version number of cstring
 */

/** \def SS_WIN_DISKUTIL_VER
 * The current composite version number of cstring
 */

#ifndef SS_WIN_DISKUTIL_DOCUMENTATION_SKIP_SECTION

# define SS_WIN_DISKUTIL_VER_0_2_1          0x000201ff
# define SS_WIN_DISKUTIL_VER_0_2_2          0x000202ff
# define SS_WIN_DISKUTIL_VER_0_2_3          0x000203ff
# define SS_WIN_DISKUTIL_VER_0_2_4_ALPHA1   0x00020441
# define SS_WIN_DISKUTIL_VER_0_2_4_ALPHA2   0x00020442
# define SS_WIN_DISKUTIL_VER_0_2_4_ALPHA3   0x00020443

# define SS_WIN_DISKUTIL_VER                SS_WIN_DISKUTIL_VER_0_2_4_ALPHA3
#else /* ? SS_WIN_DISKUTIL_DOCUMENTATION_SKIP_SECTION */

# define SS_WIN_DISKUTIL_VER                0x00020442
#endif /* !SS_WIN_DISKUTIL_DOCUMENTATION_SKIP_SECTION */

#define SS_WIN_DISKUTIL_VER_MAJOR       0
#define SS_WIN_DISKUTIL_VER_MINOR       2
#define SS_WIN_DISKUTIL_VER_REVISION    4


/* /////////////////////////////////////////////////////////////////////////
 * inclusion control
 */

#ifdef STLSOFT_CF_PRAGMA_ONCE_SUPPORT
# pragma once
#endif /* STLSOFT_CF_PRAGMA_ONCE_SUPPORT */

#endif /* !SS_WIN_DISKUTIL_h_version */

/* ///////////////////////////// end of file //////////////////////////// */

