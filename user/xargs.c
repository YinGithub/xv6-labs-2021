/* 
    echo "1\n2" | xargs -n 1 echo line
    line 1
    line 2
*/

/*
Use fork and exec to invoke the command on each line of input. Use wait in the parent to wait for the child to complete the command.
To read individual lines of input, read a character at a time until a newline ('\n') appears.
kernel/param.h declares MAXARG, which may be useful if you need to declare an argv array.
Add the program to UPROGS in Makefile.
Changes to the file system persist across runs of qemu; to get a clean file system run make clean and then make qemu.
*/
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"
/*
int p[2];
char *argv[2];
argv[0] = "wc";
argv[1] = 0;
pipe(p);
if(fork() == 0) {
close(0);
dup(p[0]);
close(p[0]);
close(p[1]);
exec("/bin/wc", argv);
} else {
close(p[0]);
write(p[1], "hello world\n", 12);
close(p[1]
}

 find . b | xargs grep hello
echo "1\n2" | xargs -n 1 echo line

 */
#define NULL 0

int split(char *str, const char *key, char **str_arr)
{
    int count = 0;
    int klen = strlen(key);
    char *p;

    if (str == NULL || str_arr == NULL || key == NULL || klen == 0)
        return -1;

    if (*str == '\0')
        return 0;

    /* 第一段从开头开始 */
    str_arr[count++] = str;
    // printf("str strlen keyword %d,strlen string %d .\n",klen,strlen(str));

    for (p = str; *p; p++) {
        /* 在 p 处往后看 klen 个字节是否等于 key */
        if (memcmp(p, key,strlen(key)) == 0) {
            /* 把 key 的第一个字节位置写成 '\0'，相当于把整段切下来 */
            *p = '\0';
            // printf("str_arr%s.\n",str_arr[count-1]);
            p += klen;
            /* 末尾没有更多字符了 */
            if (*p == '\0'  || *p == '\n')
            {
                *p = '\0';
                break;
            }
            /* 下一段从 key 之后开始 */
            str_arr[count++] = p;
            p--;   /* for 循环还要 p++，所以这里 -1 抵消 */
        }
    }

    return count;
}

int main(int argc, char *argv[])
{
    int i_arg = 0; //pointer to arg
    int n_arg = 0;
    char * argv_exec[MAXARG] = {0};
    char * splited_list[MAXARG] = {0};
    char * p_xarg_pram = NULL ;
    char * p_str = NULL;
    char buf_parms[MAXARG] = {0};
    int result;
    int i=0;

    /* patameter  checking */
    i_arg = 1;
    if(argc <  (i_arg+1)){
        printf("usage,\n");
        exit(0);
    }
    if( memcmp(argv[1],"-",strlen("-"))==0)
    {
        p_xarg_pram = argv[1]+1;
    }
    if(p_xarg_pram != NULL)
    {
        i_arg++;
        if(argc <  (i_arg+1)){
        printf("usage,\n");
        exit(0);
        }
        if(strcmp(p_xarg_pram,"n") ==0)
        {
            n_arg = atoi(argv[i_arg]);
            // printf("n_arg is %d",n_arg);
            i_arg++;
        }
        /* others */
        
    }
    p_str = buf_parms;
    while((i= read(0,p_str,MAXARG))>0)
    {
        p_str += i;
    }
    // buf_parms[i-1] = '\0';
    if(buf_parms[0] == '\"' && buf_parms[strlen(buf_parms)-1] == '\"' )
    {
        // printf("* match \n");
        p_str = buf_parms + 1 ;
        buf_parms[strlen(buf_parms)-1] = '\0';
    }
    else
        p_str = buf_parms; 
    if(argc <  (i_arg+1)){
    printf("usage,\n");
    exit(0);
    }
    // printf("p_str:%s.\n",p_str);
    int i_argv_exec= 0;
    /*cmd */
    char strcmd_exec[20] = "/";
    memcpy(strcmd_exec + strlen(strcmd_exec),argv[i_arg],sizeof(argv[i_arg]));
    // printf("strcmd_exec:%s.\n",strcmd_exec);
    for(;i_arg<argc;i_arg++)
    {
        argv_exec[i_argv_exec++] = argv[i_arg];
    }
    split(p_str,"\n",splited_list);
    if(n_arg == 0)
    {
        for(i= 0;i< sizeof(splited_list)/sizeof(splited_list[0]) ;i++)
        {
            if(splited_list[i] == NULL)
                break;
            argv_exec[i_argv_exec++] = splited_list[i];
        }
        if(fork() == 0)
        {
            // for(int i = 0; i < sizeof(argv_exec)/sizeof(argv_exec[0]) ;i++ )
            // {
            //     if(argv_exec[i] == NULL)
            //         break;
            //     printf("argv_exec :%s\n",argv_exec[i]);
            // }
            exec(strcmd_exec,argv_exec);
            // printf("done\n");
        }
        else
            wait(&result);
    }    
    else
    {
        int k = i_argv_exec;
        for(i= 0;i< sizeof(splited_list)/sizeof(splited_list[0]) ;i++)
        {            
            if(splited_list[i] == NULL)
                break;
            // printf("argv_exec :%s\n",splited_list[i]);
            argv_exec[k++] = splited_list[i];
            if((k-i_argv_exec) == n_arg)
            {
                k = i_argv_exec;
                if(fork() == 0)
                {
                    exec(strcmd_exec,argv_exec);
                    break;
                }
                else
                    wait(&result);
            }
        }
    }  
    exit(0);
}