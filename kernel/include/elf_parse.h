#ifndef __ELF_PARSE_H
#define __ELF_PARSE_H
#include <stddef.h>
#include <stdint.h>

uint64_t copy_section(const char *pbuff,char *buf,const char *section_name,uint64_t *offset);
uintptr_t get_symbol_offset(const char *pbuff,const char *symbol_name);
#endif