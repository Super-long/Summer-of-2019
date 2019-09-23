#include<stdio.h>
#include<unistd.h>
//#include<Linux/limits.h>

void my_err(const char *err_string,unsigned int line)
{
    fprintf(stderr,"line:%d",line);
    perror(err_string);
    exit(1);
}

int main(int argc,char **argv)
{
    char buf[251];
    if(argc<2)
    {
        printf("参数不够!\n");
        exit(1);
    }

    if(chdir(argv[1])<0)
    {
        my_err("chdir",__LINE__);
    }
    if(getcwd(buf,512)<0)
    my_err("getcwd",__LINE__);

    printf("%s",buf);
    return 0;

}