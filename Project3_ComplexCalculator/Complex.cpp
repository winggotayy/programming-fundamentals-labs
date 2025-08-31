#include "Complex.h"

const double PI = 3.14159265358979323846;
const double EPS = 1e-12;

// 加法运算符重载
Complex Complex::operator+(Complex& c)
{
	double x = real + c.real;
	double y = image + c.image;
	return Complex(x, y);
}
// 减法运算符重载
Complex Complex::operator-(Complex& c)
{
	double x = real - c.real;
	double y = image - c.image;
	return Complex(x, y);
}
// 乘法运算符重载
Complex Complex::operator*(Complex& c)
{
	double x = real * c.real - image * c.image;
	double y = real * c.image + image * c.real;
	return Complex(x, y);
}
// 除法运算符重载
Complex Complex::operator/(Complex& c)
{
	double r= c.real * c.real + c.image * c.image;
	double x = (real * c.real + image * c.image) / r;
	double y = (image * c.real - real * c.image) / r;
	return Complex(x, y);
}
// n次幂 Z^n
// 计算复数的n次幂
Complex Complex::operator^(int n)
{
	double mag = mod();
	double angle = arg();

	double x = pow(mag, n) * cos(angle * n);
	double y = pow(mag, n) * sin(angle * n);

	return Complex(x, y);
}

// 取模 ||
// Gives the magnitude of the two numbers
double Complex::mod()
{
	double mag = sqrt(pow(real, 2) + pow(image, 2));
	return mag;
}

// 共轭 cjg(Z)
// Create the complex conjugate
Complex Complex::cjg() 
{
	double x = real;
	double y = -image;
	return Complex(x, y);
}

// 辐角主值 arg(Z)
// Gives the angle between real and image
double Complex::arg()
{

	double angle = atan2(image, real);
	if (angle > PI)
		angle -= 2 * PI;
	return angle;
/*
	if (image == 0 && real == 0)
		return 0;
	else if (image >= 0 && real > 0)
		return atan(image / real);
	else if (real == 0 && image > 0)
		return acos(0.0);
	else if (real < 0 && image > 0)
		return acos(-1.0) - atan(image / real);
	else if (real < 0 && image == 0)
		return acos(-1.0);
	else if (real < 0 && image < 0)
		return atan(image / real) - acos(-1.0);
	else if (real == 0 && image < 0)
		return -acos(0.0);
	else if (real > 0 && image < 0)
		return atan(image / real);
*/
}

// 重载输出运算符，使程序可以直接输出一个复数类
ostream& operator<<(ostream& os, Complex& c)
{/*
	os << "结果为：";
	if (c.image == 0)
		os << fixed << setprecision(6) << c.real;
	else if (c.real == 0)
		os << c.image << "i";
	else if (c.image > 0 && c.real != 0)
		os << c.real << "+" << c.image << "i";
	else
		os << c.real << c.image << "i";
	return os;
*/
	if (fabs(c.real - 0) < EPS)	// 检查实部是否接近于零
	{
		if (fabs(c.image - 0) > EPS) // 检查虚部是否接近于零 
		{
			os << fixed << setprecision(6) << c.image;
			os << "i";
		}
		else 
		{
			os << "0";
		}
	}
	else 
	{
		os << fixed << setprecision(6) << c.real;	
		if (c.image > 0) 
		{
			os << "+";
		}
		if (fabs(c.image - 0) > EPS) // 检查虚部是否接近于零
		{
			os << fixed << setprecision(6) << c.image;
			os << "i";
		}
	}

	return os;
}

// 运算符优先级
int Complex::getPriority(char opr) 
{
	if (opr == '(' || opr == ')') 
		return 1;
	else if ((opr == 'm' || opr == 'c' || opr == 'a'))
		return 2;
	else if (opr == '^')
		return 3;
	else if (opr == '*' || opr == '/')
		return 4;
	else if (opr == '+' || opr == '-') 
		return 5;
	else 
		return -1;
}

// 判断运算符是否为操作数
bool Complex::isDigit(char c)
{
	return (c >= '0' && c <= '9');
}
// 判断运算符是否为操作符
bool Complex::isOperator(char opr) 
{
	if (opr == '+' || opr == '-' || opr == '*' || opr == '/' || opr == '^' 
		|| opr == 'm' || opr == 'c' || opr == 'a' 
		|| opr == '(' || opr == ')')
		return 1;
	else 
		return 0;
}

// 运算
Complex Complex::Calculate(Complex c1, Complex c2, char opr)
{
	if (opr == '+')
	{
		return c1.operator+(c2);
	}
	else if (opr == '-')
	{
		return c1.operator-(c2);
	}
	else if (opr == '*')
	{
		return c1.operator*(c2);
	}
	else if (opr == '/')
	{
		return c1.operator/(c2);
	}
	else if (opr == '^')
	{
		return c1.operator^(static_cast<int>(c2.real));
	}
	else if (opr == 'm')
	{
		return Complex(c1.mod(), 0);
	}
	else if (opr == 'c')
	{
		return c1.cjg();
	}
	else if (opr == 'a')
	{
		return Complex(c1.arg(), 0);
	}
	else
	{
		return Complex();
	}
}

// 逆波兰表达式运算
Complex Complex::RPN(string exp)
{
	stack<char> oprStack;		// 操作符栈
	stack<Complex> numStack;	// 操作数栈

	for (int i = 0; i < exp.length(); i++)
	{
	/* 操作数 */
		if (isDigit(exp[i]))
		{
			string dec_num;
			while (isDigit(exp[i]) || exp[i] == '.')
			{
				// 连续读取数字字符，包括小数点
				dec_num += exp[i];
				i++;
			}
			double d;
			istringstream(dec_num) >> d; // 将字符串转换为 double 类型的数字

			if (i < exp.length())
			{
				if (exp[i] != 'i') // 如果不是虚数单位字符 'i'
				{
					numStack.push(Complex(d, 0)); // 将实部入栈
					i--;
					continue;
				}
				else if (exp[i] == 'i') // 如果是虚数单位字符 'i'
				{
					if ((i < exp.length() - 1) && (exp[i + 1] == '^')) // 如果下一个字符是 '^'
					{
						numStack.push(Complex(d, 0)); // 将实部入栈
						i--;
						continue;
					}
					else
					{
						numStack.push(Complex(0, d)); // 将虚部入栈
						continue;
					}
				}
			}
			else // 表达式的末尾  
			{
				numStack.push(Complex(d, 0)); // 将实部入栈 
				i--;
				continue;
			}
		}
		/* 处理虚数单位字符 'i' */
		if (i < exp.length() && i > 0 && exp[i] == 'i')
		{
			if (!isDigit(exp[i - 1]))
			{
				numStack.push(Complex(0, 1));
				continue;
			}
			else if (isDigit(exp[i - 1]) && exp[i + 1] == '^')
			{
				numStack.push(Complex(0, 1));
				continue;
			}

		}
		/* 处理虚数单位字符 'i' */
		if (i == 0 && exp[i] == 'i')
		{
			numStack.push(Complex(0, 1));
		}

	/* 操作符 */
	/* 注意：优先级——小于即高于 */
		if (isOperator(exp[i]))
		{
			// 处理：oprStack为空；操作符为 'arg'，'cjg'，'mod'（优先级为2）；操作符为 '('  
			if ((oprStack.empty() && exp[i] != '(') || getPriority(exp[i]) == 2 || exp[i] == '(') // getPriority = 2（a, c, m）
			{
				oprStack.push(exp[i]);
				continue;
			}

			// 当前操作符优先级高于栈顶操作符：当前不是 ')'
			else if (!oprStack.empty() && exp[i] != ')' && getPriority(exp[i]) <= getPriority(oprStack.top()))
			{
				oprStack.push(exp[i]);
				continue;
			}
			// 当前操作符优先级低于栈顶操作符：下一个不是 '('，栈顶不是 '('
			else if (!oprStack.empty() && getPriority(exp[i]) > getPriority(oprStack.top()) && exp[i + 1] != '(' && oprStack.top() != '(')
			{
				while (!oprStack.empty() && getPriority(exp[i]) > getPriority(oprStack.top()) && oprStack.top() != '(')
				{
					Complex c2 = numStack.top();
					numStack.pop();
					Complex c1 = numStack.top();
					numStack.pop();
					Complex res = Calculate(c1, c2, oprStack.top());
					numStack.push(res);
					oprStack.pop();
				}
				if (oprStack.empty())
				{
					oprStack.push(exp[i]);
					continue;
				}
			}
			// 当前操作符优先级低于栈顶操作符：当前不是 ')'，下一个不是 '('，栈顶是 '('，
			else if (!oprStack.empty() && getPriority(exp[i]) > getPriority(oprStack.top()) && exp[i] != ')' && exp[i + 1] != '(' && oprStack.top() == '(')
			{
				oprStack.push(exp[i]);
				continue;
			}

			// 当前不是 '('，当前不是 ')'，下一个是 '('
			else if (i < exp.size() - 1 && exp[i] != '(' && exp[i] != ')' && exp[i + 1] == '(')
				oprStack.push(exp[i]);

			// 当前是 ')'
			else if (exp[i] == ')')
			{
				// 操作符栈非空，并且栈顶操作符的优先级大于2,
				if (!oprStack.empty() && getPriority(oprStack.top()) > 2)
				{
					if (!oprStack.empty() && oprStack.top() != '(')
					{
						// 则从操作数栈中取出两个操作数和栈顶操作符进行运算，
						Complex c2 = numStack.top();
						numStack.pop();
						Complex c1 = numStack.top();
						numStack.pop();
						Complex res = Calculate(c1, c2, oprStack.top());
						numStack.push(res); // 将结果入栈 numStack。
						oprStack.pop();
					}
				}
				// 操作符栈非空，并且栈顶操作符是 '('，
				if (!oprStack.empty() && oprStack.top() == '(')
				{
					oprStack.pop(); // 则将其弹出，
					// 并继续判断栈顶操作符的优先级。
					if (!oprStack.empty() && getPriority(oprStack.top()) != 2)
					{
						oprStack.push('(');
						continue;
					}
					if (!oprStack.empty() && getPriority(oprStack.top()) == 2)
					{
						Complex c = numStack.top();
						numStack.pop();
						Complex res = Calculate(c, Complex(0, 1), oprStack.top());
						numStack.push(res);
						oprStack.pop();
						if (!oprStack.empty() && oprStack.top() == '(')
						{
							oprStack.pop();
							if (!oprStack.empty() && getPriority(oprStack.top()) == 2)
							{
								oprStack.push('(');
								continue;
							}
							else if (((oprStack.empty())) || (!oprStack.empty() && getPriority(oprStack.top()) != 2))
							{
								oprStack.push('(');
								continue;
							}
						}
					}
				}
			}
		}
	}

	/* 操作符栈中剩余的操作符 */
	while (!oprStack.empty() && numStack.size() >= 2)
	{
		// 从操作数栈中取出相应的操作数，进行运算
		if (getPriority(oprStack.top()) == 3 || getPriority(oprStack.top()) == 4 || getPriority(oprStack.top()) == 5)
		{
			Complex c2 = numStack.top();
			numStack.pop();
			Complex c1 = numStack.top();
			numStack.pop();
			Complex res = Calculate(c1, c2, oprStack.top());
			numStack.push(res);
			oprStack.pop();
		}
		else if (!oprStack.empty() && getPriority(oprStack.top()) == 2)
		{
			Complex c = numStack.top();
			numStack.pop();
			Complex res = Calculate(c, c, oprStack.top());
			numStack.push(res);
			oprStack.pop();
		}
		else if (!oprStack.empty() && oprStack.top() == '(')
			oprStack.pop();
	}

	return numStack.top();	// 将结果入栈 numStack
}
