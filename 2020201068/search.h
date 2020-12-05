#ifndef SEARCH_H
#define SEARCH_H

#include"copy.h"
#include"command.h"
#include"variable.h"
vector<string> res;

void SearchRecursion(string path1, string name)
{
    //std::vector<string> res;
    DIR* dp;
    dp = opendir(path1.c_str());
    struct dirent* d;
    if (dp == NULL) {
        perror("opendir");
        return ;
    }
    while ((d = readdir(dp))){
      if (strcmp(d->d_name, ".") == 0)
           continue;
      else if(strcmp(d->d_name, "..") == 0)
           continue;     
        else {
            string path2 = path1 + "/" + string(d->d_name);
            if (strcmp(d->d_name, name.c_str()) == 0){
                res.push_back(path2);
           }
                //res.push_back(path2);
            if (IsDirect(path2))
               return  SearchRecursion(path2, name);
        }
    }
    return ;
}

void Search()
{
    if (Command.size() != 2){
        printf("uneual parameters");
        return;
    }
        
        BackArray.push_back(currentDir);
        string name = Command[1];
        
        SearchRecursion(currentDir, name);
        if (res.size() == 0) {
            cout << "False" << endl;
            return;
        }
        else
        {
             cout<<"True"<<endl;
        }
           
    return;
}
void Goto()
{
    if (Command.size() != 2)
        printf("Improper Arguments");
    else {
        string path = AbsoluteFile(Command[1]);
        if(!IsDirect(path)) {
            cout << " Invalid path " << endl;
            return;
        }
        BackArray.push_back(currentDir);
        strcpy(currentDir, path.c_str());
        goto_flag = true;
    }
    return;
}


#endif
