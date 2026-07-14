#include <iostream>
#include <functional> // 必须包含此头文件以使用 std::function

int demo1() {
    bool is_vip = true;

    // 使用 + 号将无捕获的 lambda 强制提升为普通的 C 风格函数指针。
    // 这里的 auto 最终推导出的类型是： double (*)(double) 
    // NOTES  使用前提是lambda 无捕获，可以退化为函数指针
    auto calculate_price = is_vip 
        ? +[](double price) { return price * 0.8; }  // VIP 打八折
        : +[](double price) { return price; };       // 普通用户原价

    double original_price = 100.0;
    
    std::cout << "Original Price: " << original_price << "\n";
    std::cout << "Final Price: " << calculate_price(original_price) << "\n";

    // 测试条件切换
    is_vip = false;
    calculate_price = is_vip 
        ? +[](double price) { return price * 0.8; }
        : +[](double price) { return price; };

    std::cout << "Final Price (Non-VIP): " << calculate_price(original_price) << "\n";

    return 0;
}

int demo2() {
    // 准备要捕获的外部变量
    double vip_discount = 0.75;    // 75折
    double normal_discount = 0.95; // 95折
    double shipping_fee = 10.0;    // 运费

    bool is_vip = true;

    // 使用带有捕获的 lambda，并且用三元运算符
    // 注意：我们在第一个 lambda 前面显式构造了 std::function<double(double)>
    auto calculate_total = is_vip 
        ? std::function<double(double)>([vip_discount](double price) { 
              return price * vip_discount; // VIP 免运费
          })
        : [normal_discount, shipping_fee](double price) { 
              return price * normal_discount + shipping_fee; // 普通用户收运费
          };
    // NOTES 捕获发生在lambda定义时，而非调用时

    // std::function<double(double)> calculate_total = is_vip
    //     ? [vip_discount](double price) { 
    //           return price * vip_discount; // VIP 免运费
    //       }
    //     : [normal_discount, shipping_fee](double price) { 
    //           return price * normal_discount + shipping_fee; // 普通用户收运费
    //       };

    // 此时 calculate_total 的真实类型被自动推导为 std::function<double(double)>
    
    double cart_value = 100.0;  // lambda无法捕获定义之后出现的变量
    std::cout << "Cart Value: $" << cart_value << "\n";
    std::cout << "VIP Total: $" << calculate_total(cart_value) << "\n";

    // 测试切换状态
    is_vip = false;
    
    // 重新赋值（既然前面 auto 已经推导出它是 std::function，这里可以直接用 if-else 或再次使用三元）
    calculate_total = is_vip 
        ? std::function<double(double)>([vip_discount](double price) { return price * vip_discount; })
        : [normal_discount, shipping_fee](double price) { return price * normal_discount + shipping_fee; };

    std::cout << "Normal Total: $" << calculate_total(cart_value) << "\n";

    return 0;
}

int main() {
    return demo2();
}
