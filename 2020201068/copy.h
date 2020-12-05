// /*#include"command.h"
// #ifndef VARIABLE_H
// #define VARIABLE_H
// #include "variable.h"
// #endif

 #ifndef COPY_H
 #define COPY_H


 #include "command.h"




void CopyFile(string from, string to)
{
    char c;
    FILE *FromFile;
    FILE *ToFile;
    if (((FromFile = fopen(from.c_str(), "r")) == NULL) || ((ToFile = fopen(to.c_str(), "w")) == NULL)) {
        perror("");
        return;
    }
    
    while (!feof(FromFile)) {
        c = getc(FromFile);
        putc(c, ToFile);
    }
    struct stat StatFile;
    stat(from.c_str(), &StatFile);
    chown(to.c_str(), StatFile.st_uid, StatFile.st_gid);
    chmod(to.c_str(), StatFile.st_mode);
    fclose(FromFile);
    fclose(ToFile);
    return;
}
string FileSplit(string str)
{
    size_t found;
    found = str.find_last_of("/\\");
    return str.substr(0, found);
}
string AbsoluteFile(string f1)
{
    string AbsFile = "";
    if (f1[0] == '/') {
        AbsFile = string(root) + f1;
    }
    else if (f1[0] == '.' && f1[1] == '/') {
        AbsFile =  f1.substr(1, f1.length()) + string(currentDir);
    }
    else if (f1[0] == '~') {
        f1 = f1.substr(1, f1.length());
        AbsFile = string(root) + f1;
    }
    else {
        AbsFile = string(currentDir) + "/" + f1;
    }
    return AbsFile;
}

void DeleteFile()
{
    if (Command.size() <= 1 )
        printf(" few arguments\n");
    else {
            string remove_path = AbsoluteFile(Command[1]);
            if (remove(remove_path.c_str()) != 0)
                perror("");
            else
                cout << "Deleted = " << Command[1] << endl;
    
    }
    return;
}


bool IsDirect(string file1)
{
    struct stat s1;
    if (stat(file1.c_str(), &s1) != 0) {
        perror(file1.c_str());
        return false;
    }
    if (S_ISDIR(s1.st_mode))
        return true;
    else
        return false;
}
/*============================================================
this function takes directory as path and create replica
of the folder in Destination folder.
=============================================================*/
void CopyDir(string from, string to)
{
    struct dirent* d;
    DIR* dp;
    dp = opendir(from.c_str());
    if (dp == NULL) {
        perror("opendir");
        return;
    }
    while ((d = readdir(dp))) {
        if (strcmp(d->d_name, ".") == 0 )
            continue;
        else if(strcmp(d->d_name, "..") == 0)
         continue;    
        else {
            string FromPath = from + "/" + string(d->d_name);
            string ToPath = to + "/" + string(d->d_name);
            if (IsDirect(FromPath)) {
                if (mkdir(ToPath.c_str(), 0755) == -1) {
                    perror("");
                    return;
                }
                else
                    CopyDir(FromPath, ToPath);
            }
            else {
                CopyFile(FromPath.c_str(), ToPath.c_str());
            }
        }
    }
    closedir(dp);
    return;
}
/*============================================================
this command performs copy on multiple arguments one by one
by calling respective CopyDir or CopyFile function.
eg : copy <f1> <f2> <d1> <f3> <Destination Folder>
=============================================================*/
int Copy()
{
    if (Command.size() <= 2)
        printf("less arguments\n");
    else {
        string DestFolder = AbsoluteFile(Command[Command.size() - 1]);
        if (!IsDirect(DestFolder)) {
            cout << "**Destination is not a folder**" << endl;
            return 1;
        }
        unsigned int i = 1;
        while(i < Command.size() - 1) {
            string FromPath = AbsoluteFile(Command[i]);
            size_t found = FromPath.find_last_of("/\\");
            string ToPath = DestFolder + "/" + FromPath.substr(found + 1, FromPath.length() - found);
            if (IsDirect(FromPath)) {
                if (mkdir(ToPath.c_str(), 0755) != 0) {
                    perror("");
                    return 1;
                }
                CopyDir(FromPath, ToPath);
            }
            else {
                CopyFile(FromPath, ToPath);
            }
            i++;
        }
    }
    return 0;
}

/*============================================================
deletes all directories at given list.
eg : DeleteDir <d1> <d2> <d3>
=============================================================*/
void DeleteDir()
{
    if (Command.size() != 2)
        printf("arguments are unequal\n");
    else {
            string d_path = AbsoluteFile(Command[1]);
            if (!IsDirect(d_path)) {
                cout << "Not a Directory" << endl;
            }
            delete_recursive(d_path);

        
    }
}


#endif