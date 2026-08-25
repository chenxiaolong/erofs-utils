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

unsigned int linux_major(linux_dev_t dev)
{
    return ((dev >> 32) & 0xfffff000) | ((dev >> 8) & 0xfff);
}

unsigned int linux_minor(linux_dev_t dev)
{
    return ((dev >> 12) & 0xffffff00) | (dev & 0xff);
}

linux_dev_t linux_makedev(unsigned int maj, unsigned int min)
{
    return ((linux_dev_t) (maj & 0xfffff000UL) << 32)
        | ((linux_dev_t) (maj & 0xfffUL) << 8)
        | ((linux_dev_t) (min & 0xffffff00UL) << 12)
        | ((linux_dev_t) min & 0xffUL);
}
