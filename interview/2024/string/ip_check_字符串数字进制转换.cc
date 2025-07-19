// https://www.nowcoder.com/practice/55fb3c68d08d46119f76ae2df7566880?tpId=295&tqId=1024725&ru=/exam/oj&qru=/ta/format-top101/question-ranking&sourceUrl=%2Fexam%2Foj

class Solution {
public:
    /**
     * 代码中的类名、方法名、参数名已经指定，请勿修改，直接返回方法规定的值即可
     *
     * 验证IP地址
     * @param IP string字符串 一个IP地址字符串
     * @return string字符串
     */

    bool check_IPv4(const std::string& ip) {
        vector<string> segs;
        istringstream iss(ip);
        std::string seg;

        while (std::getline(iss, seg, '.')) {
            segs.emplace_back(seg);
        }

        if (segs.size() != 4) {
            return false;
        }

        for (auto& segment : segs) {
            if (segment.empty() || segment.length() > 3 || segment[0] == '0') {
                return false;
            }

            int ip_num = 0;

            try {
                size_t n = 0;
                ip_num = std::stoi(segment, &n);
                if (n != segment.length()) {
                    return false;
                }
            } catch (...) {
                return false;
            }

            if (ip_num > 255 || ip_num <= 0) {
                return false;
            }
        }

        return true;
    }

    bool check_IPv6(const std::string& ip) {
        vector<string> segs;
        istringstream iss(ip);
        std::string seg;
        while (std::getline(iss, seg, ':')) {  // NOTE
            segs.emplace_back(seg);
        }
        if (segs.size() != 8) {
            return false;
        }

        for (auto& segment : segs) {
            if (segment.empty()) {
                return false;
            }
            if (segment.length() == 1 && segment[0] != '0') {  // NOTE
                return false;
            }
            if (segment.length() > 4) {
                return false;
            }
            long ip_num;
            try {
                size_t n = 0;
                ip_num = std::stol(segment, &n, 16);
                if (n != segment.length()) {
                    return false;
                }
            } catch (...) {
                return false;
            }
        }
        return true;
    }

    string solve(string IP) {
        // write code here
        if (IP.find(':') != string::npos) {
            bool ret = check_IPv6(IP);
            return (ret ? "IPv6" : "Neither");
        } else if (IP.find('.') != string::npos) {
            bool ret = check_IPv4(IP);
            return (ret ? "IPv4" : "Neither");
        } else {
            return "Nither";
        }
    }
};
