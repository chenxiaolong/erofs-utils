/*
 * SPDX-FileCopyrightText: 2026 Andrew Gunnerson
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "erofs/linux_compat.h"

#include <sys/stat.h>

unsigned short stat_mode_to_linux_mode(mode_t mode)
{
    unsigned short erofs_mode = mode & ~S_IFMT;

    switch (mode & S_IFMT) {
#ifdef S_IFSOCK
    case S_IFSOCK: erofs_mode |= LINUX_S_IFSOCK; break;
#endif
#ifdef S_IFLNK
    case S_IFLNK:  erofs_mode |= LINUX_S_IFLNK;  break;
#endif
    case S_IFREG:  erofs_mode |= LINUX_S_IFREG;  break;
    case S_IFBLK:  erofs_mode |= LINUX_S_IFBLK;  break;
    case S_IFDIR:  erofs_mode |= LINUX_S_IFDIR;  break;
    case S_IFCHR:  erofs_mode |= LINUX_S_IFCHR;  break;
    case S_IFIFO:  erofs_mode |= LINUX_S_IFIFO;  break;
    }

    return erofs_mode;
}
