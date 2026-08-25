/*
 * SPDX-FileCopyrightText: 2026 Andrew Gunnerson
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "erofs/linux_compat.h"

#include <errno.h>
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

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

#ifdef WIN32
int fsync(int fd)
{
    HANDLE handle = (HANDLE) _get_osfhandle(fd);\
    if (handle == INVALID_HANDLE_VALUE) {
        errno = EBADF;
        return -1;
    }

    if (!FlushFileBuffers(handle)) {
        switch (GetLastError()) {
        case ERROR_INVALID_HANDLE:
            errno = EINVAL;
            break;
        default:
            errno = EIO;
            break;
        }

        return -1;
    }

    return 0;
}

int lstat(const char *pathname, struct stat *statbuf)
{
    return stat(pathname, statbuf);
}

char * realpath(const char *path, char *resolved_path)
{
    if (!resolved_path) {
        return strdup(path);
    }

    size_t n = strlen(path);
    if (n >= PATH_MAX) {
        errno = ENAMETOOLONG;
        return NULL;
    }

    strcpy(resolved_path, path);
    return resolved_path;
}

ssize_t readlink(const char *pathname, char *buf, size_t bufsiz)
{
    errno = EINVAL;
    return -1;
}

#ifdef EROFS_MT_ENABLED
#error pread/pwrite implementations are single threaded
#endif

ssize_t pread(int fd, void *buf, size_t len, off_t offset)
{
    off_t old_pos = lseek(fd, offset, SEEK_SET);
    if (old_pos < 0) {
        return -1;
    }

    ssize_t ret = read(fd, buf, len);
    int saved_errno = errno;

    if (lseek(fd, old_pos, SEEK_SET) < 0) {
        if (ret < 0) {
            errno = saved_errno;
        }
        return -1;
    }

    return ret;
}

ssize_t pwrite(int fd, const void *buf, size_t len, off_t offset)
{
    off_t old_pos = lseek(fd, offset, SEEK_SET);
    if (old_pos < 0) {
        return -1;
    }

    ssize_t ret = write(fd, buf, len);
    int saved_errno = errno;

    if (lseek(fd, old_pos, SEEK_SET) < 0) {
        if (ret < 0) {
            errno = saved_errno;
        }
        return -1;
    }

    return ret;
}

int getpagesize(void)
{
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    return si.dwPageSize;
}
#endif
