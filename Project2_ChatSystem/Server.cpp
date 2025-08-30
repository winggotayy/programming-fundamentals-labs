#include "Server.h"

vector<UserData> users;

//生成验证码
int Server::Rand_Code()
{
	srand(time(NULL));
	int seed = rand() % 9000 + 1000;
	return seed;
}

bool Server::Search_User(string& phonenum)
{
	map<string, UserData>::iterator it = users.begin();
	for (; it != users.end(); it++)
	{
		if (it->second.phonenum == phonenum)
		{
			return true;
		}
	}
	return false;
}

bool Server::Add_User(string& phonenum, UserData& user)
{
	users[phonenum] = user;

	search_way[phonenum] = user.search_way;
	add_way[phonenum] = user.add_way;
	moments_privacy[phonenum] = user.moments_privacy;
	questions[phonenum] = user.question;

	return true;
}

bool Server::Search_and_Check_Password(string& phonenum, string& password) 
{
	if (this->users[phonenum].password == password)
		return true;
	return false;
}

bool Server::Search_Friend(string& phonenum1, string& phonenum2)
{
	// 是否存在此用户（phonenum2）
	auto it2 = users.begin();
	for (; it2 != users.end(); it2++)
	{
		if (it2->first == phonenum2)
		{
			break;
		}
	}
	if (it2 == users.end()) 
	{
		cout << "此用户不存在！" << endl;
		return true;
	}

	// 是否已是好友关系
	map<string, vector<string>>::iterator it1 = relationship.begin();
	for (; it1 != relationship.end(); it1++) 
	{
		if (it1->first == phonenum1) 
		{
			for (const auto& x : it1->second) 
			{
				if (x == phonenum2)
				{
					cout << "您和对方已是好友." << endl;
					return true;
				}
			}
		}
	}

	return false;
}

bool Server::Make_Friend(string& phonenum1, string& phonenum2)
{
	this->relationship[phonenum1].push_back(phonenum2);
	this->relationship[phonenum2].push_back(phonenum1);
	return true;
}

// 好友列表
bool Server::Friends_List(string &phonenum1)
{
	int count = 0;
	for (auto it = relationship[phonenum1].begin(); it != relationship[phonenum1].end(); it++) 
	{
		count++;
		cout << "\n好友 " << count << ":" << endl;
		cout << *it << endl;
	};
	cout << endl;
	return true;
}
  
/*留言*/

// 给好友留言
bool Server::Send_Message(string& phonenum1, string& phonenum2, string& msg)
{
	// phonenum1：自己 // phonenum2：朋友
	pair<string, string> relation{ phonenum1, phonenum2 };
	relation_msg[relation].push_back(msg);
	return true;
}

// 查看聊天记录
bool Server::Search_Message_friend(string& phonenum1, string& phonenum2,vector<string>& msg)
{
	pair<string, string> relation{ phonenum1, phonenum2 };
	msg = relation_msg[relation];
	return true;
}

/*朋友圈*/

// 发布朋友圈
bool Server::Send_Moments(string& phonenum1, string& content, bool privacy)
{
	Moment new_moment = 
	{
		phonenum1,
		content,
		static_cast<enum MOMENTS_PRIVACY>(privacy),
		0,
		{},
		map<string, bool>()
	};
	this->moments_map[phonenum1].push_back(new_moment);
	return true;
}

// 查看朋友圈
vector<Moment> Server::Checkout_Moments(string& phonenum1)
{
	vector<Moment> moments = this->moments_map[phonenum1];
	for (auto& myfriend : this->relationship[phonenum1]) 
	{
		for (auto& friendMoment : this->moments_map[myfriend])
			if (friendMoment.privacy == MOMENTS_PRIVACY::FRIENDS)
				moments.push_back(friendMoment);
	}
	return moments;
}

// 点赞朋友圈
bool Server::Like_Moments(string& phonenum1, string& phonenum2, string& moments)
{
	unsigned moment_index = 0;
	while (this->moments_map[phonenum2][moment_index].content != moments) moment_index++;
	if (this->moments_map[phonenum2][moment_index].liked[phonenum1] == false) 
	{
		this->moments_map[phonenum2][moment_index].liked[phonenum1] = true;
		this->moments_map[phonenum2][moment_index].likes++;
		return true;
	}
	return false;
}

// 评论朋友圈
bool Server::Comment_Moments(string& phonenum1, string& phonenum2, string& comment, string& moments)
{
	unsigned moment_index = 0;
	while (this->moments_map[phonenum2][moment_index].content != moments) moment_index++;
	this->moments_map[phonenum2][moment_index].comments.push_back({ phonenum1 , comment });
	return true;
}

/*设置*/

// 修改昵称
void Server::set_user_nickname(string& myphone, string newnic) 
{
	this->users[myphone].nickname = newnic;
}

// 修改密码
void Server::set_user_password(string& myphone, string newpass) 
{
	this->users[myphone].password = newpass;
}

// 搜索权限
void Server::set_search_way(string& myphone, enum SEARCH_USER_WAY choice)
{
	this->users[myphone].search_way = choice;
}

// 通过好友请求方式
void Server::set_add_way(string& myphone, enum ADD_USER_WAY choice, pair<string, string> question)
{
	this->users[myphone].add_way = choice;
	if (choice == ADD_USER_WAY::QUESTION)
		this->users[myphone].question = question;
}

// 显示系统当前已注册用户
void Server::show_Users_Data() 
{
	if (this->users.size() == 0) 
	{
		cout << "当前无用户\n";
		return;
	}
	cout << endl;
	for (auto user = this->users.begin(); user != this->users.end(); user++) 
	{	
		cout << "\t手机号码: " << user->second.phonenum << endl;
		cout << "\t密码: " << user->second.password << endl;
		cout << "\t昵称: " << user->second.nickname << endl;
		cout << "\t注册验证码: " << user->second.code << endl;
		cout << "\t好友: ";
		for (auto myfriend : this->relationship[user->second.phonenum])
			cout << " " << myfriend << endl;
		cout << endl << endl;
	}
	cout << " " << endl;
}

UserData Server::getUserData(string identifier, enum SEARCH_USER_WAY search_way) 
{
	switch (search_way) 
	{
	case PHONENUM:
		return this->users[identifier];
	case NICKNAME:
		for (auto& user : this->users) 
		{
			if (user.second.nickname == identifier) 
				return user.second;
		}
	}
}

enum ADD_USER_WAY Server::getUserAddWay(string identifier, enum SEARCH_USER_WAY search_way) 
{
	switch (search_way) 
	{
	case PHONENUM:
		return this->users[identifier].add_way;
	case NICKNAME:
		for (auto& user : this->users) 
		{
			if (user.second.nickname == identifier) 
				return user.second.add_way;
		}
	}
}