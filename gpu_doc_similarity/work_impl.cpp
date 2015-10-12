#include <cmath>
#include "work.h"


// global variables
TermWordSet      termWordSet;
TermCollection   termCollection;


/* Definition of Term */

TermPtr Term::newInstance( const char *_wordStr, uint32 _NO )
{
    AllocatorType alloc;
    Term *p = alloc.allocate(1);
    new (p) Term(_wordStr, _NO );    //!! alloc fail must be handled
    return TermPtr(p, FreeInstance<Term>);
}

void Term::computeIDF2()
{
    uint32 n = docCollection.size();
    uint32 nk = getNumDoc();
    idf2 = 1.0 + log2( (double)n / nk );
    idf2 *= idf2;
}

/* End Definition of Term */



/* Definition of Document */

DocumentPtr Document::newInstance( uint32 NO, const char *ID )
{
    AllocatorType alloc;
    Document *p = alloc.allocate(1);
    new (p) Document(NO, ID);
    return DocumentPtr(p, FreeInstance<Document>);
}

void Document::addTerm( const TermPtr pTerm )
{
    TermCountSetInsertRetType ret = termCount.insert( TermCount::newInstance(pTerm) );
    if( !(ret.second) )
        (*(ret.first))->addCount();
}

void Document::computeLength()
{
    for( TermCountSet::const_iterator it = getTermCount().begin();
                    it != getTermCount().end(); ++it ) {
        uint32 freq = (*it)->getCount();
        maxFreq = freq > maxFreq ? freq : maxFreq;
        length += (double)freq * freq;
    } // for
    
    length = sqrt( length );
}

/* End Definition of Document */

















