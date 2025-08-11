#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <unordered_set>
#include <set>

using namespace std;

/* 地铁信息查询 */
// 线路查询
void Line_Inquiry(string line_check)
{
	ifstream infile("地铁信息.txt");
	if (!infile) 
	{
		cerr << "无法打开此文件！" << endl;
		exit(0);
	}
	
	string buffer;
	bool found = false;
	while (getline(infile, buffer)) 
	{
		if (buffer == line_check) 
		{
			found = true;
			cout << line_check << "的所有站点：" << endl;
			continue;
		}
		if (found) 
		{
			istringstream iss(buffer);	// 解析为单词（以空格分隔）
			string station;
			iss >> station; // 将解析的单词读取到 station 变量中
			cout << station << endl;
			if (buffer.empty()) 
			{
				break; // 遇到空行停止输出
			}
		}
	}

	infile.close();

	if (!found) 
	{
		cout << "此线路不存在！" << endl;
	}
}
// 站点查询
void Site_Inquiry()
{
	string site_check;
	//cin >> site_check;

	ifstream infile("地铁信息1.txt");
	if (!infile)
	{
		cerr << "无法打开此文件！" << endl;
		exit(0);
	}

	map<string, set<string>> line;

	string lineName;
	string site;

	// 读取地铁信息文件，构建线路信息的数据结构
	while (getline(infile, lineName))
	{
		while (getline(infile, site) && !site.empty())
		{
			line[lineName].insert(site);
		}
	}

	// 查询站点所属的线路
	bool found = false;
	bool found_exact = false;
	set<string> stations;

	while (!found_exact) 
	{
		cin >> site_check;
		cout << "站点所属的线路:" << endl;
	 	found = false;
		for (const auto& entry : line)
		{
			const set<string>& sites = entry.second;
			for (const auto& site : sites) 
			{
				if (site == site_check)
				{
					cout << entry.first << endl;
					found_exact = true;
					found = true;
				}
				else if (site.find(site_check) != string::npos)
				{
					stations.insert(site);
					found = true;
				}
			}
		}
		// 字符串子串匹配（关键词搜索）
		if (found && !found_exact) 
		{
			cout << "\n";
			cout << "--------------------------------" << endl;
			cout << "您可能要选择的站点：" << endl;
			for (const auto& station : stations) 
			{
				cout << station << endl;
			}
			cout << "请重新输入：" << endl;
			cout << "--------------------------------" << endl;
		}
		else if (!found && found_exact == false) 
		{
			cout << "此站点不存在！" << endl;
			return;
		}
	}

	infile.close();
}

/* 地铁票务查询 */

/*
bool is_in_line(map<string, int>& stations, string station) 
{
	return stations.find(station) != stations.end();
}
*/

// 将字符串转换为整数
int mystod(string s) 
{
	int result = 0;
	for (int i = 0; i < s.size(); i++) 
	{
		result = result * 10 + (s[i] - '0');
	}
	return result;
}

// 将地铁线路名称转换为对应的编号
int linkedline(string subway) 
{
	if (subway == "1号线") return 1;
	else if (subway == "2号线") return 2;
	else if (subway == "3号线") return 3;
	else if (subway == "4号线") return 4;
	else if (subway == "10号线") return 5;
	else if (subway == "S1线") return 6;
	else if (subway == "S3线") return 7;
	else if (subway == "S7线") return 8;
	else if (subway == "S8线") return 9;
	else if (subway == "S9线") return 10;
	return 0;
}

// 票价计算
int Price(int dis)
{
	if (dis >= 0 && dis <= 10000)
		return 2;
	else if (dis > 10000 && dis <= 16000)
		return 3;
	else if (dis > 16000 && dis <= 22000)
		return 4;
	else if (dis > 22000 && dis <= 30000)
		return 5;
	else if (dis > 30000 && dis <= 38000)
		return 6;
	else if (dis > 38000 && dis <= 48000)
		return 7;
	else if (dis > 48000 && dis <= 58000)
		return 8;
	else if (dis > 58000 && dis <= 70000)
		return 9;
	else if (dis > 70000)
		return 10 + (dis - 70000) / 14000;
	else
		return -1; // Invalid distance
}

#pragma warning (disable : 4996)
int CalTicket(string begin, string end)
{
	ifstream infile("地铁信息2.txt");
	if (!infile)
	{
		cerr << "无法打开此文件！" << endl;
		exit(0);
	}

	// 每条地铁线上的站点数
	int every_stations[11] = { 0, 27, 26, 29, 18, 14, 8, 19, 10, 17, 6 };
	// 存储各个地铁线路的站点和距离
	vector<pair<string, int>> lines[11];
	vector<tuple<int, int, int>> allLinesIncludeBegin2End; // 1st : line, 2nd : begin, 3rd : end

	//map<string, map<string, int>> line;
	string lineName;
	string site;
	string _distance;
	int subway_num = 0; //result of linkedliine(lineName)
	
	// Read subway information file and store station distances in the data structure
	while (getline(infile, lineName))
	{
		subway_num = linkedline(lineName);
		int check = 0, beginint, endint;
		for (int x = 0; x < every_stations[subway_num]; x++)
		{
			getline(infile, site);
			//getline(infile, _distance);
			istringstream iss(site);
			iss >> site >> _distance;
			if (site == begin) {
				beginint = x;
				check++;
			}
			else if (site == end) {
				endint = x;
				check++;
			}
			//d = mystod(_distance);
			lines[subway_num].emplace_back(make_pair(site, mystod(_distance)));
			_distance = "";
		}
		getline(infile, site);
		if (check == 2) allLinesIncludeBegin2End.emplace_back(make_tuple(subway_num, beginint, endint));
	}
	
	int dis = 21000000;
	for (int i = 0; i < allLinesIncludeBegin2End.size(); i++) 
	{
		int temp = 0;
		for (int j = get<1>(allLinesIncludeBegin2End[i]); j < get<2>(allLinesIncludeBegin2End[i]); j++) 
		{
			temp += lines[get<0>(allLinesIncludeBegin2End[i])][j].second;
		}
		if (dis > temp) dis = temp;
	}
	
	cout << "\n距离：" << dis << "米" << endl;

	int price = Price(dis);
	return price;
}

// 字符串子串匹配（关键词搜索）
unordered_set<string> findMatchingStrings(const string& substr, string filename)
{
	unordered_set<string> matches;
	ifstream infile(filename);
	string buffer;
	while (getline(infile, buffer))
	{
		if (buffer.find(substr) != string::npos)
		{
			matches.insert(buffer);
		}
	}
	infile.close();
	return matches;
}

// 线路说明（直达）
void Route_Straight()
{
	ifstream infile("地铁信息1.txt");
	if (!infile)
	{
		cerr << "无法打开此文件！" << endl;
		exit(0);
	}

	string begin, end;
	
	// 输入起点
	cout << "请输入起点站：";
	cin >> begin;
	
	// 子串匹配
	unordered_set<string> match_begin = findMatchingStrings(begin, "地铁信息1.txt");
	if (match_begin.empty())
	{
		cout << "没有找到匹配的站点。" << endl;
	}
	else
	{
		bool exactMatch = false;  // 判断是否有完全匹配的站点
		for (const string& station : match_begin)
		{
			if (station == begin)
			{
				exactMatch = true;
				break;
			}
		}

		if (!exactMatch)
		{
			cout << "--------------------------------" << endl;
			cout << "您可能要选择的是：" << endl;
			for (const string& station : match_begin)
			{
				cout << station << endl;
			}
			cout << "--------------------------------" << endl;
			cout << "请重新输入：";
			cin >> begin;
		}
	}
	
	// 输入终点
	cout << "请输入终点站：";
	cin >> end;

	// 子串匹配
	unordered_set<string> match_end = findMatchingStrings(end, "地铁信息1.txt");
	if (match_end.empty())
	{
		cout << "没有找到匹配的站点。" << endl;
	}
	else
	{
		bool exactMatch = false;  // 判断是否有完全匹配的站点
		for (const string& station : match_end)
		{
			if (station == end)
			{
				exactMatch = true;
				break;
			}
		}

		if (!exactMatch)
		{
			cout << "--------------------------------" << endl;
			cout << "您可能要选择的是：" << endl;
			for (const string& station : match_end)
			{
				cout << station << endl;
			}
			cout << "--------------------------------" << endl;
			cout << "请重新输入：";
			cin >> end;
		}
	}

	map<string, set<pair<int, string>>>line;
	string lineName;
	string site;
	int i = 0;
	// 读取地铁信息文件，构建线路信息的数据结构
	while (getline(infile, lineName))
	{
		for (; getline(infile, site) && !site.empty(); i++)
		{
			line[lineName].insert({ i, site });
		}
	}

/* 直达 */
	// 检查起点和终点是否在同一个set中
	bool straight = false;
	vector<string> stationsBetween;  // 存储起点和终点之间的所有站点
	string curLine;	// 当前线路名称
	for (const auto& entry : line)
	{
		const string& lineName = entry.first;  // 获取线路名称
		const set<pair<int, string>>& stations = entry.second;
		int beginOrder = -1;  // 起点的顺序
		int endOrder = -1;    // 终点的顺序

		// 遍历当前线路的所有站点，并记录起点和终点的顺序
		for (const auto& station : stations)
		{
			if (station.second == begin)
			{
				beginOrder = station.first;
			}
			else if (station.second == end)
			{
				endOrder = station.first;
			}

			// 如果起点和终点的顺序都已找到，则跳出循环
			if (beginOrder != -1 && endOrder != -1)
			{
				break;
			}
		}

		if (beginOrder != -1 && endOrder != -1)
		{
			straight = true;
			curLine = lineName;  // 记录当前线路名称

			// 根据起点和终点的顺序，添加起点到终点之间的站点到stationsBetween向量中
			for (const auto& station : stations)
			{
				if (station.first > beginOrder && station.first < endOrder)
				{
					stationsBetween.push_back(station.second);
				}
			}
			break;
		}
	}

	// 输出结果
	if (straight)
	{
		int count = 1;
		cout << "\n* 直达 *" << endl;
		cout << curLine << "：" << endl;  // 输出当前线路名称
		cout << begin << "->";
		for (const string& station : stationsBetween)
		{
			cout << station << "->";
			++count;
		}
		cout << end << endl;
		cout << "-- 共 " << count << " 站 --" << endl;
	}
	else
	{
		cout << "无直达路径。" << endl;
	}

	infile.close();

/*票价（直达）*/
	cout << "\n票价：" << CalTicket(begin, end) << "元" << endl;
}

void Menu()
{
	system("color 71");
	cout << endl;
	cout << "\t\t\t\t\t\t    欢迎使用南京地铁票务系统";
	cout << endl;
	cout << "\t\t\t\t\t\t--------------------------------";
	cout << endl;
	cout << "\t\t\t\t\t\t请输入：" << endl;
	cout << "\t\t\t\t\t\t        1   线路查询" << endl;
	cout << "\t\t\t\t\t\t        2   站点查询" << endl;
	cout << "\t\t\t\t\t\t        3   票务查询" << endl;
	cout << "\t\t\t\t\t\t        4   退出" << endl;
	cout << "\n** 请输入您想要的操作编号：";
}

void Function(int opt)
{
	switch (opt)
	{
	case 1:
	{
		cout << "------------------------------------------------------------------------------------" << endl;
		cout << "线路：" << endl;
		cout << "1号线\t2号线\t3号线\t4号线\t10号线\tS1线\tS3线\tS7线\tS8线\tS9线\n";
		cout << "------------------------------------------------------------------------------------" << endl;
		cout << "请输入要查询的线路：";

		string line_check;
		cin >> line_check;
		Line_Inquiry(line_check);

		cout << "** 请输入您想要的操作编号：";
		int new_opt;
		cin >> new_opt;
		Function(new_opt);
	}
	case 2:
	{
		cout << "请输入要查询的站点：";
		Site_Inquiry();

		cout << "\n** 请输入您想要的操作编号：";
		int new_opt;
		cin >> new_opt;
		Function(new_opt);
	}
	case 3:; 
	{
		Route_Straight();

		cout << "\n** 请输入您想要的操作编号：";
		int new_opt;
		cin >> new_opt;
		Function(new_opt);
	}
	case 4:
	{
		cout << "谢谢使用！" << endl;
		exit(0);
		break;
	}
	default:
	{
		cout << "请重新输入：";
		int new_opt;
		cin >> new_opt;
		Function(new_opt);
	}
	}
}

int main()
{	
	Menu();

	int opt;
	cin >> opt;
	Function(opt);

	return 0;
}