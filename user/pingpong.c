#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int main()
{
    int p[2];
    pipe(p);
    if(fork() == 0) {
    unsigned char c_buf[10];
    read(p[0],c_buf,1);
    printf("%d: received ping\n",getpid());
    write(p[1],c_buf,1);
    close(p[0]);
    close(p[1]);
    } else {
    unsigned char p_buf[10];
    p_buf[0] = 0;
    write(p[1], p_buf, 1);
    read(p[0],p_buf,1);
    printf("%d: received pong\n",getpid());
    close(p[0]);
    close(p[1]);
    }
    exit(0);
}
