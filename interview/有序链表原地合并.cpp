#include <cstdio>
/*
struct ListNode {
	int val;
	struct ListNode *next;
	ListNode(int x) :
			val(x), next(NULL) {
	}
};*/
class Solution {
public:
    ListNode* Merge(ListNode* pHead1, ListNode* pHead2)
    {
        if (!pHead1) return pHead2;
        if (!pHead2) return pHead1;

        ListNode *pRet = NULL, *pCur = NULL;

        while (pHead1 && pHead2) {
            if (pHead1->val < pHead2->val) {
                if (!pRet) pRet = pHead1;
                pCur = pHead1;
                pHead1 = pHead1->next;
                pCur->next = pHead2;
            } else {
                if (!pRet) pRet = pHead2;
                pCur = pHead2;
                pHead2 = pHead2->next;
                pCur->next = pHead1;
            } // if
        } // while

        return pRet;
    }
};

