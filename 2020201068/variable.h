#ifndef COMMAND_H
#define COMMAND_H
#include "command.h"


#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
using namespace std;
#include <bits/stdc++.h>
#include <stdio.h>
#include <dirent.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
string parent = "..";
string current = ".";
bool search_flag = false;
int ArgNo = 0;
char** ArgValue;
char root[4096];
char currentDir[4096];
std::vector<string> DataFile;
std::vector<string> BackArray;
std::vector<string> ForwardArray;
vector<string> Command;
std::vector<char> CommString;
unsigned int curser_x = 1;
unsigned int curser_y = 1;
unsigned int TermRow;
unsigned int TermColumn;
int cur_window = 0;
bool goto_flag, search_flag_c;


#endif
