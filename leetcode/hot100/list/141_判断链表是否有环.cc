// https://leetcode.cn/problems/linked-list-cycle/solutions/440042/huan-xing-lian-biao-by-leetcode-solution/?envType=problem-list-v2&envId=2cktkvj

class Solution {
public:
    bool hasCycle(ListNode* head) {
        if (head == nullptr || head->next == nullptr) {
            return false;
        }
        ListNode* slow = head;
        ListNode* fast = head->next;
        while (slow != fast) {
            if (fast == nullptr || fast->next == nullptr) {
                return false;
            }
            slow = slow->next;
            fast = fast->next->next;
        }
        return true;
    }

    bool hasCycle(ListNode* head) {
        if (!head || !head->next) {
            return false;
        }

        ListNode *slow = head, *fast = head;
        do {
            slow = slow->next;
            if (fast) {
                fast = fast->next;
            } else {
                return false;
            }
            if (fast) {
                fast = fast->next;
            } else {
                return false;
            }
        } while (slow != fast);

        return true;
    }
};
