#include "User.h"

void Menu()
{
	cout << "=========����ģ�����=========" << endl;
	cout << " " << endl;
	cout << "--- ���Ѳ˵� ---" << endl;
	cout << "1. ��Ӻ���" << endl;
	cout << "2. �����б�" << endl;
	cout << " " << endl;
	cout << "-- ���� --" << endl;
	cout << "3. ����������" << endl;
	cout << "4. �鿴�����¼" << endl;
	cout << " " << endl;
	cout << "-- ����Ȧ --" << endl;
	cout << "5. ��������Ȧ" << endl;
	cout << "6. �鿴����Ȧ" << endl;
	cout << "7. ��������Ȧ" << endl;
	cout << "8. ��������Ȧ" << endl;
	cout << " " << endl;
	cout << "-- �û����� --" << endl;
	cout << "9. �޸��ǳ�" << endl;
	cout << "10. �޸�����" << endl;
	cout << "11. ����Ȩ��" << endl;
	cout << "12. ͨ����������ʽ" << endl;
	cout << " " << endl;
	cout << "-- ���� --" << endl;
	cout << "13. �ǳ�" << endl;
	cout << "14. �˳�ϵͳ........" << endl;
	cout << " " << endl;
	cout << "=============================" << endl;
}

bool Register_or_Login(User& user, Server& manager) 
{
	int action = 2;
	while (action > 1) 
	{
		cout << "ע�� - 0\n��¼ - 1\n";
		cin >> action;
		if (action == 0) 
		{
			if (user.Register(manager)) 
			{
				cout << "\tע��ɹ���\n";
				return true;
			}
		}
		else if (action == 1) 
		{
			if (user.Login(manager))
			{
				cout << "\t��¼�ɹ���\n";
				return true;
			}
		}
	}
}

void Function(User& user)
{
	Server manager;

	while (1)
	{
		if (!user.isLoggedIn)
		{
			if (Register_or_Login(user, manager))
				cout << "\n\t��ӭ���� >>>" /*user.getUserData()->nickname*/ << endl;
			else
				cout << "\t�����µ�¼/ע��.\n";
		}
		else
		{
			int func = -1;
			if (1)
			{
				Menu();
				cout << "����������Ҫ�����ı��: ";
				cin >> func;
				switch (func)
				{
				case 1: // ��Ӻ���
				{
					int search_way;
					cout << "�����������ķ�ʽ��" << endl;
					cout << "0 - �ֻ�����\n1 - �ǳ�\n";
					cin >> search_way;
					if (search_way != 1 && search_way != 0) 
					{
						cout << "������ 0 ���� 1" << endl;
						break;
					}
					cout << "����������ӵ��û����ֻ�����/�ǳƣ� ";
					string identifier;
					cin >> identifier;
					UserData otheruserdata = manager.getUserData(identifier, static_cast<SEARCH_USER_WAY>(search_way));
					if (otheruserdata.search_way == SEARCH_USER_WAY::CLOSE)
					{
						cout << "�޷���ѯ���û�." << endl;
					}
					else 
					{
						if (otheruserdata.add_way == ADD_USER_WAY::QUESTION) 
						{
							string answer;
							cout << "���⣺\n" << otheruserdata.question.first << endl;
							cout << "�������" << endl;
							cin >> answer;
							if (answer == otheruserdata.question.second) 
							{
								cout << "��ȷ!\n";
							}
							else 
							{
								cout << "����ȷ!\n";
								cout << "��Ӻ���ʧ��." << endl;
								break;
							}
						}
						if (user.Add_Friend(manager, identifier, static_cast<SEARCH_USER_WAY>(search_way)))
						{
							cout << "��Ӻ��ѳɹ�." << endl;
							break;
						}	
						else
						{
							cout << "��Ӻ���ʧ��." << endl;
							break;
						}
					}
					
					break;
				}
				case 2: // �����б�
				{
					cout << "=========�����б�=======" << endl;
					user.Friends_List(manager);
					cout << "=======================" << endl;
					break;
				}
				case 3: // ����������
				{
					string phone, msg;
					cout << "����������ֻ������������Ϣ: " << endl;
					cin >> phone >> msg;
					user.Send_Message(manager, phone, msg);
					break;
				}
				case 4: // �鿴�����¼
				{
					vector<string> inboxes;
					string phone, inbox;

					cout << "����������ֻ�����: ";
					cin >> phone;
					user.Search_Message_friend(manager, phone,inboxes);

					cout << "=========��������=======" << endl;
					for (const auto& x : inboxes)
					{
						cout << x << endl;
					}
					cout << "=======================" << endl;
					break;
				}
				case 5: // ��������Ȧ
				{
					string moment_content;
					bool privacy;
					cout << "����������Ȧ����: ";
					cin >> moment_content;
					cout << "չʾ��Χ��\n";
					cout << "0 - ���ѿɼ�\n1 - ���Լ��ɼ�\n";
					cin >> privacy;
					user.Post_Moments(manager, moment_content, privacy);
					break;
				}
				case 6: // �鿴����Ȧ
				{
					cout << "==========����Ȧ��Ϣ=========" << endl;
					cout << " " << endl;
					user.Checkout_Moments(manager);
					cout << " " << endl;
					cout << "==============================" << endl;
					break;
				}
				case 7: // ��������Ȧ
				{
					string phonenum2, moments;
					cout << "����������ֻ�����: " << endl;
					cin >> phonenum2;
					cout << "����������Ȧ��Ϣ: " << endl;
					cin >> moments;
					user.Like_Moments(manager, phonenum2, moments);
					break;
				}
				case 8: // ��������Ȧ
				{
					string phonenum2, comment, moments;
					cout << "����������ֻ�����: " << endl;
					cin >> phonenum2;
					cout << "��������������: " << endl;
					cin >> comment;
					cout << "����������Ȧ��Ϣ: " << endl;
					cin >> moments;
					user.Comment_Moments(manager, phonenum2, comment, moments);
					break;
				}
				case 9: // �޸��ǳ�
				{
					string newnic;
					cout << "���������ǳƣ�" << endl;
					cin >> newnic;
					user.set_nickname(manager, newnic);

					break;
				}
				case 10: // �޸�����
				{
					string newpasswd;
					cout << "�����������룺" << endl;
					cin >> newpasswd;
					if (!user.testpwd(newpasswd))
					{
						cout << "���벻�Ϸ���" << endl;
						break;
					}
					cout << "���ٴ��������룺" << endl;
					string newpasswd1;
					cin >> newpasswd1;
					if (newpasswd1 != newpasswd || !user.testpwd(newpasswd1))
					{
						cout << "�����������." << endl;
						break;
					}
					user.set_password(manager, newpasswd1);

					break;
				}
				case 11: // ����Ȩ��
				{
					cout << "�����ã�" << endl;
					cout << "\t1 - �ֻ�������ǳ�\n";
					cout << "\t2 - �ر����з�ʽ\n";
					int num;
					cin >> num;
					SEARCH_USER_WAY choice = static_cast<enum SEARCH_USER_WAY>(num);
					user.set_search_way(manager, choice);
					break;
				}
				case 12: // ͨ����������ʽ
				{
					pair<string, string> question;
					cout << "�����ã�" << endl;
					cout << "\t0 - Ĭ��\n";
					cout << "\t1 - �ش�������֤\n";
					int num;
					cin >> num;
					ADD_USER_WAY choice = static_cast<enum ADD_USER_WAY>(num);
					if (choice == ADD_USER_WAY::QUESTION) 
					{
						cout << "���������⣺" << endl;
						cin >> question.first;
						cout << "������𰸣�" << endl;
						cin >> question.second;
					}
					user.set_add_way(manager, choice, question);
					break;
				}
				case 13: // �ǳ�
				{
					user.Logout(manager);
					break;
				}
				case 14: // �˳�ϵͳ........
				{
					cout << "�˳�ϵͳ..." << endl;
					return;
				}
				case 100: // ��ʾϵͳ��ǰ��ע���û�
				{
					manager.show_Users_Data();
					break;
				}
				default:
				{
					cout << "����������." << endl;
					break;
				}
				}
			}
		}
	}
}

int main()
{
	User user;
	Function(user);
	return 0;
}