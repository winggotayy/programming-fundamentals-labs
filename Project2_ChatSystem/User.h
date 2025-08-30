#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <regex>

#include "Server.h"

class User
{
public:
	User() : m_user_data{ new UserData } 
	{
		this->isLoggedIn = false;
	}
	~User()
	{
		delete m_user_data;
	}
	void config(UserData& userdata);
	UserData* getUserData() 
	{
		return this->m_user_data;
	}
public:
	// 登入与否
	bool isLoggedIn;
	
	// 注册
	bool Register(Server& manager);
	// 登录
	bool Login(Server& manager);
	// 判断密码合法性
	bool testpwd(string s);
	// 登出
	bool Logout(Server& manager);
	
/*好友菜单*/
	// 添加好友
	bool Add_Friend(Server& manager, string& phonenum2, enum SEARCH_USER_WAY search_way = SEARCH_USER_WAY::PHONENUM);
	// 好友列表
	void Friends_List(Server& manager);
	
/*留言*/
	// 给好友留言
	void Send_Message(Server& manager, string& phonenum2, string& messages);
	// 查看聊天记录
	void Search_Message_friend(Server& manager, string& phonenum2, vector<string>& message);

/*朋友圈*/
	// 发布朋友圈
	void Post_Moments(Server& manager, string& moment, bool privacy);
	// 查看朋友圈
	void Checkout_Moments(Server& manager);
	// 点赞朋友圈
	void Like_Moments(Server& manager, string& phonenum2, string& moments);
	// 评论朋友圈
	void Comment_Moments(Server& manager, string& phonenum2, string& comment, string& moments);

/*设置*/
	// 修改昵称
	void set_nickname(Server& manager, string newnic);
	// 修改密码
	void set_password(Server& manager, string newpass);
	// 搜索权限
	void set_search_way(Server& manager, SEARCH_USER_WAY choice);
	// 通过好友请求方式
	void set_add_way(Server& manager, enum ADD_USER_WAY choice, pair<string, string> question);

private:
	UserData* m_user_data;
};