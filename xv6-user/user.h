#include "kernel/include/types.h"
#include "kernel/include/stat.h"
#include "kernel/include/fcntl.h"

struct stat;
struct rtcdate;
struct sysinfo;

// system calls
int clone(void);
int exit(int) __attribute__((noreturn));
int wait4(int*);
int pipe2(int*);
int write(int fd, const void *buf, int len);
int read(int fd, void *buf, int len);
int close(int fd);
// int kill(int pid);
int execve(char*, char**);
int openat(const char *filename, int mode);
int fstat(int fd, struct stat*);
int mkdirat(const char *dirname);
int chdir(const char *dirname);
int dup(int fd);
int getpid(void);
char* brk(int size);
int getppid(void);
void *mmap(void *addr,size_t length,int prot,int flags,int fd,off_t offset);
int munmap(void *addr,size_t length);
// int sleep(int ticks);
// int uptime(void);
// int test_proc(int);
int dev(int, short, short);
// int readdir(int fd, struct stat*);
int getcwd(char *buf);
// int remove(char *filename);
// int trace(int mask);
// int sysinfo(struct sysinfo *);
// int rename(char *old, char *new);

// ulib.c
int stat(const char*, struct stat*);
char* strcpy(char*, const char*);
char* strcat(char*, const char*);
void *memmove(void*, const void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void fprintf(int, const char*, ...);
void printf(const char*, ...);
char* gets(char*, int max);
uint strlen(const char*);
void* memset(void*, int, uint);
void* malloc(uint);
void free(void*);
int atoi(const char*);
int memcmp(const void *, const void *, uint);
void *memcpy(void *, const void *, uint);
