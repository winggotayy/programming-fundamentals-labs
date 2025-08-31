#include "Complex.h"

#include <windows.h>
#include <cctype>
#include <regex>
#include <cstring>
#include <fstream>

#pragma warning (disable : 4996)

void errorPrint(string str, int pos, int error);
bool Check(string str);
string Parse(string str);

int main()
{
	ofstream outputFile("history.txt");
	
	string str;
	Complex com;

	while (str != "quit")
	{
		cout << "请输入你要计算的表达式：" << endl;
		getline(cin, str);

		if (str == "quit")
		{
			cout << "退出程序" << endl;
			break;
		}
			
		if (str != "quit" && Check(str))
		{
			str = Parse(str);
			//cout << str << endl;
			Complex res = com.RPN(str);
			cout << res << endl;
			cout << " " << endl;

			// 合法算式的计算历史记录
			outputFile << "请输入你要计算的表达式：" << endl;
			outputFile << str << endl;
			outputFile << "结果为：" << res << endl;
			outputFile << " " << endl;
		}
	}

	outputFile.close();

	return 0;
}

void errorPrint(string s, int pos, int error)
{
	string temp;
	for (int i = 0; i < s.length(); i++)
	{
		if (i == pos)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4); // 红色
			cout << s[i];
			temp = s[i];
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // 白色
		}
		else
		{
			cout << s[i];
		}
	}
	cout << ", ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4); // 红色
	cout << "error: ";
	switch (error)
	{
	case 1:
	{
		cout << "开头不合法" << endl;
		cout << " " << endl;
		break;
	}
	case 2:
	{
		cout << "结尾不合法" << endl;
		cout << " " << endl;
		break;
	}
	case 3:
	{
		cout << "非法操作符后继" << endl;
		cout << " " << endl;
		break;
	}
	case 4:
	{
		cout << "非法实数后继" << endl;
		cout << " " << endl;
		break;
	}
	case 5:
	{
		cout << "非法i后继" << endl;
		cout << " " << endl;
		break;
	}
	case 6:
	{
		cout << "括号内的结果必须是实数" << endl;
		cout << " " << endl;
		break;
	}
	case 7:
	{
		cout << "非法 ( 后继" << endl;
		cout << " " << endl;
		break;
	}
	case 8:
	{
		cout << "非法 ) 后继" << endl;
		cout << " " << endl;
		break;
	}
	case 9:
	{
		cout << "括弧不匹配" << endl;
		cout << " " << endl;
		break;
	}
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

bool Check(string s)
{
	int error = 0;

	int len = s.length();

	// 开头不合法
	if (!isdigit(s[0]) && s[0] != '|' && s[0] != '-' && s[0] != 'i' && s[0] != '(' && s[0] != 'a' && s[0] != 'c')
	{
		error = 1;
		errorPrint(s, 0, error);
		return false;
	}
	// 结尾不合法
	else if (!isdigit(s[len - 1]) && s[len - 1] != '|' && s[len - 1] != 'i' && s[len - 1] != ')')
	{
		error = 2;
		errorPrint(s, len - 1, error);
		return false;
	}

	int cnt = 0;
	for (int i = 0; i < len; i++)
	{
		int len_check = len - 1;
		// 非法操作符后继(+-*/)
		if (i < len_check
			&& (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/')
			&& (s[i + 1] == ')' || s[i + 1] == '+' || s[i + 1] == '-' || s[i + 1] == '*' || s[i + 1] == '/'))
		{
			error = 3;
			errorPrint(s, i + 1, error);
			return false;
		}
		// 非法实数后继
		else if (i < len_check
			&& isdigit(s[i])
			&& (s[i + 1] == '|' || s[i + 1] == '('))
		{
			error = 4;
			errorPrint(s, i + 1, error);
			return false;
		}
		// 非法i后继
		else if (i < len_check
			&& s[i] == 'i' 
			&& (s[i + 1] == 'i' || s[i + 1] == '('))
		{
			error = 5;
			errorPrint(s, i + 1, error);
			return false;
		}
		// 括号内的结果必须是实数
		else if (i < len_check - 1 && s[i] == 'i' && s[i + 1] == ')' && s[i + 2] == 'i')
		{
			error = 6;
			errorPrint(s, i + 1, error);
			return false;
		}
		// 非法 ( 后继
		else if (i < len_check
			&& s[i] == '('
			&& (s[i + 1] == ')' || s[i + 1] == '+' || s[i + 1] == '*' || s[i + 1] == '/'))
		{
			error = 7;
			errorPrint(s, i + 1, error);
			return false;
		}
		// 非法 ) 后继
		if (i < len_check
			&& s[i] == ')' 
			&& (s[i + 1] == '(' || isdigit(s[i + 1])))
		{
			error = 8;
			errorPrint(s, i + 1, error);
			return false;
		}
		// 括弧不匹配
		else 
		{
			if (s[i] == '(')
				cnt++;
			if (s[i] == ')')
				cnt--;
			if (cnt < 0)
			{
				error = 9;
				errorPrint(s, i, error);
				return false;
			}
		}
	}
	// 括弧不匹配
	if (cnt != 0) 
	{
		error = 9;
		errorPrint(s, s.find("("), error);
		return false;
	}
}

string Parse(string s)
{
	// 在开头插入0，表示负数的0实部
	if (s[0] == '-')
		s.insert(0, "0");

	// 替换辐角主值(arg)和共轭(cjg)操作符
	s = regex_replace(s, regex("arg"), "a");
	s = regex_replace(s, regex("cjg"), "c");

	int cnt = 0;
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] == '|')
		{
			cnt++;
			if (cnt % 2 == 1)
			{
				// |后面紧跟负号，插入0，表示负数的0实部
				if (i < s.length() - 1 && s[i + 1] == '-')
					s.insert(i + 1, "0");

				// |前面紧跟右括号，插入*，表示乘法
				if (i > 0 && s[i - 1] == ')')
					s.insert(i, "*");
			}
		}
	}

	// 替换剩余的|为取模操作符(m)
	while (s.find("|") != -1) 
	{
		int pos = s.find("|");
		s.replace(pos, 1, "m(");
		pos = s.find("|", pos + 1);
		s.replace(pos, 1, ")");
	}
	/*
	s = regex_replace(s, regex("\\|"), "m(");
	s = regex_replace(s, regex("\\|"), ")");
	*/
	// 处理特殊情况，如果括号前面是负号，则在负号后面插入0，表示负数的0实部
	for (int i = 0; i < s.length() - 1; i++)
	{
		if (s[i] == '(' && s[i + 1] == '-')
			s.insert(i + 1, "0");
	}

	return s;
}