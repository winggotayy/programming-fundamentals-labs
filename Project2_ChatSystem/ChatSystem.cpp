#include "User.h"

void Menu()
{
	cout << "=========聊天模拟程序=========" << endl;
	cout << " " << endl;
	cout << "--- 好友菜单 ---" << endl;
	cout << "1. 添加好友" << endl;
	cout << "2. 好友列表" << endl;
	cout << " " << endl;
	cout << "-- 留言 --" << endl;
	cout << "3. 给好友留言" << endl;
	cout << "4. 查看聊天记录" << endl;
	cout << " " << endl;
	cout << "-- 朋友圈 --" << endl;
	cout << "5. 发布朋友圈" << endl;
	cout << "6. 查看朋友圈" << endl;
	cout << "7. 点赞朋友圈" << endl;
	cout << "8. 评论朋友圈" << endl;
	cout << " " << endl;
	cout << "-- 用户设置 --" << endl;
	cout << "9. 修改昵称" << endl;
	cout << "10. 修改密码" << endl;
	cout << "11. 搜索权限" << endl;
	cout << "12. 通过好友请求方式" << endl;
	cout << " " << endl;
	cout << "-- 其他 --" << endl;
	cout << "13. 登出" << endl;
	cout << "14. 退出系统........" << endl;
	cout << " " << endl;
	cout << "=============================" << endl;
}

bool Register_or_Login(User& user, Server& manager) 
{
	int action = 2;
	while (action > 1) 
	{
		cout << "注册 - 0\n登录 - 1\n";
		cin >> action;
		if (action == 0) 
		{
			if (user.Register(manager)) 
			{
				cout << "\t注册成功！\n";
				return true;
			}
		}
		else if (action == 1) 
		{
			if (user.Login(manager))
			{
				cout << "\t登录成功！\n";
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
				cout << "\n\t欢迎来到 >>>" /*user.getUserData()->nickname*/ << endl;
			else
				cout << "\t请重新登录/注册.\n";
		}
		else
		{
			int func = -1;
			if (1)
			{
				Menu();
				cout << "请输入您想要操作的编号: ";
				cin >> func;
				switch (func)
				{
				case 1: // 添加好友
				{
					int search_way;
					cout << "请输入搜索的方式：" << endl;
					cout << "0 - 手机号码\n1 - 昵称\n";
					cin >> search_way;
					if (search_way != 1 && search_way != 0) 
					{
						cout << "请输入 0 或者 1" << endl;
						break;
					}
					cout << "请输入想添加的用户的手机号码/昵称： ";
					string identifier;
					cin >> identifier;
					UserData otheruserdata = manager.getUserData(identifier, static_cast<SEARCH_USER_WAY>(search_way));
					if (otheruserdata.search_way == SEARCH_USER_WAY::CLOSE)
					{
						cout << "无法查询此用户." << endl;
					}
					else 
					{
						if (otheruserdata.add_way == ADD_USER_WAY::QUESTION) 
						{
							string answer;
							cout << "问题：\n" << otheruserdata.question.first << endl;
							cout << "请输入答案" << endl;
							cin >> answer;
							if (answer == otheruserdata.question.second) 
							{
								cout << "正确!\n";
							}
							else 
							{
								cout << "不正确!\n";
								cout << "添加好友失败." << endl;
								break;
							}
						}
						if (user.Add_Friend(manager, identifier, static_cast<SEARCH_USER_WAY>(search_way)))
						{
							cout << "添加好友成功." << endl;
							break;
						}	
						else
						{
							cout << "添加好友失败." << endl;
							break;
						}
					}
					
					break;
				}
				case 2: // 好友列表
				{
					cout << "=========好友列表=======" << endl;
					user.Friends_List(manager);
					cout << "=======================" << endl;
					break;
				}
				case 3: // 给好友留言
				{
					string phone, msg;
					cout << "请输入好友手机号码和留言信息: " << endl;
					cin >> phone >> msg;
					user.Send_Message(manager, phone, msg);
					break;
				}
				case 4: // 查看聊天记录
				{
					vector<string> inboxes;
					string phone, inbox;

					cout << "请输入好友手机号码: ";
					cin >> phone;
					user.Search_Message_friend(manager, phone,inboxes);

					cout << "=========好友留言=======" << endl;
					for (const auto& x : inboxes)
					{
						cout << x << endl;
					}
					cout << "=======================" << endl;
					break;
				}
				case 5: // 发布朋友圈
				{
					string moment_content;
					bool privacy;
					cout << "请输入朋友圈内容: ";
					cin >> moment_content;
					cout << "展示范围：\n";
					cout << "0 - 好友可见\n1 - 仅自己可见\n";
					cin >> privacy;
					user.Post_Moments(manager, moment_content, privacy);
					break;
				}
				case 6: // 查看朋友圈
				{
					cout << "==========朋友圈信息=========" << endl;
					cout << " " << endl;
					user.Checkout_Moments(manager);
					cout << " " << endl;
					cout << "==============================" << endl;
					break;
				}
				case 7: // 点赞朋友圈
				{
					string phonenum2, moments;
					cout << "请输入好友手机号码: " << endl;
					cin >> phonenum2;
					cout << "请输入朋友圈信息: " << endl;
					cin >> moments;
					user.Like_Moments(manager, phonenum2, moments);
					break;
				}
				case 8: // 评论朋友圈
				{
					string phonenum2, comment, moments;
					cout << "请输入好友手机号码: " << endl;
					cin >> phonenum2;
					cout << "请输入评论内容: " << endl;
					cin >> comment;
					cout << "请输入朋友圈信息: " << endl;
					cin >> moments;
					user.Comment_Moments(manager, phonenum2, comment, moments);
					break;
				}
				case 9: // 修改昵称
				{
					string newnic;
					cout << "请输入新昵称：" << endl;
					cin >> newnic;
					user.set_nickname(manager, newnic);

					break;
				}
				case 10: // 修改密码
				{
					string newpasswd;
					cout << "请输入新密码：" << endl;
					cin >> newpasswd;
					if (!user.testpwd(newpasswd))
					{
						cout << "密码不合法！" << endl;
						break;
					}
					cout << "请再次输入密码：" << endl;
					string newpasswd1;
					cin >> newpasswd1;
					if (newpasswd1 != newpasswd || !user.testpwd(newpasswd1))
					{
						cout << "密码输入错误." << endl;
						break;
					}
					user.set_password(manager, newpasswd1);

					break;
				}
				case 11: // 搜索权限
				{
					cout << "请设置：" << endl;
					cout << "\t1 - 手机号码和昵称\n";
					cout << "\t2 - 关闭所有方式\n";
					int num;
					cin >> num;
					SEARCH_USER_WAY choice = static_cast<enum SEARCH_USER_WAY>(num);
					user.set_search_way(manager, choice);
					break;
				}
				case 12: // 通过好友请求方式
				{
					pair<string, string> question;
					cout << "请设置：" << endl;
					cout << "\t0 - 默认\n";
					cout << "\t1 - 回答问题验证\n";
					int num;
					cin >> num;
					ADD_USER_WAY choice = static_cast<enum ADD_USER_WAY>(num);
					if (choice == ADD_USER_WAY::QUESTION) 
					{
						cout << "请输入问题：" << endl;
						cin >> question.first;
						cout << "请输入答案：" << endl;
						cin >> question.second;
					}
					user.set_add_way(manager, choice, question);
					break;
				}
				case 13: // 登出
				{
					user.Logout(manager);
					break;
				}
				case 14: // 退出系统........
				{
					cout << "退出系统..." << endl;
					return;
				}
				case 100: // 显示系统当前已注册用户
				{
					manager.show_Users_Data();
					break;
				}
				default:
				{
					cout << "请重新输入." << endl;
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