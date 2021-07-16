#ifndef __STAT_H
#define __STAT_H

#define T_DIR     1   // Directory
#define T_FILE    2   // File
#define T_DEVICE  3   // Device

#define STAT_MAX_NAME 32
#include <sys/types.h>

struct stat {
  char name[STAT_MAX_NAME + 1];
  int dev;     // File system's disk device
  short type;  // Type of file
  uint64 size; // Size of file in bytes
};

struct kstat {
	uint64 st_dev;
	uint64 st_ino;
	mode_t st_mode;
	uint32 st_nlink;
	uint32 st_uid;
	uint32 st_gid;
	uint64 st_rdev;
	unsigned long __pad;
	off_t st_size;
	uint32 st_blksize;
	int __pad2;
	uint64 st_blocks;
	long st_atime_sec;
	long st_atime_nsec;
	long st_mtime_sec;
	long st_mtime_nsec;
	long st_ctime_sec;
	long st_ctime_nsec;
};

struct user_dirent {
    uint64 d_ino;	// 索引结点号
    long d_off;	// 到下一个dirent的偏移
    unsigned short d_reclen;	// 当前dirent的长度
    unsigned char d_type;	// 文件类型
    char d_name[STAT_MAX_NAME + 1];	//文件名
};

// struct stat {
//   int dev;     // File system's disk device
//   uint ino;    // Inode number
//   short type;  // Type of file
//   short nlink; // Number of links to file
//   uint64 size; // Size of file in bytes
// };

#endif