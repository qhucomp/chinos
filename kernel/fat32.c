#include <stdint.h>
#include <stddef.h>
#include "include/fat32.h"
#include "include/diskio.h"
#include "include/printk.h"
#include "include/kmalloc.h"
#include "include/vfs.h"
#include "include/string.h"

static size_t fat32_read_file(dentry_struct *p,void *buf,size_t size) {
    size_t result = 0;
    for(size_t i = 0;i < p->sector_count;i++) {
        if (result >= size)
            break;
        if (disk_read(0,buf + i*512,p->sectorno_list[i],1) == RES_ERROR)
            break;
        result += 512;
    }
    return result;
}

/**
 * @brief 从长目录项里读取文件名
 * 
 * @param[in] dentry要被读取的目录项
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

dentry_struct *fat32_lookup(fat32_fs *fs,dentry_struct *dentry,const char *name);
fat32_fs *fat32_init(void) {
    mbr_struct mbr;
    fat32_fs *fs =  kmalloc(sizeof(fat32_fs));
    if(disk_read(0,(uint8_t *)&mbr,0,1) == RES_ERROR)
        goto error;
    if(disk_read(0,(uint8_t *)&fs->boot,mbr.parts[0].lba,1) == RES_ERROR)
        goto error;
    rw_count = 0;
    uint32_t count;
    if (fs->boot.bpb_fatsz32 > DEFAULT_LOAD_SECTOR)    //最多加载128个扇区
        count = 128;
    else
        count = fs->boot.bpb_fatsz32;

    fs->fat1 = kmalloc(512*count);
    fs->start_fat_sector = mbr.parts[0].lba + fs->boot.bpb_reserved_sec_cnt;
    fs->count = count;
    fs->data_sector = fs->start_fat_sector + fs->boot.bpb_fatsz32*fs->boot.bpb_num_fats;
    if(disk_read(0,(uint8_t *)fs->fat1,mbr.parts[0].lba + fs->boot.bpb_reserved_sec_cnt,count) == RES_ERROR)
        goto error;
    if(disk_read(0,(uint8_t *)&fs->fs_info,mbr.parts[0].lba + 1,1) == RES_ERROR)
        goto error;

    printk("fat LBA:%d,fat sector count:%d,data LBA:%d\n",fs->start_fat_sector,fs->count,fs->data_sector);
    printk("num fats:%d,fatsz32:%d\n",(uint32_t)fs->boot.bpb_num_fats,fs->boot.bpb_fatsz32);
    printk("FAT32 init..........OK\n");
    return fs;
error:
    panic("out of memory!");
}

size_t fat32_write(fat32_fs *fat32,const char *path,const void *data,size_t size) {
    return 1;
}

uint32_t clusno_to_sectorno(fat32_fs *fs,uint32_t clusno) {
    return (clusno - 2)*fs->boot.bpb_sec_per_clus + fs->data_sector;
}

uint32_t get_fat_sectorno(fat32_fs *fs,uint32_t no) {
    return fs->start_fat_sector + (no >> 7);
}

uint32_t get_fat_no(uint32_t fat_no) {
    return fat_no >> 9;
}

//需要优化
dentry_struct *fat32_lookup(fat32_fs *fs,dentry_struct *dentry,const char *name) {
    dentry_struct *dentry_p = NULL;
    int root_size = 1;
    char *root_buf = NULL;
    short_dir_entry *entry = NULL;
    if (dentry == NULL) {
        // 加载根目录下的所有目录项
        int next = 2;
        dentry = create_dentry();
        push_sectorno(dentry,clusno_to_sectorno(fs,next));

        if (fs->fat1[2] < FILE_END) {
            while (1) {
                if (next < fs->count) {
                    // 根目录FAT索引未超过缓存好的FAT的情况
                    push_sectorno(dentry,clusno_to_sectorno(fs,fs->fat1[next]));
                    root_size++;
                } else {
                    if (next > FILE_END)
                        break;
                    // 根目录的FAT索引超过缓存好的FAT的情况
                    if(disk_read(0,(uint8_t *)fs->temp_fat,get_fat_sectorno(fs,next),4) == RES_ERROR) {
                        kfree(dentry->sectorno_list);
                        kfree(dentry);
                        goto read_disk_error;
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
    root_buf = kmalloc(512*dentry->sector_count);
    if (root_buf == NULL)
        goto oom_error;
    for(uint32_t count = 0;count < dentry->sector_count;count++)
        if (disk_read(0,(uint8_t *)root_buf + 512*count,dentry->sectorno_list[count],1) == RES_ERROR)
            goto read_disk_error;

    //搜索文件
    for(uint32_t count = 0;count < 16*dentry->sector_count;count++) {
        entry = (void *)(root_buf +count*32);
        if (is_short_entry(entry)) {
            char *dname = get_entry_name((void *)(root_buf + count*32));
            if (!strncmp(name,dname,MAX_NAME_LEN)) {
                dentry_p = create_dentry();
                dentry_p->name = dname;
                uint32_t sector_index = 0;
                uint32_t clusno = ((uint32_t)entry->dir_fst_clus_hi << 16) | (uint32_t)entry->dir_fst_clus_lo;
                uint32_t next_clusno = clusno;

                push_sectorno(dentry_p,clusno_to_sectorno(fs,clusno));
                while(1) {
                    if (next_clusno < fs->count*128) {
                        push_sectorno(dentry_p,clusno_to_sectorno(fs,fs->fat1[next_clusno]));
                        next_clusno = fs->fat1[next_clusno];
                    } else {
                        if (next_clusno >= FILE_END)
                            goto end;

                        if(disk_read(0,(uint8_t *)fs->temp_fat,get_fat_sectorno(fs,clusno),4) == RES_ERROR) {
                            kfree(dentry_p->sectorno_list);
                            kfree(dentry_p);
                            goto read_disk_error;
                        }
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
                kfree(dname);
            }
        }
    }
end:
    kfree(root_buf);
    return dentry_p;

read_disk_error:
    panic("read disk failed");
oom_error:
    panic("out of memory!");
}

size_t fat32_read(fat32_fs *fat32,const char *path,const void *data,size_t size) {
    // 未使用
    return fat32_read_file(NULL,NULL,0);
}