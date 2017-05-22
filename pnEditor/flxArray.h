#ifndef flxArray_h__
#define flxArray_h__

#include "pxDef.h"

#define FLATARRY_MAX_LEVELS	4		/* 128^5 records is enough */
#define FLATARRAY_PTRS_IN_NOD	128

/* Structs used by heap-database-handler */
typedef struct
{
  byte *blocks[FLATARRAY_PTRS_IN_NOD];		/* pointers to flatArrayPtrs or records */
} flatArrayPtrs;

struct flatArrayLevelInfo
{
  /* Number of unused slots in *last_blocks flatArrayPtrs block (0 for 0th level) */
  uint free_ptrs_in_block;
  
  /*
    Maximum number of records that can be 'contained' inside of each element
    of last_blocks array. For level 0 - 1, for level 1 - FLATARRAY_PTRS_IN_NOD, for 
    level 2 - FLATARRAY_PTRS_IN_NOD^2 and so forth.
  */
  ulong records_under_level;

  /*
    Ptr to last allocated flatArrayPtrs (or records buffer for level 0) on this 
    level.
  */
  flatArrayPtrs *last_blocks;			
};

typedef struct 
{
	flatArrayPtrs *root;                        /* Top-level block */ 
	struct flatArrayLevelInfo level_info[FLATARRY_MAX_LEVELS+1];
	uint levels;                          /* number of used levels */
	uint records_in_block;		/* Records in one heap-block */
	uint record_len;			/* Length of one saved record */
	ulong last_allocated; /* number of records there is allocated space for */
} flatArray;

void flatArrayInit(flatArray *block, uint reclength, ulong min_records, ulong max_records);
void flatArrayFree(flatArray *block);
byte *flatArrayGetAt(flatArray *block, ulong pos);
int flatArrayExpand(flatArray *block, size_t *alloc_length);

#endif // flxArray_h__
