#include "memlib.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "mm.h"
#include "hw4.h"

struct node * root_table[ROOT_NR] = {};

int in_use(unsigned int * hdrp) {
  return (*hdrp) & 0x1u;
}

// the three lowest bits are not part of the size
// 0x1 is already used for free blocks; we use 0x2
#define MARK_BIT (((unsigned int)0x2))

// marking related operations
int is_marked(unsigned int * hdrp) {
  return ((*hdrp) & MARK_BIT) >> 1; // return 1 or 0
}

void mark(unsigned int * hdrp) {
  (*hdrp) |= MARK_BIT;
}

void unmark(unsigned int * hdrp) {
  (*hdrp) &= ~MARK_BIT;
}

// same to GET_SIZE in mm.c
unsigned int block_size(unsigned int * hdrp) {
  return (*hdrp) & (~7u);
}

void* next_hdrp(unsigned int * hdrp) {
  const unsigned int size = block_size(hdrp);
  if (size == 0) {
    fprintf(stderr,"%s Panic, chunk is of zero size.\n", __func__);
    exit(-1);
  }
  hdrp = (unsigned int *)(((char *)hdrp) + size);
  return block_size(hdrp) ? hdrp : NULL;
}

void heap_stat(const char * msg)
{
  void *hdrp = mm_first_hdr();
  size_t nr_inuse = 0;
  size_t sz_inuse = 0;
  size_t nr_free = 0;
  size_t sz_free = 0;

  while (hdrp && block_size(hdrp)) {
    if (in_use(hdrp)) {
      nr_inuse++;
      sz_inuse += block_size(hdrp);
    } else {
      nr_free++;
      sz_free += block_size(hdrp);
    }
    if (is_marked(hdrp))
      printf("%s WARNING: found a mark\n", __func__);
    hdrp = next_hdrp(hdrp);
  }

  printf("%s: %s: heapsize %zu  inuse %zu %zu  free %zu %zu\n", __func__, msg,
      mem_heapsize(), nr_inuse, sz_inuse, nr_free, sz_free);
}

// ========
// add helper functions here if any
void mark() 
{ 
	if ((b = isPtr(p)) == NULL)
		return; 
	if (blockMarked(b)) 
		return; 
	markBlock(b); 
	len = length(b); 
	for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) { 
		mark(b[i]); 
	return; 
}
	unsigned int len;
#define DSIZE 8
#define PREV_BLKP(bp) ((char *)(bp)-block_size(((unsigned int *)(bp)-DSIZE)))
static char *heap_listp = 0;
void mark_function() 
{ 	
	
	void *bp = mm_first_hdr();
	static unsigned int len = 0; 
	if(len==block_size((unsigned int*)HDRP(bp)){ printf("weird.\n"); return; }
	if (is_marked(bp)) 
		return; 
	mark(bp); 
	//len = block_size((unsigned int*) bp); 
	for (bp = heap_listp; block_size((unsigned int*)HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) { 
		mark_function(bp); }
	return; 
}

#define WSIZE 4
#define DSIZE 8
#define NEXT_BLKP(bp) ((char *)(bp) + block_size(((unsigned int *)(bp)-WSIZE)))
static char *heap_listp = 0;
#define HDRP(bp) ((char *)(bp)-WSIZE)

// garbage collection: Mark and Sweep
void gc() {
  void *bp = mm_first_hdr();
 unsigned int len = 0;
 printf("%d\n",block_size(bp));
  while(len !=block_size(bp))
  {
    mark(bp);
    bp = next_hdrp(bp);
    len+=4;
    printf("marking\n");
  }
  printf("completed marking");
  
  sweep();
}
void mark_function()
{
	void* hdpr = mm_first_hdr();
	int i=0;
	while(hdpr && block_size(hdrp))
	{
		while(bp!=NULL)
		{
			void*bp=root_table[i];
			
			if(bp && !is_marked(bp))
				mark(bp);
				
			bp = root_table[i]->next;
			i++;
		}
		hdpr=next_hdrp(hdpr);
	}
}

void mark_function(void* hdrp, void* end)
{
	while(hdrp < (void*)mem_heapsize(hdrp))
	{
		void*bp=mm_first_hdr();
		
		if(bp && !is_marked(bp))
			mark(bp);
				
		mark_function(bp ,next_hdrp(hdrp)))
	}
}

void gc()
{
	void*bp;
	bp=mm_first_hdr();
	mark_function(bp, block_size(bp));
	sweep();
}
