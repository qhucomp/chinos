#include "include/string.h"
#include "include/elf.h"
#include "include/printk.h"
#include <stddef.h>
#include <stdint.h>

uint64_t copy_section(const char *pbuff,char *buf,const char *section_name,uint64_t *offset)
{
    //60 偏移位置得到节区数量
    int nNumSec = *(Elf64_Half*)(pbuff + 60);
    //get shstrndex
    Elf64_Ehdr* pfilehead = (Elf64_Ehdr*)pbuff;
    Elf64_Half eshstrndx = pfilehead->e_shstrndx;
    //得到偏移地址
    Elf64_Shdr* psecheader = (Elf64_Shdr*)(pbuff + pfilehead->e_shoff);
    Elf64_Shdr* pshstr = (Elf64_Shdr*)(psecheader + eshstrndx);
    char* pshstrbuff = (char *)(pbuff + pshstr->sh_offset);
    int64_t section_name_len = strlen(section_name);
    uint64_t section_size = 0;
    for(int i = 1;i < nNumSec;i++) {
        char *section =  (char *)(psecheader[i].sh_name + pshstrbuff);
        if (!strncmp(section,section_name,section_name_len))  {
            section_size = psecheader[i].sh_size;
            if (offset != NULL)
                *offset = psecheader[i].sh_addr;
            memcpy(buf + psecheader[i].sh_addr,pbuff + psecheader[i].sh_offset,psecheader[i].sh_size);
            break;
        }
    }
    return section_size;
}

static int64_t find_symbol(const Elf64_Sym *psym, const char *pbuffstr, int ncount,const char *symbol_name)
{
    int64_t symbol_len = strlen(symbol_name);
    for(int i = 0;i<ncount;++i)
        if (!strncmp(symbol_name,psym[i].st_name + pbuffstr,symbol_len))
            return psym[i].st_value;
    return -1;
}

//获取符号在内存映像中的偏移
uintptr_t get_symbol_offset(const char *pbuff,const char *symbol_name)
{
    //从节区里面定位到偏移
    Elf64_Ehdr* pfilehead = (Elf64_Ehdr *)pbuff;
    Elf64_Half eshstrndx = pfilehead->e_shstrndx;
    Elf64_Shdr* psecheader = (Elf64_Shdr *)(pbuff + pfilehead->e_shoff);
    Elf64_Shdr* pshstr = (Elf64_Shdr *)(psecheader + eshstrndx);
    char* pshstrbuff = (char *)(pbuff + pshstr->sh_offset);
    int64_t addr;
    for(int i = 0;i<pfilehead->e_shnum;i++)
    {
        if(!strncmp(psecheader[i].sh_name + pshstrbuff, ".dynsym",7) || !strncmp(psecheader[i].sh_name + pshstrbuff, ".symtab",7))
        {
            Elf64_Sym* psym = (Elf64_Sym*)(pbuff + psecheader[i].sh_offset);
            int ncount = psecheader[i].sh_size / psecheader[i].sh_entsize;
            char* pbuffstr = (char*)((psecheader + psecheader[i].sh_link)->sh_offset + pbuff);

            addr = find_symbol(psym, pbuffstr, ncount,symbol_name);
            if (addr != -1)
                return addr;
            continue;
        }
    }
    return 0;
}