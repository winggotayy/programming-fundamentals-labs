#pragma once

#include <iostream>
#include <cmath>
#include <stack>
#include <string>
#include <iomanip>
#include <sstream>

using namespace std;

class Complex
{
public:
	// 构造函数（Double类型）
	Complex(double r = 0.0, double i = 0.0) : real(r), image(i) {}

	// 加法运算符重载
	Complex operator+(Complex& c);
	// 减法运算符重载
	Complex operator-(Complex& c);
	// 乘法运算符重载
	Complex operator*(Complex& c); 
	// 除法运算符重载
	Complex operator/(Complex& c);
	// n次幂 Z^n
	// 计算复数的n次幂
	Complex operator^(int n);

	// 重载输出运算符，使程序可以直接输出一个复数类
	friend ostream& operator<<(ostream& os, Complex& c);

	// 取模 ||
	// Gives the magnitude of the two numbers
	double mod();

	// 共轭 cjg(Z)
	// Create the complex conjugate
	Complex cjg();

	// 辐角主值 arg(Z)
	// Gives the angle between real and image
	double arg();

	// 运算符优先级
	int getPriority(char c);

	// 判断运算符是否为操作数
	bool isDigit(char c);
	// 判断运算符是否为操作符
	bool isOperator(char op);
	
	// 运算
	Complex Calculate(Complex c1, Complex c2, char op);
	// 逆波兰表达式运算
	Complex RPN(string exp);
	
private:
	double real;
	double image;
};
