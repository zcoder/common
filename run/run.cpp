#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "../error/error.h"

pid_t run_fork_execl(const char* p_cmd, const char* param = "-c", const char* command, const int newstdout, const int newstdin)
{
int retcode;
pid_t pid = fork();
if ( pid == -1 )
    {
    error_fork( errno );
    return -1;
    }
if ( pid == 0 )
    {
    retcode = dup2( newstdout, STDOUT_FILENO );
    if ( retcode == -1 )
        {
        error_dup2( errno );
        return -1;
        }
    retcode = dup2( newstdin, STDIN_FILENO );
    if ( retcode == -1 )
        {
        error_dup2( errno );
        return -1;
        }
    execl( p_cmd, p_cmd, param, command, (char*)NULL );
    error_exec( errno );
    exit( -1 );
    }
return pid;
}

pid_t run_vfork_execl(const char* p_cmd, const char* param = "-c", const char* command, const int newstdout, const int newstdin)
{
int retcode;
pid_t pid = vfork();
if ( pid == -1 )
    {
    error_fork( errno );
    return -1;
    }
if ( pid == 0 )
    {
    retcode = dup2( newstdout, STDOUT_FILENO );
    if ( retcode == -1 )
        {
        error_dup2( errno );
        return -1;
        }
    retcode = dup2( newstdin, STDIN_FILENO );
    if ( retcode == -1 )
        {
        error_dup2( errno );
        return -1;
        }
    execl( p_cmd, p_cmd, param, command, (char*)NULL );
    error_exec( errno );
    exit( -1 );
    }
return pid;
}
