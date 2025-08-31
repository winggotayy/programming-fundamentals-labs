#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <ctime>

#include <graphics.h>
#include <conio.h>

#include "QuestionBank.h"

using namespace std;

void Init();
wchar_t* to_wchar(string text);
void Register();
void Login();
void Mode();
void Exercise();
void MockExam(int questionCount, int timeLimit);
void Exam(int questionCount, int timeLimit);

struct User
{
	string username;
	string password; 
	int score;
	int answerTime;
};

// 全局变量
vector<User> users;
string usersfile = "users.txt";
//string rankfile = "rank.txt";

//void DisplayRanking();

// 首页
void Init()
{
	// 读取用户信息文件
	ifstream file(usersfile);
	if (file.is_open()) 
	{
		string line;
		while (getline(file, line)) 
		{
			istringstream iss(line);
			User user;
			iss >> user.username >> user.password;
			users.push_back(user);
		}
		file.close();
	}
	else 
	{
		MessageBox(GetHWnd(), L"无法打开文件", L"错误", MB_OK);
		exit(0);
	}

	// 初始化图形界面并设置窗口大小为屏幕大小
	initgraph(800, 450);

	// 清空图形界面
	cleardevice();

	setbkmode(TRANSPARENT);

	// 创建一个IMAGE对象
	IMAGE bgimg;
	// 加载背景图片
	loadimage(&bgimg, _T("init.jpg"));
	// 在图形界面上绘制背景图片
	putimage(0, 0, &bgimg);

	// 绘制标题
	settextstyle(50, 0, _T("隶书"));
	settextcolor(BLACK);
	outtextxy(250, 50, _T("知识竞赛小程序"));

	// 绘制选项
	setfillcolor(WHITE);
	setlinecolor(BLACK);
	settextstyle(25, 0, _T("黑体"));
	settextcolor(BLACK);

	fillrectangle(370, 125, 470, 160);
	outtextxy(395, 130, _T("注册"));

	fillrectangle(370, 175, 470, 210);
	outtextxy(395, 180, _T("登录"));

	fillrectangle(370, 225, 470, 260);
	outtextxy(395, 230, _T("退出"));

	MOUSEMSG m;
	while (true)
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (m.x > 370 && m.x < 470 && m.y > 125 && m.y < 160)
			{
				// 注册功能
				Register();
				break;
			}
			else if (m.x > 370 && m.x < 470 && m.y > 175 && m.y < 210)
			{
				// 登录功能
				Login();
				break;
			}
			else if (m.x > 370 && m.x < 470 && m.y > 225 && m.y < 260)
			{
				// 退出功能
				closegraph();
			}
		}
	}
}

// 将 string 转换成 wchar
wchar_t* to_wchar(string text)
{
	static wchar_t result[99];
	unsigned len = 0;
	for (auto& sym : text)
	{
		result[len++] = (wchar_t)sym;
	}
	result[len] = (wchar_t)'\0';
	return result;
}

// 实现文本框控件
class EasyTextBox
{
private:
	int left = 0, top = 0, right = 0, bottom = 0;	// 控件坐标
	string text = "";								// 控件内容
	size_t maxlen = 0;								// 文本框最大内容长度

public:
	void Create(int x1, int y1, int x2, int y2, int max)
	{
		maxlen = max;
		text = "";
		left = x1, top = y1, right = x2, bottom = y2;

		// 绘制用户界面
		Show();
	}
	/*
	~EasyTextBox()
	{
		if (text != NULL)
			delete[] text;
	}
	*/
	string Text()
	{
		return text;
	}

	bool Check(int x, int y)
	{
		return (left <= x && x <= right && top <= y && y <= bottom);
	}

	// 绘制界面
	void Show()
	{
		// 备份环境值
		int oldlinecolor = getlinecolor();
		int oldbkcolor = getbkcolor();
		int oldfillcolor = getfillcolor();

		setlinecolor(LIGHTGRAY);		// 设置画线颜色
		setbkcolor(0xeeeeee);			// 设置背景颜色
		setfillcolor(0xeeeeee);			// 设置填充颜色
		fillrectangle(left, top, right, bottom);
		outtextxy(left + 10, top + 5, to_wchar(text));

		// 恢复环境值
		setlinecolor(oldlinecolor);
		setbkcolor(oldbkcolor);
		setfillcolor(oldfillcolor);
	}

	void OnMessage()
	{
		// 备份环境值
		int oldlinecolor = getlinecolor();
		int oldbkcolor = getbkcolor();
		int oldfillcolor = getfillcolor();

		setlinecolor(BLACK);			// 设置画线颜色
		setbkcolor(WHITE);				// 设置背景颜色
		setfillcolor(WHITE);			// 设置填充颜色
		fillrectangle(left, top, right, bottom);
		outtextxy(left + 10, top + 5, to_wchar(text));

		int width = textwidth(to_wchar(text));	// 字符串总宽度
		int counter = 0;						// 光标闪烁计数器
		bool binput = true;						// 是否输入中

		ExMessage msg;
		while (binput)
		{
			while (binput && peekmessage(&msg, EX_MOUSE | EX_CHAR, false))	// 获取消息，但不从消息队列拿出
			{
				if (msg.message == WM_LBUTTONDOWN)
				{
					// 如果鼠标点击文本框外面，结束文本输入
					if (msg.x < left || msg.x > right || msg.y < top || msg.y > bottom)
					{
						binput = false;
						break;
					}
				}
				else if (msg.message == WM_CHAR)
				{
					size_t len = wcslen(to_wchar(text));
					switch (msg.ch)
					{
					case '\b':				// 用户按退格键，删掉一个字符
						if (len > 0)
						{
							text.pop_back();
							width = textwidth(to_wchar(text));
							counter = 0;
							clearrectangle(left + 10 + width, top + 1, right - 1, bottom - 1);
						}
						break;
					case '\r':				// 用户按回车键，结束文本输入
					case '\n':
						binput = false;
						break;
					default:				// 用户按其它键，接受文本输入
						if (len < maxlen - 1)
						{
							text.push_back(msg.ch);
							//text += msg.ch;
							//text[++len] = 0;

							clearrectangle(left + 10 + width + 1, top + 3, left + 10 + width + 1, bottom - 3);	// 清除画的光标
							width = textwidth(to_wchar(text));					// 重新计算文本框宽度
							counter = 0;
							outtextxy(left + 10, top + 5, to_wchar(text));		// 输出新的字符串
						}
					}
				}
				peekmessage(NULL, EX_MOUSE | EX_CHAR);				// 从消息队列抛弃刚刚处理过的一个消息
			}

			// 绘制光标（光标闪烁周期为 20ms * 32）
			counter = (counter + 1) % 32;
			if (counter < 16)
				line(left + 10 + width + 1, top + 3, left + 10 + width + 1, bottom - 3);				// 画光标
			else
				clearrectangle(left + 10 + width + 1, top + 3, left + 10 + width + 1, bottom - 3);		// 擦光标

			// 延时 20ms
			Sleep(20);
		}

		clearrectangle(left + 10 + width + 1, top + 3, left + 10 + width + 1, bottom - 3);				// 擦光标

		// 恢复环境值
		setlinecolor(oldlinecolor);
		setbkcolor(oldbkcolor);
		setfillcolor(oldfillcolor);

		Show();
	}
};

// 实现按钮控件
class EasyButton
{
private:
	int left = 0, top = 0, right = 0, bottom = 0;	// 控件坐标
	wchar_t* text = NULL;							// 控件内容
	void (*userfunc)() = NULL;						// 控件消息

public:
	void Create(int x1, int y1, int x2, int y2, const wchar_t* title, void (*func)())
	{
		text = new wchar_t[wcslen(title) + 1];
		wcscpy_s(text, wcslen(title) + 1, title);
		left = x1, top = y1, right = x2, bottom = y2;
		userfunc = func;

		// 绘制用户界面
		Show();
	}

	~EasyButton()
	{
		if (text != NULL)
			delete[] text;
	}

	bool Check(int x, int y)
	{
		return (left <= x && x <= right && top <= y && y <= bottom);
	}

	// 绘制界面
	void Show()
	{
		int oldlinecolor = getlinecolor();
		int oldbkcolor = getbkcolor();
		int oldfillcolor = getfillcolor();

		setlinecolor(BLACK);			// 设置画线颜色
		setbkcolor(WHITE);				// 设置背景颜色
		setfillcolor(WHITE);			// 设置填充颜色
		fillrectangle(left, top, right, bottom);
		outtextxy(left + (right - left - textwidth(text) + 1) / 2, top + (bottom - top - textheight(text) + 1) / 2, text);

		setlinecolor(oldlinecolor);
		setbkcolor(oldbkcolor);
		setfillcolor(oldfillcolor);
	}

	void OnMessage()
	{
		if (userfunc != NULL)
			userfunc();
	}
};

// 定义控件
EasyTextBox txtName;
EasyTextBox txtPwd;
EasyButton btnOK;
EasyButton btnReturn; // 回到首页

// 按钮 btnOK 的点击事件，即注册按钮的点击事件处理函数
void On_btnReg_Click()
{
	// 检查用户名唯一性
	for (const auto& user : users)
	{
		if (user.username == txtName.Text())
		{
			MessageBox(GetHWnd(), L"用户名已存在！", L"错误", MB_OK | MB_ICONERROR);
			return;
		}
	}

	// 检查密码长度
	if (wcslen(to_wchar(txtPwd.Text())) < 8)
	{
		MessageBox(GetHWnd(), L"密码长度必须至少为八位！", L"错误", MB_OK | MB_ICONERROR);
		return;
	}

	// 创建新用户
	User newUser;
	newUser.username = txtName.Text();
	newUser.password = txtPwd.Text();

	// 将新用户添加到用户列表
	users.push_back(newUser);

	// 保存用户信息到文件
	ofstream file(usersfile, ios::app);
	if (file.is_open())
	{
		file << newUser.username << ' ' << newUser.password << endl;
		file.close();
	}
	else
	{
		MessageBox(GetHWnd(), L"无法打开文件", L"错误", MB_OK | MB_ICONERROR);
		return;
	}

	MessageBox(GetHWnd(), L"注册成功！", L"成功", MB_OK | MB_ICONINFORMATION);
	Init();
}

// 按钮 btnReturn 的点击事件
void On_btnReturn_Click()
{
	// 回到首页
	Init();
}

void Register()
{
	// 绘制注册界面
	setbkcolor(0xeeeeee);
	cleardevice();
	settextcolor(BLACK);
	settextstyle(18, 0, _T("黑体"));
	outtextxy(50, 55, L"用户名：");
	txtName.Create(120, 50, 400, 75, 10);						// 创建用户名文本框控件
	outtextxy(50, 105, L"密　码：");
	txtPwd.Create(120, 100, 400, 125, 10);						// 创建密码文本框控件
	btnOK.Create(320, 150, 400, 175, L"注册", On_btnReg_Click);	// 创建按钮控件
	btnReturn.Create(320, 200, 400, 225, L"返回", On_btnReturn_Click); // 创建返回按钮控件


	ExMessage msg;
	while (true)
	{
		msg = getmessage(EX_MOUSE);			// 获取消息输入

		if (msg.message == WM_LBUTTONDOWN)
		{
			// 判断控件
			if (txtName.Check(msg.x, msg.y))	txtName.OnMessage();

			// 判断控件
			if (txtPwd.Check(msg.x, msg.y))		txtPwd.OnMessage();

			// 判断控件
			if (btnOK.Check(msg.x, msg.y))		btnOK.OnMessage();

			// 判断控件
			if (btnReturn.Check(msg.x, msg.y))	btnReturn.OnMessage();
		}
	}
}

// 按钮 btnOK 的点击事件，即登录按钮的点击事件处理函数
void On_btnLog_Click()
{
	string username = txtName.Text();
	string password = txtPwd.Text();

	bool isLoggedIn = false;

	for (const auto& user : users)
	{
		if (user.username == username && user.password == password)
		{
			isLoggedIn = true;
			break;
		}
	}
	if (isLoggedIn)
	{
		MessageBox(NULL, _T("登录成功！"), _T("登录成功"), MB_OK | MB_ICONINFORMATION);
		// 登录成功后，跳转到选择界面
		Mode();
	}
	else
	{
		// 登录失败的处理
		MessageBox(NULL, _T("用户名或密码错误，请重新输入！"), _T("登录失败"), MB_OK | MB_ICONERROR);
		return;
	}
}

void Login()
{
	// 绘制注册界面
	setbkcolor(0xeeeeee);
	cleardevice();
	settextcolor(BLACK);
	settextstyle(18, 0, _T("黑体"));
	outtextxy(50, 55, L"用户名：");
	txtName.Create(120, 50, 400, 75, 10);						// 创建用户名文本框控件
	outtextxy(50, 105, L"密　码：");
	txtPwd.Create(120, 100, 400, 125, 10);						// 创建密码文本框控件
	btnOK.Create(320, 150, 400, 175, L"登录", On_btnLog_Click);	// 创建按钮控件
	btnReturn.Create(320, 200, 400, 225, L"返回", On_btnReturn_Click); // 创建返回按钮控件


	ExMessage msg;
	while (true)
	{
		msg = getmessage(EX_MOUSE);			// 获取消息输入

		if (msg.message == WM_LBUTTONDOWN)
		{
			// 判断控件
			if (txtName.Check(msg.x, msg.y))	txtName.OnMessage();

			// 判断控件
			if (txtPwd.Check(msg.x, msg.y))		txtPwd.OnMessage();

			// 判断控件
			if (btnOK.Check(msg.x, msg.y))		btnOK.OnMessage();

			// 判断控件
			if (btnReturn.Check(msg.x, msg.y))	btnReturn.OnMessage();
		}
	}
}

void Mode()
{
	// 初始化图形界面并设置窗口大小为屏幕大小
	initgraph(800, 450);

	setbkmode(TRANSPARENT);
	setbkcolor(LIGHTGRAY);
	// 清空图形界面
	cleardevice();
	
	// 绘制标题
	settextstyle(50, 0, _T("隶书"));
	settextcolor(BLACK);
	outtextxy(275, 50, _T("选择模式"));

	// 绘制选项
	setfillcolor(WHITE);
	setlinecolor(BLACK);
	settextstyle(25, 0, _T("黑体"));
	settextcolor(BLACK);

	fillrectangle(275, 125, 470, 160);
	outtextxy(350, 130, _T("练习"));

	fillrectangle(275, 175, 470, 210);
	outtextxy(325, 180, _T("模拟竞赛"));

	fillrectangle(275, 225, 470, 260);
	outtextxy(325, 230, _T("正式竞赛"));

	fillrectangle(275, 275, 470, 310);
	outtextxy(325, 280, _T("查看排行"));

	fillrectangle(275, 325, 470, 360);
	outtextxy(325, 330, _T("返回首页"));

	MOUSEMSG m;
	while (true)
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (m.x > 275 && m.x < 470 && m.y > 125 && m.y < 160)
			{
				// 练习
				Exercise();
				break;
			}
			else if (m.x > 275 && m.x < 470 && m.y > 175 && m.y < 210)
			{
				// 模拟竞赛
				MockExam(20, 30); // 20题，30秒
				break;
			}
			else if (m.x > 275 && m.x < 470 && m.y > 225 && m.y < 260)
			{
				// 正式竞赛
				Exam(20, 30); // 20题，30秒
				break;
			}
			else if (m.x > 275 && m.x < 470 && m.y > 275 && m.y < 310)
			{
				// 查看排行
				// DisplayRanking();
				break;
			}
			else if (m.x > 275 && m.x < 470 && m.y > 325 && m.y < 360)
			{
				// 回到首页
				Init();
				break;
			}
		}
	}
}

void InitQues()
{
	for (unsigned i = 0; i < question_content.size(); i++) 
	{
		questions.push_back(
			new_Question(
				question_content[i],
				question_optionA[i],
				question_optionB[i],
				question_optionC[i],
				question_answer[i]
			)
		);
	}
#if (0)
	Question q2;
	wcscpy_s(q2.content, L"1947 年夏，刘伯承、邓小平率领_____解放军主力挺进大别山，人民解放军转入战略进攻。");
	wcscpy_s(q2.optionA, L"A. 陕甘宁");
	wcscpy_s(q2.optionB, L"B. 中原");
	wcscpy_s(q2.optionC, L"C. 晋冀鲁豫");
	wcscpy_s(q2.answer, L"C. 晋冀鲁豫");
	questions.push_back(q2);

	Question q3;
	wcscpy_s(q3.content, L"马克思主义最根本的世界观和方法论是_____。");
	wcscpy_s(q3.optionA, L"A. 辩证唯物主义和历史唯物主义");
	wcscpy_s(q3.optionB, L"B. 唯物主义和辩证法");
	wcscpy_s(q3.optionC, L"C. 剩余价值学说和劳动价值论");
	wcscpy_s(q3.answer, L"A. 辩证唯物主义和历史唯物主义");
	questions.push_back(q3);

	Question q4;
	wcscpy_s(q4.content, L"发展国民经济的第一个五年计划是_____年开始的。");
	wcscpy_s(q4.optionA, L"A. 1951");
	wcscpy_s(q4.optionB, L"B. 1952");
	wcscpy_s(q4.optionC, L"C. 1953");
	wcscpy_s(q4.answer, L"C. 1953");
	questions.push_back(q4);

	Question q5;
	wcscpy_s(q5.content, L"1961 年 1 月，党的八届九中全会正式决定对国民经济实行“_____、巩固、充实、提高”的八字方针。");
	wcscpy_s(q5.optionA, L"A. 调整");
	wcscpy_s(q5.optionB, L"B. 整顿");
	wcscpy_s(q5.optionC, L"C. 修整");
	wcscpy_s(q5.answer, L"A. 调整");
	questions.push_back(q5);

	Question q6;
	wcscpy_s(q6.content, L"安徽凤阳_____农民首先实行分田包产到户，自负盈亏。这种做法得到中央的肯定。随后，在中央的指导下，农村逐步实行家庭联产责任承包制，农业生产得到极大发展。");
	wcscpy_s(q6.optionA, L"A. 华西村");
	wcscpy_s(q6.optionB, L"B. 小岗村");
	wcscpy_s(q6.optionC, L"C. 大寨村");
	wcscpy_s(q6.answer, L"B. 小岗村");
	questions.push_back(q6);

	Question q7;
	wcscpy_s(q7.content, L"1920 年 6 月，陈独秀、李汉俊、俞秀松等人在上海陈独秀寓所开会，决定成立共产党，初步定名社会共产党。后征求_____的意见定名为共产党。");
	wcscpy_s(q7.optionA, L"A. 李大钊");
	wcscpy_s(q7.optionB, L"B. 李达");
	wcscpy_s(q7.optionC, L"C. 邵力子");
	wcscpy_s(q7.answer, L"A. 李大钊");
	questions.push_back(q7);
	
	Question q8;
	wcscpy_s(q8.content, L"1928 年的中共“六大”在中国国内白色恐怖氛围的情况下，选择在_____召开，这是中国共产党仅有的一次在国外召开的代表大会。");
	wcscpy_s(q8.optionA, L"A. 巴黎");
	wcscpy_s(q8.optionB, L"B. 莫斯科");
	wcscpy_s(q8.optionC, L"C. 柏林");
	wcscpy_s(q8.answer, L"B. 莫斯科");
	questions.push_back(q8);

	Question q9;
	wcscpy_s(q9.content, L"1934 年，中华苏维埃第二次全国代表大会召开，_____作中华苏维埃共和国中央执行委员会两年来工作报告，朱德作红军建设报告。");
	wcscpy_s(q9.optionA, L"A. 张国焘");
	wcscpy_s(q9.optionB, L"B. 陈独秀");
	wcscpy_s(q9.optionC, L"C. 毛泽东");
	wcscpy_s(q9.answer, L"C. 毛泽东");
	questions.push_back(q9);

	Question q10;
	wcscpy_s(q10.content, L"1937 年_____，卢沟桥事变，全国性的抗日战争从此爆发。在民族危机空前严重的时刻，国共两党再次合作，正式建立抗日民族统一战线。");
	wcscpy_s(q10.optionA, L"A. 7 月 5 日");
	wcscpy_s(q10.optionB, L"B. 7 月 6 日");
	wcscpy_s(q10.optionC, L"C. 7 月 7 日");
	wcscpy_s(q10.answer, L"C. 7 月 7 日");
	questions.push_back(q10);

	Question q11;
	wcscpy_s(q11.content, L"抗日战争胜利后，为尽一切可能争取国内和平，戳穿蒋介石假和平的阴谋，1945 年 8 月，毛泽东在周恩来等人的陪同下，赴_____同国民党进行谈判。");
	wcscpy_s(q11.optionA, L"A. 重庆");
	wcscpy_s(q11.optionB, L"B. 南京");
	wcscpy_s(q11.optionC, L"C. 武汉");
	wcscpy_s(q11.answer, L"A. 重庆");
	questions.push_back(q11);

	Question q12;
	wcscpy_s(q12.content, L"《共产党宣言》中文全译本首译者是_____。");
	wcscpy_s(q12.optionA, L"A. 陈独秀");
	wcscpy_s(q12.optionB, L"B. 陈望道");
	wcscpy_s(q12.optionC, L"C. 蔡和森");
	wcscpy_s(q12.answer, L"B. 陈望道");
	questions.push_back(q12);

	Question q13;
	wcscpy_s(q13.content, L"人类历史上第一次获得胜利的社会主义革命是_____。");
	wcscpy_s(q13.optionA, L"A. 法国巴黎公社");
	wcscpy_s(q13.optionB, L"B. 俄国二月革命");
	wcscpy_s(q13.optionC, L"C. 俄国十月革命");
	wcscpy_s(q13.answer, L"C. 俄国十月革命");
	questions.push_back(q13);

	Question q14;
	wcscpy_s(q14.content, L"中华人民共和国国歌《义勇军进行曲》由_____作曲。");
	wcscpy_s(q14.optionA, L"A. 田汉");
	wcscpy_s(q14.optionB, L"B. 聂耳");
	wcscpy_s(q14.optionC, L"C. 冼星海");
	wcscpy_s(q14.answer, L"B. 聂耳");
	questions.push_back(q14);

	Question q15;
	wcscpy_s(q15.content, L"1954 年 9 月，第一届全国人民代表大会在北京召开。大会制定了《中华人民共和国宪法》，宪法规定我国坚持_____，坚持社会主义道路，一切权力属于人民。");
	wcscpy_s(q15.optionA, L"A. 人民民主专政");
	wcscpy_s(q15.optionB, L"B. 人民当家作主");
	wcscpy_s(q15.optionC, L"C. 民主集中制");
	wcscpy_s(q15.answer, L"A. 人民民主专政");
	questions.push_back(q15);

	Question q16;
	wcscpy_s(q16.content, L"1978 年 5 月，_____发表《实践是检验真理的唯一标准》的文章，引起了思想理论界关于真理标准问题的讨论。");
	wcscpy_s(q16.optionA, L"A. 《人民日报》");
	wcscpy_s(q16.optionB, L"B. 《光明日报》");
	wcscpy_s(q16.optionC, L"C. 《求是》杂志");
	wcscpy_s(q16.answer, L"B. 《光明日报》");
	questions.push_back(q16);

	Question q17;
	wcscpy_s(q17.content, L"“一国两制”构想的提出最初是为了解决_____。");
	wcscpy_s(q17.optionA, L"A. 香港问题");
	wcscpy_s(q17.optionB, L"B. 台湾问题");
	wcscpy_s(q17.optionC, L"C. 澳门问题");
	wcscpy_s(q17.answer, L"B. 台湾问题");
	questions.push_back(q17);

	Question q18;
	wcscpy_s(q18.content, L"2000 年 2 月 20 日至 25 日，江泽民在_____考察工作期间提出了“三个代表”重要思想。");
	wcscpy_s(q18.optionA, L"A. 上海");
	wcscpy_s(q18.optionB, L"B. 江苏");
	wcscpy_s(q18.optionC, L"C. 广东");
	wcscpy_s(q18.answer, L"C. 广东");
	questions.push_back(q18);

	Question q19;
	wcscpy_s(q19.content, L"2005 年 12 月，十届全国人大常委会第十九次会议决定，自 2006 年 1 月 1 日起废止《中华人民共和国_____条例》。\
		由此，一个在我国存在两千多年的古老税种宣告终结。");
	wcscpy_s(q19.optionA, L"A. 农业税");
	wcscpy_s(q19.optionB, L"B. 财产税");
	wcscpy_s(q19.optionC, L"C. 增值税");
	wcscpy_s(q19.answer, L"A. 农业税");
	questions.push_back(q19);

	Question q20;
	wcscpy_s(q20.content, L"科学发展观的第一要义是_____。");
	wcscpy_s(q20.optionA, L"A. 发展");
	wcscpy_s(q20.optionB, L"B. 统筹兼顾");
	wcscpy_s(q20.optionC, L"C. 以人为本");
	wcscpy_s(q20.answer, L"A. 发展");
	questions.push_back(q20);

	Question q21;
	wcscpy_s(q21.content, L"党的十九大报告进一步提出了加快建设创新型国家，到_____年中国跻身创新型国家前列的目标。");
	wcscpy_s(q21.optionA, L"A. 2030");
	wcscpy_s(q21.optionB, L"B. 2035");
	wcscpy_s(q21.optionC, L"C. 2050");
	wcscpy_s(q21.answer, L"B. 2035");
	questions.push_back(q21);

	Question q22;
	wcscpy_s(q22.content, L"党的十九大报告指出，十八大以来我国经济保持中高速增长，在世界主要国家中名列前茅，国内生产总值从 54 万\
							亿元增长到 80 万亿元，稳居世界_____。");
	wcscpy_s(q22.optionA, L"A. 第二");
	wcscpy_s(q22.optionB, L"B. 第三");
	wcscpy_s(q22.optionC, L"C. 第四");
	wcscpy_s(q22.answer, L"A. 第二");
	questions.push_back(q22);

	Question q23;
	wcscpy_s(q23.content, L"党的十九大报告指出，建设现代化经济体系，必须把发展经济的着力点放在_____上，\
							把提高供给体系质量作为主攻方向，显著增强我国经济质量优势。");
	wcscpy_s(q23.optionA, L"A. 实体经济");
	wcscpy_s(q23.optionB, L"B. 电商经济");
	wcscpy_s(q23.optionC, L"C. 跨境经济");
	wcscpy_s(q23.answer, L"A. 实体经济");
	questions.push_back(q23);

	Question q24;
	wcscpy_s(q24.content, L"党的十九大报告强调要全面实施市场准入_____制度，清理废除妨碍统一市场和公平竞争的各种规定和做法，支持民营企业发展，激发各类市场主体活力。");
	wcscpy_s(q24.optionA, L"A. 负面清单");
	wcscpy_s(q24.optionB, L"B. 正面清单");
	wcscpy_s(q24.optionC, L"C. 行政许可");
	wcscpy_s(q24.answer, L"A. 负面清单");
	questions.push_back(q24);

	Question q25;
	wcscpy_s(q25.content, L"党的十九大提出，保持土地承包关系稳定并长久不变，第二轮土地承包到期后再延长_____。");
	wcscpy_s(q25.optionA, L"A. 20 年");
	wcscpy_s(q25.optionB, L"B. 30 年");
	wcscpy_s(q25.optionC, L"C. 50 年");
	wcscpy_s(q25.answer, L"B. 30 年");
	questions.push_back(q25);

	Question q26;
	wcscpy_s(q26.content, L"中共中央政治局 2012 年 12 月 4 日召开会议，审议中央政治局关于改进工作作风、密切联系群众的_____规定，分\
		析研究 2013 年经济工作。会议强调，制定这方面的规定，指导思想就是从严要求，体现从严治党。");
	wcscpy_s(q26.optionA, L"A. 六项");
	wcscpy_s(q26.optionB, L"B. 七项");
	wcscpy_s(q26.optionC, L"C. 八项");
	wcscpy_s(q26.answer, L"C. 八项");
	questions.push_back(q26);

	Question q27;
	wcscpy_s(q27.content, L"2013 年 11 月，习近平到_____考察时首次作出了“实事求是、因地制宜、分类指导、精准扶贫”的重要指示。");
	wcscpy_s(q27.optionA, L"A. 贵州遵义");
	wcscpy_s(q27.optionB, L"B. 湖北宜昌");
	wcscpy_s(q27.optionC, L"C. 湖南湘西");
	wcscpy_s(q27.answer, L"C. 湖南湘西");
	questions.push_back(q27);

	Question q28;
	wcscpy_s(q28.content, L"2013 年 11 月，习近平到_____考察时首次作出了“实事求是、因地制宜、分类指导、精准扶贫”的重要指示。");
	wcscpy_s(q28.optionA, L"A. 贵州遵义");
	wcscpy_s(q28.optionB, L"B. 湖北宜昌");
	wcscpy_s(q28.optionC, L"C. 湖南湘西");
	wcscpy_s(q28.answer, L"C. 湖南湘西");
	questions.push_back(q28);

	Question q29;
	wcscpy_s(q29.content, L"2018 年 4 月 13 日，习近平总书记宣布，党中央决定支持海南逐步探索、稳步推进中国特色_____建设。");
	wcscpy_s(q29.optionA, L"A. 国际旅游岛");
	wcscpy_s(q29.optionB, L"B. 高新技术开发区");
	wcscpy_s(q29.optionC, L"C. 自由贸易港");
	wcscpy_s(q29.answer, L"C. 自由贸易港");
	questions.push_back(q29);

	Question q30;
	wcscpy_s(q30.content, L"_____通过宪法修正案，郑重地把习近平新时代中国特色社会主义思想载入宪法。");
	wcscpy_s(q30.optionA, L"A. 十二届全国人大三次会议");
	wcscpy_s(q30.optionB, L"B. 十二届全国人大六次会议");
	wcscpy_s(q30.optionC, L"C. 十三届全国人大一次会议");
	wcscpy_s(q30.answer, L"C. 十三届全国人大一次会议");
	questions.push_back(q30);

	Question q31;
	wcscpy_s(q31.content, L"习近平总书记在党的十九大报告中指出，我国社会生产力水平总体上显著提高，\
		社会生产能力在很多方面进入世界前列，更加突出的问题是_____，这已经成为满足人民日益增长的美好生活需要的主要制约因素。");
	wcscpy_s(q31.optionA, L"A. 产能过剩严重");
	wcscpy_s(q31.optionB, L"B. 社会分配不公平");
	wcscpy_s(q31.optionC, L"C. 靠物质资源消耗实现的粗放型高速增长");
	wcscpy_s(q31.answer, L"B. 社会分配不公平");
	questions.push_back(q31);

	Question q32;
	wcscpy_s(q32.content, L"习近平总书记明确提出，供给侧结构性改革的最终目的是满足需求，根本途径是_____。");
	wcscpy_s(q32.optionA, L"A. 提高和发展生产力");
	wcscpy_s(q32.optionB, L"B. 深化改革");
	wcscpy_s(q32.optionC, L"C. 调整经济结构");
	wcscpy_s(q32.answer, L"B. 深化改革");
	questions.push_back(q32);

	Question q33;
	wcscpy_s(q33.content, L"习近平总书记在关于《中共中央关于制定国民经济和社会发展第十四个五年规划和二 O 三五年远景目标的建议》的说明中指出，\
							当前，我国社会主要矛盾已经转化为人民日益增长的美好生活需要和不平衡不充分的发展之间的矛盾，\
							发展中的矛盾和问题集中体现在_____上。");
	wcscpy_s(q33.optionA, L"A. 发展速度");
	wcscpy_s(q33.optionB, L"B. 发展动力");
	wcscpy_s(q33.optionC, L"C. 发展质量");
	wcscpy_s(q33.answer, L"C. 发展质量");
	questions.push_back(q33);

	Question q34;
	wcscpy_s(q34.content, L"十八届三中全会指出，我国全面深化改革的出发点和落脚点是_____。");
	wcscpy_s(q34.optionA, L"A. 为人民服务");
	wcscpy_s(q34.optionB, L"B. 促进社会公平正义、增进人民福祉");
	wcscpy_s(q34.optionC, L"C. 实现中国梦");
	wcscpy_s(q34.answer, L"B. 促进社会公平正义、增进人民福祉");
	questions.push_back(q34);

	Question q35;
	wcscpy_s(q35.content, L"1921 年_____，中共第一次全国代表大会在上海法租界望志路 106 号 (今兴业路 76 号) 开幕。李达、张国焘、\
						毛泽东、董必武等 13 人作为全国 50 多名党员的代表出席会议。");
	wcscpy_s(q35.optionA, L"A. 7 月 16 日");
	wcscpy_s(q35.optionB, L"B. 7 月 23 日");
	wcscpy_s(q35.optionC, L"C. 7 月 20 日");
	wcscpy_s(q35.answer, L"B. 7 月 23 日");
	questions.push_back(q35);

	Question q36;
	wcscpy_s(q36.content, L"1929 年 12 月 11 日，邓小平、张云逸、雷经天等在广西_____举行起义，\
							成立右江工农民主政府，开始了广西西部的工农武装割据。");
	wcscpy_s(q36.optionA, L"A. 百色");
	wcscpy_s(q36.optionB, L"B. 宜山");
	wcscpy_s(q36.optionC, L"C. 邕宁");
	wcscpy_s(q36.answer, L"A. 百色");
	questions.push_back(q36);

	Question q37;
	wcscpy_s(q37.content, L"1935 年 10 月 19 日，中共中央率领中国工农红军陕甘支队到达陕甘革命根据地。\
中共中央、红一方面军主力的长征历时一年，经福建、江西等_____个省，行程二万五千里。");
	wcscpy_s(q37.optionA, L"A. 11");
	wcscpy_s(q37.optionB, L"B. 12");
	wcscpy_s(q37.optionC, L"C. 13");
	wcscpy_s(q37.answer, L"A. 11");
	questions.push_back(q37);

	Question q38;
	wcscpy_s(q38.content, L"1937 年 9 月 25 日，八路军第一一五师主力在晋东北_____附近伏击日军，\
取得全国抗战以来中国军队的第一次大胜利，粉碎了日军不可战胜的神话。");
	wcscpy_s(q38.optionA, L"A. 嘉峪关");
	wcscpy_s(q38.optionB, L"B. 娘子关");
	wcscpy_s(q38.optionC, L"C. 平型关");
	wcscpy_s(q38.answer, L"C. 平型关");
	questions.push_back(q38);

	Question q39;
	wcscpy_s(q39.content, L"将毛泽东思想确立为党的指导思想并写入党章是在 1945 年党的_____。");
	wcscpy_s(q39.optionA, L"A. 六大");
	wcscpy_s(q39.optionB, L"B. 七大");
	wcscpy_s(q39.optionC, L"C. 八大");
	wcscpy_s(q39.answer, L"B. 七大");
	questions.push_back(q39);

	Question q40;
	wcscpy_s(q40.content, L"1847 年，马克思、恩格斯创立了第一个国际性的工人阶级政党_____。");
	wcscpy_s(q40.optionA, L"A. 共产主义者同盟");
	wcscpy_s(q40.optionB, L"B. 社会工人党");
	wcscpy_s(q40.optionC, L"C. 第一国际");
	wcscpy_s(q40.answer, L"A. 共产主义者同盟");
	questions.push_back(q40);

	Question q41;
	wcscpy_s(q41.content, L"1948 年 9 月到 1949 年 1 月，人民解放军发动举世闻名的三大战役，其先后顺序是_____。");
	wcscpy_s(q41.optionA, L"A. 辽沈、淮海、平津");
	wcscpy_s(q41.optionB, L"B. 辽沈、平津、淮海");
	wcscpy_s(q41.optionC, L"C. 平津、辽沈、淮海");
	wcscpy_s(q41.answer, L"A. 辽沈、淮海、平津");
	questions.push_back(q41);

	Question q42;
	wcscpy_s(q42.content, L"1950 年 6 月，朝鲜内战爆发，美国悍然出兵朝鲜，将战火烧到中国边境。\
为抗美援朝、保家卫国，1950 年 10 月，以_____为司令员兼政治委员的中国人民志愿军开赴朝鲜前线。");
	wcscpy_s(q42.optionA, L"A. 彭德怀");
	wcscpy_s(q42.optionB, L"B. 林彪");
	wcscpy_s(q42.optionC, L"C. 陈毅");
	wcscpy_s(q42.answer, L"A. 彭德怀");
	questions.push_back(q42);

	Question q43;
	wcscpy_s(q43.content, L"1956 年 2 月至 4 月间，中共中央政治局分别约集三十多个经济部门的负责同志座谈，\
讨论社会主义建设中存在的各种问题。毛泽东集中大家的意见，在 4 月的政治局扩大会议上作_____报告。");
	wcscpy_s(q43.optionA, L"A. 《论联合政府》");
	wcscpy_s(q43.optionB, L"B. 《论十大关系》");
	wcscpy_s(q43.optionC, L"C. 《关于农业合作化问题》");
	wcscpy_s(q43.answer, L"B. 《论十大关系》");
	questions.push_back(q43);

	Question q44;
	wcscpy_s(q44.content, L"_____年底，中共中央在北京召开十一届三中全会。这次会议彻底否定“两个凡是”的方针，\
确立解放思想、实事求是的思想路线。");
	wcscpy_s(q44.optionA, L"A. 1977");
	wcscpy_s(q44.optionB, L"B. 1978");
	wcscpy_s(q44.optionC, L"C. 1979");
	wcscpy_s(q44.answer, L"B. 1978");
	questions.push_back(q44);

	Question q45;
	wcscpy_s(q45.content, L"澳门于 1999 年_____结束了葡萄牙的殖民统治，回归祖国怀抱。");
	wcscpy_s(q45.optionA, L"A. 7 月 1 日");
	wcscpy_s(q45.optionB, L"B. 12 月 1 日");
	wcscpy_s(q45.optionC, L"C. 12 月 20 日");
	wcscpy_s(q45.answer, L"C. 12 月 20 日");
	questions.push_back(q45);

	Question q46;
	wcscpy_s(q46.content, L"1943 年 7 月 5 日，_____在《中国共产党与中国民族解放的道路》一文中，\
首次使用了“毛泽东思想”这个概念，指出“毛泽东思想就是中国的马克思列宁主义”。");
	wcscpy_s(q46.optionA, L"A. 张如心");
	wcscpy_s(q46.optionB, L"B. 刘少奇");
	wcscpy_s(q46.optionC, L"C. 王稼祥");
	wcscpy_s(q46.answer, L"C. 王稼祥");
	questions.push_back(q46);

	Question q47;
	wcscpy_s(q47.content, L"科学发展观是发展中国特色社会主义必须坚持和贯彻的重大战略思想。其核心是_____。");
	wcscpy_s(q47.optionA, L"A. 以人为本");
	wcscpy_s(q47.optionB, L"B. 全面发展");
	wcscpy_s(q47.optionC, L"C. 可持续发展");
	wcscpy_s(q47.answer, L"A. 以人为本");
	questions.push_back(q47);

	Question q48;
	wcscpy_s(q48.content, L"新时代中国特色社会主义思想，明确中国特色社会主义最本质的特征是_____。");
	wcscpy_s(q48.optionA, L"A. “五位一体”总体布局");
	wcscpy_s(q48.optionB, L"B. 人民利益为根本出发点");
	wcscpy_s(q48.optionC, L"C. 中国共产党领导");
	wcscpy_s(q48.answer, L"C. 中国共产党领导");
	questions.push_back(q48);

	Question q49;
	wcscpy_s(q49.content, L"实现“两个一百年”奋斗目标、实现中华民族伟大复兴的中国梦，\
		不断提高人民生活水平，必须坚定不移把_____作为党执政兴国的第一要务。");
	wcscpy_s(q49.optionA, L"A. 创新");
	wcscpy_s(q49.optionB, L"B. 改革");
	wcscpy_s(q49.optionC, L"C. 发展");
	wcscpy_s(q49.answer, L"C. 发展");
	questions.push_back(q49);

	Question q50;
	wcscpy_s(q50.content, L"_____年 8 月 1 日爆发的南昌起义，打响了武装反抗国民党反动派的第一枪，\
标志着中国共产党独立地创造革命军队和领导革命战争的开始。");
	wcscpy_s(q50.optionA, L"A. 1925");
	wcscpy_s(q50.optionB, L"B. 1926");
	wcscpy_s(q50.optionC, L"C. 1927");
	wcscpy_s(q50.answer, L"C. 1927");
	questions.push_back(q50);

	Question q51;
	wcscpy_s(q51.content, L"1931 年 11 月 7 日至 20 日，中华苏维埃第一次全国代表大会在江西_____召开，宣告中华苏维埃共和国临时中央\
		政府的成立，并选出 63 人为中央执行委员。");
	wcscpy_s(q51.optionA, L"A. 黄陂");
	wcscpy_s(q51.optionB, L"B. 会宁");
	wcscpy_s(q51.optionC, L"C. 瑞金");
	wcscpy_s(q51.answer, L"C. 瑞金");
	questions.push_back(q51);

	Question q52;
	wcscpy_s(q52.content, L"1936 年 10 月，三大主力红军在甘肃_____胜利会师，红军长征宣告胜利结束。");
	wcscpy_s(q52.optionA, L"A. 会宁");
	wcscpy_s(q52.optionB, L"B. 瑞金");
	wcscpy_s(q52.optionC, L"C. 静宁");
	wcscpy_s(q52.answer, L"A. 会宁");
	questions.push_back(q52);

	Question q53;
	wcscpy_s(q53.content, L"1937 年 10 月 12 日，南方 8 省 13 个地区 (不包括琼崖红军游击队) 的红军和游击队改编为国民革命军陆军新编第四军，\
_____任军长。");
	wcscpy_s(q53.optionA, L"A. 陈毅");
	wcscpy_s(q53.optionB, L"B. 叶挺");
	wcscpy_s(q53.optionC, L"C. 项英");
	wcscpy_s(q53.answer, L"B. 叶挺");
	questions.push_back(q53);

	Question q54;
	wcscpy_s(q54.content, L"1942 年，中共中央宣传部决定，在全党开展反对主观主义以整顿学风，反对_____以整顿党风，\
反对党八股以整顿文风的整风运动。");
	wcscpy_s(q54.optionA, L"A. 官僚主义");
	wcscpy_s(q54.optionB, L"B. 宗派主义");
	wcscpy_s(q54.optionC, L"C. 自由主义");
	wcscpy_s(q54.answer, L"B. 宗派主义");
	questions.push_back(q54);

	Question q55;
	wcscpy_s(q55.content, L"1848 年 2 月， B  的发表，标志着科学社会主义的问世。");
	wcscpy_s(q55.optionA, L"A. 《资本论》第一卷");
	wcscpy_s(q55.optionB, L"B. 《共产党宣言》");
	wcscpy_s(q55.optionC, L"C. 唯物史观的形成");
	wcscpy_s(q55.answer, L"B. 《共产党宣言》");
	questions.push_back(q55);

	Question q56;
	wcscpy_s(q56.content, L"1938 年，毛泽东发表的系统阐述抗日战争特点、前途和发展规律的著作是_____。");
	wcscpy_s(q56.optionA, L"A. 《论反对日本帝国主义的策略》");
	wcscpy_s(q56.optionB, L"B. 《论持久战》");
	wcscpy_s(q56.optionC, L"C. 《论新阶段》");
	wcscpy_s(q56.answer, L"B. 《论持久战》");
	questions.push_back(q56);

	Question q57;
	wcscpy_s(q57.content, L"在中国人民解放战争即将取得全国胜利的前夕，1949 年 3 月 5 日至 13 日，中共中央在河北省平山县_____村召开了七届二中全会。");
	wcscpy_s(q57.optionA, L"A. 杨家岭");
	wcscpy_s(q57.optionB, L"B. 西柏坡");
	wcscpy_s(q57.optionC, L"C. 北町村");
	wcscpy_s(q57.answer, L"B. 西柏坡");
	questions.push_back(q57);

	Question q58;
	wcscpy_s(q58.content, L"《为人民服务》是毛泽东同志在纪念_____同志追悼会上的讲话。");
	wcscpy_s(q58.optionA, L"A. 张思德");
	wcscpy_s(q58.optionB, L"B. 白求恩");
	wcscpy_s(q58.optionC, L"C. 刘胡兰");
	wcscpy_s(q58.answer, L"A. 张思德");
	questions.push_back(q58);

	Question q59;
	wcscpy_s(q59.content, L"土地改革后，国家又对农业、手工业、资本主义工商业进行社会主义改造。到_____年底，三大改造基本完成，我国初步确立起社会主义基本制度。");
	wcscpy_s(q59.optionA, L"A. 1954");
	wcscpy_s(q59.optionB, L"B. 1955");
	wcscpy_s(q59.optionC, L"C. 1956");
	wcscpy_s(q59.answer, L"C. 1956");
	questions.push_back(q59);

	Question q60;
	wcscpy_s(q60.content, L"1964 年，新中国在科技领域取得的重大成果是_____。");
	wcscpy_s(q60.optionA, L"A. 第一颗原子弹试验成功");
	wcscpy_s(q60.optionB, L"B. 第一颗氢弹试验成功");
	wcscpy_s(q60.optionC, L"C. 第一颗人造地球卫星发射成功");
	wcscpy_s(q60.answer, L"A. 第一颗原子弹试验成功");
	questions.push_back(q60);

	Question q61;
	wcscpy_s(q61.content, L"党的十一届三中全会后，党和政府实行改革开放政策，改革是从_____ 开始。");
	wcscpy_s(q61.optionA, L"A. 城市");
	wcscpy_s(q61.optionB, L"B. 农村");
	wcscpy_s(q61.optionC, L"C. 特区");
	wcscpy_s(q61.answer, L"B. 农村");
	questions.push_back(q61);

	Question q62;
	wcscpy_s(q62.content, L"歌曲《春天的故事》唱道:“1979 年，那是一个春天，有一位老人在中国的南海边画了一个圈，神话般的崛起座座城市，奇迹般的聚起座座金山”其中，“画了一个圈”是指_____。");
	wcscpy_s(q62.optionA, L"A. 建立长江三角洲经济特区");
	wcscpy_s(q62.optionB, L"B. 设立海南经济特区");
	wcscpy_s(q62.optionC, L"C. 在闽粤沿海设立经济特区");
	wcscpy_s(q62.answer, L"C. 在闽粤沿海设立经济特区");
	questions.push_back(q62);

	Question q63;
	wcscpy_s(q63.content, L"邓小平理论的精髓是_____ 和实事求是。");
	wcscpy_s(q63.optionA, L"A. 群众路线");
	wcscpy_s(q63.optionB, L"B. 独立自主");
	wcscpy_s(q63.optionC, L"C. 解放思想");
	wcscpy_s(q63.answer, L"C. 解放思想");
	questions.push_back(q63);

	Question q64;
	wcscpy_s(q64.content, L"中国共产党坚持对人民解放军和其他人民武装力量的绝对领导，贯彻_____思想，加强人民解放军的建设。");
	wcscpy_s(q64.optionA, L"A. 习近平强军");
	wcscpy_s(q64.optionB, L"B. 习近平外交");
	wcscpy_s(q64.optionC, L"C. 习近平强国");
	wcscpy_s(q64.answer, L"A. 习近平强军");
	questions.push_back(q64);

	Question q65;
	wcscpy_s(q65.content, L"革命一声炮响，给中国送来了马克思列宁主义。陈独秀、_____ 等人积极传播马克思主义，倡导运用马克思主义改造中国社会。");
	wcscpy_s(q65.optionA, L"A. 梁启超");
	wcscpy_s(q65.optionB, L"B. 章炳麟");
	wcscpy_s(q65.optionC, L"C. 李大钊");
	wcscpy_s(q65.answer, L"C. 李大钊");
	questions.push_back(q65);

	Question q66;
	wcscpy_s(q66.content, L"马克思主义最鲜明的政治立场是_____。");
	wcscpy_s(q66.optionA, L"A. 致力于实现无产阶级政党执政");
	wcscpy_s(q66.optionB, L"B. 致力于消灭私有制和剥削现象");
	wcscpy_s(q66.optionC, L"C. 致力于实现最广大人民的根本利益");
	wcscpy_s(q66.answer, L"C. 致力于实现最广大人民的根本利益");
	questions.push_back(q66);

	Question q67;
	wcscpy_s(q67.content, L"我国现行《宪法》自颁布后，共进行了_____ 次修改。");
	wcscpy_s(q67.optionA, L"A. 3");
	wcscpy_s(q67.optionB, L"B. 4");
	wcscpy_s(q67.optionC, L"C. 5");
	wcscpy_s(q67.answer, L"C. 5");
	questions.push_back(q67);

	Question q68;
	wcscpy_s(q68.content, L"2011 年 9 月 29 日，中国首个空间实验室“天宫一号”在_____ 卫星发射中心发射成功。");
	wcscpy_s(q68.optionA, L"A. 西昌");
	wcscpy_s(q68.optionB, L"B. 酒泉");
	wcscpy_s(q68.optionC, L"C. 太原");
	wcscpy_s(q68.answer, L"B. 酒泉");
	questions.push_back(q68);

	Question q69;
	wcscpy_s(q69.content, L"_____是中国特色社会主义的本质要求和重要保障。");
	wcscpy_s(q69.optionA, L"A. 全面依法治国");
	wcscpy_s(q69.optionB, L"B. 全面从严治党");
	wcscpy_s(q69.optionC, L"C. 全面发展经济");
	wcscpy_s(q69.answer, L"A. 全面依法治国");
	questions.push_back(q69);

	Question q70;
	wcscpy_s(q70.content, L"抗日战争结束后，中共中央提出_____ 的战略方针，这是一个具有远见卓识的战略决策，对争取解放战争的胜利具有重大而深远的意义。");
	wcscpy_s(q70.optionA, L"A. “进军东北，保卫华北”");
	wcscpy_s(q70.optionB, L"B. “向北防御，向南发展”");
	wcscpy_s(q70.optionC, L"C. “巩固华北，发展东北”");
	wcscpy_s(q70.answer, L"C. “巩固华北，发展东北”");
	questions.push_back(q70);

	Question q71;
	wcscpy_s(q71.content, L"在十八届中央政治局第九次集体学习时的讲话中，习近平总书记指出_____ 是第一资源，也是创新活动中最为活跃、最为积极的因素。");
	wcscpy_s(q71.optionA, L"A. 自然资源");
	wcscpy_s(q71.optionB, L"B. 文化资源");
	wcscpy_s(q71.optionC, L"C. 人才资源");
	wcscpy_s(q71.answer, L"C. 人才资源");
	questions.push_back(q71);

	Question q72;
	wcscpy_s(q72.content, L"习近平总书记对广大青年提出了_____、练就过硬本领、勇于创新创造、矢志艰苦奋斗、锤炼高尚品格”的“五点希望”。");
	wcscpy_s(q72.optionA, L"A. 建设伟大祖国");
	wcscpy_s(q72.optionB, L"B. 坚定理想信念");
	wcscpy_s(q72.optionC, L"C. 弘扬传统文化");
	wcscpy_s(q72.answer, L"B. 坚定理想信念");
	questions.push_back(q72);

	Question q73;
	wcscpy_s(q73.content, L"中国共产党人的初心和使命，就是_____。这个初心和使命是激励中国共产党人不断前进的根本动力。");
	wcscpy_s(q73.optionA, L"A. 为中国人民谋幸福，为中华民族谋未来");
	wcscpy_s(q73.optionB, L"B. 为中国人民谋生活，为中华名族谋复兴");
	wcscpy_s(q73.optionC, L"C. 为中国人民谋幸福，为中华民族谋复兴");
	wcscpy_s(q73.answer, L"C. 为中国人民谋幸福，为中华民族谋复兴");
	questions.push_back(q73);

	Question q74;
	wcscpy_s(q74.content, L"1952 年 7 月 29 日，我国参加奥林匹克运动会的第一个体育代表团抵达芬兰首都赫尔辛基，这是五星红旗_____ 在奥运会上升起。");
	wcscpy_s(q74.optionA, L"A. 第一次");
	wcscpy_s(q74.optionB, L"B. 第二次");
	wcscpy_s(q74.optionC, L"C. 第三次");
	wcscpy_s(q74.answer, L"A. 第一次");
	questions.push_back(q74);

	Question q75;
	wcscpy_s(q75.content, L"社会主义的本质，是解放生产力，发展生产力，消灭剥削，消除_____，最终达到共同富裕。");
	wcscpy_s(q75.optionA, L"A. 贫富悬殊");
	wcscpy_s(q75.optionB, L"B. 绝对贫困");
	wcscpy_s(q75.optionC, L"C. 两极分化");
	wcscpy_s(q75.answer, L"C. 两极分化");
	questions.push_back(q75);

	Question q76;
	wcscpy_s(q76.content, L"北伐战争时期，农民运动的中心在_____。");
	wcscpy_s(q76.optionA, L"A. 江西");
	wcscpy_s(q76.optionB, L"B. 湖南");
	wcscpy_s(q76.optionC, L"C. 广东");
	wcscpy_s(q76.answer, L"B. 湖南");
	questions.push_back(q76);

	Question q77;
	wcscpy_s(q77.content, L"我党提出全面抗战路线的会议是_____。");
	wcscpy_s(q77.optionA, L"A. 遵义会议");
	wcscpy_s(q77.optionB, L"B. 瓦窑堡会议");
	wcscpy_s(q77.optionC, L"C. 洛川会议");
	wcscpy_s(q77.answer, L"C. 洛川会议");
	questions.push_back(q77);

	Question q78;
	wcscpy_s(q78.content, L"党在民主革命时期的三大法宝是_____。");
	wcscpy_s(q78.optionA, L"A. 实事求是、独立民主、群众路线");
	wcscpy_s(q78.optionB, L"B. 统一战线、武装斗争、党的建设");
	wcscpy_s(q78.optionC, L"C. 解放思想、实事求是、与时俱进");
	wcscpy_s(q78.answer, L"B. 统一战线、武装斗争、党的建设");
	questions.push_back(q78);

	Question q79;
	wcscpy_s(q79.content, L"中华人民共和国建国后头三年的主要任务是_____。");
	wcscpy_s(q79.optionA, L"A. 没收官僚资本");
	wcscpy_s(q79.optionB, L"B. 恢复国民经济");
	wcscpy_s(q79.optionC, L"C. 进行社会主义改造");
	wcscpy_s(q79.answer, L"B. 恢复国民经济");
	questions.push_back(q79);

	Question q80;
	wcscpy_s(q80.content, L"中国新民主主义革命伟大开端的标志是_____。");
	wcscpy_s(q80.optionA, L"A. 辛亥革命");
	wcscpy_s(q80.optionB, L"B. 五四运动");
	wcscpy_s(q80.optionC, L"C. 新文化运动");
	wcscpy_s(q80.answer, L"B. 五四运动");
	questions.push_back(q80);

	Question q81;
	wcscpy_s(q81.content, L"区分新旧民主革命的主要标志是_____。");
	wcscpy_s(q81.optionA, L"A. 革命纲领不同");
	wcscpy_s(q81.optionB, L"B. 革命路线不同");
	wcscpy_s(q81.optionC, L"C. 革命领导权不同");
	wcscpy_s(q81.answer, L"C. 革命领导权不同");
	questions.push_back(q81);

	Question q82;
	wcscpy_s(q82.content, L"1931 年 11 月 27 日，中华苏维埃共和国临时中央政府在_____ 成立。");
	wcscpy_s(q82.optionA, L"A. 瑞金");
	wcscpy_s(q82.optionB, L"B. 兴国");
	wcscpy_s(q82.optionC, L"C. 北平");
	wcscpy_s(q82.answer, L"A. 瑞金");
	questions.push_back(q82);

	Question q83;
	wcscpy_s(q83.content, L"抗日战争时期，中国共产党解决农民问题的基本政策是_____。");
	wcscpy_s(q83.optionA, L"A. 没收地主阶级的土地和富农多余的土地");
	wcscpy_s(q83.optionB, L"B. 减租减息");
	wcscpy_s(q83.optionC, L"C. 开仓放粮");
	wcscpy_s(q83.answer, L"B. 减租减息");
	questions.push_back(q83);

	Question q84;
	wcscpy_s(q84.content, L"中国是于_____ 恢复在联合国的合法席位的。");
	wcscpy_s(q84.optionA, L"A. 1971 年 9 月");
	wcscpy_s(q84.optionB, L"B. 1972 年 2 月");
	wcscpy_s(q84.optionC, L"C. 1972 年 9 月");
	wcscpy_s(q84.answer, L"A. 1971 年 9 月");
	questions.push_back(q84);

	Question q85;
	wcscpy_s(q85.content, L"中国获得奥运史上的第一块金牌运动员是_____。");
	wcscpy_s(q85.optionA, L"A. 容国团");
	wcscpy_s(q85.optionB, L"B. 许海峰");
	wcscpy_s(q85.optionC, L"C. 李宁");
	wcscpy_s(q85.answer, L"B. 许海峰");
	questions.push_back(q85);

	Question q86;
	wcscpy_s(q86.content, L"《告台湾同胞书》是以_____ 的名义发布的。");
	wcscpy_s(q86.optionA, L"A. 邓小平");
	wcscpy_s(q86.optionB, L"B. 江泽民");
	wcscpy_s(q86.optionC, L"C. 彭德怀");
	wcscpy_s(q86.answer, L"C. 彭德怀");
	questions.push_back(q86);

	Question q87;
	wcscpy_s(q87.content, L"党在社会主义初级阶段的基本路线的核心是_____。");
	wcscpy_s(q87.optionA, L"A. 一个中心，两个基本点");
	wcscpy_s(q87.optionB, L"B. 以经济建设为中心");
	wcscpy_s(q87.optionC, L"C. 坚持四项基本原则");
	wcscpy_s(q87.answer, L"A. 一个中心，两个基本点");
	questions.push_back(q87);

	Question q88;
	wcscpy_s(q88.content, L"中华人民共和国成立后制定的第一部正式法律是_____。");
	wcscpy_s(q88.optionA, L"A. 《婚姻法》");
	wcscpy_s(q88.optionB, L"B. 《宪法》");
	wcscpy_s(q88.optionC, L"C. 《土地改革法》");
	wcscpy_s(q88.answer, L"A. 《婚姻法》");
	questions.push_back(q88);

	Question q89;
	wcscpy_s(q89.content, L"中华人民共和国首次以五大国之一的身份参与讨论国际问题的会议是_____。");
	wcscpy_s(q89.optionA, L"A. 万隆会议");
	wcscpy_s(q89.optionB, L"B. 古田会议");
	wcscpy_s(q89.optionC, L"C. 日内瓦会议");
	wcscpy_s(q89.answer, L"C. 日内瓦会议");
	questions.push_back(q89);

	Question q90;
	wcscpy_s(q90.content, L"1958 年 6 月 1 日，中共中央的理论刊物正式创刊，它是_____。");
	wcscpy_s(q90.optionA, L"A. 《红旗》");
	wcscpy_s(q90.optionB, L"B. 《求是》");
	wcscpy_s(q90.optionC, L"C. 《党建》");
	wcscpy_s(q90.answer, L"A. 《红旗》");
	questions.push_back(q90);

	Question q91;
	wcscpy_s(q91.content, L"1989 年中国青少年发展基金会倡导并组织的资助贫困地区失学儿童重返校园的公益事业是_____。");
	wcscpy_s(q91.optionA, L"A. 爱心工程");
	wcscpy_s(q91.optionB, L"B. 希望工程");
	wcscpy_s(q91.optionC, L"C. 蓝天工程");
	wcscpy_s(q91.answer, L"B. 希望工程");
	questions.push_back(q91);

	Question q92;
	wcscpy_s(q92.content, L"“星火计划”是我国政府为促进_____ 发展而制定的。");
	wcscpy_s(q92.optionA, L"A. 农村地区");
	wcscpy_s(q92.optionB, L"B. 老少边穷地区");
	wcscpy_s(q92.optionC, L"C. 西部地区");
	wcscpy_s(q92.answer, L"A. 农村地区");
	questions.push_back(q92);

	Question q93;
	wcscpy_s(q93.content, L"我国铁路建设史上规模最大、投资最多、一次建成里程最长的铁路是_____。");
	wcscpy_s(q93.optionA, L"A. 京广铁路");
	wcscpy_s(q93.optionB, L"B. 京九铁路");
	wcscpy_s(q93.optionC, L"C. 京汉铁路");
	wcscpy_s(q93.answer, L"B. 京九铁路");
	questions.push_back(q93);

	Question q94;
	wcscpy_s(q94.content, L"邓小平提出的世界主题是_____。");
	wcscpy_s(q94.optionA, L"A. 战争与和平");
	wcscpy_s(q94.optionB, L"B. 战争与革命");
	wcscpy_s(q94.optionC, L"C. 和平与发展");
	wcscpy_s(q94.answer, L"C. 和平与发展");
	questions.push_back(q94);

	Question q95;
	wcscpy_s(q95.content, L" _____，北京赢得 2008 年奥运会主办权。");
	wcscpy_s(q95.optionA, L"A. 1999 年 7 月 13 日");
	wcscpy_s(q95.optionB, L"B. 2000 年 7 月 13 日");
	wcscpy_s(q95.optionC, L"C. 2001 年 7 月 13 日");
	wcscpy_s(q95.answer, L"C. 2001 年 7 月 13 日");
	questions.push_back(q95);

	Question q96;
	wcscpy_s(q96.content, L"邓小平_____ 年在接见美国和加拿大客人时，首次明确提出，社会主义也可以搞市场经济。");
	wcscpy_s(q96.optionA, L"A. 1979");
	wcscpy_s(q96.optionB, L"B. 1987");
	wcscpy_s(q96.optionC, L"C. 1993");
	wcscpy_s(q96.answer, L"A. 1979");
	questions.push_back(q96);

	Question q97;
	wcscpy_s(q97.content, L"1983 年 6 月，邓小平在会见美国学者杨立宇时，阐述了解决台湾问题的_____。");
	wcscpy_s(q97.optionA, L"A. 八项主张");
	wcscpy_s(q97.optionB, L"B. 七大主旨");
	wcscpy_s(q97.optionC, L"C. 六点构想");
	wcscpy_s(q97.answer, L"C. 六点构想");
	questions.push_back(q97);

	Question q98;
	wcscpy_s(q98.content, L" _____ 是 90 年代我国改革开放进一步深化和取得显著成就的重要标志。");
	wcscpy_s(q98.optionA, L"A. 海南洋浦的开发");
	wcscpy_s(q98.optionB, L"B. 上海浦东的开发");
	wcscpy_s(q98.optionC, L"C. 深圳经济崛起");
	wcscpy_s(q98.answer, L"B. 上海浦东的开发");
	questions.push_back(q98);

	Question q99;
	wcscpy_s(q99.content, L"第一个支持恢复中国在联合国合法席位、第一个就中国加入世贸组织与中方签署双边协议的拉美国家是_____。");
	wcscpy_s(q99.optionA, L"A. 巴西");
	wcscpy_s(q99.optionB, L"B. 智利");
	wcscpy_s(q99.optionC, L"C. 古巴");
	wcscpy_s(q99.answer, L"B. 智利");
	questions.push_back(q99);

	Question q100;
	wcscpy_s(q100.content, L"自从 20 世纪 80 年代中期以来，打破“铁饭碗”和职工“下岗”等现象，已经不再是什么希奇的事情，这主要于我国的_____ 这一重大变革有关。");
	wcscpy_s(q100.optionA, L"A. 农村经济改革");
	wcscpy_s(q100.optionB, L"B. 建立经济特区");
	wcscpy_s(q100.optionC, L"C. 国有企业改革");
	wcscpy_s(q100.answer, L"C. 国有企业改革");
	questions.push_back(q100);

	Question q101;
	wcscpy_s(q101.content, L" _____ 既是清政府第一批被迫开放的通商口岸，又是我国最早的经济特区城市。");
	wcscpy_s(q101.optionA, L"A. 珠海");
	wcscpy_s(q101.optionB, L"B. 汕头");
	wcscpy_s(q101.optionC, L"C. 厦门");
	wcscpy_s(q101.answer, L"C. 厦门");
	questions.push_back(q101);

	Question q102;
	wcscpy_s(q102.content, L"新中国成立初期，经济领域开展的“五反”运动主要是_____ 的斗争。");
	wcscpy_s(q102.optionA, L"A. 反地主阶级");
	wcscpy_s(q102.optionB, L"B. 反不法资本家");
	wcscpy_s(q102.optionC, L"C. 反官僚主义");
	wcscpy_s(q102.answer, L"C. 反官僚主义");
	questions.push_back(q102);

	Question q103;
	wcscpy_s(q103.content, L"中国共产党第十一届中央委员会第三次全体会议决定成立_____。");
	wcscpy_s(q103.optionA, L"A. 国家发展改革委员会");
	wcscpy_s(q103.optionB, L"B. 中央纪律检查委员会");
	wcscpy_s(q103.optionC, L"C. 中央顾问委员会");
	wcscpy_s(q103.answer, L"B. 中央纪律检查委员会");
	questions.push_back(q103);

	Question q104;
	wcscpy_s(q104.content, L"1919 年 5 月 4 日，五四爱国运动爆发的直接导火索是_____。");
	wcscpy_s(q104.optionA, L"A. 巴黎和会上中国外交的失败");
	wcscpy_s(q104.optionB, L"B. 日本出兵占领青岛");
	wcscpy_s(q104.optionC, L"C. 《凡尔塞和约》的签订");
	wcscpy_s(q104.answer, L"A. 巴黎和会上中国外交的失败");
	questions.push_back(q104);

	Question q105;
	wcscpy_s(q105.content, L"新文化运动的两大口号是_____。");
	wcscpy_s(q105.optionA, L"A. 民主和科学");
	wcscpy_s(q105.optionB, L"B. 民权和平等");
	wcscpy_s(q105.optionC, L"C. 自由和民主");
	wcscpy_s(q105.answer, L"A. 民主和科学");
	questions.push_back(q105);

	Question q106;
	wcscpy_s(q106.content, L"标志着第一次国共合作全面破裂的是_____。");
	wcscpy_s(q106.optionA, L"A. “四一二”反革命政变");
	wcscpy_s(q106.optionB, L"B. 马日事变");
	wcscpy_s(q106.optionC, L"C. “七一五”国民党中央“分共”会议");
	wcscpy_s(q106.answer, L"C. “七一五”国民党中央“分共”会议");
	questions.push_back(q106);

	Question q107;
	wcscpy_s(q107.content, L"中国共产党人建立苏维埃政权的首次尝试出现在_____。");
	wcscpy_s(q107.optionA, L"A. 南昌起义");
	wcscpy_s(q107.optionB, L"B. 秋收起义");
	wcscpy_s(q107.optionC, L"C. 广州起义");
	wcscpy_s(q107.answer, L"C. 广州起义");
	questions.push_back(q107);

	Question q108;
	wcscpy_s(q108.content, L"1949 年 4 月 23 日，经过渡江战役，人民解放军胜利解放_____，宣告国民党在全国反动统治的覆灭。");
	wcscpy_s(q108.optionA, L"A. 上海");
	wcscpy_s(q108.optionB, L"B. 南京");
	wcscpy_s(q108.optionC, L"C. 南昌");
	wcscpy_s(q108.answer, L"B. 南京");
	questions.push_back(q108);

	Question q109;
	wcscpy_s(q109.content, L"1949 年 3 月，中共中央在_____ 召开七届二中全会。毛泽东在会上向全党同志提出了著名的“两个务必”。");
	wcscpy_s(q109.optionA, L"A. 延安");
	wcscpy_s(q109.optionB, L"B. 北京");
	wcscpy_s(q109.optionC, L"C. 西柏坡");
	wcscpy_s(q109.answer, L"C. 西柏坡");
	questions.push_back(q109);

	Question q110;
	wcscpy_s(q110.content, L"新中国成立初期，人民政府为稳定市场，安定人心，严厉打击投机倒把活动，经过_____，稳定了物价，结束了我国连续十多年物价暴涨的局面。");
	wcscpy_s(q110.optionA, L"A. “银元之战”和“米棉之战”");
	wcscpy_s(q110.optionB, L"B. “银元之战”和“煤油之战”");
	wcscpy_s(q110.optionC, L"C. “煤油之战”和“米棉之战”");
	wcscpy_s(q110.answer, L"A. “银元之战”和“米棉之战”");
	questions.push_back(q110);

	Question q111;
	wcscpy_s(q111.content, L"新中国成立之初，中央确定调整工商业工作，必须在“公私兼顾、劳资两利”的基本方针下，重点调整_____。");
	wcscpy_s(q111.optionA, L"A. 公私关系");
	wcscpy_s(q111.optionB, L"B. 劳资关系");
	wcscpy_s(q111.optionC, L"C. 产销关系");
	wcscpy_s(q111.answer, L"A. 公私关系");
	questions.push_back(q111);

	Question q112;
	wcscpy_s(q112.content, L"1954 年 9 月 15 日至 28 日，第一届全国人民代表大会第一次会议在北京隆重召开。大会通过了_____，以根本大法的形式，把中国共产党在过渡时期的总路线作为国家在过渡时期的总任务确定下来。");
	wcscpy_s(q112.optionA, L"A. 《中华人民共和国宪法》");
	wcscpy_s(q112.optionB, L"B. 《中华人民共和国土地法》");
	wcscpy_s(q112.optionC, L"C. 《中华人民共和国第一个五年计划》 ”");
	wcscpy_s(q112.answer, L"A. 《中华人民共和国宪法》");
	questions.push_back(q112);

	Question q113;
	wcscpy_s(q113.content, L"1953 年 12 月 31 日，周恩来在接见_____ 代表团时，首次系统地提出了国家之间和平共处的五项原则。");
	wcscpy_s(q113.optionA, L"A. 越南");
	wcscpy_s(q113.optionB, L"B. 印度");
	wcscpy_s(q113.optionC, L"C. 柬埔寨");
	wcscpy_s(q113.answer, L"B. 印度");
	questions.push_back(q113);

	Question q114;
	wcscpy_s(q114.content, L"近代中国社会的性质是_____。");
	wcscpy_s(q114.optionA, L"A. 帝国主义的殖民地");
	wcscpy_s(q114.optionB, L"B. 封建社会的残余");
	wcscpy_s(q114.optionC, L"C. 半殖民地半封建社会");
	wcscpy_s(q114.answer, L"C. 半殖民地半封建社会");
	questions.push_back(q114);

	Question q115;
	wcscpy_s(q115.content, L"党对资本主义工商业的社会主义改造采取的政策是_____。");
	wcscpy_s(q115.optionA, L"A. 没收");
	wcscpy_s(q115.optionB, L"B. 和平赎买");
	wcscpy_s(q115.optionC, L"C. 公私合营");
	wcscpy_s(q115.answer, L"B. 和平赎买");
	questions.push_back(q115);

	Question q116;
	wcscpy_s(q116.content, L"被西方称作“布拉格之春”的捷克斯洛伐克事件说明了_____。");
	wcscpy_s(q116.optionA, L"A. 苏联模式适合所有国家");
	wcscpy_s(q116.optionB, L"B. 社会主义不需要市场");
	wcscpy_s(q116.optionC, L"C. 需要探索适合本国国情的社会主义发展道路");
	wcscpy_s(q116.answer, L"C. 需要探索适合本国国情的社会主义发展道路");
	questions.push_back(q116);

	Question q117;
	wcscpy_s(q117.content, L"我国处理民族关系的一项重要政治制度，是在少数民族地区实行_____。");
	wcscpy_s(q117.optionA, L"A. 民族区域自治");
	wcscpy_s(q117.optionB, L"B. 民族平等");
	wcscpy_s(q117.optionC, L"C. 民族团结");
	wcscpy_s(q117.answer, L"A. 民族区域自治");
	questions.push_back(q117);

	Question q118;
	wcscpy_s(q118.content, L"新中国成立后，毛泽东提出，我国工人阶级同民族资产阶级的矛盾属于_____。");
	wcscpy_s(q118.optionA, L"A. 敌我矛盾");
	wcscpy_s(q118.optionB, L"B. 人民内部矛盾");
	wcscpy_s(q118.optionC, L"C. 无法调和的矛盾");
	wcscpy_s(q118.answer, L"B. 人民内部矛盾");
	questions.push_back(q118);

	Question q119;
	wcscpy_s(q119.content, L" _____ 年 7 月 27 日，朝鲜停战协定在板门店正式签字，历时两年零 9 个月的抗美援朝战争宣告结束。");
	wcscpy_s(q119.optionA, L"A. 1952");
	wcscpy_s(q119.optionB, L"B. 1953");
	wcscpy_s(q119.optionC, L"C. 1954");
	wcscpy_s(q119.answer, L"B. 1953");
	questions.push_back(q119);

	Question q120;
	wcscpy_s(q120.content, L"新中国首次全国人口普查是在_____ 年 6 月 30 日，普查结果显示，全国人口总数为 6.0 亿人。");
	wcscpy_s(q120.optionA, L"A. 1952");
	wcscpy_s(q120.optionB, L"B. 1953");
	wcscpy_s(q120.optionC, L"C. 1954");
	wcscpy_s(q120.answer, L"B. 1953");
	questions.push_back(q120);

	Question q121;
	wcscpy_s(q121.content, L" _____ 年，中国在世界上首次人工合成结晶牛胰岛素。");
	wcscpy_s(q121.optionA, L"A. 1963");
	wcscpy_s(q121.optionB, L"B. 1964");
	wcscpy_s(q121.optionC, L"C. 1965");
	wcscpy_s(q121.answer, L"C. 1965");
	questions.push_back(q121);

	Question q122;
	wcscpy_s(q122.content, L"1958 年 8 月，第一辆国产_____ 牌轿车诞生。");
	wcscpy_s(q122.optionA, L"A. 红旗");
	wcscpy_s(q122.optionB, L"B. 东风");
	wcscpy_s(q122.optionC, L"C. 解放");
	wcscpy_s(q122.answer, L"A. 红旗");
	questions.push_back(q122);

	Question q123;
	wcscpy_s(q123.content, L"1950 年 2 月中苏双方签订了_____。");
	wcscpy_s(q123.optionA, L"A. 《中俄睦邻友好合作条约》");
	wcscpy_s(q123.optionB, L"B. 《中苏友好同盟互助条约》");
	wcscpy_s(q123.optionC, L"C. 《中俄联合公报》");
	wcscpy_s(q123.answer, L"B. 《中苏友好同盟互助条约》");
	questions.push_back(q123);

	Question q124;
	wcscpy_s(q124.content, L"在我国社会主义制度基本建立起来后，国家政治生活的主题是_____。");
	wcscpy_s(q124.optionA, L"A. 健全民主和法制");
	wcscpy_s(q124.optionB, L"B. 正确处理人民内部矛盾");
	wcscpy_s(q124.optionC, L"C. 坚持四项基本原则");
	wcscpy_s(q124.answer, L"B. 正确处理人民内部矛盾");
	questions.push_back(q124);

	Question q125;
	wcscpy_s(q125.content, L"中国人民政治协商会议第一次全体会议决定以_____ 为代国歌。");
	wcscpy_s(q125.optionA, L"A. 《黄河大合唱》");
	wcscpy_s(q125.optionB, L"B. 《义勇军进行曲》");
	wcscpy_s(q125.optionC, L"C. 《国际歌》");
	wcscpy_s(q125.answer, L"B. 《义勇军进行曲》");
	questions.push_back(q125);

	Question q126;
	wcscpy_s(q126.content, L"2007 年 10 月 24 日，中国第一颗绕月探测卫星“_____”发射成功，11 月 5 日进入环月轨道，标志着中国首次月球探测工程取得圆满成功。");
	wcscpy_s(q126.optionA, L"A. 嫦娥一号");
	wcscpy_s(q126.optionB, L"B. 长征三号");
	wcscpy_s(q126.optionC, L"C. 嫦娥五号");
	wcscpy_s(q126.answer, L"A. 嫦娥一号");
	questions.push_back(q126);

	Question q127;
	wcscpy_s(q127.content, L"“改革开放胆子要大一些，敢于试验，不能像小脚女人一样。要抓住时机，发展自己，......发展才是硬道理。”这句话是_____说的。");
	wcscpy_s(q127.optionA, L"A. 邓小平");
	wcscpy_s(q127.optionB, L"B. 江泽民");
	wcscpy_s(q127.optionC, L"C. 习近平");
	wcscpy_s(q127.answer, L"A. 邓小平");
	questions.push_back(q127);

	Question q128;
	wcscpy_s(q128.content, L"1931 年 11 月 7 日，根据中共中央的决定，鄂豫皖根据地的红四军和红二十五军合编为_____，徐向前任总指挥，");
	wcscpy_s(q128.optionA, L"A. 红四方面军");
	wcscpy_s(q128.optionB, L"B. 红二方面军");
	wcscpy_s(q128.optionC, L"C. 红二十八军");
	wcscpy_s(q128.answer, L"A. 红四方面军");
	questions.push_back(q128);

	Question q129;
	wcscpy_s(q129.content, L"中国共产党领导的中国工人运动史上持续时间最长的一次罢工是_____。");
	wcscpy_s(q129.optionA, L"A. 1924 年 7 月爆发的广州沙面工人大罢工");
	wcscpy_s(q129.optionB, L"B. 1925 年 2 月爆发的上海日资纱厂工人大罢工");
	wcscpy_s(q129.optionC, L"C. 1925 年 6 月爆发的省港大罢工");
	wcscpy_s(q129.answer, L"C. 1925 年 6 月爆发的省港大罢工");
	questions.push_back(q129);

	Question q130;
	wcscpy_s(q130.content, L"1923 年 2 月，在帝国主义支持下，吴佩孚为了镇压_____，对工人进行血腥屠杀，制造了震惊中外的二七惨案。");
	wcscpy_s(q130.optionA, L"A. 陇海铁路大罢工");
	wcscpy_s(q130.optionB, L"B. 京汉铁路工人罢工");
	wcscpy_s(q130.optionC, L"C. 省港工人大罢工");
	wcscpy_s(q130.answer, L"B. 京汉铁路工人罢工");
	questions.push_back(q130);

	Question q131;
	wcscpy_s(q131.content, L"1946 年 1 月，根据中共代表与国民党政府签订的停战协定，在北平设立了由国民党、共产党和美国三方各一名代表组成的_____，负责监督执行停战协定。");
	wcscpy_s(q131.optionA, L"A. 军事协调小组");
	wcscpy_s(q131.optionB, L"B. 军事调处执行部");
	wcscpy_s(q131.optionC, L"C. 军事调处部");
	wcscpy_s(q131.answer, L"B. 军事调处执行部");
	questions.push_back(q131);

	Question q132;
	wcscpy_s(q132.content, L"1935 年 1 月底，红十军团军政委员会主席_____ 被俘。他在狱中写下《可爱的中国》《清贫》《狱中纪实》等, 表现了坚贞不屈的革命气节。");
	wcscpy_s(q132.optionA, L"A. 吉鸿昌");
	wcscpy_s(q132.optionB, L"B. 夏明翰");
	wcscpy_s(q132.optionC, L"C. 方志敏");
	wcscpy_s(q132.answer, L"C. 方志敏");
	questions.push_back(q132);

	Question q133;
	wcscpy_s(q133.content, L"1998 年夏，_____ 流域发生了历史罕见的洪涝灾害，在党中央领导下，全党、全军和全国人民团结奋战，取得了抗洪抢险斗争的伟大胜利。");
	wcscpy_s(q133.optionA, L"A. 长江和黄河");
	wcscpy_s(q133.optionB, L"B. 长江和珠江");
	wcscpy_s(q133.optionC, L"C. 长江、嫩江和松花江");
	wcscpy_s(q133.answer, L"C. 长江、嫩江和松花江");
	questions.push_back(q133);

	Question q134;
	wcscpy_s(q134.content, L"1929 年 12 月，中共中央代表邓小平和张云逸、韦拔群等在广西领导了_____，成立了中国工农红军第七军和右江苏维埃政府。");
	wcscpy_s(q134.optionA, L"A. 南宁起义");
	wcscpy_s(q134.optionB, L"B. 百色起义");
	wcscpy_s(q134.optionC, L"C. 龙州起义");
	wcscpy_s(q134.answer, L"B. 百色起义");
	questions.push_back(q134);

	Question q135;
	wcscpy_s(q135.content, L"1928 年 4 月，朱德、陈毅率部与毛泽东领导的部队在宁冈砻市会师。会师后，成立_____。");
	wcscpy_s(q135.optionA, L"A. 工农革命军第一军");
	wcscpy_s(q135.optionB, L"B. 工农革命军第二军");
	wcscpy_s(q135.optionC, L"C. 工农革命军第四军");
	wcscpy_s(q135.answer, L"C. 工农革命军第四军");
	questions.push_back(q135);

	Question q136;
	wcscpy_s(q136.content, L"省港大罢工的主要领导人是_____。");
	wcscpy_s(q136.optionA, L"A. 苏兆征、邓中夏");
	wcscpy_s(q136.optionB, L"B. 廖仲恺、何香凝");
	wcscpy_s(q136.optionC, L"C. 汪精卫、胡汉民");
	wcscpy_s(q136.answer, L"A. 苏兆征、邓中夏");
	questions.push_back(q136);

	Question q137;
	wcscpy_s(q137.content, L"1949 年 6 月 30 日，毛泽东发表《论人民民主专政》一文，指出资产阶级共和国的方案在中国是行不通的, 我们所要建立的新中国, 只能是工人阶级 (经过共产党) 领导的以_____ 为基础的人民民主专政。");
	wcscpy_s(q137.optionA, L"A. 统一战线");
	wcscpy_s(q137.optionB, L"B. 多党合作");
	wcscpy_s(q137.optionC, L"C. 工农联盟");
	wcscpy_s(q137.answer, L"C. 工农联盟");
	questions.push_back(q137);

	Question q138;
	wcscpy_s(q138.content, L"在中共一大至五大上，_____ 均当选为党的最高领导人。");
	wcscpy_s(q138.optionA, L"A. 毛泽东");
	wcscpy_s(q138.optionB, L"B. 李大钊");
	wcscpy_s(q138.optionC, L"C. 陈独秀");
	wcscpy_s(q138.answer, L"C. 陈独秀");
	questions.push_back(q138);

	Question q139;
	wcscpy_s(q139.content, L"解放战争时期，_____ 是人民解放军解放敌人重点设防的大城市的开始。");
	wcscpy_s(q139.optionA, L"A. 淮海战役");
	wcscpy_s(q139.optionB, L"B. 济南战役");
	wcscpy_s(q139.optionC, L"C. 平津战役");
	wcscpy_s(q139.answer, L"B. 济南战役");
	questions.push_back(q139);

	Question q140;
	wcscpy_s(q140.content, L" _____ 年，全国城乡义务教育实现全部免费。");
	wcscpy_s(q140.optionA, L"A. 2006");
	wcscpy_s(q140.optionB, L"B. 2007");
	wcscpy_s(q140.optionC, L"C. 2008");
	wcscpy_s(q140.answer, L"C. 2008");
	questions.push_back(q140);

	Question q141;
	wcscpy_s(q141.content, L"党的十八大以来，党中央以_____ 为突破口，全面从严治党，保持和发展了党的先进性和纯洁性。");
	wcscpy_s(q141.optionA, L"A. 制度建设");
	wcscpy_s(q141.optionB, L"B. 作风建设");
	wcscpy_s(q141.optionC, L"C. 组织建设");
	wcscpy_s(q141.answer, L"B. 作风建设");
	questions.push_back(q141);

	Question q142;
	wcscpy_s(q142.content, L"抗日战争时期抗日根据地掀起的大生产运动首先在_____ 展开。");
	wcscpy_s(q142.optionA, L"A. 陕甘宁边区");
	wcscpy_s(q142.optionB, L"B. 晋察冀边区");
	wcscpy_s(q142.optionC, L"C. 晋冀鲁豫边区");
	wcscpy_s(q142.answer, L"A. 陕甘宁边区");
	questions.push_back(q142);

	Question q143;
	wcscpy_s(q143.content, L"2003 年 10 月 15 日，_____ 载人飞船成功发射，并于 16 日安全返回地面，标志着我国成为世界上第三个独立掌握载人航天技术的国家。");
	wcscpy_s(q143.optionA, L"A. 神舟三号");
	wcscpy_s(q143.optionB, L"B. 神舟四号");
	wcscpy_s(q143.optionC, L"C. 神舟五号");
	wcscpy_s(q143.answer, L"C. 神舟五号");
	questions.push_back(q143);

	Question q144;
	wcscpy_s(q144.content, L"1967 年 6 月 17 日，我国第一颗_____ 在西部地区上空爆炸成功。这次试验是中国在核武器发展方面的又一次飞跃，标志着中国核武器的发展进入了一个新阶段。");
	wcscpy_s(q144.optionA, L"A. 原子弹");
	wcscpy_s(q144.optionB, L"B. 氢弹");
	wcscpy_s(q144.optionC, L"C. 导弹");
	wcscpy_s(q144.answer, L"B. 氢弹");
	questions.push_back(q144);


	Question q145;
	wcscpy_s(q145.content, L"1964 年 10 月，中国成功地爆炸第一颗_____，有力地打破了超级大国的核垄断和核讹诈，提高了我国的国际地位。");
	wcscpy_s(q145.optionA, L"A. 原子弹");
	wcscpy_s(q145.optionB, L"B. 氢弹");
	wcscpy_s(q145.optionC, L"C. 导弹");
	wcscpy_s(q145.answer, L"A. 原子弹");
	questions.push_back(q145);

	Question q146;
	wcscpy_s(q146.content, L"中国共产党直接领导的第一支正规部队是_____。");
	wcscpy_s(q146.optionA, L"A. 1925 年 11 月成立的以共产党员叶挺为团长的国民革命军第四军独立团");
	wcscpy_s(q146.optionB, L"B. 1927 年 3 月参加上海工人第三次武装起义的武装工人纠察队");
	wcscpy_s(q146.optionC, L"C. 1927 年 9 月参加湘赣边界秋收起义的部队");
	wcscpy_s(q146.answer, L"A. 1925 年 11 月成立的以共产党员叶挺为团长的国民革命军第四军独立团");
	questions.push_back(q146);

	Question q147;
	wcscpy_s(q147.content, L"2008 年北京奥运会共创造 43 项新世界纪录及 132 项新奥运纪录，中国以_____ 金牌居金牌榜榜首。");
	wcscpy_s(q147.optionA, L"A. 50 枚");
	wcscpy_s(q147.optionB, L"B. 51 枚");
	wcscpy_s(q147.optionC, L"C. 54 枚");
	wcscpy_s(q147.answer, L"B. 51 枚");
	questions.push_back(q147);

	Question q148;
	wcscpy_s(q148.content, L" _____ 标志着祖国大陆实现了统一。");
	wcscpy_s(q148.optionA, L"A. 第一届中国人民政治协商会议的召开");
	wcscpy_s(q148.optionB, L"B. 西藏和平解放");
	wcscpy_s(q148.optionC, L"C. 中华人民共和国中央人民政府的成立");
	wcscpy_s(q148.answer, L"B. 西藏和平解放");
	questions.push_back(q148);

	Question q149;
	wcscpy_s(q149.content, L" _____ 选举产生了党的历史上第一个中央纪律检查监督机构——中央监察委员会。");
	wcscpy_s(q149.optionA, L"A. 中共一大");
	wcscpy_s(q149.optionB, L"B. 中共四大");
	wcscpy_s(q149.optionC, L"C. 中共五大");
	wcscpy_s(q149.answer, L"C. 中共五大");
	questions.push_back(q149);

	Question q150;
	wcscpy_s(q150.content, L"1923 年 6 月举行的中共三大选举产生了_____，陈独秀为委员长，毛泽东为秘书。");
	wcscpy_s(q150.optionA, L"A. 中央局");
	wcscpy_s(q150.optionB, L"B. 中央委员会");
	wcscpy_s(q150.optionC, L"C. 中央监察会");
	wcscpy_s(q150.answer, L"A. 中央局");
	questions.push_back(q150);

	Question q151;
	wcscpy_s(q151.content, L"中华人民共和国中央人民政府副主席中属于民主党派的是_____。");
	wcscpy_s(q151.optionA, L"A. 宋庆龄、李济深");
	wcscpy_s(q151.optionB, L"B. 张澜、宋庆龄");
	wcscpy_s(q151.optionC, L"C. 张澜、李济深");
	wcscpy_s(q151.answer, L"C. 张澜、李济深");
	questions.push_back(q151);

	Question q152;
	wcscpy_s(q152.content, L"中共中央主席毛泽东在 1948 年 12 月 30 日为新华社所写的新年献词中发出了_____ 的伟大号召。");
	wcscpy_s(q152.optionA, L"A. “推翻国民党反动统治”");
	wcscpy_s(q152.optionB, L"B. “消灭一切反动派”");
	wcscpy_s(q152.optionC, L"C. “将革命进行到底”");
	wcscpy_s(q152.answer, L"C. “将革命进行到底”");
	questions.push_back(q152);

	Question q153;
	wcscpy_s(q153.content, L"邓小平南方谈话中指出:“抓住时机，发展自己，关键是发展_____”。");
	wcscpy_s(q153.optionA, L"A. 科技");
	wcscpy_s(q153.optionB, L"B. 经济");
	wcscpy_s(q153.optionC, L"C. 教育");
	wcscpy_s(q153.answer, L"B. 经济");
	questions.push_back(q153);

	Question q154;
	wcscpy_s(q154.content, L"胡锦涛在纪念党的十一届三中全会召开 30 周年大会上强调，我们要更加自觉、更加坚定地牢牢扭住经济建设这个中心，继续聚精会神搞建设、一心一意谋发展，坚持走_____ 的文明发展道路。");
	wcscpy_s(q154.optionA, L"A. 快速发展、生活富裕、生态良好");
	wcscpy_s(q154.optionB, L"B. 生产发展、生态文明、生活良好");
	wcscpy_s(q154.optionC, L"C. 生产发展、生活富裕、生态良好");
	wcscpy_s(q154.answer, L"C. 生产发展、生活富裕、生态良好");
	questions.push_back(q154);

	Question q155;
	wcscpy_s(q155.content, L"1900 年，列宁与其他革命者一起创办_____，这是第一份全俄马克思主义秘密报纸。");
	wcscpy_s(q155.optionA, L"A. 《工人报》");
	wcscpy_s(q155.optionB, L"B. 《星火报》");
	wcscpy_s(q155.optionC, L"C. 《曙光》");
	wcscpy_s(q155.answer, L"B. 《星火报》");
	questions.push_back(q155);

	Question q156;
	wcscpy_s(q156.content, L"1980 年 7 月 26 日，《人民日报》发表_____ 社论，明确提出文艺工作的方向，与“双百”方针一起成为党在社会主义新时期领导文艺工作的基本遵循。");
	wcscpy_s(q156.optionA, L"A. 《文艺为人民服务、为社会主义服务》");
	wcscpy_s(q156.optionB, L"B. 《文艺为社会主义服务》");
	wcscpy_s(q156.optionC, L"C. 《文艺为工农阶级服务》");
	wcscpy_s(q156.answer, L"A. 《文艺为人民服务、为社会主义服务》");
	questions.push_back(q156);

	Question q157;
	wcscpy_s(q157.content, L"1984 年 3 月 23 日，福建省 55 位厂长经理以《请给我们“松绑”》为题向省委书记、省长发出一封公开信，《人民日报》转载了这封信，将“松绑”之风吹向全国。这里的“松绑”主要是指_____。");
	wcscpy_s(q157.optionA, L"A. 允许企业实行公私合营");
	wcscpy_s(q157.optionB, L"B. 允许企业推行公司制");
	wcscpy_s(q157.optionC, L"C. 扩大企业的经营自主权");
	wcscpy_s(q157.answer, L"C. 扩大企业的经营自主权");
	questions.push_back(q157);

	Question q158;
	wcscpy_s(q158.content, L"1996 年国务院发布了《关于环境保护若干问题的决定》，我国开始实施“三河三湖”水污染防治，其中“三湖”是指太湖、滇池、巢湖; 三河是指_____。");
	wcscpy_s(q158.optionA, L"A. 淮河、海河、辽河");
	wcscpy_s(q158.optionB, L"B. 淮河、海河、黄河");
	wcscpy_s(q158.optionC, L"C. 淮河、黄河、辽河");
	wcscpy_s(q158.answer, L"A. 淮河、海河、辽河");
	questions.push_back(q158);

	Question q159;
	wcscpy_s(q159.content, L"毛泽东在一首词中写道“雾满龙冈千嶂暗，齐声唤，前头捉了张辉瓒”，反映的历史事件是中央根据地_____ 反 “围剿”的胜利。");
	wcscpy_s(q159.optionA, L"A. 第一次");
	wcscpy_s(q159.optionB, L"B. 第二次");
	wcscpy_s(q159.optionC, L"C. 第三次");
	wcscpy_s(q159.answer, L"A. 第一次");
	questions.push_back(q159);

	Question q160;
	wcscpy_s(q160.content, L"毛泽东在一首词中写道“七百里驱十五日，赣水苍茫闽山碧，横扫千军如卷席”，反映的历史事件是中央根据地  _____   反“围剿”的胜利。");
	wcscpy_s(q160.optionA, L"A. 第一次");
	wcscpy_s(q160.optionB, L"B. 第二次");
	wcscpy_s(q160.optionC, L"C. 第三次");
	wcscpy_s(q160.answer, L"B. 第二次");
	questions.push_back(q160);

	Question q161;
	wcscpy_s(q161.content, L"1930 年 10 月，蒋介石调集重兵，向各革命根据地和红军发动大规模“围剿”。_____是国民党军事“围剿”的重点。");
	wcscpy_s(q161.optionA, L"A. 鄂豫皖革命根据地");
	wcscpy_s(q161.optionB, L"B. 中央革命根据地");
	wcscpy_s(q161.optionC, L"C. 湘鄂赣革命根据地");
	wcscpy_s(q161.answer, L"B. 中央革命根据地");
	questions.push_back(q161);

	Question q162;
	wcscpy_s(q162.content, L"标志着第一次国共合作正式形成的历史事件是_____。");
	wcscpy_s(q162.optionA, L"A. 1923 年 6 月中国共产党第三次全国代表大会召开");
	wcscpy_s(q162.optionB, L"B. 1923 年 10 月国民党改组特别会议举行");
	wcscpy_s(q162.optionC, L"C. 1924 年1 月中国国民党第一次全国代表大会召开");
	wcscpy_s(q162.answer, L"C. 1924 年1 月中国国民党第一次全国代表大会召开");
	questions.push_back(q162);

	Question q163;
	wcscpy_s(q163.content, L"新中国成立之初，第一个宣布与新中国建立外交关系的社会主义国家是_____。");
	wcscpy_s(q163.optionA, L"A. 苏联");
	wcscpy_s(q163.optionB, L"B. 朝鲜");
	wcscpy_s(q163.optionC, L"C. 蒙古");
	wcscpy_s(q163.answer, L"A. 苏联");
	questions.push_back(q163);

	Question q164;
	wcscpy_s(q164.content, L"党的十九大报告指出，中国特色社会主义进入新时代，我国社会的主要矛盾已经转化为_____。");
	wcscpy_s(q164.optionA, L"A. 人口增长与经济发展不相适应的矛盾");
	wcscpy_s(q164.optionB, L"B. 人民日益增长的物质文化需要同落后的社会生产之间的矛盾");
	wcscpy_s(q164.optionC, L"C. 人民日益增长的美好生活需要和不平衡不充分的发展之间的矛盾");
	wcscpy_s(q164.answer, L"C. 人民日益增长的美好生活需要和不平衡不充分的发展之间的矛盾");
	questions.push_back(q164);

	Question q165;
	wcscpy_s(q165.content, L"1938 年，针对抗日战争“亡国论”和“速胜论”等错误观点，毛泽东在延安撰写的著作是_____。");
	wcscpy_s(q165.optionA, L"A. 《星星之火，可以燎原》");
	wcscpy_s(q165.optionB, L"B. 《论持久战》");
	wcscpy_s(q165.optionC, L"C. 《论联合政府》");
	wcscpy_s(q165.answer, L"B. 《论持久战》");
	questions.push_back(q165);

	Question q166;
	wcscpy_s(q166.content, L"1994 年 12 月，我国_____ 工程正式开工，1997 年 11 月 8 日工程实现大江截流，2006 年 5 月 20 日大坝全线建成，成为世界上最大的钢筋混凝土重力坝。");
	wcscpy_s(q166.optionA, L"A. 黄河小浪底");
	wcscpy_s(q166.optionB, L"B. 长江三峡");
	wcscpy_s(q166.optionC, L"C. 长江葛洲坝");
	wcscpy_s(q166.answer, L"B. 长江三峡");
	questions.push_back(q166);

	Question q167;
	wcscpy_s(q167.content, L"新中国成立后，_____ 起到了临时宪法的作用。");
	wcscpy_s(q167.optionA, L"A. 《中华人民共和国临时约法》");
	wcscpy_s(q167.optionB, L"B. 《中华人民共和国宪法》");
	wcscpy_s(q167.optionC, L"C. 《共同纲领》");
	wcscpy_s(q167.answer, L"C. 《共同纲领》");
	questions.push_back(q167);

	Question q168;
	wcscpy_s(q168.content, L"2005 年 4 月 29 日，时任中共中央总书记胡锦涛与中国国民党主席连战在北京举行会谈，这是国共两党最高领导人在时隔_____ 年后的首次正式会谈。");
	wcscpy_s(q168.optionA, L"A. 40");
	wcscpy_s(q168.optionB, L"B. 50");
	wcscpy_s(q168.optionC, L"C. 60");
	wcscpy_s(q168.answer, L"C. 60");
	questions.push_back(q168);

	Question q169;
	wcscpy_s(q169.content, L"1979 年 4 月，广东正式向中央提交创办贸易合作区的建议。邓小平表态很直接: 中央没有钱，你们自己去搞，杀出一条血路来! 当时向中央提交建议的广东省省长是_____。");
	wcscpy_s(q169.optionA, L"A. 任仲夷");
	wcscpy_s(q169.optionB, L"B. 习仲勋");
	wcscpy_s(q169.optionC, L"C. 袁庚");
	wcscpy_s(q169.answer, L"B. 习仲勋");
	questions.push_back(q169);

	Question q170;
	wcscpy_s(q170.content, L"新中国成立之初，被毛泽东高度评价这个胜利“不亚于淮海战役”的是_____。");
	wcscpy_s(q170.optionA, L"A. 解放海南岛");
	wcscpy_s(q170.optionB, L"B. 土地改革");
	wcscpy_s(q170.optionC, L"C. 稳定物价和统一财经");
	wcscpy_s(q170.answer, L"C. 稳定物价和统一财经");
	questions.push_back(q170);

	Question q171;
	wcscpy_s(q171.content, L"按照党的十九大要求，到新中国成立一百周年时，要把我国建设成为_____。");
	wcscpy_s(q171.optionA, L"A. 基本实现现代化的国家");
	wcscpy_s(q171.optionB, L"B. 社会主义现代化强国");
	wcscpy_s(q171.optionC, L"C. 实现全面小康的国家");
	wcscpy_s(q171.answer, L"B. 社会主义现代化强国");
	questions.push_back(q171);

	Question q172;
	wcscpy_s(q172.content, L"1937 年 7 月 7 日，日本帝国主义者以制造_____ 为起点，发动了全面侵华战争。");
	wcscpy_s(q172.optionA, L"A. 卢沟桥事变");
	wcscpy_s(q172.optionB, L"B. 皖南事变");
	wcscpy_s(q172.optionC, L"C. 西安事变");
	wcscpy_s(q172.answer, L"A. 卢沟桥事变");
	questions.push_back(q172);

	Question q173;
	wcscpy_s(q173.content, L"1946 年 8 月，毛泽东在延安会见美国进步女记者安娜·路易斯·斯特朗时，提出了“_____”的著名论断。");
	wcscpy_s(q173.optionA, L"A. 东风压倒西风");
	wcscpy_s(q173.optionB, L"B. 没有调查就没有发言权");
	wcscpy_s(q173.optionC, L"C. 一切反动派都是纸老虎");
	wcscpy_s(q173.answer, L"C. 一切反动派都是纸老虎");
	questions.push_back(q173);

	Question q174;
	wcscpy_s(q174.content, L"1991 年 12 月 15 日，中国第一座自行设计、自行建造的核电站_____ 核电站并网发电。");
	wcscpy_s(q174.optionA, L"A. 大亚湾");
	wcscpy_s(q174.optionB, L"B. 田湾");
	wcscpy_s(q174.optionC, L"C. 秦山");
	wcscpy_s(q174.answer, L"C. 秦山");
	questions.push_back(q174);

	Question q175;
	wcscpy_s(q175.content, L"1923 年 6 月 12 日至 20 日，中国共产党在广州举行第三次全国代表大会。大会决定采取_____ 的方式实现国共合作。");
	wcscpy_s(q175.optionA, L"A. 共产党员以个人身份加入国民党");
	wcscpy_s(q175.optionB, L"B. 平等的党外合作");
	wcscpy_s(q175.optionC, L"C. 共产党组织并入国民党");
	wcscpy_s(q175.answer, L"A. 共产党员以个人身份加入国民党");
	questions.push_back(q175);

	Question q176;
	wcscpy_s(q176.content, L"敌后抗战，主要是发动和组织农民抗战。抗日根据地内停止实行没收地主土地的政策，普遍实行_____ 政策，以减轻农民所受的封建剥削，提高他们抗日和生产的积极性。");
	wcscpy_s(q176.optionA, L"A. 土地改革");
	wcscpy_s(q176.optionB, L"B. 减租减息");
	wcscpy_s(q176.optionC, L"C. 消灭地主");
	wcscpy_s(q176.answer, L"B. 减租减息");
	questions.push_back(q176);

	Question q177;
	wcscpy_s(q177.content, L"1959 年 4 月 5 日，中国运动员_____ 荣获第 25 届世界乒乓球锦标赛男子单打冠军，这是中国运动员获得的第一个世界冠军。");
	wcscpy_s(q177.optionA, L"A. 徐寅生");
	wcscpy_s(q177.optionB, L"B. 容国团");
	wcscpy_s(q177.optionC, L"C. 庄则栋");
	wcscpy_s(q177.answer, L"B. 容国团");
	questions.push_back(q177);

	Question q178;
	wcscpy_s(q178.content, L"1933 年 5 月 31 日，国民党政府与日本签订了_____，这个协定实际上承认了日本对中国东北三省和热河省的侵占。");
	wcscpy_s(q178.optionA, L"A. 《塘沽协定》");
	wcscpy_s(q178.optionB, L"B. 《秦土协定》");
	wcscpy_s(q178.optionC, L"C. 《何梅协定》");
	wcscpy_s(q178.answer, L"A. 《塘沽协定》");
	questions.push_back(q178);

	Question q179;
	wcscpy_s(q179.content, L"党的十九大报告提出，要坚决打好防范化解重大风险、_____、污染防治的攻坚战。");
	wcscpy_s(q179.optionA, L"A. 保持社会稳定");
	wcscpy_s(q179.optionB, L"B. 精准脱贫");
	wcscpy_s(q179.optionC, L"C. 铲除腐败");
	wcscpy_s(q179.answer, L"B. 精准脱贫");
	questions.push_back(q179);

	Question q180;
	wcscpy_s(q180.content, L"1970 年 7 月 1 日，_____ 全线竣工运营，成为我国路桥建设史的重大起步，被联合国称为“象征二十世纪人类征服自然的三大奇迹”之一。");
	wcscpy_s(q180.optionA, L"A. 青藏铁路");
	wcscpy_s(q180.optionB, L"B. 成昆铁路");
	wcscpy_s(q180.optionC, L"C. 湘黔铁路");
	wcscpy_s(q180.answer, L"B. 成昆铁路");
	questions.push_back(q180);

	Question q181;
	wcscpy_s(q181.content, L"1985 年 2 月 20 日，我国第一个南极考察站_____ 在南极乔治王岛建成。");
	wcscpy_s(q181.optionA, L"A. 中山站");
	wcscpy_s(q181.optionB, L"B. 昆仑站");
	wcscpy_s(q181.optionC, L"C. 长城站");
	wcscpy_s(q181.answer, L"C. 长城站");
	questions.push_back(q181);

	Question q182;
	wcscpy_s(q182.content, L"1930 年 5 月，毛泽东在_____ 一文中，提出了“没有调查，没有发言权”的重要思想。");
	wcscpy_s(q182.optionA, L"A. 《湖南农民运动考察报告》");
	wcscpy_s(q182.optionB, L"B. 《反对本本主义》");
	wcscpy_s(q182.optionC, L"C. 《中国的红色政权为什么能够存在?》");
	wcscpy_s(q182.answer, L"B. 《反对本本主义》");
	questions.push_back(q182);

	Question q183;
	wcscpy_s(q183.content, L"完成祖国统一大业，是中华民族的根本利益所在，是包括港澳台同胞在内的全中国人民的共同愿望。邓小平面对港澳台地区尚未统一的问题，提出了“和平统一，一国两制”的构想，这一构想的核心是_____。");
	wcscpy_s(q183.optionA, L"A. 坚持一个中国");
	wcscpy_s(q183.optionB, L"B. 不承诺放弃武力");
	wcscpy_s(q183.optionC, L"C. 两制并存");
	wcscpy_s(q183.answer, L"A. 坚持一个中国");
	questions.push_back(q183);

	Question q184;
	wcscpy_s(q184.content, L"2020 年 5 月 11 日至 12 日，习近平总书记在山西考察工作时强调: 历史文化遗产是不可再生、不可替代的宝贵资源，要始终把_____ 放在第一位。");
	wcscpy_s(q184.optionA, L"A. 开发");
	wcscpy_s(q184.optionB, L"B. 利用");
	wcscpy_s(q184.optionC, L"C. 保护");
	wcscpy_s(q184.answer, L"C. 保护");
	questions.push_back(q184);

	Question q185;
	wcscpy_s(q185.content, L"1972 年 2 月尼克松访华期间，中美双方发表了_____，标志着中美关系开始正常化。");
	wcscpy_s(q185.optionA, L"A. 《联合公报》");
	wcscpy_s(q185.optionB, L"B. 《联合声明》");
	wcscpy_s(q185.optionC, L"C. 《联合宣言》");
	wcscpy_s(q185.answer, L"A. 《联合公报》");
	questions.push_back(q185);

	Question q186;
	wcscpy_s(q186.content, L" _____，国际货币基金组织执董会决定将人民币纳入特别提款权 (SDR) 货币篮子。");
	wcscpy_s(q186.optionA, L"A. 2014 年");
	wcscpy_s(q186.optionB, L"B. 2015 年");
	wcscpy_s(q186.optionC, L"C. 2016 年");
	wcscpy_s(q186.answer, L"B. 2015 年");
	questions.push_back(q186);

	Question q187;
	wcscpy_s(q187.content, L"2018 年 1 月 11 日，习近平总书记在十九届中央纪委二次全会上的重要讲话指出:“坚持党的领导是_____，必须旗帜鲜明、立场坚定，决不能羞羞答答、语焉不详，决不能遮遮掩掩、搞自我麻痹。”");
	wcscpy_s(q187.optionA, L"A. 方向性问题");
	wcscpy_s(q187.optionB, L"B. 根本性问题");
	wcscpy_s(q187.optionC, L"C. 原则性问题");
	wcscpy_s(q187.answer, L"A. 方向性问题");
	questions.push_back(q187);

	Question q188;
	wcscpy_s(q188.content, L"1927 年 4 月 28 日，中国共产党主要创始人之一_____ 在北京被奉系军阀杀害。");
	wcscpy_s(q188.optionA, L"A. 李大钊");
	wcscpy_s(q188.optionB, L"B. 梁启超");
	wcscpy_s(q188.optionC, L"C. 陈独秀");
	wcscpy_s(q188.answer, L"A. 李大钊");
	questions.push_back(q188);

	Question q189;
	wcscpy_s(q189.content, L"1968 年 12 月，我国自行设计建造的当时最大的铁路、公路两用桥_____ 全面建成通车。");
	wcscpy_s(q189.optionA, L"A. 南京长江大桥");
	wcscpy_s(q189.optionB, L"B. 武汉长江大桥");
	wcscpy_s(q189.optionC, L"C. 上海长江大桥");
	wcscpy_s(q189.answer, L"A. 南京长江大桥");
	questions.push_back(q189);

	Question q190;
	wcscpy_s(q190.content, L"1930 年 1 月，毛泽东在_____ 一文中指出：红军、游击队和红色区域的建立和发展，是半殖民地中国在无产阶级领导之下的农民斗争的最高形式和半殖民地农民斗争发展的必然结果。");
	wcscpy_s(q190.optionA, L"A. 《星星之火可以燎原》");
	wcscpy_s(q190.optionB, L"B. 《论持久战》");
	wcscpy_s(q190.optionC, L"C. 《论十大关系》");
	wcscpy_s(q190.answer, L"A. 《星星之火可以燎原》");
	questions.push_back(q190);

	Question q191;
	wcscpy_s(q191.content, L"为繁荣发展社会主义文化，中央宣传部从 1991 年开始组织实施精神文明建设“五个一工程”奖评选活动。“五个一”不包括_____。");
	wcscpy_s(q191.optionA, L"A. 一本好书");
	wcscpy_s(q191.optionB, L"B. 一台好戏");
	wcscpy_s(q191.optionC, L"C. 一首好歌");
	wcscpy_s(q191.answer, L"C. 一首好歌");
	questions.push_back(q191);

	Question q192;
	wcscpy_s(q192.content, L"明确规定日本侵占中国的领土，如满洲、台湾、澎湖列岛等地必须归还中国的文件是_____。");
	wcscpy_s(q192.optionA, L"A. 凡尔塞和约");
	wcscpy_s(q192.optionB, L"B. 开罗宣言");
	wcscpy_s(q192.optionC, L"C. 波茨坦公告");
	wcscpy_s(q192.answer, L"B. 开罗宣言");
	questions.push_back(q192);

	Question q193;
	wcscpy_s(q193.content, L" _____ 年，我国在国际上首次人工合成牛胰岛素结晶。");
	wcscpy_s(q193.optionA, L"A. 1964");
	wcscpy_s(q193.optionB, L"B. 1965");
	wcscpy_s(q193.optionC, L"C. 1966");
	wcscpy_s(q193.answer, L"B. 1965");
	questions.push_back(q193);

	Question q194;
	wcscpy_s(q194.content, L"在全面分析形势和任务的基础上，习近平总书记深刻指出我国发展仍处于_____。");
	wcscpy_s(q194.optionA, L"A. 重要战略机遇期");
	wcscpy_s(q194.optionB, L"B. 触底反弹期");
	wcscpy_s(q194.optionC, L"C. 战略风口期");
	wcscpy_s(q194.answer, L"A. 重要战略机遇期");
	questions.push_back(q194);

	Question q195;
	wcscpy_s(q195.content, L"中央红军长征途中翻越的第一座大雪山是_____。");
	wcscpy_s(q195.optionA, L"A. 夹金山");
	wcscpy_s(q195.optionB, L"B. 罗霄山");
	wcscpy_s(q195.optionC, L"C. 大别山");
	wcscpy_s(q195.answer, L"A. 夹金山");
	questions.push_back(q195);

	Question q196;
	wcscpy_s(q196.content, L"作风问题本质上是党性问题，关系党的生死存亡，核心问题是_____。");
	wcscpy_s(q196.optionA, L"A. 党员提升工作能力的问题");
	wcscpy_s(q196.optionB, L"B. 党员个人价值实现的问题");
	wcscpy_s(q196.optionC, L"C. 党同人民群众的关系问题");
	wcscpy_s(q196.answer, L"C. 党同人民群众的关系问题");
	questions.push_back(q196);

	Question q197;
	wcscpy_s(q197.content, L"国共两党实现第二次合作的标志是_____。");
	wcscpy_s(q197.optionA, L"A. 西安事变的和平解决");
	wcscpy_s(q197.optionB, L"B. 红军改编为国民革命军");
	wcscpy_s(q197.optionC, L"C. 国民党公布《中共中央为公布国共合作宣言》");
	wcscpy_s(q197.answer, L"C. 国民党公布《中共中央为公布国共合作宣言》");
	questions.push_back(q197);

	Question q198;
	wcscpy_s(q198.content, L"根据用和平方式解决西安事变的方针，周恩来与张学良、杨虎城共同努力，经过谈判，迫使蒋介石作出_____ 等六项承诺。");
	wcscpy_s(q198.optionA, L"A. 停止内战，实行抗战");
	wcscpy_s(q198.optionB, L"B. 停止内战，共同抗日");
	wcscpy_s(q198.optionC, L"C. 停止剿共，联红抗日");
	wcscpy_s(q198.answer, L"C. 停止剿共，联红抗日");
	questions.push_back(q198);

	Question q199;
	wcscpy_s(q199.content, L"红军长征最直接最重要的原因是_____。");
	wcscpy_s(q199.optionA, L"A. 北上抗日");
	wcscpy_s(q199.optionB, L"B. 国共合作破裂");
	wcscpy_s(q199.optionC, L"C. 第五次反“围剿”的失败");
	wcscpy_s(q199.answer, L"C. 第五次反“围剿”的失败");
	questions.push_back(q199);

	Question q200;
	wcscpy_s(q200.content, L"在 1930 年毛泽东制定的土地和革命路线中，对富农的政策是_____。");
	wcscpy_s(q200.optionA, L"A. 限制富农");
	wcscpy_s(q200.optionB, L"B. 消灭富农");
	wcscpy_s(q200.optionC, L"C. 没收土地");
	wcscpy_s(q200.answer, L"A. 限制富农");
	questions.push_back(q200);
#endif
}

// 随机选择一道题目
Question SelectQuestion()
{
	srand(static_cast<unsigned int>(time(nullptr)));  // 初始化随机数种子
	int index = rand() % questions.size();  // 随机选择题目索引
	return questions[index];
}

void Exercise()
{
	InitQues();

	initgraph(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

	while (true)
	{
		setbkmode(TRANSPARENT);
		setbkcolor(BROWN);

		cleardevice();  // 清空绘图窗口

		// 随机选择一道题目
		Question q = SelectQuestion();

		// 绘制题目内容和选项
		settextcolor(WHITE);
		settextstyle(15, 0, L"宋体");
		outtextxy(50, 50, q.content);

		settextcolor(BLACK);
		fillrectangle(50, 90, 450, 130);
		outtextxy(75, 100, q.optionA);
		fillrectangle(50, 140, 450, 180);
		outtextxy(75, 150, q.optionB);
		fillrectangle(50, 190, 450, 230);
		outtextxy(75, 200, q.optionC);

		// 绘制退出按钮
		settextcolor(BLACK);
		settextstyle(15, 0, L"宋体");
		fillrectangle(50, 270, 150, 310);
		outtextxy(75, 280, L"退出");


		// 获取用户输入
		MOUSEMSG m;
		while (true)
		{
			m = GetMouseMsg();
			if (m.uMsg == WM_LBUTTONDOWN)
				break;
		}

		// 检查是否点击退出按钮
		if (m.x >= 50 && m.x <= 150 && m.y >= 270 && m.y <= 310)
		{
			Mode();
		}

		// 检查用户答案
		bool correct = (wcscmp(m.y < 150 ? q.optionA : (m.y < 200 ? q.optionB : q.optionC), q.answer) == 0);

		// 绘制结果
		cleardevice();
		outtextxy(50, 50, q.content);
		outtextxy(100, 100, q.optionA);
		outtextxy(100, 150, q.optionB);
		outtextxy(100, 200, q.optionC);


		if (correct) 
		{
			settextcolor(GREEN);
			outtextxy(50, 250, L"回答正确！");
		}
		else 
		{
			settextcolor(RED);
			outtextxy(50, 250, L"回答错误！");
			outtextxy(50, 300, L"正确答案是:");
			settextcolor(YELLOW);
			outtextxy(200, 300, q.answer);
		}

		settextcolor(WHITE);
		outtextxy(50, 300, L"点击鼠标继续...");

		// 等待用户点击鼠标继续
		while (true) 
		{
			m = GetMouseMsg();
			if (m.uMsg == WM_LBUTTONDOWN) 
			{
				break;
			}
		}
	}
}

void DrawCompetition(const Question& q, int timeRemaining)
{
	setbkmode(TRANSPARENT);
	setbkcolor(BROWN);

	cleardevice();  // 清空绘图窗口

	// 绘制题目内容和选项
	settextcolor(WHITE);
	settextstyle(15, 0, L"宋体");
	outtextxy(50, 50, q.content);

	settextcolor(BLACK);
	fillrectangle(50, 90, 450, 130);
	outtextxy(75, 100, q.optionA);
	fillrectangle(50, 140, 450, 180);
	outtextxy(75, 150, q.optionB);
	fillrectangle(50, 190, 450, 230);
	outtextxy(75, 200, q.optionC);

	// 绘制倒计时
	settextcolor(BLUE);
	settextstyle(15, 0, L"宋体");
	outtextxy(75, 250, L"剩余时间（秒）: ");
	outtextxy(75, 275, to_wstring(timeRemaining).c_str());
}

void DisplayResult(int correctCount, int incorrectCount, int answerTime)
{
	// 初始化图形界面并设置窗口大小为屏幕大小
	initgraph(800, 450);

	setbkmode(TRANSPARENT);
	setbkcolor(LIGHTCYAN);
	// 清空图形界面
	cleardevice();

	// 绘制标题
	settextstyle(50, 0, _T("隶书"));
	settextcolor(BLACK);
	outtextxy(275, 50, _T("竞赛结束"));

	// 绘制选项
	setfillcolor(WHITE);
	setlinecolor(BLACK);
	settextstyle(25, 0, _T("黑体"));
	settextcolor(BLACK);

	fillrectangle(275, 125, 470, 160);
	outtextxy(300, 130, L"正确：");
	outtextxy(375, 130, to_wstring(correctCount).c_str());

	fillrectangle(275, 175, 470, 210);
	outtextxy(300, 180, L"错误：");
	outtextxy(375, 180, to_wstring(incorrectCount).c_str());

	fillrectangle(275, 225, 650, 260);
	outtextxy(300, 230, L"回答用时（秒）：");
	outtextxy(500, 230, to_wstring(answerTime).c_str());

	setfillcolor(WHITE);
	setlinecolor(RED);
	settextstyle(25, 0, _T("黑体"));
	settextcolor(BLUE);
	fillrectangle(275, 275, 470, 310);
	outtextxy(325, 280, _T("返回"));

	MOUSEMSG m;
	while (true)
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (m.x > 275 && m.x < 470 && m.y > 275 && m.y < 310)
			{
				// 返回选择模式页面
				Mode();
				break;
			}
		}
	}
}

void MockExam(int questionCount, int timeLimit)
{
	InitQues();

	initgraph(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

	// 计算结束时间
	time_t startTime = time(nullptr);
	time_t endTime = startTime + timeLimit;

	int correctCount = 0;
	int incorrectCount = 0;
	int answerTime = 0;

	while (questionCount > 0 && time(nullptr) < endTime)
	{
		// 随机选择一道题目
		Question q = SelectQuestion();

		// 绘制竞赛界面
		DrawCompetition(q, endTime - time(nullptr));

		// 获取用户输入
		MOUSEMSG m;
		while (true)
		{
			m = GetMouseMsg();
			if (m.uMsg == WM_LBUTTONDOWN)
				break;
		}

		// 检查用户答案
		bool correct = (wcscmp(m.y < 150 ? q.optionA : (m.y < 200 ? q.optionB : q.optionC), q.answer) == 0);

		// 绘制结果
		cleardevice();
		settextcolor(WHITE);
		outtextxy(50, 50, q.content);
		outtextxy(100, 100, q.optionA);
		outtextxy(100, 150, q.optionB);
		outtextxy(100, 200, q.optionC);

		if (correct)
		{
			settextcolor(GREEN);
			outtextxy(50, 250, L"回答正确！");
			correctCount++;
		}
		else
		{
			settextcolor(RED);
			outtextxy(50, 250, L"回答错误！");
			outtextxy(50, 300, L"正确答案是:");
			settextcolor(YELLOW);
			outtextxy(200, 300, q.answer);
			incorrectCount++;
		}

		settextcolor(WHITE);
		outtextxy(50, 300, L"点击鼠标继续...");

		// 等待用户点击鼠标继续
		while (true)
		{
			m = GetMouseMsg();
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				break;
			}
		}

		questionCount--;
		answerTime += time(nullptr) - startTime;
		startTime = time(nullptr);
	}

	closegraph();  // 关闭绘图窗口

	DisplayResult(correctCount, incorrectCount, answerTime);
}

bool CompareUsers(const User& participant1, const User& participant2)
{
	if (participant1.score > participant2.score)
	{
		return true;
	}
	else if (participant1.score == participant2.score)
	{
		if (participant1.answerTime < participant2.answerTime)
		{
			return true;
		}
		else if (participant1.answerTime == participant2.answerTime)
		{
			return participant1.username < participant2.username;
		}
	}
	return false;
}

void Exam(int questionCount, int timeLimit)
{
	InitQues();

	initgraph(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

	// 计算结束时间
	time_t startTime = time(nullptr);
	time_t endTime = startTime + timeLimit;

	int correctCount = 0;
	int incorrectCount = 0;
	int answerTime = 0;

	while (questionCount > 0 && time(nullptr) < endTime)
	{
		// 随机选择一道题目
		Question q = SelectQuestion();

		// 绘制竞赛界面
		DrawCompetition(q, endTime - time(nullptr));

		// 获取用户输入
		MOUSEMSG m;
		while (true)
		{
			m = GetMouseMsg();
			if (m.uMsg == WM_LBUTTONDOWN)
				break;
		}

		// 检查用户答案
		bool correct = (wcscmp(m.y < 150 ? q.optionA : (m.y < 200 ? q.optionB : q.optionC), q.answer) == 0);

		// 绘制结果
		cleardevice();
		settextcolor(WHITE);
		outtextxy(50, 50, q.content);
		outtextxy(100, 100, q.optionA);
		outtextxy(100, 150, q.optionB);
		outtextxy(100, 200, q.optionC);

		if (correct)
		{
			settextcolor(GREEN);
			outtextxy(50, 250, L"回答正确！");
			correctCount++;
		}
		else
		{
			settextcolor(RED);
			outtextxy(50, 250, L"回答错误！");
			outtextxy(50, 300, L"正确答案是:");
			settextcolor(YELLOW);
			outtextxy(200, 300, q.answer);
			incorrectCount++;
		}

		settextcolor(WHITE);
		outtextxy(50, 300, L"点击鼠标继续...");

		// 等待用户点击鼠标继续
		while (true)
		{
			m = GetMouseMsg();
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				break;
			}
		}

		questionCount--;
		answerTime += time(nullptr) - startTime;
		startTime = time(nullptr);
	}

	closegraph();  // 关闭绘图窗口

	DisplayResult(correctCount, incorrectCount, answerTime);
}

int main()
{
	Init();

	return 0;
}
