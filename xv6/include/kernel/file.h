#ifndef XV6_INODE_H
#define XV6_INODE_H

#include "base.h"
#include "kernel/sleeplock.h"
#include "kernel/filesystem.h"

// Inode is in-memory copy of a struct dinode on disk.
typedef struct Inode
{
    u32 dev_num; // Device number
    u32 inum;
    i32 ref;        // Reference count: counts the number ofC pointers referring to the in-memory inode,
    Sleeplock lock; // contains a sleep-lock, which ensures exclusive access to the inode’s fields
    int valid;      // inode has been read from disk?

    i16 type; // distinguishes between files, directories, and special files (devices), 0 if on-disk inode is free.
    i16 major;
    i16 minor;
    i16 nlink;              // counts the number of directory entries that refer to a file
    u32 size;               // records the number of bytes of content in the file
    u32 addrs[NDIRECT + 1]; // records the block numbers of the disk blocks holding the file’s content

} Inode;

// table mapping major device number to
// device functions
typedef struct DeviceOperations
{
    int (*read)(Inode *, char *, int);
    int (*write)(Inode *, char *, int);
} DeviceOperations;

extern DeviceOperations devices[];

#define CONSOLE 1

#endif