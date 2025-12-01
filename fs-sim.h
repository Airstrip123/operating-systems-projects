/**
 * fs-sim.h
 * 
 * CMPUT 379 - Assignment 3
 * UNIX File System Simulator - Header File
 * 
 * This header defines the core data structures for a simulated UNIX-like
 * file system. The file system uses:
 * - A superblock containing free-space bitmap and inodes
 * - Contiguous block allocation for files
 * - Hierarchical directory structure
 * 
 * CRITICAL IMPLEMENTATION NOTES:
 * 1. isused_size packs TWO values in 1 byte:
 *    - Bit 7: inode state (1=used, 0=free)
 *    - Bits 0-6: file size in blocks (0-127)
 * 
 * 2. isdir_parent packs TWO values in 1 byte:
 *    - Bit 7: inode type (1=directory, 0=file)
 *    - Bits 0-6: parent inode index (0-125, or 127 for root)
 * 
 * 3. File names can be EXACTLY 5 characters with NO null terminator
 */

#ifndef FS_SIM_H
#define FS_SIM_H

#include <stdint.h>

/**
 * Inode Structure (8 bytes)
 * 
 * Represents metadata for a single file or directory.
 * Use __attribute__((packed)) to prevent compiler padding
 * 
 * FIELDS:
 * - name[5]: Up to 5 alphanumeric characters (may not be null-terminated)
 * - isused_size: Packed byte (bit 7 = used flag, bits 0-6 = size)
 * - start_block: Index of first data block (1-127 for files, 0 for dirs)
 * - isdir_parent: Packed byte (bit 7 = is_dir flag, bits 0-6 = parent index)
 */
 typedef struct __attribute__((packed)){
    char name[5];         // name of the file/directory
    uint8_t isused_size;  // state of inode and size of the file/directory
    uint8_t start_block;  // index of the first block of the file/directory
    uint8_t isdir_parent; // type of inode and index of the parent inode
} Inode;

/**
 * Superblock Structure (1024 bytes)
 * 
 * Use __attribute__((packed)) to prevent compiler padding
 * 
 * Block 0 of the disk, containing:
 * - Free-space bitmap (16 bytes = 128 bits for 128 blocks)
 * - Array of 126 inodes (126 * 8 = 1008 bytes)
 * Total: 16 + 1008 = 1024 bytes (exactly 1 block)
 * 
 * FREE-SPACE LIST:
 * - Each bit represents one block (bit 0 = block 0, etc.)
 * - 0 = free, 1 = used
 * - Stored as byte array (little-endian bit ordering within each byte)
 */
typedef struct __attribute__((packed)) {
    uint8_t free_block_list[16];
    Inode inode[126];
} Superblock;

/* Function prototypes - implementations in fs-sim.c */
void fs_mount(char *new_disk_name);
void fs_create(char name[5], int size);
void fs_delete(char name[5]);
void fs_read(char name[5], int block_num);
void fs_write(char name[5], int block_num);
void fs_buff(uint8_t buff[1024]);
void fs_ls(void);
void fs_defrag(void);
void fs_cd(char name[5]);

#endif /* FS_SIM_H */