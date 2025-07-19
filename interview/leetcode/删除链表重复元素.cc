/*
https://leetcode.cn/problems/remove-duplicates-from-sorted-list/description/
给定一个已排序的链表的头head， 删除所有重复的元素，使每个元素只出现一次。返回 已排序的链表。
 */
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        if (!head) {
            return head;
        }

        ListNode* cur = head;
        while (cur->next) {
            if (cur->val == cur->next->val) {
                cur->next = cur->next->next;
            }
            else {
                cur = cur->next;
            }
        }

        return head;
    }
};
