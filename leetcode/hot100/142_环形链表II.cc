// https://leetcode.cn/problems/linked-list-cycle-ii/description/

class Solution {
public:
    ListNode *detectCycle(ListNode *head) {
        if (!head || !head->next) {
            return nullptr;
        }

        ListNode *slow = head, *fast = head;

        // Step1 判断是否有环
        // slow 每次走一步，fast每次走两步，若能相遇 必定有环
        do {
            if (fast) {
                fast = fast->next;
            } else {
                return nullptr;
            }

            if (fast) {
                fast = fast->next;
            } else {
                return nullptr;
            }

            slow = slow->next;
        } while (slow != fast);

        // 若找到环，slow重定位到head
        // 之后slow fast每次各走一步，若相遇，相遇点就是环的入口
        slow = head;
        while (slow && fast && slow != fast) {
            slow = slow->next;
            fast = fast->next;
        }

        if (slow == fast) {
            return slow;
        }

        return nullptr;
    }
};
