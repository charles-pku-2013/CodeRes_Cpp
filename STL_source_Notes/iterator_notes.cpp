#include <iostream>

// 定义自己的iterator:
template <typename Item>
struct ListIter
        : public std::iterator<std::bidirectional_iterator_tag, Item> {
    // ...
};

