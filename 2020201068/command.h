#ifndef VARIABLE_H
#define VARIABLE_H
//#include "variable.h"




#include<iostream>
#include<bits/stdc++.h>
#include<dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include<string.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h> 
#include <errno.h>  
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <bits/stdc++.h>

using namespace std;
void CopyFile(string s1,string s2);
void CopyDir(string s1, string s2);
int Copy();
int ListData(const char* s);
void DisplayFile(const char* s);
string AbsoluteFile(string s1);
void Update();
bool IsDirect(string s1);
string FileSplit(string s1);
void EnterKey();
int CommandMode();
void SplitComm();
void CallFunction();
void ncanon();
void DeleteFile();
void DeleteDir();
void Rename();
void CreateFile();
void CreateDir();
void delete_recursive(string s);
void Search();
void SearchRecursion(string s1,string s2);
void Goto();
void Move();

#endif