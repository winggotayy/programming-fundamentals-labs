# programming-fundamentals-labs
Coursework projects for NJU Programming Fundamentals 程序设计基础实验  

## Contents
- Project 1: Nanjing Railway Ticketing System 南京地铁票务系统 
- Project 2: Console-based Chat System (WeChat-like Program) 聊天模拟程序
- Project 3: Complex Number Calculator 复数计算器
- Project 4: Knowledge Quiz Mini Program 知识竞赛小程序

## Documentation
Each project includes not only the **C/C++ source code**, but also the corresponding:  
- **Experiment Report (PDF)** – problem analysis, design process, implementation details, testing results  
- **Design Document (PDF)** – program structure diagrams, module descriptions, workflow explanations  

所有项目均包含 **源码** 与配套的 **实验报告 / 设计文档**，完整展示了从需求分析 → 程序设计 → 实现 → 测试的过程。  

Reports and documents can be found in the `report/` folder of each project.

---
# Project 1: Nanjing Railway Ticketing System

This project is implemented in **C/C++**, simulating a railway/metro ticketing system for Nanjing.  
It provides basic line and station queries, as well as fare calculation.

## ✨ Features
- **Line Query**: Input a line name to display all stations along the line  
- **Station Query**: Input a station name to list all lines passing through it  
- **Fare Calculation**: Calculate ticket fare based on departure and destination stations  
- **Direct Route Query**: Check whether two stations are on the same line and display intermediate stops

## 🚀 Usage
1. Compile and run `main.cpp`  
2. Make sure `线路信息.txt` and other data files are placed in the same directory as the executable  
3. Follow the menu prompts to select a function and enter station/line names

---
# Project 2: Chat System (WeChat-like Program)

This project is implemented in **C/C++**, simulating a simplified **WeChat-like program** with a console interface. 
It focuses on practicing **file I/O, string processing, and basic data management**, rather than real-time network communication.

## ✨ Features
- **User Registration & Login**  
  - Phone number and password validation (regex checks)  
  - CAPTCHA verification for enhanced security  

- **Friend Management**  
  - Add friends with verification requests  
  - Manage friend list and search permissions  

- **Messaging**  
  - Send and receive text messages (saved in local files)  
  - View chat history with friends  

- **Moments (Friend Circle)**  
  - Post text-based moments  
  - Set visibility (e.g., friends only, only self)  
  - Support likes and comments  

- **User Settings**  
  - Modify nickname and password  
  - Adjust privacy and search options  

- **System Functions**  
  - Logout / Exit  
  - Admin mode: view all registered users (for testing)

## 🚀 Usage
1. Compile and run `main.cpp`  
2. Register a new account or log in with an existing one  
3. Use the menu to add friends, chat, or post/view moments  
4. All user data (accounts, messages, posts) are stored in local text files

---
# Project 3: Complex Expression Calculator

This project is implemented in **C/C++** and simulates a console-based **complex number expression calculator**.  
It supports parsing and evaluating complex expressions, error detection, and operation history recording.

## ✨ Features
- **Expression Parsing**  
  - Parses infix expressions (with operators, parentheses, real and imaginary numbers)  
  - Supports conversion to **Reverse Polish Notation (RPN)** for evaluation  

- **Complex Number Operations**  
  - Arithmetic: addition, subtraction, multiplication, division, power  
  - Other: conjugate, modulus, argument

- **Error Detection**  
  - Identifies invalid expressions (illegal operator/operand sequences, unmatched parentheses, etc.)  
  - Displays error type and highlights the error position in **red** in the console  

- **History Recording**  
  - Stores valid expression results in `history.txt` for review  

- **Precision Control**  
  - Results are formatted with up to **6 decimal places**  

### 🛠 Technical Design
- **Complex Class**: Encapsulates real and imaginary parts, with operator overloading for arithmetic  
- **Stack-based Evaluation**: Uses two stacks (`oprStack`, `numStack`) to process operators and operands  
- **Custom Error Handling**: `Check` function validates input expressions, `errorPrint` outputs diagnostics  
- **Parsing Utilities**: `Parse` handles whitespace removal and operator normalization (`arg → a`, `cjg → c`, `mod → m`)  

## 🚀 Usage
1. Compile and run `main.cpp`  
2. Input a complex expression (e.g., `(3+2i) * (1-i)`)  
3. The program will validate, evaluate, and display the result  
4. If the expression is invalid, an error message with position highlight will be shown  
5. Valid results are appended to `history.txt`  

---
# Project 4: Knowledge Quiz Mini Program

This project is implemented in **C/C++** using the **EasyX graphics library**, simulating a **knowledge quiz application**.
It demonstrates integration of graphical interfaces with data management systems, featuring user authentication, randomized content selection, and performance tracking.

## ✨ Features
- **User Registration & Login**
  - Unique username and password validation
  - Data stored in `users.txt`

- **Multiple Quiz Modes**
  - **Practice Mode**: Randomly select a question for practice with instant feedback
  - **Mock Competition**: Timed quiz with random questions and result display
  - **Formal Competition**: Scored competition with ranking system

- **Leaderboard**
  - View rankings from formal competitions based on score and time

- **Question Bank Management**
  - Structured storage of questions with content, options, and answers
  - Random question selection algorithm

### 🛠 Technical Design
- **EasyX Graphics**: Provides graphical user interface for all interactions
- **User Management**: User struct with username/password storage in vector container
- **Question System**: Question struct with content, options, and answer fields
- **UI Components**: Custom EasyTextBox and EasyButton classes for input and interaction
- **File I/O**: User data and question bank stored in external files (`user.txt`, `rank.txt`)

## 🚀 Usage
1. Compile and run the main program (ensure EasyX library is installed)
2. Register a new account or login with existing credentials
3. Select from practice, mock competition, or formal competition modes
4. Answer questions by clicking on options
5. View results and rankings after competitions

---
