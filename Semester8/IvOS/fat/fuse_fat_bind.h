#ifndef FUSE_FAT_BIND_H
#define FUSE_FAT_BIND_H

#define FUSE_USE_VERSION 31

#include <fuse.h>

#include "fat.h"

extern FILE *data;
extern FatFileSystem fs;

int bind_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                 off_t offset, struct fuse_file_info *fi,
                 enum fuse_readdir_flags flags);

int bind_getattr(const char *path, struct stat *stbuf,
                 struct fuse_file_info *fi);

int bind_open(const char *path, struct fuse_file_info *fi);

int bind_read(const char *path, char *buf, size_t size, off_t offset,
              struct fuse_file_info *fi);

int bind_write(const char *path, const char *buf, size_t size, off_t offset,
               struct fuse_file_info *fi);

int bind_unlink(const char *path);

int bind_create(const char *path, mode_t mode, struct fuse_file_info *fi);

#endif
