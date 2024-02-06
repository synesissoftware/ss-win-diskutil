/* /////////////////////////////////////////////////////////////////////////
 * File:    ss-win-diskutil/implicit_link.h
 *
 * Purpose: Synesis Software Disk Utility library, for Windows: implicit
 *          link.
 *
 * Created: 2nd August 2019
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


#ifndef SS_WIN_DISKUTIL_h_implicit_link
#define SS_WIN_DISKUTIL_h_implicit_link

/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <ss-win-diskutil/version.h>


/* /////////////////////////////////////////////////////////////////////////
 * macros and definitions
 */

#ifndef SS_WIN_DISKUTIL_DOCUMENTATION_SKIP_SECTION
# define SS_WIN_DISKUTIL_STRINGIZE_(x)                      #x
# define SS_WIN_DISKUTIL_STRINGIZE(x)                       SS_WIN_DISKUTIL_STRINGIZE_(x)
#endif /* !SS_WIN_DISKUTIL_DOCUMENTATION_SKIP_SECTION */


/* /////////////////////////////////////////////////////////////////////////
 * architecture discrimination
 */

#if defined(__amd64__) || \
    defined(__amd64) || \
    defined(_AMD64_) || \
    defined(_M_AMD64) || \
    defined(_M_X64)
# define SS_WIN_DISKUTIL_ARCH_IS_X64
#elif defined(__ia64__) || \
      defined(__ia64) || \
      defined(_IA64_) || \
      defined(_M_IA64)
# define SS_WIN_DISKUTIL_ARCH_IS_IA64
#elif defined(__i386__) || \
      defined(__i386) || \
      defined(_X86_) || \
      defined(_M_IX86)
# define SS_WIN_DISKUTIL_ARCH_IS_X86
#endif /* _M_?? */


/* /////////////////////////////////////////////////////////////////////////
 * implicit linking
 */

#if defined(_WIN32) || \
    defined(_WIN64)

 /* architecture */
# if !defined(SS_WIN_DISKUTIL_ARCH_IS_X86) && \
     !defined(SS_WIN_DISKUTIL_ARCH_IS_X64) && \
     !defined(SS_WIN_DISKUTIL_ARCH_IS_IA64)

  /* see if using PlatformSTL */
#  ifdef PLATFORMSTL_OS_IS_WINDOWS

   /* prefer PlatformSTL discrimination */
#   if defined(SS_WIN_DISKUTIL_ARCH_IS_X86)
#    define SS_WIN_DISKUTIL_ARCH_IS_X86
#   elif defined(SS_WIN_DISKUTIL_ARCH_IS_X64)
#    define SS_WIN_DISKUTIL_ARCH_IS_X64
#   elif defined(SS_WIN_DISKUTIL_ARCH_IS_IA64)
#    define SS_WIN_DISKUTIL_ARCH_IS_IA64
#   endif

#  else /* ? PLATFORMSTL_OS_IS_WINDOWS */

   /* custom discrimination */

#   if defined(_M_IA64)
#    define SS_WIN_DISKUTIL_ARCH_IS_IA64
#   elif defined(_M_X64) || \
         defined(_M_AMD64)
#    define SS_WIN_DISKUTIL_ARCH_IS_X64
#   elif defined(_M_IX86)
#    define SS_WIN_DISKUTIL_ARCH_IS_X86
#   endif /* arch */

#  endif /* PLATFORMSTL_OS_IS_WINDOWS */
# endif /* !SS_WIN_DISKUTIL_ARCH_IS_???? */


# if defined(__BORLANDC__) || \
     /* defined(__DMC__) || */ \
     defined(__INTEL_COMPILER) || \
     defined(__MWERKS__) || \
     defined(_MSC_VER)
#  if !defined(__COMO__)
#   define SS_WIN_DISKUTIL_IMPLICIT_LINK_SUPPORT
#  endif /* compiler */
# endif /* compiler */

# if defined(SS_WIN_DISKUTIL_IMPLICIT_LINK_SUPPORT) && \
     defined(SS_WIN_DISKUTIL_NO_IMPLICIT_LINK)
#  undef SS_WIN_DISKUTIL_IMPLICIT_LINK_SUPPORT
# endif /* SS_WIN_DISKUTIL_IMPLICIT_LINK_SUPPORT && SS_WIN_DISKUTIL_NO_IMPLICIT_LINK */

# if defined(SS_WIN_DISKUTIL_IMPLICIT_LINK_SUPPORT)

  /* prefix */

#  define SS_WIN_DISKUTIL_IMPL_LINK_PREFIX

  /* library basename */

#  define SS_WIN_DISKUTIL_IMPL_LINK_LIBRARY_BASENAME        "ss-win-diskutil"

  /* major version */

#  define SS_WIN_DISKUTIL_IMPL_LINK_MAJOR_VERSION           "." SS_WIN_DISKUTIL_STRINGIZE(SS_WIN_DISKUTIL_VER_MAJOR)

  /* library module */

#  define SS_WIN_DISKUTIL_IMPL_LINK_LIBRARY_MODULE          "core"

  /* module name */

#  define SS_WIN_DISKUTIL_IMPL_LINK_MODULE_NAME             ""

  /* compiler tag */

#  if defined(__BORLANDC__)
#   if __BORLANDC__ == 0x0550
#    define SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME         "bc55"
#   elif (__BORLANDC__ == 0x0551)
#    define SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME         "bc551"
#   elif (__BORLANDC__ == 0x0560)
#    define SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME         "bc56"
#   elif (__BORLANDC__ == 0x0564)
#    define SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME         "bc564"
#   elif (__BORLANDC__ == 0x0582)
#    define SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME         "bc582"
#   elif (0x0590 == (__BORLANDC__ & 0xfff0))
#    define SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME         "bc59x"
#   elif (0x0610 == (__BORLANDC__ & 0xfff0))
#    define SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME         "bc61x"
#   else /* ? __BORLANDC__ */
#    error Unrecognised value of __BORLANDC__
#   endif /* __BORLANDC__ */

/*
#  elif defined(__DMC__)
#   define SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME          "dm"
 */

#  elif defined(__INTEL_COMPILER)
#   if (__INTEL_COMPILER == 600)
#    define   SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME       "icl6"
#   elif (__INTEL_COMPILER == 700)
#    define   SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME       "icl7"
#   elif (__INTEL_COMPILER == 800)
#    define   SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME       "icl8"
#   elif (__INTEL_COMPILER == 900)
#    define   SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME       "icl9"
#   elif (__INTEL_COMPILER == 1000)
#    define   SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME       "icl10"
#   elif (__INTEL_COMPILER == 1100)
#    define   SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME       "icl11"
#   else /* ? __INTEL_COMPILER */
#    error Intel C/C++ version not supported
#   endif /* __INTEL_COMPILER */

#  elif defined(__MWERKS__)
#   if ((__MWERKS__ & 0xFF00) == 0x2400)
#    define SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME         "cw7"
#   elif ((__MWERKS__ & 0xFF00) == 0x3000)
#    define SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME         "cw8"
#   elif ((__MWERKS__ & 0xFF00) == 0x3200)
#    define SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME         "cw9"
#   else /* ? __MWERKS__ */
#    error CodeWarrior version not supported
#   endif /* __MWERKS__ */

#  elif defined(_MSC_VER)
#   if _MSC_VER == 1000
#    define SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME         "vc4"
#   elif _MSC_VER == 1020
#    define SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME         "vc42"
#   elif _MSC_VER == 1100
#    define SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME         "vc5"
#   elif _MSC_VER == 1200
#    define SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME         "vc6"
#   elif _MSC_VER == 1300
#    define SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME         "vc7"
#   elif _MSC_VER == 1310
#    define SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME         "vc71"
#   elif _MSC_VER == 1400
#    define SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME         "vc8"
#   elif _MSC_VER == 1500
#    define SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME         "vc9"
#   elif _MSC_VER == 1600
#    define SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME         "vc10"
#   elif _MSC_VER == 1700
#    define SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME         "vc11"
#   elif _MSC_VER == 1800
#    define SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME         "vc12"
#   elif _MSC_VER == 1900
#    define SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME         "vc14"
#   elif _MSC_VER >= 1910
#    if 0
#    elif _MSC_VER < 1920
#     define SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME        "vc15"
#    elif _MSC_VER <= 1929
#     define SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME        "vc16"
#    else
#     error Visual C++ version that is >= vc16 is not recognised
#    endif
#   else /* ? _MSC_VER */
#    error Visual C++ version not supported
#   endif /* _MSC_VER */

#  else /* ? compiler */
#   error Unrecognised compiler
#  endif /* compiler */


  /* operating system tag */

#  if !defined(SS_WIN_DISKUTIL_IMPL_LINK_OS_TAG)
#   define SS_WIN_DISKUTIL_IMPL_LINK_OS_TAG                 ""
#  endif /* !SS_WIN_DISKUTIL_IMPL_LINK_OS_TAG */


  /* architecture tag */

#  if defined(SS_WIN_DISKUTIL_ARCH_IS_X86)
#   define SS_WIN_DISKUTIL_IMPL_LINK_ARCH_TAG               ""
#  elif defined(SS_WIN_DISKUTIL_ARCH_IS_X64)
#   define SS_WIN_DISKUTIL_IMPL_LINK_ARCH_TAG               ".x64"
#  elif defined(SS_WIN_DISKUTIL_ARCH_IS_IA64)
#   define SS_WIN_DISKUTIL_IMPL_LINK_ARCH_TAG               ".ia64"
#  endif /* arch */

#  if !defined(SS_WIN_DISKUTIL_IMPL_LINK_ARCH_TAG)
#   define SS_WIN_DISKUTIL_IMPL_LINK_ARCH_TAG               ""
#  endif /* !SS_WIN_DISKUTIL_IMPL_LINK_ARCH_TAG */


  /* encoding tag */

#  if defined(SS_WIN_DISKUTIL_CHAR_TYPE_IS_WCHAR)
#   define SS_WIN_DISKUTIL_IMPL_LINK_ENCODING_TAG           ".widestring"
#  else /* ? SS_WIN_DISKUTIL_CHAR_TYPE_IS_WCHAR */
#   define SS_WIN_DISKUTIL_IMPL_LINK_ENCODING_TAG           ""
#  endif /* SS_WIN_DISKUTIL_CHAR_TYPE_IS_WCHAR */


  /* threading tag */

#  if defined(__MT__) || \
      defined(_REENTRANT) || \
      defined(_MT)
#   if defined(_DLL) || \
       defined(__DLL)
#    define SS_WIN_DISKUTIL_IMPL_LINK_THREADING_TAG         ".dll"
#   else /* ? dll */
#    define SS_WIN_DISKUTIL_IMPL_LINK_THREADING_TAG         ".mt"
#   endif /* dll */
#  else /* ? mt */
#    define SS_WIN_DISKUTIL_IMPL_LINK_THREADING_TAG         ""
#  endif /* mt */


  /* debug tag */

#  if !defined(NDEBUG) && \
      defined(_DEBUG)
#   define SS_WIN_DISKUTIL_IMPL_LINK_DEBUG_TAG              ".debug"
#  else /* ? debug */
#   define SS_WIN_DISKUTIL_IMPL_LINK_DEBUG_TAG              ""
#  endif /* debug */


  /* suffix */

#  define SS_WIN_DISKUTIL_IMPL_LINK_SUFFIX                  ".lib"


   /* Library name is:
    *
    * [lib]<library-basename>.<major-version>.<module-name>.<compiler-name>[.<os-arch-tag>][.<char-encoding-tag>][.<threading-tag>][.<nox-tag>][.<debug-tag>].{a|lib}
    */

#  define SS_WIN_DISKUTIL_IMPL_LINK_LIBRARY_NAME            SS_WIN_DISKUTIL_IMPL_LINK_PREFIX \
                                                            SS_WIN_DISKUTIL_IMPL_LINK_LIBRARY_BASENAME \
                                                            SS_WIN_DISKUTIL_IMPL_LINK_MAJOR_VERSION \
                                                            "." SS_WIN_DISKUTIL_IMPL_LINK_LIBRARY_MODULE \
                                                            SS_WIN_DISKUTIL_IMPL_LINK_MODULE_NAME \
                                                            "." SS_WIN_DISKUTIL_IMPL_LINK_COMPILER_NAME \
                                                            SS_WIN_DISKUTIL_IMPL_LINK_OS_TAG \
                                                            SS_WIN_DISKUTIL_IMPL_LINK_ARCH_TAG \
                                                            SS_WIN_DISKUTIL_IMPL_LINK_ENCODING_TAG \
                                                            SS_WIN_DISKUTIL_IMPL_LINK_THREADING_TAG \
                                                            SS_WIN_DISKUTIL_IMPL_LINK_DEBUG_TAG \
                                                            SS_WIN_DISKUTIL_IMPL_LINK_SUFFIX


#  pragma message("lib: " SS_WIN_DISKUTIL_IMPL_LINK_LIBRARY_NAME)

#  pragma comment(lib, SS_WIN_DISKUTIL_IMPL_LINK_LIBRARY_NAME)

# endif /* SS_WIN_DISKUTIL_IMPLICIT_LINK_SUPPORT */

#endif /* Win-32 || Win-64 */


/* /////////////////////////////////////////////////////////////////////////
 * inclusion control
 */

#ifdef STLSOFT_CF_PRAGMA_ONCE_SUPPORT
# pragma once
#endif /* STLSOFT_CF_PRAGMA_ONCE_SUPPORT */

#endif /* !SS_WIN_DISKUTIL_h_implicit_link */

/* ///////////////////////////// end of file //////////////////////////// */

