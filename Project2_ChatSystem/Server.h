#pragma once

#include <cstdlib>
#include <ctime>

#include "data_define.h"

#define DEBUG 0

class Server
{
public:
	Server() {
	}
	~Server() {}
public:
	//生成验证码
	int Rand_Code();
	bool Search_User(string& phonenum);
	bool Add_User(string& phonenum, UserData& user);
	bool Search_and_Check_Password(string& phonenum, string& password);
	
/*好友菜单*/
	bool Search_Friend(string& phonenum1, string& phonenum2);
	bool Make_Friend(string& phonenum1, string& phonenum2);
	// 好友列表
	bool Friends_List(string& phonenum1);

/*留言*/
	// 给好友留言
	bool Send_Message(string& phonenum1, string& phonenum2, string& messages);
	// 查看聊天记录
	bool Search_Message_friend(string& phonenum1, string& phonenum2, vector<string>& msg);

/*朋友圈*/
	// 发布朋友圈
	bool Send_Moments(string& phonenum1, string& content, bool privacy);
	// 查看朋友圈
	vector<Moment> Checkout_Moments(string& phonenum1);
	// 点赞朋友圈
	bool Like_Moments(string& phonenum1, string& phonenum2, string& moments);
	// 评论朋友圈
	bool Comment_Moments(string& phonenum1, string& phonenum2, string& comment, string& moments);

/*设置*/
	//修改昵称
	void set_user_nickname(string& myphone, string newnic);
	// 修改密码
	void set_user_password(string& myphone, string newpass);
	// 搜索权限
	void set_search_way(string& myphone, enum SEARCH_USER_WAY choice);
	// 通过好友请求方式
	void set_add_way(string& myphone, enum ADD_USER_WAY choice, pair<string, string> question = {"", ""});

	// 显示系统当前已注册用户
	void show_Users_Data();

	UserData getUserData(string identifier, enum SEARCH_USER_WAY search_way = SEARCH_USER_WAY::PHONENUM);
	enum ADD_USER_WAY getUserAddWay(string identifier, enum SEARCH_USER_WAY search_way = PHONENUM);

private:
	// 用户信息
	map<string, UserData>users;
	map<string, enum ADD_USER_WAY> add_way;
	map<string, enum SEARCH_USER_WAY> search_way;
	map<string, enum MOMENTS_PRIVACY> moments_privacy;
	map<string, pair<string, string>> questions;

	// 好友关系
	map<string, vector<string>>relationship;

	// 留言信息
	map<pair<string, string>, vector<string>>relation_msg;
	
	// 朋友圈信息
	map<string, vector<Moment>> moments_map;
};