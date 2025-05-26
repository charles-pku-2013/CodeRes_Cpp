#if 0
https://leetcode.cn/problems/merge-k-sorted-lists/description/?envType=problem-list-v2&envId=2cktkvj
给你一个链表数组，每个链表都已经按升序排列。
请你将所有链表合并到一个升序链表中，返回合并后的链表。

示例 1：
输入：lists = [[1,4,5],[1,3,4],[2,6]]
输出：[1,1,2,3,4,4,5,6]
解释：链表数组如下：
[
  1->4->5,
  1->3->4,
  2->6
]
将它们合并到一个有序链表中得到。
1->1->2->3->4->4->5->6

示例 2：
输入：lists = []
输出：[]

示例 3：
输入：lists = [[]]
输出：[]
#endif

class Solution {
public:
    ListNode* mergeTwoLists(ListNode *a, ListNode *b) {
        if ((!a) || (!b)) return a ? a : b;
        ListNode head, *tail = &head, *aPtr = a, *bPtr = b;
        while (aPtr && bPtr) {
            if (aPtr->val < bPtr->val) {
                tail->next = aPtr; aPtr = aPtr->next;
            } else {
                tail->next = bPtr; bPtr = bPtr->next;
            }
            tail = tail->next;
        }
        tail->next = (aPtr ? aPtr : bPtr);
        return head.next;
    }

    ListNode* mergeKLists(vector<ListNode*>& lists) {
        ListNode *ans = nullptr;
        for (size_t i = 0; i < lists.size(); ++i) {
            ans = mergeTwoLists(ans, lists[i]);
        }
        return ans;
    }

    ListNode* mergeKLists(vector<ListNode*>& lists) {
        ListNode* retList = nullptr;
        ListNode* retListCur = nullptr;

        lists.erase(
            std::remove_if(lists.begin(), lists.end(), [](ListNode *p)->bool {
                return (p == nullptr);
            }),
            lists.end()
        );

        if (lists.empty()) {
            return retList;
        }

        while (!lists.empty()) {
            auto it = std::min_element(lists.begin(), lists.end(), [](const ListNode* lhs, const ListNode* rhs)->bool {
                return lhs->val < rhs->val;
            });

            if (it == lists.end()) {
                break;
            }

            auto& node = *it;  // NOTE 需要修改lists数组内容
            if (!retList) {
                retList = retListCur = node;
            } else {
                retListCur->next = node;
                retListCur = node;
            }
            node = node->next;

            lists.erase(
                std::remove_if(lists.begin(), lists.end(), [](ListNode *p)->bool {
                    return (p == nullptr);
                }),
                lists.end()
            );
        }

        return retList;
    }
};
