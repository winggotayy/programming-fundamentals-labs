#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <utility>

using namespace std;

#define DEBUG 0

/*�û�����*/
enum SEARCH_USER_WAY
{
	PHONENUM,
	NICKNAME,
	CLOSE
};

enum ADD_USER_WAY
{
	DEFAULT,
	QUESTION,
	NONE
};

enum MOMENTS_PRIVACY
{
	FRIENDS,
	ONLY_ME
};

/*�û���Ϣ*/
struct UserData
{
	string phonenum;
	string nickname;
	string password;
	string code;
	enum SEARCH_USER_WAY search_way;
	enum ADD_USER_WAY add_way;
	enum MOMENTS_PRIVACY moments_privacy;

	// ���⣬��
	pair<string, string>question;
};

/*ģ�����죨����ģʽ��*/
enum OPERATOR
{
	REGISTER,
	LOGIN,
	ADD_FRIEND,
	SEND_MESSAGE,
	SEARCH_MESSAGE,
	POST_MOMENTS,
	SEARCH_MOMENTS,
	LIKE_MOMENTS,
	COMMENT_MOMENTS
};

/*����Ȧ��Ϣ*/
struct Moment
{
	string phonenum;
	string content;
	enum MOMENTS_PRIVACY privacy;
	int likes;
	vector<pair<string, string>> comments;
	map<string, bool> liked;
};