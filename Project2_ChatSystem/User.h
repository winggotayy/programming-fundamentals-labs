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
	// �������
	bool isLoggedIn;
	
	// ע��
	bool Register(Server& manager);
	// ��¼
	bool Login(Server& manager);
	// �ж�����Ϸ���
	bool testpwd(string s);
	// �ǳ�
	bool Logout(Server& manager);
	
/*���Ѳ˵�*/
	// ��Ӻ���
	bool Add_Friend(Server& manager, string& phonenum2, enum SEARCH_USER_WAY search_way = SEARCH_USER_WAY::PHONENUM);
	// �����б�
	void Friends_List(Server& manager);
	
/*����*/
	// ����������
	void Send_Message(Server& manager, string& phonenum2, string& messages);
	// �鿴�����¼
	void Search_Message_friend(Server& manager, string& phonenum2, vector<string>& message);

/*����Ȧ*/
	// ��������Ȧ
	void Post_Moments(Server& manager, string& moment, bool privacy);
	// �鿴����Ȧ
	void Checkout_Moments(Server& manager);
	// ��������Ȧ
	void Like_Moments(Server& manager, string& phonenum2, string& moments);
	// ��������Ȧ
	void Comment_Moments(Server& manager, string& phonenum2, string& comment, string& moments);

/*����*/
	// �޸��ǳ�
	void set_nickname(Server& manager, string newnic);
	// �޸�����
	void set_password(Server& manager, string newpass);
	// ����Ȩ��
	void set_search_way(Server& manager, SEARCH_USER_WAY choice);
	// ͨ����������ʽ
	void set_add_way(Server& manager, enum ADD_USER_WAY choice, pair<string, string> question);

private:
	UserData* m_user_data;
};