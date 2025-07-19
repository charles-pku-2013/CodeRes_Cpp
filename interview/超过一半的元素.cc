#include <iostream>
#include <vector>

/*
 * https://www.cnblogs.com/HuangYJ/p/13510653.html
 * 采用阵地攻守的思想：
 * 第一个数字作为第一个士兵，守阵地；count = 1；
 * 遇到相同元素，count++;
 * 遇到不相同元素，即为敌人，同归于尽,count--；当遇到count为0的情况，又以新的i值作为守阵地的士兵，
 * 继续下去，到最后还留在阵地上的士兵，有可能是主元素。
 * 再加一次循环，记录这个士兵的个数看是否大于数组一半即可。
 */
int more_than_half(std::vector<int>& arr) {
    if (arr.empty()) { return 0; }

    int target = arr[0];

    if (arr.size() == 1) 
    { return target; }

    int cnt = 1;
    for (auto i = 1; i < arr.size(); ++i) {
        if (arr[i] == target) {
            ++cnt;
        } else {
            if (--cnt == 0) {
                target = arr[i];
                cnt = 1;
            }
        }
    }

    // 检验出现频率最高的数字出现的次数是否真的超过数组长度的一半
    // 因为该方法只能保证如果存在超过一半的数就是target，但不代表target一定超过一半
    cnt = 0;
    for (int x : array){
        if (x == target)
            ++cnt;
    }
    return cnt > arr.size() / 2 ? target : 0;
}
