#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char *argv[])
{
  FILE *f;
  struct dirent *ent;

  if (argc != 2)
  {
    fprintf(stderr, "no arguments\n");
    exit(1);
  }
  f = fopen(argv[1], "rw");
  // ファイルはリネームされる
  rename(argv[1], "./foo");
  fclose(f);
}
