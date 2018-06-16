/*------------------------------------------*/
/*  Memory Overcommitment                   */
/*------------------------------------------*/

#include <doscall.h>  /* OS/2 API dynamic link lib */
#include <stdio.h>    /* C std I/O run time lib */
#include <string.h>   /* C string lib */
#include <stdlib.h>

  unsigned  ErrorCode = 0;   /* error code return from OS/2 fnct calls */
  unsigned  far *SelectorTable[256];  /* array of far ptrs */
  int SegmentCount = 0;
  int TableIndex=0;
  int MaxIndex=0;
  unsigned long PointerBuilder;  /* used to build a far ptr */
  unsigned FakeDataValue; /* used to touch a new segment */

/* DosAllocSeg vars */
  unsigned Size=0; /* means size = 64K */
  unsigned Selector;  /* selector returned */
  unsigned AllocFlags = 0;  /* not sharable or discardable */
  
/* DosExit vars */
  unsigned ActionCode = 1;  /* exit all threads in process */
  unsigned ResultCode = 0;  /* result save for DosCWait */
 
  void main(argc,argv,envp)
   int argc;             /* argc is count of arg strings */
   char *argv[ ];        /* argv array contains arg strings */
   char *envp[ ];        /* envp is env strings */
 
   {
    SegmentCount=atoi(argv[1]);         /* get # of segs to alloc */
    printf ("Number of 64K segments to allocate is %d\n",SegmentCount);
    do {            /* loop to allocate segments */
        ErrorCode=DOSALLOCSEG (Size,(unsigned far *)&Selector,
                               AllocFlags);   
        if (ErrorCode == 0)
           printf ("Successfully allocated segment %d\n",TableIndex+1);
        PointerBuilder = Selector;
        SelectorTable[TableIndex] = PointerBuilder << 16; /* put selector in
                                                             ptr table */
        ++TableIndex;
        --SegmentCount;
       }  while (SegmentCount >0);
    MaxIndex = TableIndex - 1;
    TableIndex = 0;
 
    for (;;) {  /* endless loop to touch segments */
      FakeDataValue = *SelectorTable[TableIndex];
      printf ("Just accessed segment %d\n",TableIndex+1);
      ++TableIndex;
      if (TableIndex > MaxIndex)  TableIndex = 0;
      }
 
   DOSEXIT(ActionCode,ResultCode);
   }
