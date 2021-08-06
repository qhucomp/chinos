#ifndef __SYSNUM_H
#define __SYSNUM_H

// System call numbers
// #define SYS_fork         1
// #define SYS_exit         2
// #define SYS_wait         3
// #define SYS_pipe         4
// #define SYS_read         5
// #define SYS_kill         6
// #define SYS_exec         7
// #define SYS_fstat        8
// #define SYS_chdir        9
// #define SYS_dup         10
// #define SYS_getpid      11
// #define SYS_sbrk        12
// #define SYS_sleep       13
// #define SYS_uptime      14
// #define SYS_open        15
// #define SYS_write       16
// #define SYS_remove      17
// #define SYS_trace       18
// #define SYS_sysinfo     19
// #define SYS_mkdir       20
// #define SYS_close       21
// #define SYS_test_proc   22
#define SYS_dev         1
// #define SYS_readdir     24
// #define SYS_getcwd      25
// #define SYS_rename      26

#define SYS_getcwd                  17
#define SYS_dup                     23
#define SYS_dup3                    24
#define SYS_mkdirat                 34
#define SYS_unlinkat                35
#define SYS_linkat                  37
#define SYS_umount2                 39
#define SYS_mount                   40
#define SYS_faccessat               48	
#define SYS_chdir                   49
#define SYS_openat                  56
#define SYS_close                   57
#define SYS_pipe2                   59
#define SYS_getdents64              61
#define SYS_read                    63
#define SYS_write                   64
#define SYS_fstat                   80
#define SYS_exit                    93
#define SYS_nanosleep               101
#define SYS_sched_yield             124
#define SYS_times                   153
#define SYS_uname                   160
#define SYS_gettimeofday            169
#define SYS_getpid                  172
#define SYS_getppid                 173
#define SYS_brk                     214
#define SYS_munmap                  215
#define SYS_clone                   220
#define SYS_execve                  221
#define SYS_mmap                    222
#define SYS_wait4                   260

#define SYS_sched_setaffinity       122
#define SYS_sched_getaffinity       123
#define SYS_sched_get_priority_max  125
#define SYS_sched_get_priority_min  126

#define SYS_kill                    129
#define SYS_tkill                   130

#define SYS_rt_sigaction            134
#define SYS_rt_sigprocmask          135
#define SYS_rt_sigreturn            139

#define SYS_removexattr             14

#define SYS_setpriority             140
#define SYS_getpriority             141

#define SYS_reboot                  142

#define SYS_setgid                  144
#define SYS_setuid                  146
#define SYS_getresuid               147
#define SYS_getresgid               150

#define SYS_lremovexattr            148

#define SYS_times                   153 //未实现

#define SYS_setpgid                 154
#define SYS_getpgid                 155
#define SYS_getsid                  156
#define SYS_setsid                  157
#define SYS_getgroups               158
#define SYS_setgroups               159

#define SYS_sethostname             161

#define SYS_prctl                   167 //未实现

#define SYS_getrlimit               163
#define SYS_setrlimit               164
#define SYS_umask                   166
#define SYS_adjtimex                171 //未实现
#define SYS_getuid                  174
#define SYS_geteuid                 175
#define SYS_getgid                  176
#define SYS_getegid                 177
#define SYS_gettid                  178
#define SYS_sysinfo                 179

//共享内存相关
#define SYS_msgget                  186
#define SYS_msgctl                  187
#define SYS_semget                  190
#define SYS_semctl                  191
#define SYS_semop                   193
#define SYS_shmget                  194
#define SYS_shmctl                  195
#define SYS_shmat                   196
#define SYS_shmdt                   197

#define SYS_shutdown                210
#define SYS_readahead               213

//swap
#define SYS_swapon                  224
#define SYS_swapoff                 225

#define SYS_mprotect                226
#define SYS_mlock                   228
#define SYS_munlock                 229
#define SYS_madvise                 233
#define SYS_prlimit64               261
#define SYS_lock_adjtime            266
#define SYS_syncfs                  267
#define SYS_setns                   268
#define SYS_renameat2               276
#define SYS_ioctl                   29
#define SYS_flock                   32
#define SYS_mknodat                 33
#define SYS_symlinkat               36
#define SYS_linkat                  37
#define SYS_pivot_root              41
#define SYS_fallocate               47
#define SYS_faccessat               48
#define SYS_setxattr                5
#define SYS_fchdir                  50
#define SYS_chroot                  51

#define SYS_fchmod                  52
#define SYS_fchmodat                53
#define SYS_fchownat                54
#define SYS_fchown                  55
#define SYS_lsetxattr               6

#define SYS_lseek                   62
#define SYS_readv                   65
#define SYS_writev                  66
#define SYS_sendfile                71
#define SYS_ppoll                   73
#define SYS_readlinkat              78
#define SYS_sync                    81
#define SYS_utimensat               88
#define SYS_acct                    89
#define SYS_capget                  90
#define SYS_capset                  91
#define SYS_personality             92
#define SYS_exit_group              94
#define SYS_set_tid_address         96
#define SYS_unshare                 97
#define SYS_futex                   98 
// #define SYS_ioctl                   29

// struct fs_link {
//     char magic[16];
//     char path[FAT32_MAX_PATH];
// };
// struct timeval {
//     long      tv_sec;     /* seconds */
//     long tv_usec;    /* microseconds */
// };

// struct timex {
//     int  modes;      /* Mode selector */
//     long offset;     /* Time offset; nanoseconds, if STA_NANO
//                         status flag is set, otherwise
//                         microseconds */
//     long freq;       /* Frequency offset; see NOTES for units */
//     long maxerror;   /* Maximum error (microseconds) */
//     long esterror;   /* Estimated error (microseconds) */
//     int  status;     /* Clock command/status */
//     long constant;   /* PLL (phase-locked loop) time constant */
//     long precision;  /* Clock precision
//                         (microseconds, read-only) */
//     long tolerance;  /* Clock frequency tolerance (read-only);
//                         see NOTES for units */
//     struct timeval time;
//                     /* Current time (read-only, except for
//                         ADJ_SETOFFSET); upon return, time.tv_usec
//                         contains nanoseconds, if STA_NANO status
//                         flag is set, otherwise microseconds */
//     long tick;       /* Microseconds between clock ticks */
//     long ppsfreq;    /* PPS (pulse per second) frequency
//                         (read-only); see NOTES for units */
//     long jitter;     /* PPS jitter (read-only); nanoseconds, if
//                         STA_NANO status flag is set, otherwise
//                         microseconds */
//     int  shift;      /* PPS interval duration
//                         (seconds, read-only) */
//     long stabil;     /* PPS stability (read-only);
//                         see NOTES for units */
//     long jitcnt;     /* PPS count of jitter limit exceeded
//                         events (read-only) */
//     long calcnt;     /* PPS count of calibration intervals
//                         (read-only) */
//     long errcnt;     /* PPS count of calibration errors
//                         (read-only) */
//     long stbcnt;     /* PPS count of stability limit exceeded
//                         events (read-only) */
//     int tai;         /* TAI offset, as set by previous ADJ_TAI
//                         operation (seconds, read-only,
//                         since Linux 2.6.26) */
//     /* Further padding bytes to allow for future expansion */
// };

// struct sysinfo {
//     long uptime;             /* Seconds since boot */
//     unsigned long loads[3];  /* 1, 5, and 15 minute load averages */
//     unsigned long totalram;  /* Total usable main memory size */
//     unsigned long freeram;   /* Available memory size */
//     unsigned long sharedram; /* Amount of shared memory */
//     unsigned long bufferram; /* Memory used by buffers */
//     unsigned long totalswap; /* Total swap space size */
//     unsigned long freeswap;  /* Swap space still available */
//     unsigned short procs;    /* Number of current processes */
//     unsigned long totalhigh; /* Total high memory size */
//     unsigned long freehigh;  /* Available high memory size */
//     unsigned int mem_unit;   /* Memory unit size in bytes */
//     char _f[20-2*sizeof(long)-sizeof(int)];
//                             /* Padding to 64 bytes */
// };

#endif