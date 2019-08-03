# ss-win-diskutil
**S**ynesis **S**oftware **Disk** **Util**ity library, for **Win**dows

## Introduction

**ss-win-diskutil** is a small, independent, C-language open-source library providing disk-related facilities, for Windows.

It has **no dependencies** on any other non-standard library.

## Table of Contents

1. [Introduction](#introduction)
2. [Installation](#installation)
3. [Components](#components)
4. [Project Information](#project-information)

## Installation

T.B.C.

## Components

T.B.C.

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

## Project Information

### Where to get help

[GitHub Page](https://github.com/synesissoftware/ss-win-diskutil "GitHub Page")

### Contribution guidelines

Defect reports, feature requests, and pull requests are welcome on https://github.com/synesissoftware/ss-win-diskutil.

### Related projects

T.B.C.

### License

**ss-win-diskutil** is released under the 3-clause BSD license. See [LICENSE](./LICENSE) for details.

