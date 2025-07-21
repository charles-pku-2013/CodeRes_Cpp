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
    // 纵向
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        if (lists.empty()) { return nullptr; }
        if (lists.size() == 1) { return lists[0]; }

        ListNode head;
        ListNode* prev = &head;

        while (true) {
            // 重新定义最小元素 nullptr永远是最大
            auto it = std::min_element(lists.begin(), lists.end(),
                [](ListNode* lhs, ListNode* rhs)->bool {
                    if (!rhs) { return true; }
                    if (!lhs) { return false; }
                    return lhs->val < rhs->val;
                }
            );

            if (it == lists.end()) { break; }

            auto& p = *it;  // 这里必须用引用，必须在lists里修改
            if (!p) { break; }

            prev->next = p;
            prev = p;
            p = p->next;
        }

        return head.next;
    }

    // 用有序容器实现纵向
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        if (lists.empty()) { return nullptr; }
        if (lists.size() == 1) { return lists[0]; }

        auto cmp = [](const ListNode* lhs, const ListNode* rhs)->bool {
            return lhs->val < rhs->val;
        };

        // NOTE!!! set custom cmp 必须用multiset
        std::multiset<ListNode*, decltype(cmp)> slists(cmp);

        // NOTE!!! 尽量用for_each替代其他的函数如copy_if
        std::for_each(lists.begin(), lists.end(), [&slists](auto& node) {
            if (node) { slists.insert(node); }
        });

        ListNode head;
        ListNode* prev = &head;

        while (!slists.empty()) {
            auto* node = *(slists.begin());

            prev->next = node;
            prev = prev->next;

            slists.erase(slists.begin());
            if (node->next) {
                slists.insert(node->next);
            }
        }

        return head.next;
    }

    // 横向
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        if (lists.empty()) { return nullptr; }
        if (lists.size() == 1) { return lists[0]; }

        auto merge_2_lists = [](ListNode* l1, ListNode* l2)->ListNode* {
            ListNode head;
            ListNode* prev = &head;

            while (l1 && l2) {
                if (l1->val < l2->val) {
                    prev->next = l1;
                    l1 = l1->next;
                } else {
                    prev->next = l2;
                    l2 = l2->next;
                }
                prev = prev->next;
            }

            prev->next = l1 ? l1 : l2;  // NOTE

            return head.next;
        };

        ListNode* result = lists[0];

        for (auto i = 1; i < lists.size(); ++i) {
            result = merge_2_lists(result, lists[i]);
        }

        return result;
    }
};
