#ifndef __ELF_H
#define __ELF_H

// Format of an ELF executable file

#define ELF_MAGIC 0x464C457FU  // "\x7FELF" in little endian

// File header
struct elfhdr {
  uint magic;  // must equal ELF_MAGIC
  uchar elf[12];
  ushort type;
  ushort machine;
  uint version;
  uint64 entry;
  uint64 phoff;
  uint64 shoff;
  uint flags;
  ushort ehsize;
  ushort phentsize;
  ushort phnum;
  ushort shentsize;
  ushort shnum;
  ushort shstrndx;
};

// Program section header
struct proghdr {
  uint32 type;
  uint32 flags;
  uint64 off;
  uint64 vaddr;
  uint64 paddr;
  uint64 filesz;
  uint64 memsz;
  uint64 align;
};

// Values for Proghdr type
#define ELF_PROG_LOAD           1

// Flag bits for Proghdr flags
#define ELF_PROG_FLAG_EXEC      1
#define ELF_PROG_FLAG_WRITE     2
#define ELF_PROG_FLAG_READ      4

#define AT_NULL 0      /* end of vector */
#define AT_IGNORE 1    /* entry should be ignored */
#define AT_EXECFD 2    /* file descriptor of program */
#define AT_PHDR 3      /* program headers for program */
#define AT_PHENT 4     /* size of program header entry */
#define AT_PHNUM 5     /* number of program headers */
#define AT_PAGESZ 6    /* system page size */
#define AT_BASE 7      /* base address of interpreter */
#define AT_FLAGS 8     /* flags */
#define AT_ENTRY 9     /* entry point of program */
#define AT_NOTELF 10   /* program is not ELF */
#define AT_UID 11      /* real uid */
#define AT_EUID 12     /* effective uid */
#define AT_GID 13      /* real gid */
#define AT_EGID 14     /* effective gid */
#define AT_PLATFORM 15 /* string identifying CPU for optimizations */
#define AT_HWCAP 16    /* arch dependent hints at CPU capabilities */
#define AT_CLKTCK 17   /* frequency at which times() increments */
/* AT_* values 18 through 22 are reserved */
#define AT_SECURE 23 /* secure mode boolean */
#define AT_BASE_PLATFORM                                                       \
  24 /* string identifying real platform, may differ from AT_PLATFORM. */
#define AT_RANDOM 25 /* address of 16 random bytes */

#define AT_EXECFN 31 /* filename of program */

#define AT_VECTOR_SIZE_BASE 19 /* NEW_AUX_ENT entries in auxiliary table */

#endif