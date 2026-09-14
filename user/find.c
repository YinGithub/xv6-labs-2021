#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
// #include "string.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}
#define NULL 0
char* strstr(const char *haystack, const char *needle) {
    // 边界:空 needle →返回 haystack
    if (*needle == '\0')
        return (char*)haystack;
 // 从 haystack 的每个位置 i 试一次
    for (int i = 0; haystack[i] != '\0'; i++) {
        if (memcmp(haystack + i, needle,strlen(needle)) == 0)
            return (char*)(haystack + i);
    }
    return NULL; // 没找到
}
void
find(char *path, char * keyword )
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    printf( "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    printf("ls: cannot stat %s\n", path);
    close(fd);
    return;
  }
//   printf("path is %s keyword %s\n",path,keyword);
  if(strstr(path,keyword) != NULL)
    printf("%s\n",path);
  
  switch(st.type){
  case T_FILE:
    // printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
    //   char * base = fmtname(buf);
    //   printf("base:%s\n",de.name);
      if(strcmp(de.name,".")==0 || strcmp(de.name,"..")==0)
        continue;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      find(buf,keyword);
        //   printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc != 3){
    printf("usage find . .");
    exit(0);
  }
  find(argv[1],argv[2]);
  exit(0);
}
