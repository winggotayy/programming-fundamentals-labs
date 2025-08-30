# programming-fundamentals-labs
Coursework projects for NJU Programming Fundamentals 程序设计基础实验  

---
# Project 1: Nanjing Railway Ticketing System

This project is implemented in **C/C++**, simulating a railway/metro ticketing system for Nanjing.  
It provides basic line and station queries, as well as fare calculation.

## Features
- **Line Query**: Input a line name to display all stations along the line  
- **Station Query**: Input a station name to list all lines passing through it  
- **Fare Calculation**: Calculate ticket fare based on departure and destination stations  
- **Direct Route Query**: Check whether two stations are on the same line and display intermediate stops

## Usage
1. Compile and run `main.cpp`  
2. Make sure `线路信息.txt` and other data files are placed in the same directory as the executable  
3. Follow the menu prompts to select a function and enter station/line names

---
# Project 2: Chat System (WeChat-like Program)

This project is implemented in **C/C++**, simulating a simplified **WeChat-like program** with a console interface. 
It focuses on practicing **file I/O, string processing, and basic data management**, rather than real-time network communication.

## Features
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

## Usage
1. Compile and run `main.cpp`  
2. Register a new account or log in with an existing one  
3. Use the menu to add friends, chat, or post/view moments  
4. All user data (accounts, messages, posts) are stored in local text files

---
