#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

struct ListNode;
typedef ListNode *ListPtr;

struct ListNode {
    ListNode( char ch ) : data(ch), next(NULL) {}
    char data;
    ListPtr next;
};


ListPtr head = NULL;

void CreateList( int n )
{
    char data;
    ListPtr node;
    ListPtr prev = NULL;
    for( int i = 0; i < n; ++i ) {
        cin >> data;
        node = new ListNode(data);
        if( !head ) {
            head = prev = node;
        } else {
            prev->next = node;
            prev = node;
        }
    } // for
}


void PrintList()
{
    for( ListPtr p = head; p; p = p->next )
        cout << p->data << " ";
    cout << endl;
}


void ExchangeList()
{
    ListPtr p1 = head;
    ListPtr p2, p3, p4;
    
    while( p1 ) {
        p2 = p1->next;
        // jump 2
        if(!p2) break;
        p3 = p2->next;
        if(!p3) break;
        p4 = p3->next;
        
        p1->next = p3->next;
        if(head == p1) head = p3;
        p3->next = p2;
        p2->next = p1;
        
        p1 = p4;
    } // while
}


int main()
{
    CreateList(6);
    PrintList();
    ExchangeList();
    PrintList();
    return 0;
}















