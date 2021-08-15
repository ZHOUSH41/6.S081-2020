#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
void find(char *path, char *filename)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, 0)) < 0)
  {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0)
  {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }
  while (read(fd, &de, sizeof(de)) == sizeof(de))
  {
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    if (de.inum == 0)
      continue;
    
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    // buf就是全名，de.name是当前名字
    if (stat(buf, &st) < 0)
    {
      printf("find: cannot stat %s\n", buf);
      continue;
    }
    switch (st.type)
    {
    case T_FILE:
      if (strcmp(de.name, filename) == 0)
      {
        printf("%s\n", buf);
      }
      break;
    case T_DIR:
      if (strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0)
      {
        find(buf, filename);
      }
      break;
    }
  }
  close(fd);
  return;
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    fprintf(2, "Usage: find filepath filename\n");
    exit(1);
  }
  find(argv[1], argv[2]);
  exit(0);
}
