#ifndef _HTTPFS_GENERATORS_H
#define _HTTPFS_GENERATORS_H

struct httpfs_generator
{
    const char *name;
    void ( *function )();
};

extern const struct httpfs_generator HTTPFS_GENERATORS[];

int httpfs_generate( const char *name );

#define _( x ) void httpfs_generate_##x();
#include "generators.def"

#endif
