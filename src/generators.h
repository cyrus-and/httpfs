#ifndef _HTTPFS_GENERATORS_H
#define _HTTPFS_GENERATORS_H

#define _( x ) void httpfs_generate_##x();
#include "generators.def"

#endif
