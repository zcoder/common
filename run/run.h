#ifndef RUNH
#define RUNH

pid_t run_fork_execl(const char* , const char* , const char* , const int , const int );
pid_t run_vfork_execl(const char* , const char* , const char* , const int , const int );
#endif

