#include <stdint.h>
#include <stddef.h>
#include "include/fat32.h"
#include "include/diskio.h"
#include "include/printk.h"
#include "include/kmalloc.h"
#include "include/vfs.h"
#include "include/string.h"
#include "include/sdcard.h"
fat32_fs *fs;
size_t fat32_read(dentry_struct *p,void *buf,size_t size) {
    size_t result = 0;
    size_t s = size / 512;
    if (size % (512 * fs->boot.bpb_sec_per_clus) != 0)
        s++;
    if (p->sector_count * 512 < size)
        s = p->sector_count;

    void *buffer = kmalloc(512*s*fs->boot.bpb_sec_per_clus);
    if (!buffer)
        panic("out of memory!");
    memset(buffer,0,512*s*fs->boot.bpb_sec_per_clus);
    for(size_t i = 0;i < s;i++) {

        if (result >= size)
            break;
        if (disk_read(0,buffer + 512*i*fs->boot.bpb_sec_per_clus,p->sectorno_list[i],fs->boot.bpb_sec_per_clus) == RES_ERROR)
            break;
        result += 512;
    }
    if (result > size)
        result = size;
    if (result > p->file_size && p->file_size != 0)
        result = p->file_size;
    memcpy(buf,buffer,result);
    // if (result > size) {
    //     memcpy(buf,buffer,size);
    //     result = size;
    // } else
    //     memcpy(buf,buffer,result);
    // kfree(buffer);
    kfree(buffer);
    return result;
}

/**
 * @brief 从长目录项里读取文件名
 * 
 * @param[in] dentry 要被读取的目录项
 * 
 * @return 返回256字节大小的文件名
 */
static char *get_entry_name(long_dir_entry *dentry) {
    char *name = kmalloc(256);
    int index = 0;
    dentry--;
    while(dentry->ldir_ord != 0xe5 || dentry->ldir_attr != 0xf) {
        int x;
        for(x = 0;x < 5;x++)
            if (dentry->ldir_name_1[x] && dentry->ldir_name_1[x] != 0xffff)
                name[index++] = (char)dentry->ldir_name_1[x];
        for(x = 0;x < 6;x++)
            if (dentry->ldir_name_2[x] && dentry->ldir_name_2[x] != 0xffff)
                name[index++] = (char)dentry->ldir_name_2[x];
        for(x = 0;x < 2;x++)
            if (dentry->ldir_name_3[x] && dentry->ldir_name_3[x] != 0xffff)
                name[index++] = (char)dentry->ldir_name_3[x];
        if ((dentry->ldir_ord & 0x40))
            break;
        dentry--;
    }
    name[index] = 0;
    return name;
}

dentry_struct *fat32_lookup(dentry_struct *dentry,const char *name);
fat32_fs *fat32_init(void) {
    mbr_struct mbr;
    fs =  kmalloc(sizeof(fat32_fs));
    if(disk_read(0,(uint8_t *)&mbr,0,1) == RES_ERROR)
        panic("read disk error");
    if(disk_read(0,(uint8_t *)&fs->boot,mbr.parts[0].lba,1) == RES_ERROR)
        panic("read disk error");
    rw_count = 0;
    uint32_t count;
    if (fs->boot.bpb_fatsz32 > DEFAULT_LOAD_SECTOR)    //最多加载128个扇区
        count = DEFAULT_LOAD_SECTOR;
    else
        count = fs->boot.bpb_fatsz32;

    fs->fat1 = kmalloc(512*count);
    fs->start_fat_sector = mbr.parts[0].lba + fs->boot.bpb_reserved_sec_cnt;
    fs->count = count;
    fs->data_sector = fs->start_fat_sector + fs->boot.bpb_fatsz32*fs->boot.bpb_num_fats;
    if(disk_read(0,(uint8_t *)&fs->fs_info,mbr.parts[0].lba + 1,1) == RES_ERROR)
        panic("read disk error");
    if(disk_read(0,(uint8_t *)fs->fat1,mbr.parts[0].lba + fs->boot.bpb_reserved_sec_cnt,count) == RES_ERROR)
        panic("read disk error");

    printk("fat LBA:%d,fat sector count:%d,data LBA:%d\n",fs->start_fat_sector,fs->count,fs->data_sector);
    printk("num fats:%d,fatsz32:%d\n",(uint32_t)fs->boot.bpb_num_fats,fs->boot.bpb_fatsz32);
    printk("FAT32 init..........OK\n");
    return fs;
}

uint32_t clusno_to_sectorno(fat32_fs *fs,uint32_t clusno) {
    return (clusno - 2)*fs->boot.bpb_sec_per_clus + fs->data_sector;
}

uint32_t get_fat_sectorno(fat32_fs *fs,uint32_t no) {
    return fs->start_fat_sector + (no >> 7);
}
uint32_t fat_sectorno_to_clusno(fat32_fs *fs,uint32_t sectorno) {
    return sectorno - fs->start_fat_sector;
}
uint32_t get_fat_no(uint32_t fat_no) {
    return fat_no >> 9;
}
uint8_t calc_checksum(const char *name) {
    uint8_t checksum = 0;
    for(int i = 0;i < 11;i++)
        checksum = ((checksum & 1) ? 0x80 : 0) + (checksum >> 1) + name[i];
    return checksum;
}

char *get_short_name(const char *name,char *buf) {
    //char name_buffer[12];
    memset(buf,0x20,11);
    size_t name_len = strlen(name);
    buf[11] = 0;

    char *c = strchr(name,'.');
    int prefix_len = c - name;
    if (c == NULL)
        prefix_len = name_len;
    if (prefix_len < 8) {
        memcpy(buf,name,prefix_len);
        int size = name_len - prefix_len;
        if (size > 3)
            size = 3;
        memcpy(buf + 8,c + 1,size);
        strupr(buf);
    }
    return buf;
}

//需要优化
dentry_struct *fat32_lookup(dentry_struct *dentry,const char *name) {
    dentry_struct *dentry_p = NULL;
    int root_size = 1;
    char *root_buf = NULL;
    short_dir_entry *entry = NULL;
    if (dentry == NULL) {
        // 加载根目录下的所有目录项
        int next = fs->boot.bpb_root_clus;
        dentry = create_dentry();
        push_sectorno(dentry,clusno_to_sectorno(fs,next));

        next = fs->fat1[next];
        if (next < FILE_END) {
            while (1) {
                if (next < fs->count*128) {
                    // 根目录FAT索引未超过缓存好的FAT的情况
                    push_sectorno(dentry,clusno_to_sectorno(fs,next));
                    root_size++;

                    next = fs->fat1[next];
                } else {
                    if (next >= FILE_END)
                        break;
                    // 根目录的FAT索引超过缓存好的FAT的情况
                    if(disk_read(0,(uint8_t *)fs->temp_fat,get_fat_sectorno(fs,next),4) == RES_ERROR) {
                        kfree(dentry->sectorno_list);
                        kfree(dentry);
                        panic("read disk failed");
                    }

                    uint32_t sector_index = 0;
                    push_sectorno(dentry,clusno_to_sectorno(fs,next));
                    next = next - fs->count*128;

                    while(1) {
                        next = fs->temp_fat[next] - sector_index*512 - fs->count*128;

                        //读到文件尾
                        if (next < 512 && fs->temp_fat[next] >= FILE_END) {
                            push_sectorno(dentry,clusno_to_sectorno(fs,next));
                            break;
                        }

                        if (next >= 511) {
                            //搜索下一个FAT
                            uint32_t temp = fs->temp_fat[next];
                            push_sectorno(dentry,clusno_to_sectorno(fs,fs->temp_fat[next]));
                            disk_read(0,(uint8_t *)fs->temp_fat,get_fat_sectorno(fs,fs->temp_fat[next]),4);
                            sector_index++;
                            next = temp - sector_index*512;
                            continue;
                        }
                        push_sectorno(dentry,clusno_to_sectorno(fs,next));
                    }
                    break;
                }
            }
        }
    }
    // 读取根目录项
    root_buf = kmalloc(fs->boot.bpb_sec_per_clus*dentry->sector_count);
    if (root_buf == NULL)
        panic("out of memory!");
    for(uint32_t count = 0;count < dentry->sector_count;count++)
        if (disk_read(0,(uint8_t *)root_buf + 512*fs->boot.bpb_sec_per_clus*count,dentry->sectorno_list[count],fs->boot.bpb_sec_per_clus) == RES_ERROR)
            panic("read disk failed");
    char short_name_buffer[12];
    get_short_name(name,short_name_buffer);
    size_t name_len = strlen(name);
    char *dname;
    char *buffer;
    int max_len = 0;
    // 搜索文件
    for(uint32_t count = 0;count < fs->boot.bpb_sec_per_clus*16*dentry->sector_count;count++) {
        entry = (void *)(root_buf +count*32);
        if (is_short_entry(entry)) {

            if (name_len > 12) {
                dname = get_entry_name((void *)(root_buf + count*32));
                buffer = (char *)name;
                max_len = MAX_NAME_LEN;
            } else {
                dname = entry->dir_name;
                buffer = short_name_buffer;
                max_len = 11;
            }
            if (!strncmp(dname,buffer,max_len)) {
                dentry_p = create_dentry();
                dentry_p->name = dname;
                dentry_p->file_size = entry->dir_file_size;
                uint32_t sector_index = 0;
                uint32_t clusno = ((uint32_t)entry->dir_fst_clus_hi << 16) | (uint32_t)entry->dir_fst_clus_lo;
                uint32_t next_clusno = clusno;

                push_sectorno(dentry_p,clusno_to_sectorno(fs,clusno));
                while(1) {
                    if (next_clusno < fs->count*128) {
                        if (fs->fat1[next_clusno] >= FILE_END)
                            goto end;
                        push_sectorno(dentry_p,clusno_to_sectorno(fs,fs->fat1[next_clusno]));
                        next_clusno = fs->fat1[next_clusno];
                    } else {
                        if (next_clusno >= FILE_END)
                            goto end;

                        if(disk_read(0,(uint8_t *)fs->temp_fat,get_fat_sectorno(fs,clusno),4) == RES_ERROR)
                            panic("read disk failed");

                        next_clusno = next_clusno - fs->count*128;
                        while(1) {
                            next_clusno = fs->temp_fat[next_clusno] - sector_index*512 - fs->count*128;

                            //读到文件尾
                            if (next_clusno < 512 && fs->temp_fat[next_clusno] >= FILE_END) {
                                push_sectorno(dentry_p,clusno_to_sectorno(fs,next_clusno));
                                break;
                            }

                            if (next_clusno >= 511) {
                                //搜索下一个FAT
                                uint32_t temp = fs->temp_fat[next_clusno];
                                push_sectorno(dentry_p,clusno_to_sectorno(fs,fs->temp_fat[next_clusno]));
                                disk_read(0,(uint8_t *)fs->temp_fat,get_fat_sectorno(fs,fs->temp_fat[next_clusno]),4);
                                sector_index++;
                                next_clusno = temp - sector_index*512;
                                continue;
                            }
                            push_sectorno(dentry_p,clusno_to_sectorno(fs,next_clusno));
                        }
                        break;
                    }
                }
            } else {
                if (name_len > 12)
                    kfree(dname);
            }
        }
    }
end:
    kfree(root_buf);
    return dentry_p;
}

dentry_struct *fat32_open(const char *path) {
    char name[256];
    dentry_struct *entry = NULL;
    int i = 0;
    int flag = 0;
    memset(name,0,256);
    for(i = 0;path[i] != '/';i++);
    i++;
    while(1) {
        for(int index = 0;path[i] != '/';i++,index++) {
            if (path[i] == '.' && path[i+1] == '/')
                continue;
            name[index] = path[i];
            if (path[i] == '\0') {
                flag = 1;
                break;
            }
        }
        for(;path[i] == '/' && path[i] != '\0';i++);
        if (path[i] == '\0')
            flag = 1;
        dentry_struct *_entry = fat32_lookup(entry,name);
        kfree(entry);
        entry = _entry;
        if(entry == NULL || flag)
            break;
    }
    uint8_t buf[512];
    // //预读
    for(i = 0;i < entry->sector_count;i++)
        if (disk_read(0,buf,entry->sectorno_list[i],fs->boot.bpb_sec_per_clus) == RES_ERROR) 
            break;
    return entry;
}

// size_t fat32_read_sector(uint32_t *sectorno_list,size_t sectorno_list_size,void *data,size_t size) {

// }
// static uint32_t *alloc_fat_index(fat32_fs *fs,size_t size,size_t *fat_index_len) {
//     size_t len = size / 512;
//     if (size % 512 != 0)
//         len++;
//     printk("len:%d\n",len);
//     uint32_t *fat_index_list = kmalloc(sizeof(uint32_t)*len);
//     //fat_index_list[0] = fs->fs_info.fsi_next_free;
//     uint32_t search_index = 3; //= fs->fs_info.fsi_next_free;
//     //printk("free_index:%d\n",fs->fs_info.fsi_next_free);
//     for(size_t i = 0;i < (len + 1);i++) {
//         while(1) {
//             printk("search_index:%d\n",search_index);
//             if (search_index < fs->count*128) {

//                 printk("value:%d\n",fs->fat1[search_index]);
//                 if (fs->fat1[search_index] <= EMPTY_CLUSNO) {
//                     if (i < len) {
//                         fs->fat1[fat_index_list[i - 1]] = search_index;
//                         fat_index_list[i] = search_index;
//                     } else {
//                         fs->fat1[fat_index_list[i - 1]] = FILE_END;
//                         fs->fs_info.fsi_next_free = search_index;
//                     }
//                     break;
//                 }
//             }
//             search_index++;
//         }
//     }
//     *fat_index_len = len;
//     return fat_index_list;
// }
// static void alloc_dentry(fat32_fs *fs,dentry_struct *dentry,char *name,uint32_t first_fat_index) {
// start: ;
//     void *buf = kmalloc(dentry->sector_count*512);
//     if(!buf)
//         goto oom_error;
//     int len = strlen(name);
//     int c = len / 13 + 1;
//     if (len % 13 == 0)
//         c++;
//     printk("c=%d\n",c);
//     int count = 0;
//     short_dir_entry *entry = buf;
//     size_t start_index = 0;
//     for(size_t i = 0;i < 16*dentry->sector_count;i++) {
//         if (IS_EMPTY_DIR_ENTRY(entry + i))
//             count++;
//         else {
//             count = 0;
//             start_index = 0;
//         }
//             printk("dir_name[0]=%d\n",entry->dir_name[0]);
//         if(!start_index)
//             start_index = i;
//         if (count == c)
//             break;
//     }
//     if (!count) {
//         printk("realloc\n");
//         size_t index;
//         index = *alloc_fat_index(fs,512,&index);
//         fs->fat1[dentry->sectorno_list[dentry->sector_count - 1]] = index;
//         disk_write(0,(uint8_t *)&fs->fat1[fat_sectorno_to_clusno(fs,dentry->sectorno_list[dentry->sector_count - 1]) << 7],dentry->sectorno_list[dentry->sector_count - 1],1);
//         push_sectorno(dentry,get_fat_sectorno(fs,index));
//         //disk_write(0,(uint8_t *)&fs->fat1[(index >> 7) << 7],get_fat_sectorno(fs,index),1);
//         kfree(buf);
//         goto start;
//     }
//     long_dir_entry *lentry = buf + (start_index)*32;
//     entry = buf + (start_index + len - 1) * 32;
//     int str_len = strlen(name);
//     memset(entry->dir_name,0x20,11);
//     for(count = str_len;name[count] != '.';count--);
//     int dot_pos = count++;
//     for(count = 0;count < 8 && count < str_len;count++) {
//         entry->dir_name[count] = name[count];
//     }
//     if (entry->dir_name[6] != 0x20) {
//         entry->dir_name[6] = '~';
//         entry->dir_name[7] = '1';
//     }
//     for(count = 8;count < 11 && count < str_len;count++)
//         entry->dir_name[count] = name[dot_pos];
//     entry->dir_fst_clus_hi = (uint16_t)(first_fat_index >> 16);
//     entry->dir_fst_clus_lo = (uint16_t)(first_fat_index & 0xffff);

//     uint8_t checksum = calc_checksum(entry->dir_name);
//     count = 0;
//     str_len++;
//     for(int i = len - 2;i >= 0;i--) {
//         lentry[i].ldir_chksum = checksum;
//         lentry[i].ldir_attr = 0x0f;
//         lentry[i].ldir_ord = len - 2 - i;
//         lentry[i].ldir_type = 0;
//         for(int j = 0;j < 5;j++) {
//             if (j < str_len)
//                 lentry[i].ldir_name_1[j] = name[count++];
//             else
//                 lentry[i].ldir_name_1[j] = NO_CHAR;
//         }
//         for(int j = 0;j < 6;j++) {
//             if (j < str_len)
//                 lentry[i].ldir_name_2[j] = name[count++];
//             else
//                 lentry[i].ldir_name_2[j] = NO_CHAR;
//         }
//         for(int j = 0;j < 2;j++) {
//             if (j < str_len)
//                 lentry[i].ldir_name_3[j] = name[count++];
//             else
//                 lentry[i].ldir_name_3[j] = NO_CHAR;
//         }
//     }
//     lentry[0].ldir_ord = 1 | FILE_NAME_END;
//     // for(count = 0;count < dentry->sector_count;count++) {
//     //     disk_write(0,buf + count*512,dentry->sectorno_list[count],1);
//     // }
// oom_error:
//     panic("out of memory!");
// }
// size_t fat32_write(fat32_fs *fat32,const char *path,const void *data,size_t size) {
//     char name[256];
//     dentry_struct *entry = NULL;
//     int i = 0;
//     int flag = 0;
//     size_t result = 0;
//     memset(name,0,256);
//     printk("fat32_write():entry\n");
//     while(1) {
//         for(int index = 0;path[i] != '/';i++,index++) {
//             name[index] = path[i];
//             if (path[i] == '\0') {
//                 flag = 1;
//                 break;
//             }
//         }
//         for(;path[i] == '/' && path[i] != '\0';i++);

//         if (path[i] == '\0')
//             flag = 1;
//         printk("write_file_name:%s\n",name);
//         if (!flag) {
//             entry = fat32_lookup(fat32,entry,name);
//             if(entry == NULL) {
//                 result = -1;
//                 break;
//             }
//         } else {
//             size_t len;
//             uint32_t *index_list;
//             printk("search index\n");
//             index_list = alloc_fat_index(fat32,size,&len);
//             // printk("entry:alloc_dentry()\n");
//             alloc_dentry(fat32,entry,name,index_list[0]);
//             // printk("alloc_dentry():ok!\n");
//             // uint32_t fat_sectorno;
//             // for(size_t i = 0;i < len;i++) {
//             //     fat_sectorno = get_fat_sectorno(fat32,index_list[i]);
//             //     disk_write(0,(uint8_t *)&fat32->fat1[(index_list[i] >> 7) << 7],fat_sectorno,1);
//             //     if (disk_write(0,data + i*512,clusno_to_sectorno(fat32,index_list[i]),1) == RES_ERROR)
//             //         break;
//             //     result += 512;
//             // }
//         }
//     }
//     return result;
// }