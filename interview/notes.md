# static_cast dynamic_cast 区别 原理
static_cast 编译时检查
            无安全检查，多态向下转换
dynamic_cast  运行时检查

# webrtc protocols 协议
https://blog.csdn.net/allen1707/article/details/144140351
websocket based on http 实时全双工信令交换
SDP 会话描述协议，媒体能力，支持的编解码器
STUN 打洞
TURN 中继
UDP 音视频数据

# 不用虚函数实现多态
重载 模板 成员函数指针

# 网络分层
物理层（Physical Layer）
    功能：负责通过物理媒介（如电缆、光纤、无线电波）传输原始比特流，定义电气、机械等硬件规范。
    设备：中继器、集线器、网卡等。

数据链路层（Data Link Layer）
    功能：将比特流封装成帧，实现相邻节点间的可靠传输，包括错误检测（如CRC）、MAC地址寻址等。
    协议/设备：以太网协议、PPP协议；交换机、网桥等。

网络层（Network Layer）
    功能：负责数据包的逻辑寻址（IP地址）和路由选择🍎🍎，确保数据跨网络传输到目标主机。
    协议/设备：IP协议、ICMP协议；路由器🍎🍎、三层交换机等。

传输层（Transport Layer）
    功能：提供端到端的数据传输服务，通过TCP（可靠传输）和UDP（高效传输）实现流量控制🍎🍎、错误恢复等功能。
    关键机制：数据分段、端口寻址、流量控制。

会话层 表示层

应用层（Application Layer）
    功能：直接为应用程序提供网络服务，涵盖用户接口和数据交互。例如：
    HTTP（网页访问）
    FTP（文件传输）
    SMTP/POP3（电子邮件）
    DNS（域名解析）。

# c++11 14 17
. C++11
auto decltype
using  typedef
template 可变模板参数
lambda
mutable
noexcept
constexpr
initialization list
stl: tupe
std::move
shared_ptr

. C++14
auto return type
lambda auto param
变量模板
make_unique
shared_lock
std::quoted

C++17
std::variant optional any filesystem string_view

# 大文件排序
. 分治法
先部分排序，得到N个有序子集，再用小根堆合并
每 pop 一个元素，就根据它的文件号去对应的文件里，补充一个元素进入堆中，直到那个文件中的元素被拿完
https://zhuanlan.zhihu.com/p/484732622
. 位图法
. 数据库

# NOTICE
. lambda capture list
. 不可以连续初始化变量
ListNode* result = nullptr, p = nullptr;
. lambda check capture type &
