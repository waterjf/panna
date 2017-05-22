#include "precomp.h"
#include "flxArray.h"

const ulong flx_default_record_cache_size = 0x4000;

/*
  Find record according to record-position.
      
  The record is located by factoring position number pos into (p_0, p_1, ...) 
  such that
     pos = SUM_i(block->level_info[i].records_under_level * p_i)
  {p_0, p_1, ...} serve as indexes to descend the blocks tree.
*/

byte *flatArrayGetAt(flatArray *block, ulong pos)
{
  int i;
  flatArrayPtrs *ptr = 0; /* block base ptr */

  for (i=block->levels-1, ptr=block->root ; i > 0 ; i--)
  {
    ptr=(flatArrayPtrs*)ptr->blocks[pos/block->level_info[i].records_under_level];
    pos%=block->level_info[i].records_under_level;
  }
  return (byte*) ptr + pos*block->record_len;
}


/*
  Get one 
  block-of-records. Alloc ptr to block if needed

  SYNOPSIS
    flatArrayExpand()
      block             flatArray tree-like block
      alloc_length OUT  Amount of memory allocated from the heap
      
  Interrupts are stopped to allow ha_panic in interrupts 
  RETURN
    0  OK
    1  Out of memory
*/

int flatArrayExpand(flatArray *block, size_t *alloc_length)
{
  uint i,j;
  flatArrayPtrs *root;
  size_t t;
  if(!alloc_length)
	  alloc_length = &t;

  for (i=0 ; i < block->levels ; i++)
    if (block->level_info[i].free_ptrs_in_block)
      break;

  /*
    Allocate space for leaf block plus space for upper level blocks up to
    first level that has a free slot to put the pointer. 
    In some cases we actually allocate more then we need:
    Consider e.g. a situation where we have one level 1 block and one level 0
    block, the level 0 block is full and this function is called. We only 
    need a leaf block in this case. Nevertheless, we will get here with i=1 
    and will also allocate sizeof(flatArrayPtrs) for non-leaf block and will never 
    use this space.
    This doesn't add much overhead - with current values of sizeof(flatArrayPtrs) 
    and my_default_record_cache_size we get about 1/128 unused memory.
   */
  *alloc_length=sizeof(flatArrayPtrs)*i+block->records_in_block* block->record_len;
  root=(flatArrayPtrs*) malloc(*alloc_length);
  if (!root)
    return 1;

  if (i == 0)
  {
    block->levels=1;
    block->root=block->level_info[0].last_blocks=root;
  }
  else
  {
    if ((uint) i == block->levels)
    {
      /* Adding a new level on top of the existing ones. */
      block->levels=i+1;
      /*
        Use first allocated flatArrayPtrs as a top-level block. Put the current
        block tree into the first slot of a new top-level block.
      */
      block->level_info[i].free_ptrs_in_block=FLATARRAY_PTRS_IN_NOD-1;
      ((flatArrayPtrs**) root)[0]= block->root;
      block->root=block->level_info[i].last_blocks= root++;
    }
    /* Occupy the free slot we've found at level i */
    block->level_info[i].last_blocks->
      blocks[FLATARRAY_PTRS_IN_NOD - block->level_info[i].free_ptrs_in_block--]=
	(byte*) root;
    
    /* Add a block subtree with each node having one left-most child */
    for (j=i-1 ; j >0 ; j--)
    {
      block->level_info[j].last_blocks= root++;
      block->level_info[j].last_blocks->blocks[0]=(byte*) root;
      block->level_info[j].free_ptrs_in_block=FLATARRAY_PTRS_IN_NOD-1;
    }
    
    /* 
      root now points to last (block->records_in_block* block->record_len)
      allocated bytes. Use it as a leaf block.
    */
    block->level_info[0].last_blocks= root;
  }
  return 0;
}

/* free all blocks under level */
static byte *flatArrayFreeLevel(flatArray *block, uint level, flatArrayPtrs *pos, byte *last_pos)
{
  int i,max_pos;
  byte *next_ptr;

  if (level == 1)
    next_ptr=(byte*) pos+block->record_len;
  else
  {
    max_pos= (block->level_info[level-1].last_blocks == pos) ?
      FLATARRAY_PTRS_IN_NOD - block->level_info[level-1].free_ptrs_in_block :
    FLATARRAY_PTRS_IN_NOD;

    next_ptr=(byte*) (pos+1);
    for (i=0 ; i < max_pos ; i++)
      next_ptr=flatArrayFreeLevel(block,level-1,
			      (flatArrayPtrs*) pos->blocks[i],next_ptr);
  }
  if ((byte*) pos != last_pos)
  {
    free(pos);
    return last_pos;
  }
  return next_ptr;			/* next memory position */
}

void flatArrayInit(flatArray *block, uint reclength, ulong min_records,
					   ulong max_records)
{
	uint i,recbuffer,records_in_block;
	
	max_records= max(min_records,max_records);
	if (!max_records)
		max_records= 1000;			/* As good as quess as anything */
	recbuffer= (uint) (reclength + sizeof(byte**) - 1) & ~(sizeof(byte**) - 1);
	records_in_block= max_records / 10;
	if (records_in_block < 10 && max_records)
		records_in_block= 10;
	if (!records_in_block || records_in_block*recbuffer >
		(flx_default_record_cache_size-sizeof(flatArrayPtrs)*FLATARRY_MAX_LEVELS))
		records_in_block= (flx_default_record_cache_size - sizeof(flatArrayPtrs) *
		      FLATARRY_MAX_LEVELS) / recbuffer + 1;
	block->records_in_block= records_in_block;
	block->record_len= recbuffer;
	block->last_allocated= 0L;
	
	for (i= 0; i <= FLATARRY_MAX_LEVELS; i++)
		block->level_info[i].records_under_level=
		(!i ? 1 : i == 1 ? records_in_block :
	FLATARRAY_PTRS_IN_NOD * block->level_info[i - 1].records_under_level);
}

void flatArrayFree(flatArray *block)
{
	if (block->levels)
		(void) flatArrayFreeLevel(block,block->levels,block->root,
		(byte*) 0);
	
	block->levels=0;
}
