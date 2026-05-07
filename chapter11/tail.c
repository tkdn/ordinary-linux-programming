// https://github.com/aamine/stdlinux2-source/blob/d1be9e63556e4640edf1a78cd47c7c39032af4cc/tail2.c#L23
#include <stdio.h>
#include <stdlib.h>

#define N_LINES 10
#define MAX_LINE_LENGTH 1024

static void do_tail(FILE *f, int nlines);
static unsigned char *readline(FILE *f);

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s n\n", argv[0]);
    exit(1);
  }
  do_tail(stdin, atoi(argv[1]));
  exit(0);
}

static void do_tail(FILE *f, int nlines)
{
  unsigned char **ringbuf;
  unsigned char **p;
  unsigned char *line;
  int n;

#define INC(ptrvar)                 \
  do                                \
  {                                 \
    ptrvar++;                       \
    if (ptrvar >= ringbuf + nlines) \
      ptrvar = ringbuf;             \
  } while (0)

  if (nlines == 0)
    return;
  ringbuf = calloc(nlines, sizeof(char *));
  if (ringbuf == NULL)
    exit(1);
  p = ringbuf;
  while (line = readline(f))
  {
    if (*p)
      free(*p);
    *p = line;
    INC(p);
  }
  if (*p == NULL)
    p = ringbuf;
  for (n = nlines; n > 0 && *p; n--)
  {
    printf("%s", *p);
    free(*p);
    INC(p);
  }
  free(ringbuf);
}

unsigned char *readline(FILE *f)
{
  unsigned char *buf, *p;
  size_t buflen = BUFSIZ;
  int c;

  buf = p = malloc(sizeof(char) * buflen);
  if (buf == NULL)
    exit(1);
  for (;;)
  {
    c = getc(f);
    if (c == EOF)
    {
      if (buf == p)
      {
        free(buf);
        return NULL;
      }
      break;
    }
    *p++ = c;
    if (p >= buf + buflen - 1)
    {
      buflen *= 2;
      buf = realloc(buf, buflen);
      if (buf == NULL)
        exit(1);
    }
    if (c == '\n')
      break;
  }
  *p++ = '\0';
  return buf;
}
