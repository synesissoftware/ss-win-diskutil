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

### Related projects

T.B.C.

### License

**ss-win-diskutil** is released under the 3-clause BSD license. See [LICENSE](./LICENSE) for details.

