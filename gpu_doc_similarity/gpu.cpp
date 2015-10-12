#include <cstdio>
#include <cstdlib>
#include <cuda_runtime.h>
#include "gpu.h"


__device__
double ComputeSimilarity( const GPU_Doc &doc1, const GPU_Doc &doc2, uint32 nTotalDocs )
{
    double lenDoc1 = doc1.length, lenDoc2 = doc2.length;     // 模，要开平方
    double weight = 0.0;                     // 分子
    double max1 = doc1.maxFreq, max2 = doc2.maxFreq;           // 最大词频
    double freq1, freq2;                     // 词频
    double retVal = 0.0;
    uint32 i = 0, j = 0;
    
    while( i < doc1.nTerms && j < doc2.nTerms ) {
        const GPU_Term &term1 = doc1.terms[i];
        const GPU_Term &term2 = doc2.terms[j];
        freq1 = term1.count;
        freq2 = term2.count;
        if( term1.termNO < term2.termNO ) {
            ++i;
        } else if( term1.termNO > term2.termNO ) {
            ++j;
        } else {
            weight += freq1 * freq2 * term1.idf2;
            ++i; ++j;
        } // if
    } // while
    
    weight /= max1 * max2;
    retVal = weight / ( lenDoc1 * lenDoc2 );
    return retVal;
}


__global__
void DeviceRoutine( const GPU_Doc *pDocs, GPU_Result *pResults, const uint32 nTotalDocs )
{
    uint32 index = blockDim.x * blockIdx.x + threadIdx.x;
    uint32 i = pResults[index].docNO1;
    uint32 j = pResults[index].docNO2;  //!! should shared memory
    if( i == j ) {
//        printf( "Invalid task!\n" );
        return;
    }

    //    printf( "DeviceRoutine working %u, %u.\n", i, j );
    pResults[index].docNO1 = pDocs[i].docNO;         // write back result
    pResults[index].docNO2 = pDocs[j].docNO;
    double ans = ComputeSimilarity( pDocs[i], pDocs[j], nTotalDocs );
    //    printf( "ans = %lf\n", ans );
    pResults[index].similarity = ans;
//    printf( "Similarity between %u and %u is %lf\n", pResults[index].docNO1, pResults[index].docNO2, pResults[index].similarity );
}


//__global__
//void DeviceRoutine( const GPU_Doc *pDocs, const uint32 nDocs, GPU_Result* pResults,
//               const uint32 nResults, const uint32 nTotalDocs )
//{
//    uint32 index = blockDim.x * blockIdx.x + threadIdx.x;
//    if( index >= nResults ) {
////        printf( "thread index out of range %u!\n", index );
//        return;
//    } // if
//    uint32 i = pResults[index].docNO1;
//    uint32 j = pResults[index].docNO2;  //!! should shared memory
//    if( i == j ) {
////        printf( "Invalid task!\n" );
//        return;
//    }
//    if( i >= nDocs || j >= nDocs ) {
////        printf( "doc index out of range %u!\n", index );
//        return;
//    } // if
//    if( pDocs[i].nTerms == 0 || pDocs[j].nTerms == 0 )
//        return;             // empty padding doc
////    printf( "DeviceRoutine working %u, %u.\n", i, j );
//    pResults[index].docNO1 = pDocs[i].docNO;         // write back result
//    pResults[index].docNO2 = pDocs[j].docNO;
//    double ans = ComputeSimilarity( pDocs[i], pDocs[j], nTotalDocs );
////    printf( "ans = %lf\n", ans );
//    pResults[index].similarity = ans;
//    printf( "Similarity between %u and %u is %lf\n", pResults[index].docNO1, pResults[index].docNO2, pResults[index].similarity );
//}


//int InvokeDeviceWorkFunc( const uint32 nBlocks, const uint32 nThreadsPerBlock,
//                const GPU_Doc *pDocs, const uint32 nDocs, GPU_Result *pResults,
//                const uint32 nResults, const uint32 nTotalDocs )
//{
//    cudaError_t err = cudaSuccess;
//    
//    printf( "InvokeDeviceWorkFunc nBlocks = %u, nThreadsPerBlock = %u, nDocs = %u, nResults = %u, nTotalDocs = %u\n",
//                nBlocks, nThreadsPerBlock, nDocs, nResults, nTotalDocs );
//    
//    DeviceRoutine <<< nBlocks, nThreadsPerBlock >>> ( pDocs, nDocs, pResults,
//                             nResults, nTotalDocs );
//    
//    err = cudaGetLastError();
//    return err == cudaSuccess ? 0 : -1;
//}


int InvokeDeviceWorkFunc( const uint32 nBlocks, const uint32 nThreadsPerBlock,
                                const GPU_Doc *pDocs, GPU_Result *pResults, const uint32 nTotalDocs )
{
    cudaError_t err = cudaSuccess;

//    printf( "InvokeDeviceWorkFunc nBlocks = %u, nThreadsPerBlock = %u, nTotalDocs = %u\n", nBlocks, nThreadsPerBlock, nTotalDocs );
    DeviceRoutine <<< nBlocks, nThreadsPerBlock >>> ( pDocs, pResults, nTotalDocs );

    err = cudaGetLastError();
    return err == cudaSuccess ? 0 : -1;
}


void *DeviceMalloc( size_t size )
{
    void *p = NULL;
    cudaError_t err = cudaMalloc(&p, size);
    
    if (err != cudaSuccess) {
        fprintf(stderr, "Failed to allocate device memory: %s\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }
    
    return p;
}


void DeviceMemFree( void *p )
{
    cudaError_t err = cudaFree( p );
    if (err != cudaSuccess) {
        fprintf(stderr, "Fail to free device memory: %s\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    } // if
}


void CopyToDevice( void *dDst, void *hSrc, size_t size )
{
    cudaError_t err = cudaMemcpy( dDst, hSrc, size, cudaMemcpyHostToDevice );
    
    if (err != cudaSuccess) {
        fprintf(stderr, "Failed to copy to device: %s\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    } // if
}


void CopyFromDevice( void *hDst, void *dSrc, size_t size )
{
    cudaError_t err = cudaMemcpy( hDst, dSrc, size, cudaMemcpyDeviceToHost );

    if (err != cudaSuccess) {
        fprintf(stderr, "Failed to copy from device: %s\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    } // if
}


void SyncDevice()
{
    cudaDeviceSynchronize();
}











