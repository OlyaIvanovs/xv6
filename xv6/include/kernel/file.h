#ifndef XV6_INODE_H
#define XV6_INODE_H

#include "base.h"
#include "kernel/sleeplock.h"
#include "kernel/filesystem.h"

typedef struct Inode
{
    u32 dev_num; // Device number
    u32 inum;
    i32 ref;        // Reference count: counts the number ofC pointers referring to the in-memory inode,
    Sleeplock lock; // contains a sleep-lock, which ensures exclusive access to the inode’s fields

    i16 type;               // distinguishes between files, directories, and special files (devices)
    i16 nlink;              //   counts the number of directory entries that refer to a file
    u32 size;               // records the number of bytes of content in the file
    u32 addrs[NDIRECT + 1]; // records the block numbers of the disk blocks holding the file’s content

} Inode;

#endif