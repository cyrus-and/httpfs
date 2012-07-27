#ifndef _HTTPFS_FUSE_API_H
#define _HTTPFS_FUSE_API_H

int httpfs_getattr( const char * , struct stat * );
int httpfs_readlink( const char * , char * , size_t );
int httpfs_mknod( const char * , mode_t , dev_t );
int httpfs_mkdir( const char * , mode_t );
int httpfs_unlink( const char * );
int httpfs_rmdir( const char * );
int httpfs_symlink( const char * , const char * );
int httpfs_rename( const char * , const char * );
int httpfs_link( const char * , const char * );
int httpfs_chmod( const char * , mode_t );
int httpfs_chown( const char * , uid_t , gid_t );
int httpfs_truncate( const char * , off_t );
int httpfs_utime( const char * , struct utimbuf * );
int httpfs_open( const char * , struct fuse_file_info * );
int httpfs_read( const char * , char * , size_t , off_t , struct fuse_file_info * );
int httpfs_write( const char * , const char * , size_t , off_t , struct fuse_file_info * );
int httpfs_statfs( const char * , struct statvfs * );
int httpfs_flush( const char * , struct fuse_file_info * );
int httpfs_release( const char * , struct fuse_file_info * );
int httpfs_fsync( const char * , int , struct fuse_file_info * );
int httpfs_setxattr( const char * , const char * , const char * , size_t , int );
int httpfs_getxattr( const char * , const char * , char * , size_t );
int httpfs_listxattr( const char * , char * , size_t );
int httpfs_removexattr( const char * , const char * );
int httpfs_opendir( const char * , struct fuse_file_info * );
int httpfs_readdir( const char * , void * , fuse_fill_dir_t , off_t , struct fuse_file_info * );
int httpfs_releasedir( const char * , struct fuse_file_info * );
int httpfs_fsyncdir( const char * , int , struct fuse_file_info * );
int httpfs_access( const char * , int );
int httpfs_create( const char * , mode_t , struct fuse_file_info * );
int httpfs_ftruncate( const char * , off_t , struct fuse_file_info * );
int httpfs_fgetattr( const char * , struct stat * , struct fuse_file_info * );
int httpfs_lock( const char * , struct fuse_file_info * , int , struct flock * );
int httpfs_utimens( const char * , const struct timespec [2] );
int httpfs_bmap( const char * , size_t , uint64_t * );
int httpfs_ioctl( const char * , int , void * , struct fuse_file_info * , unsigned int , void * );
int httpfs_poll( const char * , struct fuse_file_info * , struct fuse_pollhandle * , unsigned * );

#endif
