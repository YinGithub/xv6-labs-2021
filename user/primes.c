#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define FD_READ 0
#define FD_WRITE 1
int main()
{
    int num = 0;
    int prime_num = 0;
    int counter = 0;
    int left_pipe_fd[2] ={0};
    int right_pipe_fd[2] = {0};
    pipe(left_pipe_fd);
    int result;

    if(fork() == 0)
    {

        close(left_pipe_fd[FD_WRITE]);
        while(read(left_pipe_fd[FD_READ],&num,sizeof(int))>0)
        {
            counter++;
            if(counter == 1)
            {
                prime_num = num;
                printf("prime %d.\n",prime_num);
            }
            if(counter == 2)
            {
                pipe(right_pipe_fd);
                if(fork() == 0)
                {
                    /* Release */
                    close(left_pipe_fd[FD_READ]);
                    memcpy(left_pipe_fd,right_pipe_fd,sizeof(left_pipe_fd));   
                    close(left_pipe_fd[FD_WRITE]);
                    counter = 0;
                    continue;
                }
            }
            if(num%prime_num != 0)
            {
                write(right_pipe_fd[FD_WRITE],&num,sizeof(int));
            }
            // num = 4;    
        }
        close(left_pipe_fd[FD_READ]);
        if(counter >= 2)
        {
            /* have son*/
            close(right_pipe_fd[FD_READ]);
            close(right_pipe_fd[FD_WRITE]);
            wait(&result);            
        }
    }
    else{
        int i;
        for(i= 2; i <= 35; i++)
        {
            write(left_pipe_fd[1],&i,sizeof(int));
        }
        close(left_pipe_fd[0]);
        close(left_pipe_fd[1]);
        wait(&result);
    }
    exit(0);
}
