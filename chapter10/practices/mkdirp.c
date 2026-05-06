#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

static void make_path(const char *path);
static void die(const char *s);

int main(int argc, char *argv[])
{
  int i;

  if (argc < 2)
  {
    fprintf(stderr, "%s: no arguments\n", argv[0]);
    exit(1);
  }
  for (i = 1; i < argc; i++)
  {
    make_path(argv[i]);
  }
  exit(0);
}

static void make_path(const char *path)
{
  if (mkdir(path, 0777) == 0) // success
  {
    return;
  }
  if (errno == EEXIST)
  {
    struct stat st;
    if (stat(path, &st) < 0)
      die("stat");
    if (!S_ISDIR(st.st_mode))
    {
      fprintf(stderr, "file exists but is not a directory: %s\n", path);
      exit(1);
    }
    return; // given path is a directory.
  }
  else if (errno == ENOENT)
  {
    // parent path is not directory
    char *parent_path = strdup(path);
    if (!parent_path)
      die("strdup");
    // remove duplicated slashes '/'
    char *last = parent_path + strlen(parent_path) - 1;
    while (*last == '/' && last != parent_path)
    {
      *last-- = '\0';
    }

    if (strcmp(parent_path, "/") == 0)
    {
      fprintf(stderr, "error: root directory is not a directory???\n");
      exit(1);
    }

    char *sep = strrchr(parent_path, '/');
    if (!sep) // slash '/' not found, e.g. component
    {
      fprintf(stderr, "error: current directory is not a directory???\n");
      exit(1);
    }
    else if (sep == parent_path) // e.g. /component
    {
      fprintf(stderr, "error: root directory is not a directory???\n");
      exit(1);
    }
    // e.g. /a/b/c -> /a/b/\0c
    *sep = '\0';

    make_path(parent_path);
    if (mkdir(path, 0777) < 0)
      die(path);
    return;
  }
  else
  {
    die(path);
  }
}

static void die(const char *s)
{
  perror(s);
  exit(1);
}
