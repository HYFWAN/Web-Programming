#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

/*守护进程(daemon)是一类在后台运行的特殊进程，用于执行特定的系统任务。
很多守护进程在系统引导的时候启动，并且一直运行直到系统关闭。
另一些只在需要的时候才启动，完成任务后就自动结束。 
*/
//将普通进程改造为守护进程

bool daemonize()
{
    //创建子进程，关闭父进程
    pid_t pid = fork();
    if (pid < 0)
        return false;
    else if (pid > 0)
        exit(0);
    //设置文件权限
    umask(0);
    //创建新会话，设置本进程为进程组的首领
    pid_t sid = setsid();
    if (sid < 0)
        return false;
    //切换工作目录
    if ((chdir("/")) < 0)
        return false;
}