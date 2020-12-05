

#include"variable.h"
#include"command.h"
#include"copy.h"
#include"move.h"
#include"search.h"

struct termios raw, newraw;
struct winsize Term;

void SplitComm()
{
    string tmp = "";
    for (unsigned int i = 0; i < CommString.size(); i++) {
        if (CommString[i] == '\n ' || CommString[i] == ' ' ) {
            if(tmp.size() > 0) Command.push_back(tmp);
            tmp = "";
        }
        else if (CommString[i] == '\\')
            tmp =tmp + CommString[++i];
        else
            tmp = tmp + CommString[i];
    }
    return;
}



void RefreshCursor()
{
    curser_x = 1;
    //curser_y = 1;
    cur_window = 0;
    printf("%c[%d;%dH",27,curser_x,curser_y) ;
}
void CallFunction()
{
    string s = Command[0];
    if (s == "copy")
        Copy();
    else if (s == "DeleteFile")
        DeleteFile();    
    else if (s == "move")
        Move();
    else if (s == "CreateDir")
        CreateDir();    
    else if (s == "rename")
        Rename();
    else if (s == "CreateFile")
        CreateFile();
    else if (s == "goto")
        Goto();
    else if (s == "search")
        Search();
    else if (s == "DeleteDir")
        DeleteDir();
    
    return;
}
int CommandMode()
{
    while (1) {
        goto_flag = false;
        CommString.clear();
        
        search_flag_c = false;
        curser_x =Term.ws_row;
        curser_y = 1;
        Command.clear();
         printf("%c[%d;%dH",27,curser_x,curser_y) ;
        printf("\x1b[0K");
        printf(":");
        fflush(0);
        curser_y=curser_y+1;
        char ch[3] = { 0 };
       
        while (1) {
            if (read(STDIN_FILENO, ch, 3) == 0)
                continue;
            if (ch[0] == 27 && ch[1] == 0 && ch[2] == 0) {
                curser_y = 1;
                curser_x = 1;
                
                printf("%c[%d;%dH",27,curser_x,curser_y) ;
                return 0;
            }
            else if (ch[0] == 10) {
                CommString.push_back('\n');
                SplitComm();
                CallFunction();
                if (goto_flag)
                    return 1;
                if (search_flag_c)
                    return 2;
                break;
            }
            else if (ch[0] == 27 && ch[1] == '[' && (ch[2] == 'C' || ch[2] == 'D' || ch[2] == 'A' || ch[2] == 'B')) {
                continue;
            }
            else if (ch[0] == 127) {
                if (curser_y > 2) {
                    curser_y--;
                     printf("%c[%d;%dH",27,curser_x,curser_y) ;
                    printf("\x1b[0K");
                    CommString.pop_back();
                }
            }
            else {
                 cout << ch[0];
                 curser_y++;
                  printf("%c[%d;%dH",27,curser_x,curser_y);
                 CommString.push_back(ch[0]);
            }
            fflush(0);
            memset(ch, 0, 3 * sizeof(ch[0]));
        }
    }
    return 0;
}
void ncanon()
{
    tcgetattr(STDIN_FILENO, &raw);
    newraw = raw;
   
    newraw.c_lflag &= ~(ICANON | ECHO | IEXTEN | ISIG);
    newraw.c_iflag &= ~(BRKINT);

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &newraw) != 0)
        fprintf(stderr, "Cant be done\n");
    else {
        char ch[3] = { 0 };
        while (1) {
         
            printf("%c[%d;%dH",27,curser_x,curser_y) ;
            fflush(0);
            if (read(STDIN_FILENO, ch, 3) == 0)
                continue;
            else if (ch[0] == 27 && ch[1] == '[' && ch[2] == 'B'){    //downarrow
                if (curser_x <= TermRow && curser_x < DataFile.size()) {
                    curser_x++;
                    printf("%c[%d;%dH",27,curser_x,curser_y) ;
                 }
            }    
            else if (ch[0] == 27 && ch[1] == '[' && ch[2] == 'A'){    //uparrow
                    if (curser_x > 1) {
                        curser_x--;
                        printf("%c[%d;%dH",27,curser_x,curser_y) ;
                    }
                }

            else if (ch[0] == 127){
                 if ((strcmp(currentDir, root)) != 0) {             //backspace key
                    string s1Name = FileSplit(string(currentDir));
                    strcpy(currentDir, s1Name.c_str());
                    BackArray.push_back(currentDir);
                    ListData(currentDir);
                }   
             }
             else if (ch[0] == 10)
                EnterKey();
             else if (ch[0] == 27 && ch[1] == '[' && ch[2] == 'D'){   //left arrow
                if (BackArray.size() > 1) {
                    string p = BackArray.back();
                    BackArray.pop_back();
                    ForwardArray.push_back(p);
                    p = BackArray.back();
                    strcpy(currentDir, p.c_str());
                    ListData(currentDir);
                }
                else if (BackArray.size() == 1) {
                    string p = BackArray.back();
                    strcpy(currentDir, p.c_str());
                    ListData(currentDir);
                }
             }
               //LeftArrow();
             else if (ch[0] == 27 && ch[1] == '[' && ch[2] == 'C'){   //right arrow
                if (!ForwardArray.empty()) {
                    string p = ForwardArray.back();
                    ForwardArray.pop_back();
                    strcpy(currentDir, p.c_str());
                    BackArray.push_back(currentDir);
                    ListData(currentDir);
                }
             }

             else if (ch[0] == 'H' || ch[0] == 'h'){  //home key
                 RefreshCursor();
                 strcpy(currentDir, root);
                if( strcmp(currentDir,root) != 0 )BackArray.push_back(currentDir);
                    ListData(currentDir);
                }
            else if(ch[0]==':'){
                int ret=CommandMode();
                curser_x=1;
                if(ret==2){
                    search_flag=true;
               }
                else
                {
                    ListData(currentDir);
               }
                
            } 
              
            else if (ch[0] == 'q') {
                write(STDOUT_FILENO, "\x1b[2J", 4);
                tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
                curser_x = 1;
                curser_y = 1;
                printf("%c[%d;%dH",27,curser_x,curser_y) ;
                exit(1);
            }
            fflush(0);
            memset(ch, 0, 3 * sizeof(ch[0]));
        }
    }
}



void EnterKey(){
 
     if (DataFile[cur_window + curser_x - 1] == parent) {
        // if selected directory is '..' .
        string s1Name = currentDir;
        s1Name = FileSplit(s1Name);
        strcpy(currentDir, s1Name.c_str());
        BackArray.push_back(currentDir);
      
        ListData(currentDir);
    }
    else {
        
        char* p_file1;
        char* f_file1;
        
            string cur_d = "/" + DataFile[cur_window + curser_x - 1];
            printf("%c[%d;%dH",27,curser_x,curser_y) ;
            p_file1 = currentDir;
            f_file1 = new char[cur_d.length() + strlen(p_file1) + 1];
            strcpy(f_file1, p_file1);
            strcat(f_file1, cur_d.c_str());
            strcpy(currentDir, f_file1);
       
        BackArray.push_back(currentDir);
        if (IsDirect(f_file1)) {
            RefreshCursor();
            ListData(currentDir);
        }
        else {
            BackArray.pop_back();
            string top = BackArray.back();
            strcpy(currentDir, top.c_str());
            pid_t pid = fork();
            if (pid == 0) {
                close(2);
                execlp("xdg-open", "xdg-open", f_file1, NULL);
                exit(0);
            }
        }
    }
}

int ListData(const char* file1)
{
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &Term);
    TermRow = Term.ws_row - 2;
    TermColumn = Term.ws_col;
    write(STDOUT_FILENO, "\x1b[2J", 4); //to clear screen
    struct dirent* d;
    DIR* dir1;
    dir1 = opendir(file1);
    if (dir1 == NULL) {
        return -1;
    }
    DataFile.clear();
    while ((d = readdir(dir1))) {
        if (strcmp(file1, root) == 0) {
            strcpy(currentDir, root);
            if (strcmp(d->d_name, "..") == 0)
                continue;
        }
        DataFile.push_back(d->d_name);
    }
    sort(DataFile.begin(), DataFile.end());
    Update();
    curser_x = 1;
    //curser_y = 1;
    printf("%c[%d;%dH",27,curser_x,curser_y) ;
    RefreshCursor();
    closedir(dir1);
    return 0;
}


void Update()
{
    write(STDOUT_FILENO, "\x1b[2J", 4); //to clear screen
    curser_y = 1;
    printf("%c[%d;%dH",27,curser_x,curser_y) ;
    int vecsize = DataFile.size();
    int from = cur_window;
    int to;
    if ((unsigned)vecsize <= TermRow)
        to = vecsize - 1;
    else
        to = TermRow + cur_window;
        int i=from;
    while( i <= to) {
        string t = DataFile[i];
        DisplayFile(t.c_str());
        i++;
    }
    return;
}


void DisplayFile(const char* dirName)
{
    curser_y = 0;
    struct stat stab;
    string f1=dirName;
    string AbsFile = "";
    if (f1[0] == '/') {
        AbsFile = string(root) + f1;
    }
    else if (f1[0] == '~') {
        f1 = f1.substr(1, f1.length());
        AbsFile = string(root) + f1;
    }
    else if (f1[0] == '.' && f1[1] == '/') {
        AbsFile =  f1.substr(1, f1.length()) + string(currentDir);
    }
    else {
        AbsFile = string(currentDir) + "/" + f1;
    }
    stat(AbsFile.c_str(), &stab);
    printf((stab.st_mode & S_IRUSR) ? "r" : "-");
    printf((stab.st_mode & S_IWUSR) ? "w" : "-");
    printf((stab.st_mode & S_IXUSR) ? "x" : "-");
    printf((stab.st_mode & S_IRGRP) ? "r" : "-");
    printf((stab.st_mode & S_IWGRP) ? "w" : "-");
    printf((stab.st_mode & S_IXGRP) ? "x" : "-");
    printf((stab.st_mode & S_IROTH) ? "r" : "-");
    printf((stab.st_mode & S_IWOTH) ? "w" : "-");
    printf((stab.st_mode & S_IXOTH) ? "x" : "-");
    curser_y =curser_y + 10;

    struct passwd* user_pass;
    user_pass = getpwuid(stab.st_uid);
    string uname = user_pass->pw_name;
    curser_y =curser_y+ printf(" %10s ", uname.c_str());

    struct group* grp;
    grp = getgrgid(stab.st_gid);
    string gname = grp->gr_name;
    curser_y =curser_y + printf(" %10s ", gname.c_str());

    long long x = stab.st_size;
    if (x >= (1 << 30))
        curser_y = curser_y + printf("%4lldG ", x / (1 << 30));
    else if (x >= (1 << 20))
        curser_y = curser_y + printf("%4lldM ", x / (1 << 20));
    else if (x >= (1 << 10))
        curser_y =curser_y + printf("%4lldK ", x / (1 << 10));
    else
        curser_y =curser_y + printf("%4lldB ", x);

    string m_time = string(ctime(&stab.st_mtime));
    m_time = m_time.substr(4, 12);
    curser_y =curser_y + printf(" %-12s ", m_time.c_str());

    if (IsDirect(AbsFile))
        printf("%c[33m", 27);
    printf(" %-20s\n", dirName);
    if (IsDirect(AbsFile))
        printf("%c[0m", 27);
    curser_y=curser_y+1;
}
int main(int argc, char** argv)
{
    ArgNo = argc;
    ArgValue = argv;
    if (ArgNo == 2)
        strcpy(root, ArgValue[1]);// setting root as default file1.
      
    else if (ArgNo == 1) {
         getcwd(root, sizeof(root));  //set root as current working dir.
    }
    else {
        cout << " many arguments\n" << endl;
        exit(0);
    }
    strcat(currentDir, root);
    BackArray.push_back(root); //BackArray saves history of visited dir.
    printf("%c[?1049h",27); //Alternate buffer on
    ListData(root); //list directories inside root.
    ncanon(); //enable non-canonical mode.
    return 0;
}