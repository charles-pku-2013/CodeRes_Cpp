#include <iostream>

using namespace std;

struct RoiConfig {
    int start_x = 0;
    int start_y = 0;
    int end_x   = 0;
    int end_y   = 0;
    int cut_w   = 0;
    int cut_h   = 0;
    int step_w  = 0;
    int step_h  = 0;
    int format  = 3;   // ROI/source image channels (current hardware assumes 3)
};

// 全局 operator== 重载
bool operator==(const RoiConfig& lhs, const RoiConfig& rhs) {
    return lhs.start_x == rhs.start_x &&
           lhs.start_y == rhs.start_y &&
           lhs.end_x   == rhs.end_x   &&
           lhs.end_y   == rhs.end_y   &&
           lhs.cut_w   == rhs.cut_w   &&
           lhs.cut_h   == rhs.cut_h   &&
           lhs.step_w  == rhs.step_w  &&
           lhs.step_h  == rhs.step_h  &&
           lhs.format  == rhs.format;
}

int main()
{
    RoiConfig c1, c2;

    if (c1 == c2) {
        cout << "equal!" << endl;
    }

    return 0;
}
