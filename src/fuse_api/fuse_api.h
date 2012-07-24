#ifndef _PHPFS_FUSE_API_H
#define _PHPFS_FUSE_API_H

int phpfs_getattr( const char * , struct stat * );
int phpfs_readlink( const char * , char * , size_t );
int phpfs_mknod( const char * , mode_t , dev_t );
int phpfs_mkdir( const char * , mode_t );
int phpfs_unlink( const char * );
int phpfs_rmdir( const char * );
int phpfs_symlink( const char * , const char * );
int phpfs_rename( const char * , const char * );
int phpfs_link( const char * , const char * );
int phpfs_chmod( const char * , mode_t );
int phpfs_chown( const char * , uid_t , gid_t );
int phpfs_truncate( const char * , off_t );
int phpfs_utime( const char * , struct utimbuf * );
int phpfs_open( const char * , struct fuse_file_info * );
int phpfs_read( const char * , char * , size_t , off_t , struct fuse_file_info * );
int phpfs_write( const char * , const char * , size_t , off_t , struct fuse_file_info * );
int phpfs_statfs( const char * , struct statvfs * );
int phpfs_flush( const char * , struct fuse_file_info * );
int phpfs_release( const char * , struct fuse_file_info * );
int phpfs_fsync( const char * , int , struct fuse_file_info * );
int phpfs_setxattr( const char * , const char * , const char * , size_t , int );
int phpfs_getxattr( const char * , const char * , char * , size_t );
int phpfs_listxattr( const char * , char * , size_t );
int phpfs_removexattr( const char * , const char * );
int phpfs_opendir( const char * , struct fuse_file_info * );
int phpfs_readdir( const char * , void * , fuse_fill_dir_t , off_t , struct fuse_file_info * );
int phpfs_releasedir( const char * , struct fuse_file_info * );
int phpfs_fsyncdir( const char * , int , struct fuse_file_info * );
int phpfs_access( const char * , int );
int phpfs_create( const char * , mode_t , struct fuse_file_info * );
int phpfs_ftruncate( const char * , off_t , struct fuse_file_info * );
int phpfs_fgetattr( const char * , struct stat * , struct fuse_file_info * );
int phpfs_lock( const char * , struct fuse_file_info * , int , struct flock * );
int phpfs_utimens( const char * , const struct timespec [2] );
int phpfs_bmap( const char * , size_t , uint64_t * );
int phpfs_ioctl( const char * , int , void * , struct fuse_file_info * , unsigned int , void * );
int phpfs_poll( const char * , struct fuse_file_info * , struct fuse_pollhandle * , unsigned * );

#endif
