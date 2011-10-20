#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>
#include <signal.h>
#include "../error/error.h"

pid_t run_fork_execl(const char* p_cmd, const char* param, const char* command, const int newstdout, const int newstdin)
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

int run_fork_execv_waitpid(const char* p_cmd, char* const argv[], const int newstdout, const int newstdin)
{
pid_t pid = fork();
if ( pid == -1 )
    {
    error_fork( errno );
    return -1;
    }
if ( pid == 0 )
    {
    int retcode = 0;
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
    execv( p_cmd, argv );
    error_exec( errno );
    exit( -1 );
    }
else
{
  int retcode = 0;
  waitpid(pid, &retcode, WNOHANG );
  if (WIFEXITED(retcode))
  {
      retcode = retcode >> 8;
      return retcode;
  }
  else
  {
      return -2;
  }
}
return -1; // error
}

// v(ait)fork 
pid_t run_vfork_execl(const char* p_cmd, const char* param, const char* command, const int newstdout, const int newstdin)
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
