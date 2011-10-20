#ifndef RUNH
#define RUNH

pid_t run_fork_execl(const char* , const char* , const char* , const int , const int );
int run_fork_execv_waitpid(const char* p_cmd, char* const argv[], const int newstdout, const int newstdin);
pid_t run_vfork_execl(const char* , const char* , const char* , const int , const int );
#endif

