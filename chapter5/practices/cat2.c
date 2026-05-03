#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

static void do_cat(const char *path);
static void do_cat2(FILE *f);
static void die(const char *s);

int main(int argc, char *argv[])
{
  int i;
  if (argc == 1)
  {
    do_cat2(stdin);
    exit(0);
  }
  for (i = 1; i < argc; i++)
  {
    do_cat(argv[i]);
  }
  exit(0);
}

#define BUFFER_SIZE 2048

static void
do_cat(const char *path)
{
  int fd;
  unsigned char buf[BUFFER_SIZE];
  int n;

  fd = open(path, O_RDONLY);
  if (fd < 0)
    die(path);
  for (;;)
  {
    n = read(fd, buf, sizeof buf);
    if (n < 0)
      die(path);
    if (n == 0)
      break;
    if (write(STDOUT_FILENO, buf, n) < 0)
      die(path);
  }
  if (close(fd) < 0)
    die(path);
}

static void
die(const char *s)
{
  perror(s);
  exit(1);
}

// https://github.com/aamine/stdlinux2-source/blob/master/cat3.c
void do_cat2(FILE *f)
{
  int c;

  while ((c = fgetc(f)) != EOF)
  {
    if (putchar(c) < 0)
      exit(1);
  }
}
