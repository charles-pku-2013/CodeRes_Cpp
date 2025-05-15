#include <iostream>
#include <string>
#include <vector>

int main() {
    // test string
    {
        // NOTE string 必须足够长才有效果，否则系统会优化存储到栈上
        std::string s("梁森森邀请您参加面试 候选人姓名：孙超面试职位：C/C++ , 15-30K面试时间：2025-05-14 19:00面试号：hGD52a2G面试链接：https://zpurl.cn/irnsvOM5您可复制该链接前往 APP面试日程页或者从浏览器打开进入面试间");
        printf("%llx\n", (uint64_t)(s.c_str()));

        std::string t(std::move(s));
        printf("%llx\n", (uint64_t)(t.c_str()));

        printf("%llx\n", (uint64_t)(s.c_str()));
    }

    // test vector
    {
        std::vector<int> v1{1,2,3,4,5};
        printf("%llx\n", (uint64_t)(v1.data()));

        std::vector<int> v2(std::move(v1));
        printf("%llx\n", (uint64_t)(v2.data()));

        printf("%llx\n", (uint64_t)(v1.data()));
    }

    return 0;
}

#if 0
14d604450
14d604450
16d86efc0
600001e0d220
600001e0d220
0
#endif
