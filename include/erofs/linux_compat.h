/*
 * SPDX-FileCopyrightText: 2026 Andrew Gunnerson
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <fcntl.h>
#include <inttypes.h>
#include <sys/types.h>

#define LINUX_S_IFIFO  0010000
#define LINUX_S_IFCHR  0020000
#define LINUX_S_IFDIR  0040000
#define LINUX_S_IFBLK  0060000
#define LINUX_S_IFREG  0100000
#define LINUX_S_IFLNK  0120000
#define LINUX_S_IFSOCK 0140000
#define LINUX_S_IFMT   0170000

#define LINUX_S_ISFIFO(m) (((m) & LINUX_S_IFMT) == LINUX_S_IFIFO)
#define LINUX_S_ISCHR(m)  (((m) & LINUX_S_IFMT) == LINUX_S_IFCHR)
#define LINUX_S_ISDIR(m)  (((m) & LINUX_S_IFMT) == LINUX_S_IFDIR)
#define LINUX_S_ISBLK(m)  (((m) & LINUX_S_IFMT) == LINUX_S_IFBLK)
#define LINUX_S_ISREG(m)  (((m) & LINUX_S_IFMT) == LINUX_S_IFREG)
#define LINUX_S_ISLNK(m)  (((m) & LINUX_S_IFMT) == LINUX_S_IFLNK)
#define LINUX_S_ISSOCK(m) (((m) & LINUX_S_IFMT) == LINUX_S_IFSOCK)

#define LINUX_S_IXUSR 00100
#define LINUX_S_IWUSR 00200
#define LINUX_S_IRUSR 00400
#define LINUX_S_IRWXU 00700

#define LINUX_S_IXGRP 00010
#define LINUX_S_IWGRP 00020
#define LINUX_S_IRGRP 00040
#define LINUX_S_IRWXG 00070

#define LINUX_S_IXOTH 00001
#define LINUX_S_IWOTH 00002
#define LINUX_S_IROTH 00004
#define LINUX_S_IRWXO 00007

#ifdef _WIN32
#define stat_blksize(st) 512
#else
#define stat_blksize(st) st.st_blksize
#endif

// unsigned short instead of umode_t because we can't include erofs/internal.h.
unsigned short stat_mode_to_linux_mode(mode_t mode);

typedef uint64_t linux_dev_t;

unsigned int linux_major(linux_dev_t dev);

unsigned int linux_minor(linux_dev_t dev);

linux_dev_t linux_makedev(unsigned int maj, unsigned int min);

typedef uint64_t linux_ino_t;

#ifdef _WIN32
#define fchmod(fd, mode) 0

int fsync(int fd);

struct stat;

int lstat(const char *pathname, struct stat *statbuf);

char * realpath(const char *path, char *resolved_path);

ssize_t readlink(const char *pathname, char *buf, size_t bufsiz);

ssize_t pread(int fd, void *buf, size_t len, off_t offset);

ssize_t pwrite(int fd, const void *buf, size_t len, off_t offset);

int getpagesize(void);

char * strndup(const char *s, size_t n);
#endif
