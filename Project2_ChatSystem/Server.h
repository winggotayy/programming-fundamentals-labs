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
	//������֤��
	int Rand_Code();
	bool Search_User(string& phonenum);
	bool Add_User(string& phonenum, UserData& user);
	bool Search_and_Check_Password(string& phonenum, string& password);
	
/*���Ѳ˵�*/
	bool Search_Friend(string& phonenum1, string& phonenum2);
	bool Make_Friend(string& phonenum1, string& phonenum2);
	// �����б�
	bool Friends_List(string& phonenum1);

/*����*/
	// ����������
	bool Send_Message(string& phonenum1, string& phonenum2, string& messages);
	// �鿴�����¼
	bool Search_Message_friend(string& phonenum1, string& phonenum2, vector<string>& msg);

/*����Ȧ*/
	// ��������Ȧ
	bool Send_Moments(string& phonenum1, string& content, bool privacy);
	// �鿴����Ȧ
	vector<Moment> Checkout_Moments(string& phonenum1);
	// ��������Ȧ
	bool Like_Moments(string& phonenum1, string& phonenum2, string& moments);
	// ��������Ȧ
	bool Comment_Moments(string& phonenum1, string& phonenum2, string& comment, string& moments);

/*����*/
	//�޸��ǳ�
	void set_user_nickname(string& myphone, string newnic);
	// �޸�����
	void set_user_password(string& myphone, string newpass);
	// ����Ȩ��
	void set_search_way(string& myphone, enum SEARCH_USER_WAY choice);
	// ͨ����������ʽ
	void set_add_way(string& myphone, enum ADD_USER_WAY choice, pair<string, string> question = {"", ""});

	// ��ʾϵͳ��ǰ��ע���û�
	void show_Users_Data();

	UserData getUserData(string identifier, enum SEARCH_USER_WAY search_way = SEARCH_USER_WAY::PHONENUM);
	enum ADD_USER_WAY getUserAddWay(string identifier, enum SEARCH_USER_WAY search_way = PHONENUM);

private:
	// �û���Ϣ
	map<string, UserData>users;
	map<string, enum ADD_USER_WAY> add_way;
	map<string, enum SEARCH_USER_WAY> search_way;
	map<string, enum MOMENTS_PRIVACY> moments_privacy;
	map<string, pair<string, string>> questions;

	// ���ѹ�ϵ
	map<string, vector<string>>relationship;

	// ������Ϣ
	map<pair<string, string>, vector<string>>relation_msg;
	
	// ����Ȧ��Ϣ
	map<string, vector<Moment>> moments_map;
};