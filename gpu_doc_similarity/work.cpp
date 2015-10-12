#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <climits>
#include <algorithm>
#include <functional>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "work.h"
#include "gpu.h"

#define N_CPU               sysconf( _SC_NPROCESSORS_ONLN )
#define BLANK_CHAR          " \t\f\r\v\n"


using namespace std;

/* global vars */
DocCollection       docCollection;
TermWordSet         stopWords;

struct ResultItem {
    ResultItem() : pDoc( DocumentPtr((Document*)NULL) ), similarity(0.0) {}
//    ResultItem( uint32 i ) : pDoc(docCollection[i]), similarity(0.0) {}
//    ResultItem( const DocumentPtr _pDoc, double _similarity ) : pDoc(_pDoc), similarity(_similarity) {}
    bool operator > ( const ResultItem &rhs ) const
    { return similarity > rhs.similarity; }
    
    DocumentPtr         pDoc;
    double              similarity;
};

typedef std::vector< ResultItem, ALLOCATOR(ResultItem) > ResultArray;
typedef std::vector< ResultArray, ALLOCATOR(ResultArray) > ResultMatrix;
ResultMatrix            resultMatrix;

/* End global vars */



/* global functions */

char* RstripString( char *s )
{
    char *p = s + strlen(s)-1;
    while( p >=s && isspace(*p) )
        *p-- = 0;
    return s;
}

inline
void AddDoc( const DocumentPtr pDoc )
{ docCollection.push_back( pDoc ); }

inline
uint32 GetNumDocs()
{ return docCollection.size(); }


void AddWord( const char *word, DocumentPtr pDoc )
{
    typedef std::pair<TermWordSet::iterator, bool> TermWordSetInsertRetType;
    
    static uint32           termIndex = 0;
    
    TermPtr pTerm = Term::newInstance( word, termIndex );
    TermWordSetInsertRetType ret = termWordSet.insert(pTerm); // 插入到全局词表
    if( ret.second ) {              // success inserted new word
        pTerm->addDoc( pDoc->getNO() );
        pDoc->addTerm( pTerm );
        termCollection.push_back( pTerm );
        ++termIndex;
        return;
    } // if
    
    // modify existing
    pTerm = *(ret.first);
    pTerm->addDoc( pDoc->getNO() );
    pDoc->addTerm( pTerm );
}
/* End global functions */


#ifdef _DEBUG
std::ostream& operator << ( std::ostream &os, const Term &term )
{
    os << "TermNO " << term.getNO() << ": " << term.getWord() << " DocSet: ";
    for( Term::DocSet::const_iterator it = term.getDocSet().begin(); it != term.getDocSet().end(); ++it )
        os << *it << " ";
    return os;
}

std::ostream& operator << ( std::ostream &os, const Document &doc )
{
    os << "DocNO " << doc.getNO() << ": ";
    const Document::TermCountSet &terms = doc.getTermCount();
    for( Document::TermCountSet::const_iterator it = terms.begin(); it != terms.end(); ++it )
        os << (*it)->getTerm()->getNO() << ":" << (*it)->getTerm()->getWord() << "(" << (*it)->getCount() << ") ";
    return os;
}

void PrintWordSet()
{
    for( TermWordSet::const_iterator it = termWordSet.begin(); it != termWordSet.end(); ++it )
        cout << **it << endl;
}

void PrintAllDocs()
{
    for( DocCollection::const_iterator it = docCollection.begin(); it != docCollection.end(); ++it )
        cout << **it << endl;
}

#endif



void TestCase1( const char *filename )
{
    uint32 count = 0;
    char buf[20];
    ifstream in( filename );
    String line;
    while( getline(in, line) ) {
        cout << line << endl;
        sprintf( buf, "Doc%u", count );
        DocumentPtr pDoc = Document::newInstance( count++, buf );
        for( char *p = strtok( const_cast<char*>(line.c_str()), BLANK_CHAR); p; p = strtok(NULL, BLANK_CHAR) )
            AddWord( p, pDoc );
        AddDoc( pDoc );
    } // while
    
    PrintWordSet();
    cout << endl;
    PrintAllDocs();
    
//    ComputeDocSimilarity( docCollection[0], docCollection[1] );
}


void LoadStopWords( const char *filename )
{
    char buf[40];
    FILE *fp;
    
    if( (fp = fopen(filename, "r")) == NULL ) {
        printf("Cannot open file %s\n", filename);
        exit(-1);
    } // if
    
    while( fgets(buf, 40, fp) ) {
        if( *RstripString(buf) )
            stopWords.insert( Term::newInstance(buf, 0) );
    } // while
    
    fclose(fp);
    
    // dbg
//    for( TermWordSet::const_iterator it = stopWords.begin(); it != stopWords.end(); ++it )
//        cout << (*it)->getWord() << endl;
//    cout << stopWords.size() << endl;
}


inline
bool IsStopWord( const char *word )
{ return stopWords.find(Term::newInstance(word, 0)) != stopWords.end(); }


// 文档编号从0开始，便于查找
void LoadDocs( const char *filename )
{
    ifstream inFile( filename );
    
    if( !inFile ) {
        printf("Cannot open file %s\n", filename);
        exit(-1);
    }
    
    char curDocID[16];
    char *prefix, *word, *pos;
    memset( curDocID, 0, sizeof(curDocID) );
    
    String line;
    uint32 docNO = 0;
    DocumentPtr pDoc;
    
    while( getline(inFile, line) ) {
        char *str = const_cast<char*>(line.c_str());
        if( !(*RstripString(str)) )
            continue;
        prefix = strtok(str, BLANK_CHAR);
        if( strncmp(prefix, curDocID, 15) ) {
            strncpy( curDocID, prefix, 15 );
            pDoc = Document::newInstance( docNO++, curDocID );
            AddDoc( pDoc );
        } // if
        
        // 分词
        while( (word = strtok(NULL, BLANK_CHAR)) ) {
            pos = strrchr(word, '/');
            *pos = 0;
//#ifdef _DEBUG
//            if( IsStopWord(word) )
//                cout << "Find stop word: " << word << endl;
//#endif
            if( !IsStopWord(word) && strcmp(pos+1, "w") )
                AddWord( word, pDoc );
        } // while
    } // while
    
//    DBG("total %u docs", GetNumDocs());
}



typedef std::pair<uint32, uint32> JobArg;
typedef std::list< JobArg, ALLOCATOR(JobArg) > JobList;
std::vector< pthread_t, ALLOCATOR(pthread_t) >  threadList(N_CPU, 0);
std::vector< JobList, ALLOCATOR(JobList) >      jobList(N_CPU);



void* GetSimilarityMatrix_ThreadFuncSort( void *arg )
{
    uint32 n = *(uint32*)(&arg);

    while( !(jobList[n].empty()) ) {
        uint32 i = jobList[n].front().first;
        jobList[n].pop_front();
        sort( resultMatrix[i].begin(), resultMatrix[i].end(), greater<ResultItem>() );
    } // while
    
    return (void*)0;
}


// 初始化发往设备的Doc数据结构
void InitDevDocs( void *pDevDocs )
{
    typedef std::vector< GPU_Doc, ALLOCATOR(GPU_Doc) >      GPU_DocCollection;
    GPU_DocCollection               gpuDocCollection( GetNumDocs() );
    
    memset( &gpuDocCollection[0], 0, sizeof(GPU_Doc)*GetNumDocs() );
    
    uint32 index;
    for( uint32 i = 0; i < GetNumDocs(); ++i ) {
        gpuDocCollection[i].docNO = docCollection[i]->getNO();
        gpuDocCollection[i].nTerms = docCollection[i]->getNumTerms();
        gpuDocCollection[i].length = docCollection[i]->getLength();
        gpuDocCollection[i].maxFreq = docCollection[i]->getMaxFreq();
        index = 0;
        for( Document::TermCountSet::const_iterator termIt = docCollection[i]->getTermCount().begin();
                                termIt != docCollection[i]->getTermCount().end(); ++termIt ) {
            gpuDocCollection[i].terms[index].termNO = (*termIt)->getTerm()->getNO();
            gpuDocCollection[i].terms[index].count = (*termIt)->getCount();
            gpuDocCollection[i].terms[index].numDocs = (*termIt)->getTerm()->getNumDoc();
            gpuDocCollection[i].terms[index].idf2 = (*termIt)->getTerm()->getIDF2();
            if( ++index == N_MAX_ITEM_PER_DOC ) // 最多取这么多词
                break;
        } // for term
    } // for i
    
    CopyToDevice( pDevDocs, (void*)&gpuDocCollection[0], sizeof(GPU_Doc)*GetNumDocs() );
}


typedef std::vector< GPU_Result, ALLOCATOR(GPU_Result) > GPU_ResultArray;
void GetResultsFromDevice( GPU_ResultArray &results, void *pDevResults, void *pDevDocs )
{
    uint32 n = results.size();
    CopyToDevice( pDevResults, (void*)&results[0], n*sizeof(GPU_Result) );
    
    uint32              nThreadBlocks = N_THREADS_PER_BATCH / N_THREADS_PER_BLOCK;
    int ret = InvokeDeviceWorkFunc( nThreadBlocks, N_THREADS_PER_BLOCK, (GPU_Doc*)pDevDocs,
                         (GPU_Result*)pDevResults, GetNumDocs() );
    if( ret ) {
        cerr << "Device working routine fail!" << endl;
        exit(-1);   //!! should use goto
    } // if
    
    CopyFromDevice( (void*)&results[0], pDevResults, n*sizeof(GPU_Result) );
    // write results to resultsMatrix
    for( uint32 k = 0; k < n; ++k ) {
        uint32 i = results[k].docNO1;
        uint32 j = results[k].docNO2;
        resultMatrix[i][j].similarity = resultMatrix[j][i].similarity = results[k].similarity;
    } // for
}


void GPU_GetSimilarityMatrix()
{
    // 初始化resultMatrix
    resultMatrix.resize( GetNumDocs() );
    for( uint32 i = 0; i < GetNumDocs(); ++i ) {
        resultMatrix[i].resize( GetNumDocs() );
        for( uint32 j = 0; j < GetNumDocs(); ++j )
            resultMatrix[i][j].pDoc = docCollection[j];
    } // for
    
    // allocate memory on card
    void *pDevDocs = DeviceMalloc( sizeof(GPU_Doc)*GetNumDocs() );
    void *pDevResults = DeviceMalloc( sizeof(GPU_Result)*N_THREADS_PER_BATCH );
    if( !pDevDocs || !pDevResults ) {
        cerr << "Malloc memory on device fail." << endl;
        exit(-1);
    } // if
    
    InitDevDocs( pDevDocs );
    
    // assign tasks
    GPU_ResultArray gpuResultArray( N_THREADS_PER_BATCH );
    memset( &gpuResultArray[0], 0, sizeof(GPU_Result)*N_THREADS_PER_BATCH );
    uint32 index = 0;
    for( uint32 i = 0; i < GetNumDocs()-1; ++i ) {
        for( uint32 j = i+1; j < GetNumDocs(); ++j ) {
            gpuResultArray[index].docNO1 = i;
            gpuResultArray[index].docNO2 = j;
            if( ++index == N_THREADS_PER_BATCH ) {
                GetResultsFromDevice( gpuResultArray, pDevResults, pDevDocs );
                index = 0;
                memset( &gpuResultArray[0], 0, sizeof(GPU_Result)*N_THREADS_PER_BATCH );
            } // if
        } // for j
    } // for i
    
    if( index )
        GetResultsFromDevice( gpuResultArray, pDevResults, pDevDocs );
    
    // free dev memory
    DeviceMemFree( pDevDocs );
    DeviceMemFree( pDevResults );
}


void SortResultMatrix()
{
    uint32 nDocs = GetNumDocs();
    for( uint32 i = 0; i < nDocs; ++i )
        jobList[ i % N_CPU ].push_back( JobArg(i, 0) );
    for( uint32 i = 0; i < N_CPU; ++i )
        pthread_create( &threadList[i], NULL, GetSimilarityMatrix_ThreadFuncSort, (void*)i );
    for( uint32 i = 0; i < N_CPU; ++i )
        pthread_join(threadList[i], NULL);
//    for( uint32 i = 0; i < GetNumDocs(); ++i )
//        sort( resultMatrix[i].begin(), resultMatrix[i].end(), greater<ResultItem>() );
}


void* GetDocLength_ThreadFunc( void *arg )
{
    uint32 n = *(uint32*)(&arg);
    
    while( !(jobList[n].empty()) ) {
        uint32 i = jobList[n].front().first;
        jobList[n].pop_front();
        docCollection[i]->computeLength();
    } // while
    
    return (void*)0;
}


void GetDocLength()
{
    uint32 nDocs = GetNumDocs();
    for( uint32 i = 0; i < nDocs; ++i )
        jobList[ i % N_CPU ].push_back( JobArg(i, 0) );
    for( uint32 i = 0; i < N_CPU; ++i )
        pthread_create( &threadList[i], NULL, GetDocLength_ThreadFunc, (void*)i );
    for( uint32 i = 0; i < N_CPU; ++i )
        pthread_join(threadList[i], NULL);
}


void* GetTermIDF2_ThreadFunc( void *arg )
{
    uint32 n = *(uint32*)(&arg);
    
    while( !(jobList[n].empty()) ) {
        uint32 i = jobList[n].front().first;
        jobList[n].pop_front();
        termCollection[i]->computeIDF2();
    } // while
    
    return (void*)0;
}


void GetTermIDF2()
{
    uint32 nTerms = termCollection.size();
    for( uint32 i = 0; i < nTerms; ++i )
        jobList[ i % N_CPU ].push_back( JobArg(i, 0) );
    for( uint32 i = 0; i < N_CPU; ++i )
        pthread_create( &threadList[i], NULL, GetTermIDF2_ThreadFunc, (void*)i );
    for( uint32 i = 0; i < N_CPU; ++i )
        pthread_join(threadList[i], NULL);
}


void WriteMatrixToFile( const char *filename )
{
    ofstream ofs(filename);
    uint32 nDocs = GetNumDocs();
    for( uint32 i = 0; i < nDocs; ++i ) {
        for( uint32 j = 0; j < nDocs; ++j )
            ofs << resultMatrix[i][j].similarity << " ";
        ofs << endl;
    } // for
}


void WriteResultToFile( const char *filename, const uint32 topN = UINT_MAX )
{
    ofstream ofs(filename);
    uint32 nDocs = GetNumDocs();
    uint32 nResults = topN < nDocs ? topN : nDocs;
    
    for( uint32 i = 0; i < nDocs; ++i ) {
        ofs << docCollection[i]->getID() << ": ";
        for( uint32 j = 0; j < nResults; ++j ) {
            if( resultMatrix[i][j].pDoc->getNO() != i )
                ofs << resultMatrix[i][j].pDoc->getID() << " ";
//                ofs << resultMatrix[i][j].pDoc->getID() << "(" << resultMatrix[i][j].similarity << ")" << " ";
//                ofs << resultMatrix[i][j].similarity << " ";
//            ofs << resultMatrix[i][j].pDoc->getNO() << " ";
        } // for
        ofs << endl;
    } // for
}


int main( int argc, char **argv )
{
//    TermPtr p = Term::newInstance("hello", 1);
//    printf("%u\n", p->getNO());
    
//    TestCase1( "test.txt" );
    
    struct timeval start, finish, elapsed;
    double duration;
    
    gettimeofday( &start, NULL );
    LoadStopWords( "StopWords.txt" );
    LoadDocs( argv[1] );
    GetDocLength();
    GetTermIDF2();
    gettimeofday( &finish, NULL );
    timersub( &finish, &start, &elapsed );
    duration = elapsed.tv_sec + (double)(elapsed.tv_usec) / 1000000;
    printf( "All %u docs loaded, total %lu different words, cost %lf seconds.\n", GetNumDocs(), termWordSet.size(), duration );
    
    gettimeofday( &start, NULL );
    GPU_GetSimilarityMatrix();
    gettimeofday( &finish, NULL );
    timersub( &finish, &start, &elapsed );
    duration = elapsed.tv_sec + (double)(elapsed.tv_usec) / 1000000;
    printf( "Finished similarity computation, cost %lf seconds.\n", duration );
//    printf( "Writting similarity matrix to file...\n" );
//    WriteMatrixToFile( "matrix.txt" );
    printf( "Sorting the results...\n" );
    SortResultMatrix();
    printf( "Wrtting results to file...\n" );
    WriteResultToFile("result.txt", 100);
    printf( "ALL JOB DONE!!!\n" );
    
    
    // test display card
//    {
//        size_t size1 = 3147*sizeof(GPU_Doc);
//        void *p = DeviceMalloc( size1 );
//        if( p ) printf( "Malloc dev mem %lu success!\n", size1 );
//        size_t size2 = 2048*sizeof(GPU_Result);
//        void *q = DeviceMalloc( size2 );
//        if( q ) printf( "Malloc dev mem %lu success!\n", size2 );
//        vector<GPU_Doc> vec1(3147);
//        CopyToDevice( p, (void*)&vec1[0], size1);
//        DeviceMemFree( p );
//        DeviceMemFree( q );
//        printf( "%lf\n", (double)(size1+size2)/(1024*1024) );
//    }
    
//    // test
//    {
//        cout << *docCollection[0] << endl;
//        cout << "----------------" << endl;
//        cout << *(resultMatrix[0][GetNumDocs()-2].pDoc) << endl;
//    }
    
    // test 相似度最高的pairs
//    {
//        for( uint32 i = 0; i < GetNumDocs(); ++i ) {
//            cout << docCollection[i]->getID() << "\t";
//            cout << resultMatrix[i][0].pDoc->getID() << "\t";
//            cout << resultMatrix[i][0].similarity << endl;
//        } // for
//    }
    
    // 统计文章最多单词
//    {
//        uint32 max = 0;
//        for( DocCollection::const_iterator it = docCollection.begin(); it != docCollection.end(); ++it ) {
//            uint32 n = (*it)->getTermCount().size();
//            cerr << (*it)->getID() << "\t" << n << endl;
//            max = n > max ? n : max;
//        }
//        printf("max num words: %u\n", max);
//    }
    
    
    
//    DBG("main process terminating...");

    return 0;
}
























