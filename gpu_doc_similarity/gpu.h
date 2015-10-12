#ifndef __GPU_H
#define __GPU_H


#define N_THREADS_PER_BLOCK             256
#define N_THREADS_PER_BATCH             4096
#define N_MAX_ITEM_PER_DOC              1248        // 事后统计得出


typedef unsigned int            uint32;


struct GPU_Term {
    uint32          termNO;
    uint32          count;
    uint32          numDocs;
    double          idf2;
};

struct GPU_Doc {
    uint32      docNO;
    uint32      nTerms;
    uint32      maxFreq;
    double      length;
    GPU_Term    terms[N_MAX_ITEM_PER_DOC];
};

// n*(n-1)/2
struct GPU_Result {
    uint32              docNO1;
    uint32              docNO2;
    double              similarity;
};


extern void *DeviceMalloc( size_t size );
extern void DeviceMemFree( void *p );
extern void CopyToDevice( void *dDst, void *hSrc, size_t size );
extern void CopyFromDevice( void *hDst, void *dSrc, size_t size );
extern void SyncDevice();
extern int InvokeDeviceWorkFunc( const uint32 nBlocks, const uint32 nThreadsPerBlock,
                    const GPU_Doc *pDocs, GPU_Result *pResults, const uint32 nTotalDocs );




#endif













