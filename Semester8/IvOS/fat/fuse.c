/*
  FUSE: Filesystem in Userspace
  Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

  This program can be distributed under the terms of the GNU GPLv2.
  See the file COPYING.
*/

#define FUSE_USE_VERSION 31

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <assert.h>
#include <fcntl.h>

#include "fuse_fat_bind.h"

FILE *data;
FatFileSystem fs;

/*
 * Command line options
 *
 * We can't set default values for the char* fields here because
 * fuse_opt_parse would attempt to free() them when the user specifies
 * different values on the command line.
 */
static struct options
{
    const char *filename;
    const char *contents;
    int show_help;
} options;

#define OPTION(t, p) \
    {t, offsetof(struct options, p), 1}
static const struct fuse_opt option_spec[] = {
    OPTION("--name=%s", filename),
    OPTION("--contents=%s", contents),
    OPTION("-h", show_help),
    OPTION("--help", show_help),
    FUSE_OPT_END};

static void *hello_init(struct fuse_conn_info *conn,
                        struct fuse_config *cfg)
{
    (void)conn;
    cfg->kernel_cache = 1;
    return NULL;
}

static const struct fuse_operations hello_oper = {
    .init = hello_init,
    .getattr = bind_getattr,
    .readdir = bind_readdir,
    .open = bind_open,
    .read = bind_read,
};

static void show_help(const char *progname)
{
    printf("usage: %s [options] <mountpoint>\n\n", progname);
    printf("File-system specific options:\n"
           "    --name=<s>          Name of the \"hello\" file\n"
           "                        (default: \"hello\")\n"
           "    --contents=<s>      Contents \"hello\" file\n"
           "                        (default \"Hello, World!\\n\")\n"
           "\n");
}

int main(int argc, char *argv[])
{
    int ret;
    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

    /* Set defaults -- we have to use strdup so that
       fuse_opt_parse can free the defaults if other
       values are specified */
    options.filename = strdup("hello");
    options.contents = strdup("Hello World!\n");

    /* Parse options */
    if (fuse_opt_parse(&args, &options, option_spec, NULL) == -1)
        return 1;

    /* When --help is specified, first print our own file-system
       specific help text, then signal fuse_main to show
       additional help (by adding `--help` to the options again)
       without usage: line (by setting argv[0] to the empty
       string) */
    if (options.show_help)
    {
        show_help(argv[0]);
        assert(fuse_opt_add_arg(&args, "--help") == 0);
        args.argv[0][0] = '\0';
    }

    data = fopen("sd.img", "r+b");

    if (!data)
    {
        printf("Failed to open sd.img\n");
        return 1;
    }

    fat_init(&fs, data);

    PartitionTable pt[4];

    fat_read_partitions(&fs, pt);
    fat_select_partition_table(&fs, &pt[0]);
    fat_read_boot_sector(&fs);

    ret = fuse_main(args.argc, args.argv, &hello_oper, NULL);
    fuse_opt_free_args(&args);
    return ret;
}