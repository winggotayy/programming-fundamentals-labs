#include "User.h"

// 注册
bool User::Register(Server& manager)
{
	string phonenum, nickname, password, code;
	
	cout << "\t\t注册\n";
	cout << "请输入手机号码：" << endl;
	cin >> phonenum;
	// 唯一性判断
	if (manager.Search_User(phonenum))
	{
		cout << "此手机号码在此前已被注册！" << endl;
		return false;
	}
	// 合法性判断
	regex pn_reg("^1[0-9]{10}");
	smatch matchResult;
	if (DEBUG);
	else if (!regex_match(phonenum, matchResult, pn_reg))
	{
		cout << "手机号码不合法！" << endl;
		return false;
	}

	cout << "请输入昵称：" << endl;
	cin >> nickname;
	
	cout << "请输入密码：" << endl;
	cin >> password;
	if (!testpwd(password))
	{
		cout << "密码不合法！" << endl;
		return false;
	}
	cout << "请再次输入密码：" << endl;
	string password1;
	cin >> password1;
	if (password1 != password)
	{
		cout << "密码输入错误." << endl;
		return false;
	}

	string code1 = to_string(manager.Rand_Code());
	cout << "验证码: " << code1 << endl;
	cout << "请输入验证码：" << endl;
	cin >> code;
	if (code == code1) 
	{
		this->m_user_data->phonenum = phonenum;
		this->m_user_data->password = password;
		this->m_user_data->nickname = nickname;
		this->m_user_data->code = code;
		
		if (manager.Add_User(m_user_data->phonenum, *m_user_data)) 
		{
			this->isLoggedIn = true;
			return true;
		}
	}
	else 
	{
		cout << "验证码输入错误." << endl;
		return false;
	}
}

// 判断密码合法性
bool User::testpwd(string s)
{
	if (DEBUG) return true;
	regex pwd_reg = regex("(?=.*[a-z])(?=.*[A-Z])(?=.*[0-9])[a-zA-Z0-9]{8,}");
	smatch matchResult;
	if (!regex_match(s, matchResult, pwd_reg)) 
		return false;
	return true;
}

// 登录
bool User::Login(Server& manager) 
{
	string phonenum, nickname, password, code;

	cout << "\t\t登录\n";
	cout << "请输入手机号码：" << endl;
	cin >> phonenum;

	cout << "请输入密码：" << endl;
	cin >> password;

	if (!manager.Search_User(phonenum))
	{
		cout << "此用户不存在！" << endl;
		return false;
	}
	if (manager.Search_User(phonenum))
	{
		if (manager.Search_and_Check_Password(phonenum, password))
		{
			this->m_user_data->phonenum = phonenum;
			this->m_user_data->password = password;
			this->m_user_data->nickname = nickname;
			this->m_user_data->code = code;
		}
		else
		{
			cout << "密码输入错误." << endl;
			return false;
		}
	}

	string code1 = to_string(manager.Rand_Code());
	cout << "验证码: " << code1 << endl;
	cout << "请输入验证码：" << endl;
	cin >> code;

	if (code == code1)
	{
		this->isLoggedIn = true;
		return true;
	}
	else
	{
		cout << "验证码输入错误." << endl;
		return false;
	}
}

// 登出
bool User::Logout(Server& manager) 
{
	this->isLoggedIn = false;
	return this->isLoggedIn;
}

/* 好友 */

// 添加好友
bool User::Add_Friend(Server& manager, string& identifier, enum SEARCH_USER_WAY search_way)
{
	if (search_way == CLOSE) 
		return false;

	UserData userdata = manager.getUserData(identifier, search_way);

	//判断：添加的用户 —— 不是自己 & 不在朋友列表内
	if (userdata.phonenum != this->m_user_data->phonenum && !manager.Search_Friend(m_user_data->phonenum, userdata.phonenum))
	{
		manager.Make_Friend(this->m_user_data->phonenum, userdata.phonenum);
		return true;
	}
	else 
		return false;
}

// 好友列表
void User::Friends_List(Server& manager)
{
	manager.Friends_List(m_user_data->phonenum);
}

/*留言*/

// 给好友留言
void User::Send_Message(Server& manager, string& phonenum2, string& messages)
{
	manager.Send_Message(m_user_data->phonenum, phonenum2, messages);
}

// 查看聊天记录
void User::Search_Message_friend(Server& manager, string& phonenum2, vector<string>& message)
{
	manager.Search_Message_friend(m_user_data->phonenum, phonenum2, message);
}

/*朋友圈*/
	
// 发布朋友圈
void User::Post_Moments(Server& manager, string& moment, bool privacy) 
{
	manager.Send_Moments(this->m_user_data->phonenum, moment, privacy);
}

void printMoment(Moment& moment)
{
	cout << "\t用户 " << moment.phonenum << " 已发布一条朋友圈\n";
	cout << "\t" << moment.content << endl;
	cout << "\t赞 : " << moment.likes << endl;
	cout << "\t评论 : " << endl << endl;
	for (auto& comment : moment.comments) 
	{
		cout << "\t" << comment.first << " : " << comment.second << endl;

	}
}
// 查看朋友圈
void User::Checkout_Moments(Server& manager) 
{
	vector<Moment> moments = manager.Checkout_Moments(this->m_user_data->phonenum);
	for (auto& moment : moments) 
	{
		printMoment(moment);
	}
}

// 点赞朋友圈
void User::Like_Moments(Server& manager, string& phonenum2, string& moments)
{
	manager.Like_Moments(m_user_data->phonenum, phonenum2, moments);
}

// 评论朋友圈
void User::Comment_Moments(Server& manager, string& phonenum2, string& comment, string& moments)
{
	manager.Comment_Moments(m_user_data->phonenum, phonenum2, comment, moments);
}

/*用户设置*/

// 修改昵称
void User::set_nickname(Server& manager, string newnic) 
{
	this->m_user_data->nickname = newnic;
	manager.set_user_nickname(this->getUserData()->phonenum, newnic);
}

// 修改密码
void User::set_password(Server& manager, string newpass) 
{
	this->m_user_data->password = newpass;
	manager.set_user_password(this->getUserData()->phonenum, newpass);
}

// 搜索权限
void User::set_search_way(Server& manager, enum SEARCH_USER_WAY choice)
{
	this->m_user_data->search_way = choice;
	manager.set_search_way(this->getUserData()->phonenum, choice);
}

// 通过好友请求方式
void User::set_add_way(Server& manager, enum ADD_USER_WAY choice, pair<string, string> question = { "", "" })
{
	this->m_user_data->add_way = choice;
	if (choice == ADD_USER_WAY::QUESTION) {
		this->m_user_data->question = question;
	}
	manager.set_add_way(this->getUserData()->phonenum, choice, question);
}

// 封装
void User::config(UserData& userdata) 
{
	*this->m_user_data = userdata;
}