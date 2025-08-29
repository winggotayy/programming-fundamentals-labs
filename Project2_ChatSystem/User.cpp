#include "User.h"

// ע��
bool User::Register(Server& manager)
{
	string phonenum, nickname, password, code;
	
	cout << "\t\tע��\n";
	cout << "�������ֻ����룺" << endl;
	cin >> phonenum;
	// Ψһ���ж�
	if (manager.Search_User(phonenum))
	{
		cout << "���ֻ������ڴ�ǰ�ѱ�ע�ᣡ" << endl;
		return false;
	}
	// �Ϸ����ж�
	regex pn_reg("^1[0-9]{10}");
	smatch matchResult;
	if (DEBUG);
	else if (!regex_match(phonenum, matchResult, pn_reg))
	{
		cout << "�ֻ����벻�Ϸ���" << endl;
		return false;
	}

	cout << "�������ǳƣ�" << endl;
	cin >> nickname;
	
	cout << "���������룺" << endl;
	cin >> password;
	if (!testpwd(password))
	{
		cout << "���벻�Ϸ���" << endl;
		return false;
	}
	cout << "���ٴ��������룺" << endl;
	string password1;
	cin >> password1;
	if (password1 != password)
	{
		cout << "�����������." << endl;
		return false;
	}

	string code1 = to_string(manager.Rand_Code());
	cout << "��֤��: " << code1 << endl;
	cout << "��������֤�룺" << endl;
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
		cout << "��֤���������." << endl;
		return false;
	}
}

// �ж�����Ϸ���
bool User::testpwd(string s)
{
	if (DEBUG) return true;
	regex pwd_reg = regex("(?=.*[a-z])(?=.*[A-Z])(?=.*[0-9])[a-zA-Z0-9]{8,}");
	smatch matchResult;
	if (!regex_match(s, matchResult, pwd_reg)) 
		return false;
	return true;
}

// ��¼
bool User::Login(Server& manager) 
{
	string phonenum, nickname, password, code;

	cout << "\t\t��¼\n";
	cout << "�������ֻ����룺" << endl;
	cin >> phonenum;

	cout << "���������룺" << endl;
	cin >> password;

	if (!manager.Search_User(phonenum))
	{
		cout << "���û������ڣ�" << endl;
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
			cout << "�����������." << endl;
			return false;
		}
	}

	string code1 = to_string(manager.Rand_Code());
	cout << "��֤��: " << code1 << endl;
	cout << "��������֤�룺" << endl;
	cin >> code;

	if (code == code1)
	{
		this->isLoggedIn = true;
		return true;
	}
	else
	{
		cout << "��֤���������." << endl;
		return false;
	}
}

// �ǳ�
bool User::Logout(Server& manager) 
{
	this->isLoggedIn = false;
	return this->isLoggedIn;
}

/* ���� */

// ��Ӻ���
bool User::Add_Friend(Server& manager, string& identifier, enum SEARCH_USER_WAY search_way)
{
	if (search_way == CLOSE) 
		return false;

	UserData userdata = manager.getUserData(identifier, search_way);

	//�жϣ���ӵ��û� ���� �����Լ� & ���������б���
	if (userdata.phonenum != this->m_user_data->phonenum && !manager.Search_Friend(m_user_data->phonenum, userdata.phonenum))
	{
		manager.Make_Friend(this->m_user_data->phonenum, userdata.phonenum);
		return true;
	}
	else 
		return false;
}

// �����б�
void User::Friends_List(Server& manager)
{
	manager.Friends_List(m_user_data->phonenum);
}

/*����*/

// ����������
void User::Send_Message(Server& manager, string& phonenum2, string& messages)
{
	manager.Send_Message(m_user_data->phonenum, phonenum2, messages);
}

// �鿴�����¼
void User::Search_Message_friend(Server& manager, string& phonenum2, vector<string>& message)
{
	manager.Search_Message_friend(m_user_data->phonenum, phonenum2, message);
}

/*����Ȧ*/
	
// ��������Ȧ
void User::Post_Moments(Server& manager, string& moment, bool privacy) 
{
	manager.Send_Moments(this->m_user_data->phonenum, moment, privacy);
}

void printMoment(Moment& moment)
{
	cout << "\t�û� " << moment.phonenum << " �ѷ���һ������Ȧ\n";
	cout << "\t" << moment.content << endl;
	cout << "\t�� : " << moment.likes << endl;
	cout << "\t���� : " << endl << endl;
	for (auto& comment : moment.comments) 
	{
		cout << "\t" << comment.first << " : " << comment.second << endl;

	}
}
// �鿴����Ȧ
void User::Checkout_Moments(Server& manager) 
{
	vector<Moment> moments = manager.Checkout_Moments(this->m_user_data->phonenum);
	for (auto& moment : moments) 
	{
		printMoment(moment);
	}
}

// ��������Ȧ
void User::Like_Moments(Server& manager, string& phonenum2, string& moments)
{
	manager.Like_Moments(m_user_data->phonenum, phonenum2, moments);
}

// ��������Ȧ
void User::Comment_Moments(Server& manager, string& phonenum2, string& comment, string& moments)
{
	manager.Comment_Moments(m_user_data->phonenum, phonenum2, comment, moments);
}

/*�û�����*/

// �޸��ǳ�
void User::set_nickname(Server& manager, string newnic) 
{
	this->m_user_data->nickname = newnic;
	manager.set_user_nickname(this->getUserData()->phonenum, newnic);
}

// �޸�����
void User::set_password(Server& manager, string newpass) 
{
	this->m_user_data->password = newpass;
	manager.set_user_password(this->getUserData()->phonenum, newpass);
}

// ����Ȩ��
void User::set_search_way(Server& manager, enum SEARCH_USER_WAY choice)
{
	this->m_user_data->search_way = choice;
	manager.set_search_way(this->getUserData()->phonenum, choice);
}

// ͨ����������ʽ
void User::set_add_way(Server& manager, enum ADD_USER_WAY choice, pair<string, string> question = { "", "" })
{
	this->m_user_data->add_way = choice;
	if (choice == ADD_USER_WAY::QUESTION) {
		this->m_user_data->question = question;
	}
	manager.set_add_way(this->getUserData()->phonenum, choice, question);
}

// ��װ
void User::config(UserData& userdata) 
{
	*this->m_user_data = userdata;
}