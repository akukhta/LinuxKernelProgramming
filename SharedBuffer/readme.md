Shared Buffer - simple characteric driver which implements shared memory between processes.

It represents list of buffers:

*********    *********    *********
*Block 1* -> *Block 2* -> *Block 3*
*********    *********    *********

Reading returns block 1

Writing adds a block to the end of list:

*********    *********    *********
*Block 2* -> *Block 3* -> *Block 4*
*********    *********    *********

Implemented sys calls:
1. Read
2. Write
3. IOCTL (works as network module - it returns size of the current subbuffer to read)
4. fsync - to read ALL blocks to one buffer (In progress)
5. poll - to wait when read or write operation can be done (in progress)
6. mmap - to use buffer as continuous memory area (in progress)

Write operation is blocking operation and can put read procosses into sleep 


This module has been created only for educational purposes.

