#include "fuse_fat_bind.h"
#include <errno.h>
#include <string.h>
#include <stdio.h>

const char *cd_dir(const char *path, int last_dir)
{
    fs.pwd_cluster = 0;
    fs.current_file_index = -1;

    printf("[LOG] Changing directory to '%s'\n", path);

    if (strcmp(path, "/") == 0)
    {
        return path + 1;
    }

    int stop = 0;
    while (stop == 0)
    {
        const char *slash = strchr(path + 1, '/');
        if (!slash)
        {
            slash = path + strlen(path);
            stop = 1;
            if (last_dir)
            {
                break;
            }
        }

        char part[256];
        size_t len = slash - path - 1;
        if (len >= sizeof(part))
            return 0;
        strncpy(part, path + 1, len);
        part[len] = '\0';

        if (!fat_change_dir(&fs, part))
        {
            return 0;
        }

        path = slash;
    }

    printf("[LOG] Changed directory to '%s', pwd_cluster = %d\n", path, fs.pwd_cluster);

    // return ptr to filename
    return path + 1;
}

int bind_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                 off_t offset, struct fuse_file_info *fi,
                 enum fuse_readdir_flags flags)
{

    (void)offset;
    (void)fi;
    (void)flags;

    printf("[LOG] Reading directory '%s'\n", path);

    path = cd_dir(path, 0);
    if (path == 0)
    {
        return -ENOENT;
    }

    printf("[LOG] Reading directory '%s', pwd_cluster = %d\n", path, fs.pwd_cluster);

    while (true)
    {
        fat_read_directory_entry(&fs, fs.pwd_cluster);
        Fat16Entry sub_entry = fs.current_file;

        if (sub_entry.filename[0] == 0x00)
            break; // End of directory
        if (sub_entry.filename[0] == 0xE5)
            continue; // Deleted

        // skip fat name
        if (sub_entry.attributes & 0x08)
            continue;

        char name[13];
        int ptr = 0;
        int i = 0;
        while (sub_entry.filename[i] != ' ' && i < 8)
        {
            name[ptr++] = sub_entry.filename[i];
            i++;
        }

        if (!(sub_entry.attributes & 0x10))
        {
            i = 0;

            name[ptr++] = '.';

            while (sub_entry.ext[i] != ' ' && i < 3)
            {
                name[ptr++] = sub_entry.ext[i];
                i++;
            }
        }

        name[ptr] = '\0';

        filler(buf, name, NULL, 0, FUSE_FILL_DIR_DEFAULTS);
    }
    return 0;
}

time_t fat_16_entry_date_to_timestamp(Fat16Entry entry)
{
    struct tm t;
    t.tm_sec = 0;
    t.tm_min = 0;
    t.tm_hour = 0;
    t.tm_mday = entry.modify_date & 0x1F;
    t.tm_mon = ((entry.modify_date >> 5) & 0x0F) - 1;
    t.tm_year = ((entry.modify_date >> 9) & 0x7F) + 1980 - 1900;

    return mktime(&t);
}

int open_file(const char *path)
{

    // split path into name and ext by . into two ptrs ends with \0
    char name[9] = {0};
    char ext[4] = {0};
    const char *dot = strchr(path, '.');
    if (dot)
    {
        int name_len = dot - path;
        if (name_len > 8)
            name_len = 8;
        strncpy(name, path, name_len);
        strncpy(ext, dot + 1, 3);
    }
    else
    {
        strncpy(name, path, 8);
    }

    if (!fat_find_file(&fs, name, ext, &fs.current_file))
    {
        return -ENOENT;
    }

    return 0;
}

int bind_getattr(const char *path, struct stat *stbuf,
                 struct fuse_file_info *fi)
{
    if (strcmp(path, "/") == 0)
    {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        stbuf->st_size = 4096; // typical size for directories
        return 0;
    }

    (void)fi;
    int res = 0;

    memset(stbuf, 0, sizeof(struct stat));

    printf("[LOG] Getting attributes for '%s'\n", path);

    path = cd_dir(path, 1);

    if (path == 0)
    {
        return -ENOENT;
    }

    printf("[LOG] Trying to open file '%s' for getattr\n", path);

    if (open_file(path) != 0)
    {
        return -ENOENT;
    }

    printf("[LOG] Found file '%s' for getattr, attributes 0x%02X\n", path, fs.current_file.attributes);

    if (fs.current_file.attributes & 0x10)
    {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        stbuf->st_size = 4096; // typical size for directories
    }
    else
    {
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = fs.current_file.file_size;
        stbuf->st_mtime = fat_16_entry_date_to_timestamp(fs.current_file);
    }

    return res;
}

int bind_open(const char *path, struct fuse_file_info *fi)
{
    path = cd_dir(path, 1);

    if (path == 0)
    {
        return -ENOENT;
    }

    return open_file(path);
}

int bind_read(const char *path, char *buf, size_t size, off_t offset,
              struct fuse_file_info *fi)
{
    size_t len;
    (void)fi;

    printf("[LOG] Reading file '%s' with size %zu and offset %zu\n", path, size, offset);

    path = cd_dir(path, 1);
    if (path == 0)
    {
        return -ENOENT;
    }

    if (open_file(path) != 0)
    {
        return -ENOENT;
    }

    printf("[LOG] Reading file '%s' size %d bytes\n", path, fs.current_file.file_size);

    char FILE_BUFFER[4 * 1024 * 1024];

    FILE *stream = fmemopen(FILE_BUFFER, sizeof(FILE_BUFFER), "w");
    fat_read_file(&fs, &fs.current_file, stream);
    fclose(stream);

    printf("[LOG] FILE_CONTENT: %.*s\n", (int)fs.current_file.file_size, FILE_BUFFER);

    len = fs.current_file.file_size;
    if (offset < len)
    {
        if (offset + size > len)
            size = len - offset;
        memcpy(buf, FILE_BUFFER + offset, size);
    }
    else
        size = 0;

    return size;
}
