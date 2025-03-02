# ss-win-diskutil <!-- omit in toc -->

**S**ynesis **S**oftware **Disk** **Util**ity library, for **Win**dows


## Introduction

**ss-win-diskutil** is a small, independent, C-language open-source library providing disk-related facilities, for Windows.

It has **no dependencies** on any other non-standard library.


## Table of Contents <!-- omit in toc -->

- [Introduction](#introduction)
- [Installation](#installation)
- [Components](#components)
- [Examples](#examples)
- [Project Information](#project-information)
  - [Where to get help](#where-to-get-help)
  - [Contribution guidelines](#contribution-guidelines)
  - [License](#license)


## Installation

No installation instructions are provided at this time.


## Components

```C
/** String slice type
 *
 * @note A string slice type such as this is not required to refer to a
 *   nul-terminated C-style string. Refer to the documentation of the
 *   particular construct to inform whether the use(s) involve
 *   nul-terminated strings
 */
struct SSWinDiskUtil_slice_w_t
{
    size_t                              len;    /*!< The length of the string */
    wchar_t const*                      ptr;    /*!< Pointer to the first character of the string */
};
#ifndef __cplusplus
typedef struct SSWinDiskUtil_slice_w_t                      SSWinDiskUtil_slice_w_t;
#endif /* !__cplusplus */

struct SSWinDiskUtil_VolumeDescriptor_t
{
    SSWinDiskUtil_slice_w_t             id;                 /*!< The volume identifier */
    SSWinDiskUtil_slice_w_t             friendlyName;       /*!< The volume friendly name */
    SSWinDiskUtil_slice_w_t             reserved0[5];
    uint64_t                            capacityBytes;      /*!< The volume capacity */
    uint64_t                            systemFreeBytes;    /*!< The volume free-space */
    uint64_t                            callerFreeBytes;    /*!< The volume free-space as available to the caller */
    uint64_t                            statusFlags;        /*!< Status flags */
};
#ifndef __cplusplus
typedef struct SSWinDiskUtil_VolumeDescriptor_t             SSWinDiskUtil_VolumeDescriptor_t;
#endif /* !__cplusplus */

/** Volume descriptors structure
 *
 */
struct SSWinDiskUtil_VolumeDescriptors_t
{
    uint64_t                            numVolumes;         /*!< The number of volumes */
    SSWinDiskUtil_VolumeDescriptor_t    volumes[1];         /*!< The array of volumes, of the size [numVolumes] */
};
#ifndef __cplusplus
typedef struct SSWinDiskUtil_VolumeDescriptors_t            SSWinDiskUtil_VolumeDescriptors_t;
#endif /* !__cplusplus */

/** Volume descriptions handle
 *
 */
typedef SSWinDiskUtil_VolumeDescriptors_t const*            SSWinDiskUtil_VolumeDescriptions_t;


/** Obtains the volumes information for the host
 *
 * @param reserved [in] (void*) Reserved. Must be \c nullptr
 * @param flags [in] (int64_t) Flags. Currently must be \c 0
 * @param pvolumes [out] Pointer to handle (of type
 *   \c SSWinDiskUtil_VolumeDescriptions_t, which is
 *   <code>SSWinDiskUtil_VolumeDescriptors_t const*</code), to receive the
 *   volumes information. Must NOT be \c nullptr
 *
 * @retval 0 The function failed. Use GetLastError() for further information
 * @retval >0 The number of volumes for which information is obtained
 */
int
SSWinDiskUtil_LoadVolumes(
    void*                                   reserved
,   int64_t                                 flags
,   SSWinDiskUtil_VolumeDescriptions_t*     pvolumes
);

/** Releases all resources associated with the volumes
 *
 * @param reserved [in] (void*) Reserved. Must be \c nullptr
 * @param volumes [in] The descriptions handle
 *
 * @retval 0
 */
int
SSWinDiskUtil_ReleaseVolumes(
    void*                                   reserved
,   SSWinDiskUtil_VolumeDescriptions_t      volumes
);
```


## Examples

```C
#include <ss-win-diskutil.h>

. . .

    SSWinDiskUtil_VolumeDescriptions_t  volumes;

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
```

which gives results such as

```
9 volume(s)
0: id=\\?\Volume{54ba6bcd-4986-11e4-9ae4-806e6f6e6963}\ label="System" free=10848309248 capacity=53686366208
1: id=\\?\Volume{48a35c3c-4934-11e4-94a7-001c427f1a4e}\ label="Dev" free=8254717952 capacity=17182609408
2: id=\\?\Volume{5e0afee6-49d8-11e4-b415-001c427f1a4e}\ label="Programs" free=44040654848 capacity=53694623744
3: id=\\?\Volume{6827c7e0-4b47-11e4-8a10-001c42459f86}\ label="spare" free=1069277184 capacity=1077477376
4: id=\\?\Volume{6827c7e4-4b47-11e4-8a10-001c42459f86}\ label="spare" free=1069277184 capacity=1077477376
5: id=\\?\Volume{6827c7e8-4b47-11e4-8a10-001c42459f86}\ label="spare" free=1069277184 capacity=1077477376
6: id=\\?\Volume{6827c7ec-4b47-11e4-8a10-001c42459f86}\ label="spare" free=1069277184 capacity=1077477376
7: id=\\?\Volume{8c7371b8-62df-11e4-967e-001c42459f86}\ label="unused" free=8516808704 capacity=8562483200
8: id=\\?\Volume{54ba6bca-4986-11e4-9ae4-806e6f6e6963}\ label="" free=8516808704 capacity=8562483200
```


## Project Information


### Where to get help

[GitHub Page](https://github.com/synesissoftware/ss-win-diskutil "GitHub Page")


### Contribution guidelines

Defect reports, feature requests, and pull requests are welcome on https://github.com/synesissoftware/ss-win-diskutil.


### License

**ss-win-diskutil** is released under the 3-clause BSD license. See [LICENSE](./LICENSE) for details.


<!-- ########################### end of file ########################### -->

