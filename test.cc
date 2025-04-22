#include <iostream>
#include <memory>
#include <utility>

using namespace std;

class Widget : public std::enable_shared_from_this<Widget> {
 public:
    template<typename... Ts>
    static std::shared_ptr<Widget> create(Ts&&... params) {
        return std::shared_ptr<Widget>(new Widget(std::forward<Ts>(params)...));
    }

 private:
    Widget() = default;
};

int main() {
    auto p = Widget::create();

    return 0;
}
