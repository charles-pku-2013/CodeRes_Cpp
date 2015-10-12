#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000
#define LINE_MAX 4096
#define BLANK  " \t\f\r\v\n"

struct _ListNode {
    int data;
    struct _ListNode *next;
};

typedef struct _ListNode ListNode;
typedef ListNode *ListPtr;

ListPtr GetNewNode( int n )
{
    ListPtr p = (ListPtr)malloc(sizeof(ListNode));
    p->next = NULL;
    p->data = n;
    return p;
}


void PrintArray( int *a, int n )
{
    for( int i = 0; i < n; ++i )
        printf("%d ", a[i]);
    putchar('\n');
}

int cmp( const void *p1, const void *p2 )
{
    const int *pl = (const int*)p1;
    const int *pr = (const int*)p2;
    return *pl - *pr;
}


ListPtr head = NULL;

void InsertNode( int n )
{
    static ListPtr prev = NULL;
    ListPtr node = GetNewNode(n);
    if(!head) {
        head = prev = node;
    } else {
        prev->next = node;
        prev = node;
    } // if
}


void CreateSortedList( int *a1, int size1, int *a2, int size2 )
{
    int i1 = 0, i2 = 0;
    
    while( i1 < size1 && i2 < size2 ) {
        if( a1[i1] < a2[i2] ) {
            InsertNode(a1[i1++]);
        } else {
            InsertNode(a2[i2++]);
        } // if
    } // while
    
    while( i1 < size1 ) {
        InsertNode(a1[i1++]);
    }
    
    while ( i2 < size2 ) {
        InsertNode(a2[i2++]);
    }
}


void PrintList()
{
    for( ListPtr p = head; p; p = p->next )
        printf("%d ", p->data);
    putchar('\n');
}

// using recursive
void PrintReverseList( ListPtr p )
{
    if(!p) return;
    PrintReverseList(p->next);
    printf("%d ", p->data);
}



int main()
{
    int a1[MAX], a2[MAX];
    char line[LINE_MAX];
    int index;
    int size1, size2;
    
    // read the 1st list
    fgets( line, LINE_MAX, stdin );
    index = 0;
    for( char *p = strtok(line, BLANK); p; p = strtok(NULL, BLANK) ) {
        a1[index++] = atoi(p);
    } // for
    size1 = index;
//    printf("line = %s\n", line);
    
    
    // read the 2nd list
    fgets( line, LINE_MAX, stdin );
    index = 0;
    for( char *p = strtok(line, BLANK); p; p = strtok(NULL, BLANK) ) {
        a2[index++] = atoi(p);
    } // for
    size2 = index;

    
    qsort(a1, size1, sizeof(int), cmp);
    qsort(a2, size2, sizeof(int), cmp);
    
    CreateSortedList(a1,size1, a2, size2);
    PrintList();
    PrintReverseList(head);
    putchar('\n');
    
//    PrintArray(a1,size1);
//    PrintArray(a2,size2);
    
    return 0;
}












