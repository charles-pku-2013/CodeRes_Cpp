#ifndef __WORK_H
#define __WORK_H


#include <string>
#include <set>
#include <iostream>
#include <memory>
#include <boost/smart_ptr.hpp>
#include <boost/unordered_set.hpp>
#include <boost/pool/pool_alloc.hpp>
#include <sys/time.h>

//#define ALLOCATOR(type) boost::fast_pool_allocator<type>
#define ALLOCATOR(type) std::allocator<type>


#ifdef _DEBUG
#define DBG(...) do { \
            struct timeval __dbg_timeval;   \
            gettimeofday( &__dbg_timeval, NULL );   \
            fprintf( stdout, "[%llu,%llu] ", (unsigned long long)(__dbg_timeval.tv_sec), \
                            (unsigned long long)(__dbg_timeval.tv_usec) );      \
            fprintf( stdout,  "%s:%d in %s(): ", __FILE__, __LINE__, __func__ ); \
            fprintf( stdout, __VA_ARGS__ ); \
            fprintf( stdout, "\n" ); \
        } while(0)

#else
#define DBG(...)
#endif


typedef unsigned int uint32;
typedef std::basic_string< char, std::char_traits<char>, ALLOCATOR(char) > String;


// global funcs
//template< typename T >
//void FreeInstance( T *p )
//{
//    //!! 调用模板类静态函数，不用typename，那是定义用的
////    T::AllocatorType::destroy( p );             // 析构
//    p->~T();
//    T::AllocatorType::deallocate(p, 1);         // 回收内存
//}
template< typename T >
void FreeInstance( T *p )
{
    //!! 调用模板类静态函数，不用typename，那是定义用的
    typename T::AllocatorType alloc;
    p->~T();                        // 析构
    alloc.deallocate(p, 1);         // 回收内存
}


/* Declaration of Term */

class Term;
typedef boost::shared_ptr<Term>                 TermPtr;

class Term {
public:
    typedef ALLOCATOR(Term)         AllocatorType;
    typedef boost::unordered_set< uint32, boost::hash<uint32>, std::equal_to<uint32>, ALLOCATOR(uint32) >    DocSet;
public:
    const String& getWord() const
    { return word; }
    
    uint32 getNO() const
    { return NO; }
    
    void addDoc( uint32 docNO )
    { docSet.insert(docNO); }
    const DocSet& getDocSet() const
    { return docSet; }
    uint32 getNumDoc() const
    { return docSet.size(); }
    
    void computeIDF2();
    double getIDF2() const { return idf2; }
//    ~Term() { printf("Term::Destroy()\n"); }    // dbg
    
    static TermPtr newInstance( const char *_wordStr, uint32 _NO );
protected:
    Term( const char *_wordStr, uint32 _NO ) : word(_wordStr), NO(_NO), idf2(0.0) {}
protected:
    String          word;
    uint32          NO;             // 编号
    double          idf2;           // idf平方 (log(n/nk)+1)^2
    DocSet          docSet;         // 在哪些文章中出现
//    uint32          numDoc;         // 在多少篇文章中出现
public:
    struct TermHash {
        std::size_t operator()( const TermPtr p ) const
        { return boost::hash<String>()( p->getWord() ); }
    };
    
    struct TermEqualTo {
        bool operator() ( const TermPtr pl, const TermPtr pr ) const
        { return pl->getWord() == pr->getWord(); }
    };
};


//struct TermNOComparator {
//    bool operator() ( const TermPtr pl, const TermPtr pr ) const
//    { return pl->getNO() < pr->getNO(); }
//};

/* End Declaration of Term */




/* Declaration of Document */

class Document;
typedef boost::shared_ptr<Document> DocumentPtr;

class Document {
public:
    class TermCount;
    typedef boost::shared_ptr<TermCount> TermCountPtr;
    class TermCount {
    private:
        TermPtr     pTerm;
        uint32      count;
        TermCount( const TermPtr _ptr ) : pTerm(_ptr), count(1) {}
    public:
        typedef ALLOCATOR(TermCount) AllocatorType;
        void addCount() { ++count; }
        uint32 getCount() const { return count; }
        const TermPtr getTerm() const { return pTerm; }
        
        bool operator < ( const TermCount &rhs ) const
        { return pTerm->getNO() < rhs.pTerm->getNO(); }
        
        static TermCountPtr newInstance( const TermPtr ptr )
        {
            AllocatorType alloc;
            TermCount *p = alloc.allocate(1);
            new (p) TermCount( ptr );
            return TermCountPtr(p, FreeInstance<TermCount>);
        }
    public:
        struct TermCountComparator {
            bool operator() ( const TermCountPtr pl, const TermCountPtr pr ) const
            { return *pl < *pr; }
        };
    };

    typedef ALLOCATOR(Document)         AllocatorType;
    typedef std::set< TermCountPtr, TermCount::TermCountComparator, ALLOCATOR(TermCountPtr) >  TermCountSet;
    typedef std::pair<TermCountSet::iterator, bool> TermCountSetInsertRetType;
public:
    uint32 getNO() const { return NO; }
    const String& getID() const { return ID; }
    void addTerm( const TermPtr pTerm );
    const TermCountSet& getTermCount() const { return termCount; }
    bool empty() const { return termCount.empty(); }
    void computeLength();
    double getLength() const { return length; }
    uint32 getMaxFreq() const { return maxFreq; }
    uint32 getNumTerms() const { return termCount.size(); }
//    ~Document() { DBG("Doc %u destructor", getNO()); }
    
    static DocumentPtr newInstance( uint32 NO, const char *ID );
protected:
    Document( uint32 _NO, const char *_id ) : NO(_NO), ID(_id), length(0.0), maxFreq(0) {}
protected:
    uint32          NO;
    uint32          maxFreq;
    double          length;
    String          ID;
    TermCountSet    termCount;
};

/* End Declaration of Document */



/* Declaration of global variables */
typedef boost::unordered_set< TermPtr, Term::TermHash, Term::TermEqualTo, ALLOCATOR(TermPtr) >        TermWordSet;
extern TermWordSet      termWordSet;                // "word" as the key
typedef std::vector< TermPtr, ALLOCATOR(TermPtr) >              TermCollection;         // array
extern TermCollection               termCollection;
typedef std::vector< DocumentPtr, ALLOCATOR(DocumentPtr) >      DocCollection;
extern DocCollection       docCollection;
/* End Declaration of global variables */



/* Declaration of global functions */
extern void AddWord( const char *word, DocumentPtr pDoc );
/* End Declaration of global functions */



//#ifdef _DEBUG
//extern std::ostream& operator << ( std::ostream &os, const Term &term );
//extern std::ostream& operator << ( std::ostream &os, const Document &doc );
//#endif


#endif












