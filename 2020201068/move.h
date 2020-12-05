#ifndef MOVE_H
#define MOVE_H

#include"copy.h"
#include"command.h"
#include"variable.h"

void Move()
{
    if (Copy() == 1)
        return;
    Command.pop_back();
    
        for (unsigned int i = 1; i < Command.size(); i++) {
            string DestPath = AbsoluteFile(Command[i]);
            if(IsDirect(DestPath))
                 delete_recursive(DestPath);
            else{     
                if (remove(DestPath.c_str()) == -1)
                    perror("");
            }
        }
    return;
}

void delete_recursive(string DestPath)
{
    DIR* dp;
    dp = opendir(DestPath.c_str());
    struct dirent* d;
    if (dp == NULL) {
        perror("opendir");
        return;
    }
    while ((d = readdir(dp))) {
        if (strcmp(d->d_name, ".") == 0)
            continue;
        else if(strcmp(d->d_name, "..") == 0)
             continue;    
        else {
            string r_path = DestPath + "/" + string(d->d_name);
            if (IsDirect(r_path)) {
                delete_recursive(r_path);
            }
            else {
                remove(r_path.c_str());
                cout<<"directory deleted"<<endl;
            }
        }
    }
    rmdir(DestPath.c_str());
    closedir(dp);
    return;
}

void Rename()
{
    if (Command.size() < 3 || Command.size() > 3)
        printf("Improper Arguments\n");
    else {
        string old = AbsoluteFile(Command[1]);
        string _new = AbsoluteFile(Command[2]);
        if (rename(old.c_str(), _new.c_str()) == -1)
            perror("");
        else
            cout << "Done" << endl;
    }
    return;
}


void CreateFile()
{
    if (Command.size()!= 3)
        return;
    else {
        string DestFolder = AbsoluteFile(Command[Command.size() - 1]);
        //verifies if destination is directory or not.
        if(!IsDirect(DestFolder)){
            cout << "Destination is not directory" << endl;
            return;
        }
        FILE* NewFile;
            for (unsigned int i = 1; i < Command.size() - 1; i++) {
            string DestPath = DestFolder + "/" + Command[i];
            NewFile = fopen(DestPath.c_str(), "w+");
            if (NewFile == NULL)
                perror("");
            else
                cout << "file created" << endl;
            fclose(NewFile);
            }
    }                    
    return;
}
void CreateDir()
{
    if (Command.size() < 3)
        printf("too few arguments\n");
    else {
        string DestFolder = AbsoluteFile(Command[Command.size() - 1]);
        //verifies if destination is directory or not.
        if (!IsDirect(DestFolder)) {
            cout << "Destination is not directory." << endl;
            return;
        }
        //for (unsigned int i = 1; i < Command.size() - 1; i++) {
            string DestPath = DestFolder + "/" + Command[1];
            if (mkdir(DestPath.c_str(), 0755) != 0)
                perror("");
            else
                cout << "directory created successfully." << endl;
        //}
    }
    return;
}


#endif