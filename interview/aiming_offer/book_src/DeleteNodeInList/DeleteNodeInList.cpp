// DeleteNodeInList.cpp : Defines the entry point for the console application.
//

// ¡¶½£Ö¸Offer¡ª¡ªÃûÆóÃæÊÔ¹Ù¾«½²µäÐÍ±à³ÌÌâ¡·´úÂë
// Öø×÷È¨ËùÓÐÕß£ººÎº£ÌÎ

#include "stdafx.h"
#include "..\Utilities\List.h"

void DeleteNode(ListNode** pListHead, ListNode* pToBeDeleted)
{
    if(!pListHead || !pToBeDeleted)
        return;

    // ÒªÉ¾³ýµÄ½áµã²»ÊÇÎ²½áµã
    if(pToBeDeleted->m_pNext != NULL)
    {
        ListNode* pNext = pToBeDeleted->m_pNext;
        pToBeDeleted->m_nValue = pNext->m_nValue;
        pToBeDeleted->m_pNext = pNext->m_pNext;
 
        delete pNext;
        pNext = NULL;
    }
    // Á´±íÖ»ÓÐÒ»¸ö½áµã£¬É¾³ýÍ·½áµã£¨Ò²ÊÇÎ²½áµã£©
    else if(*pListHead == pToBeDeleted)
    {
        delete pToBeDeleted;
        pToBeDeleted = NULL;
        *pListHead = NULL;
    }
    // Á´±íÖÐÓÐ¶à¸ö½áµã£¬É¾³ýÎ²½áµã
    else
    {
        ListNode* pNode = *pListHead;
        while(pNode->m_pNext != pToBeDeleted)
        {
            pNode = pNode->m_pNext;            
        }
 
        pNode->m_pNext = NULL;
        delete pToBeDeleted;
        pToBeDeleted = NULL;
    }
}

// ====================²âÊÔ´úÂë====================
void Test(ListNode* pListHead, ListNode* pNode)
{
    printf("The original list is: \n");
    PrintList(pListHead);

    printf("The node to be deleted is: \n");
    PrintListNode(pNode);

    DeleteNode(&pListHead, pNode);
    
    printf("The result list is: \n");
    PrintList(pListHead);
}

// Á´±íÖÐÓÐ¶à¸ö½áµã£¬É¾³ýÖÐ¼äµÄ½áµã
void Test1()
{
    ListNode* pNode1 = CreateListNode(1);
    ListNode* pNode2 = CreateListNode(2);
    ListNode* pNode3 = CreateListNode(3);
    ListNode* pNode4 = CreateListNode(4);
    ListNode* pNode5 = CreateListNode(5);

    ConnectListNodes(pNode1, pNode2);
    ConnectListNodes(pNode2, pNode3);
    ConnectListNodes(pNode3, pNode4);
    ConnectListNodes(pNode4, pNode5);

    Test(pNode1, pNode3);

    DestroyList(pNode1);
}

// Á´±íÖÐÓÐ¶à¸ö½áµã£¬É¾³ýÎ²½áµã
void Test2()
{
    ListNode* pNode1 = CreateListNode(1);
    ListNode* pNode2 = CreateListNode(2);
    ListNode* pNode3 = CreateListNode(3);
    ListNode* pNode4 = CreateListNode(4);
    ListNode* pNode5 = CreateListNode(5);

    ConnectListNodes(pNode1, pNode2);
    ConnectListNodes(pNode2, pNode3);
    ConnectListNodes(pNode3, pNode4);
    ConnectListNodes(pNode4, pNode5);

    Test(pNode1, pNode5);

    DestroyList(pNode1);
}

// Á´±íÖÐÓÐ¶à¸ö½áµã£¬É¾³ýÍ·½áµã
void Test3()
{
    ListNode* pNode1 = CreateListNode(1);
    ListNode* pNode2 = CreateListNode(2);
    ListNode* pNode3 = CreateListNode(3);
    ListNode* pNode4 = CreateListNode(4);
    ListNode* pNode5 = CreateListNode(5);

    ConnectListNodes(pNode1, pNode2);
    ConnectListNodes(pNode2, pNode3);
    ConnectListNodes(pNode3, pNode4);
    ConnectListNodes(pNode4, pNode5);

    Test(pNode1, pNode1);

    DestroyList(pNode1);
}

// Á´±íÖÐÖ»ÓÐÒ»¸ö½áµã£¬É¾³ýÍ·½áµã
void Test4()
{
    ListNode* pNode1 = CreateListNode(1);

    Test(pNode1, pNode1);
}

// Á´±íÎª¿Õ
void Test5()
{
    Test(NULL, NULL);
}

int _tmain(int argc, _TCHAR* argv[])
{
    Test1();
    Test2();
    Test3();
    Test4();
    Test5();

    return 0;
}

