#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <unordered_set>
#include <set>

using namespace std;

/* ������Ϣ��ѯ */
// ��·��ѯ
void Line_Inquiry(string line_check)
{
	ifstream infile("������Ϣ.txt");
	if (!infile) 
	{
		cerr << "�޷��򿪴��ļ���" << endl;
		exit(0);
	}
	
	string buffer;
	bool found = false;
	while (getline(infile, buffer)) 
	{
		if (buffer == line_check) 
		{
			found = true;
			cout << line_check << "������վ�㣺" << endl;
			continue;
		}
		if (found) 
		{
			istringstream iss(buffer);	// ����Ϊ���ʣ��Կո�ָ��
			string station;
			iss >> station; // �������ĵ��ʶ�ȡ�� station ������
			cout << station << endl;
			if (buffer.empty()) 
			{
				break; // ��������ֹͣ���
			}
		}
	}

	infile.close();

	if (!found) 
	{
		cout << "����·�����ڣ�" << endl;
	}
}
// վ���ѯ
void Site_Inquiry()
{
	string site_check;
	//cin >> site_check;

	ifstream infile("������Ϣ1.txt");
	if (!infile)
	{
		cerr << "�޷��򿪴��ļ���" << endl;
		exit(0);
	}

	map<string, set<string>> line;

	string lineName;
	string site;

	// ��ȡ������Ϣ�ļ���������·��Ϣ�����ݽṹ
	while (getline(infile, lineName))
	{
		while (getline(infile, site) && !site.empty())
		{
			line[lineName].insert(site);
		}
	}

	// ��ѯվ���������·
	bool found = false;
	bool found_exact = false;
	set<string> stations;

	while (!found_exact) 
	{
		cin >> site_check;
		cout << "վ���������·:" << endl;
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
		// �ַ����Ӵ�ƥ�䣨�ؼ���������
		if (found && !found_exact) 
		{
			cout << "\n";
			cout << "--------------------------------" << endl;
			cout << "������Ҫѡ���վ�㣺" << endl;
			for (const auto& station : stations) 
			{
				cout << station << endl;
			}
			cout << "���������룺" << endl;
			cout << "--------------------------------" << endl;
		}
		else if (!found && found_exact == false) 
		{
			cout << "��վ�㲻���ڣ�" << endl;
			return;
		}
	}

	infile.close();
}

/* ����Ʊ���ѯ */

/*
bool is_in_line(map<string, int>& stations, string station) 
{
	return stations.find(station) != stations.end();
}
*/

// ���ַ���ת��Ϊ����
int mystod(string s) 
{
	int result = 0;
	for (int i = 0; i < s.size(); i++) 
	{
		result = result * 10 + (s[i] - '0');
	}
	return result;
}

// ��������·����ת��Ϊ��Ӧ�ı��
int linkedline(string subway) 
{
	if (subway == "1����") return 1;
	else if (subway == "2����") return 2;
	else if (subway == "3����") return 3;
	else if (subway == "4����") return 4;
	else if (subway == "10����") return 5;
	else if (subway == "S1��") return 6;
	else if (subway == "S3��") return 7;
	else if (subway == "S7��") return 8;
	else if (subway == "S8��") return 9;
	else if (subway == "S9��") return 10;
	return 0;
}

// Ʊ�ۼ���
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
	ifstream infile("������Ϣ2.txt");
	if (!infile)
	{
		cerr << "�޷��򿪴��ļ���" << endl;
		exit(0);
	}

	// ÿ���������ϵ�վ����
	int every_stations[11] = { 0, 27, 26, 29, 18, 14, 8, 19, 10, 17, 6 };
	// �洢����������·��վ��;���
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
	
	cout << "\n���룺" << dis << "��" << endl;

	int price = Price(dis);
	return price;
}

// �ַ����Ӵ�ƥ�䣨�ؼ���������
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

// ��·˵����ֱ�
void Route_Straight()
{
	ifstream infile("������Ϣ1.txt");
	if (!infile)
	{
		cerr << "�޷��򿪴��ļ���" << endl;
		exit(0);
	}

	string begin, end;
	
	// �������
	cout << "���������վ��";
	cin >> begin;
	
	// �Ӵ�ƥ��
	unordered_set<string> match_begin = findMatchingStrings(begin, "������Ϣ1.txt");
	if (match_begin.empty())
	{
		cout << "û���ҵ�ƥ���վ�㡣" << endl;
	}
	else
	{
		bool exactMatch = false;  // �ж��Ƿ�����ȫƥ���վ��
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
			cout << "������Ҫѡ����ǣ�" << endl;
			for (const string& station : match_begin)
			{
				cout << station << endl;
			}
			cout << "--------------------------------" << endl;
			cout << "���������룺";
			cin >> begin;
		}
	}
	
	// �����յ�
	cout << "�������յ�վ��";
	cin >> end;

	// �Ӵ�ƥ��
	unordered_set<string> match_end = findMatchingStrings(end, "������Ϣ1.txt");
	if (match_end.empty())
	{
		cout << "û���ҵ�ƥ���վ�㡣" << endl;
	}
	else
	{
		bool exactMatch = false;  // �ж��Ƿ�����ȫƥ���վ��
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
			cout << "������Ҫѡ����ǣ�" << endl;
			for (const string& station : match_end)
			{
				cout << station << endl;
			}
			cout << "--------------------------------" << endl;
			cout << "���������룺";
			cin >> end;
		}
	}

	map<string, set<pair<int, string>>>line;
	string lineName;
	string site;
	int i = 0;
	// ��ȡ������Ϣ�ļ���������·��Ϣ�����ݽṹ
	while (getline(infile, lineName))
	{
		for (; getline(infile, site) && !site.empty(); i++)
		{
			line[lineName].insert({ i, site });
		}
	}

/* ֱ�� */
	// ��������յ��Ƿ���ͬһ��set��
	bool straight = false;
	vector<string> stationsBetween;  // �洢�����յ�֮�������վ��
	string curLine;	// ��ǰ��·����
	for (const auto& entry : line)
	{
		const string& lineName = entry.first;  // ��ȡ��·����
		const set<pair<int, string>>& stations = entry.second;
		int beginOrder = -1;  // ����˳��
		int endOrder = -1;    // �յ��˳��

		// ������ǰ��·������վ�㣬����¼�����յ��˳��
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

			// ��������յ��˳�����ҵ���������ѭ��
			if (beginOrder != -1 && endOrder != -1)
			{
				break;
			}
		}

		if (beginOrder != -1 && endOrder != -1)
		{
			straight = true;
			curLine = lineName;  // ��¼��ǰ��·����

			// ���������յ��˳�������㵽�յ�֮���վ�㵽stationsBetween������
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

	// ������
	if (straight)
	{
		int count = 1;
		cout << "\n* ֱ�� *" << endl;
		cout << curLine << "��" << endl;  // �����ǰ��·����
		cout << begin << "->";
		for (const string& station : stationsBetween)
		{
			cout << station << "->";
			++count;
		}
		cout << end << endl;
		cout << "-- �� " << count << " վ --" << endl;
	}
	else
	{
		cout << "��ֱ��·����" << endl;
	}

	infile.close();

/*Ʊ�ۣ�ֱ�*/
	cout << "\nƱ�ۣ�" << CalTicket(begin, end) << "Ԫ" << endl;
}

void Menu()
{
	system("color 71");
	cout << endl;
	cout << "\t\t\t\t\t\t    ��ӭʹ���Ͼ�����Ʊ��ϵͳ";
	cout << endl;
	cout << "\t\t\t\t\t\t--------------------------------";
	cout << endl;
	cout << "\t\t\t\t\t\t�����룺" << endl;
	cout << "\t\t\t\t\t\t        1   ��·��ѯ" << endl;
	cout << "\t\t\t\t\t\t        2   վ���ѯ" << endl;
	cout << "\t\t\t\t\t\t        3   Ʊ���ѯ" << endl;
	cout << "\t\t\t\t\t\t        4   �˳�" << endl;
	cout << "\n** ����������Ҫ�Ĳ�����ţ�";
}

void Function(int opt)
{
	switch (opt)
	{
	case 1:
	{
		cout << "------------------------------------------------------------------------------------" << endl;
		cout << "��·��" << endl;
		cout << "1����\t2����\t3����\t4����\t10����\tS1��\tS3��\tS7��\tS8��\tS9��\n";
		cout << "------------------------------------------------------------------------------------" << endl;
		cout << "������Ҫ��ѯ����·��";

		string line_check;
		cin >> line_check;
		Line_Inquiry(line_check);

		cout << "** ����������Ҫ�Ĳ�����ţ�";
		int new_opt;
		cin >> new_opt;
		Function(new_opt);
	}
	case 2:
	{
		cout << "������Ҫ��ѯ��վ�㣺";
		Site_Inquiry();

		cout << "\n** ����������Ҫ�Ĳ�����ţ�";
		int new_opt;
		cin >> new_opt;
		Function(new_opt);
	}
	case 3:; 
	{
		Route_Straight();

		cout << "\n** ����������Ҫ�Ĳ�����ţ�";
		int new_opt;
		cin >> new_opt;
		Function(new_opt);
	}
	case 4:
	{
		cout << "ллʹ�ã�" << endl;
		exit(0);
		break;
	}
	default:
	{
		cout << "���������룺";
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