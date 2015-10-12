// validation of bipartite graph by BFS dyeing 用广度优先搜索解决着色问题
#define _FILE_DEBUG_
//#define _C_LAN_
//#define _DEBUG_OUTPUT_
#ifdef _FILE_DEBUG_
#include <fstream>
#endif
#include <iostream>
#include <stdio.h>
using namespace std;

#include <string>
#include <vector>
#include <map>
#include <stack>
#include <algorithm>

template< typename T >
void printMap( const T &c )
{
    for( typename T::const_iterator it = c.begin(); it != c.end(); ++it )
        cout << it->first << "\t\t" << it->second << endl;
}

// 问题本质：用两种颜色把分类，相邻块不能同色
int main(int argc, char *argv[])
{
//#ifdef _FILE_DEBUG_
//	ifstream fin;
//	fin.open("../input.txt");
//	cin.rdbuf(fin.rdbuf());
//#ifdef _C_LAN_
//	freopen("../input.txt", "r", stdin);
//#endif
//#endif
//#ifdef _FILE_DEBUG_
//	ofstream fout;
//	fout.open("../output.txt");
//	cout.rdbuf(fout.rdbuf());
//#ifdef _C_LAN_
//	freopen("../output.txt", "w", stdout);
//#endif
//#endif
    
	int case_num;
	std::cin >> case_num;
	for (int i = 0; i < case_num; ++ i)
	{
		int relation_num;
		std::cin >> relation_num;
		std::string name0, name1;
		std::map<std::string, int> color_map;
		std::multimap<std::string, std::string> relation_map;
		for (int j = 0; j < relation_num; ++ j)
		{
			std::cin >> name0 >> name1;
            // 哪些是相邻的，邻居信息表，供查询
			relation_map.insert(std::make_pair(name0, name1));
			relation_map.insert(std::make_pair(name1, name0));
			color_map[name0] = 0;
			color_map[name1] = 0;
		}
		std::cout << "Case #" << i + 1 << ": ";
		if (color_map.size() < 2)
		{
			std::cout << "No" << std::endl;
			continue;
		}
        
        // dbg
        cout << "The relation map is:" << endl;
        printMap( relation_map );
        cout << "The color map is:" << endl;
        printMap( color_map );
        
		bool flag = false;
		std::string now;
		std::stack<std::string> relation_stack;
		relation_stack.push(color_map.begin()->first);
		color_map.begin()->second = 1;      // 着色，入栈
		while (! relation_stack.empty())
		{
			now = relation_stack.top(); // 出栈
			relation_stack.pop();
            // 寻找所有的邻居，不涂上与自己不同的颜色
			std::pair<std::multimap<std::string, std::string>::iterator, std::multimap<std::string, std::string>::iterator> range = relation_map.equal_range(now);
			for (std::multimap<std::string, std::string>::iterator it = range.first; it != range.second; ++ it)
			{
				if (color_map[it->second] == color_map[now])
				{
//                    cout << it->second << "=" << now << " " << color_map[now] << endl;
//                    cout << "set flag = true" << endl;
					flag = true;        // 不可以
					break;
				}
				if (color_map[it->second] == 0)     // 未着色
				{
					color_map[it->second] = -color_map[now];   // 着色和当前(相邻)元素相反
					relation_stack.push(it->second);
				}
			}
			if (flag)
			{
				break;
			}
            // 处理孤立节点(群)
			if (relation_stack.empty())
			{
				for (std::map<std::string, int>::iterator it = color_map.begin(); it != color_map.end(); ++ it)
				{
					if (0 == it->second)
					{
						relation_stack.push(it->first);
					}
				}
			}
		}
		std::cout << (flag ? "No" : "Yes") << std::endl;
	}
    
	return 0;
}